/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_check.h"
#include "esp_timer.h"
#include "esp_lcd_panel_ops.h"
#include "mmap_generate_assets.h"

#include "bsp/esp-bsp.h"
#include "thorvg_capi.h"
#include "thorvg_display.h"

static const char *TAG = "ThorVG";

static esp_err_t example_create_lottie(int index);
static void example_argb888_to_rgb565(uint8_t *in, uint16_t *out, int width, int height);
static const char *example_get_file_extension(const char* filename);

#define LOTTIE_SIZE_HOR         (BSP_LCD_H_RES)
#define LOTTIE_SIZE_VER         (BSP_LCD_V_RES)

static int tvg_size = 240;
static int flag = 0;

extern mmap_assets_handle_t asset_handle;

static esp_err_t example_create_lottie(int index)
{
    esp_err_t ret = ESP_OK;
    Tvg_Result tvg_engine = TVG_RESULT_UNKNOWN;
    Tvg_Result tvg_res = TVG_RESULT_SUCCESS;

    uint32_t *canvas_buf = NULL;
    uint8_t *lottie_buf = NULL;

    Tvg_Animation *animation = NULL;
    Tvg_Canvas *canvas = NULL;

    canvas_buf = heap_caps_aligned_calloc(64, LOTTIE_SIZE_HOR * LOTTIE_SIZE_VER * sizeof(uint32_t), sizeof(uint8_t), MALLOC_CAP_SPIRAM);
    ESP_GOTO_ON_FALSE(canvas_buf, ESP_ERR_NO_MEM, err, TAG, "Error malloc canvas buffer");

    tvg_engine = tvg_engine_init(TVG_ENGINE_SW, 0);
    ESP_GOTO_ON_FALSE(tvg_engine == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_engine_init failed");

    canvas = tvg_swcanvas_create();
    ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_engine_init failed");

    tvg_res = tvg_swcanvas_set_target(canvas, canvas_buf, LOTTIE_SIZE_HOR, LOTTIE_SIZE_HOR, LOTTIE_SIZE_VER, TVG_COLORSPACE_ARGB8888);
    ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_engine_init failed");

    /* shape rect */
    Tvg_Paint *paint = tvg_shape_new();
    ESP_GOTO_ON_FALSE(paint, ESP_ERR_INVALID_STATE, err, TAG, "tvg_shape_new failed");

    tvg_res = tvg_shape_append_rect(paint, 0, 0, LOTTIE_SIZE_HOR, LOTTIE_SIZE_HOR, 0, 0);
    ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_shape_append_rect failed");

    tvg_res = tvg_shape_set_fill_color(paint, 0x00, 0x00, 0x00, 255);
    ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_shape_set_fill_color failed");

    tvg_res = tvg_canvas_push(canvas, paint);
    ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_canvas_push failed");

    tvg_res = tvg_canvas_draw(canvas);
    ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_canvas_draw failed");

    tvg_res = tvg_canvas_sync(canvas);
    ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_canvas_sync failed");

    example_argb888_to_rgb565((uint8_t *)canvas_buf, (uint16_t *)canvas_buf, LOTTIE_SIZE_HOR, LOTTIE_SIZE_VER);
    // esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, LOTTIE_SIZE_HOR, LOTTIE_SIZE_VER, canvas_buf);

    /* tvg Lottie */
    animation = tvg_animation_new();
    ESP_GOTO_ON_FALSE(animation, ESP_ERR_INVALID_STATE, err, TAG, "tvg_animation_new failed");

    Tvg_Paint *picture = tvg_animation_get_picture(animation);
    ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_animation_get_picture failed");

    ESP_LOGI(TAG, "load[%dKB]:[%s]", mmap_assets_get_size(asset_handle, index) / 1000, mmap_assets_get_name(asset_handle, index));
    const uint32_t len = mmap_assets_get_size(asset_handle, index);
    lottie_buf = malloc(len + 1);
    const uint8_t *p = (const uint8_t *)mmap_assets_get_mem(asset_handle, index);
    memcpy(lottie_buf, p, len);
    lottie_buf[len] = '\0';

    const char *ext = example_get_file_extension(mmap_assets_get_name(asset_handle, index));
    tvg_res = tvg_picture_load_data(picture, (const char *)lottie_buf, len, ext, false);
    ESP_GOTO_ON_FALSE(picture, ESP_ERR_INVALID_STATE, err, TAG, "tvg_picture_load_data failed");

    while (1) {
        tvg_res = tvg_picture_set_size(picture, tvg_size, tvg_size);
        ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_picture_set_size failed");

        tvg_res = tvg_canvas_push(canvas, picture);
        ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_canvas_push failed");

        if (NULL == strstr(mmap_assets_get_name(asset_handle, index), ".json")) {
            tvg_res = tvg_canvas_draw(canvas);
            ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_canvas_draw failed");

            tvg_res = tvg_canvas_sync(canvas);
            ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_canvas_sync failed");

            example_argb888_to_rgb565((uint8_t *)canvas_buf, (uint16_t *)canvas_buf, LOTTIE_SIZE_HOR, LOTTIE_SIZE_VER);
            // esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, LOTTIE_SIZE_HOR, LOTTIE_SIZE_VER, canvas_buf);

            if (tvg_size > 80 && !flag) {
                tvg_size -= 10;
            } else {
                flag = 1;
                tvg_size += 10;
                if (tvg_size == 240) {
                    flag = 0;
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(20));
    }
err:
    if (animation) {
        tvg_animation_del(animation);
    }
    if (canvas) {
        tvg_canvas_destroy(canvas);
    }
    if (TVG_RESULT_SUCCESS == tvg_engine) {
        tvg_engine_term(TVG_ENGINE_SW);
    }

    if (canvas_buf) {
        free(canvas_buf);
    }

    if (lottie_buf) {
        free(lottie_buf);
    }

    return ret;
}

static void example_argb888_to_rgb565(uint8_t *in, uint16_t *out, int width, int height)
{
    int numPixels = width * height;
    for (int i = 0; i < numPixels; i++) {
        uint8_t r = in[i * 4 + 2];
        uint8_t g = in[i * 4 + 1];
        uint8_t b = in[i * 4 + 0];

        uint16_t r565 = (r >> 3) & 0x1F;
        uint16_t g565 = (g >> 2) & 0x3F;
        uint16_t b565 = (b >> 3) & 0x1F;

        uint16_t rgb565Value = (r565 << 11) | (g565 << 5) | b565;
        out[i] = (rgb565Value >> 8) | (rgb565Value << 8);
    }
}

static const char *example_get_file_extension(const char* filename)
{
    const char* ext = strrchr(filename, '.');
    return (ext && ext != filename) ? ext + 1 : "";
}
