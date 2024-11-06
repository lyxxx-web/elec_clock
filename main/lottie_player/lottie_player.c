/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "esp_check.h"

#include "lottie_player.h"
#include "thorvg_capi.h"
#include "mmap_generate_assets.h"

static const char *TAG = "lottie_player";

typedef struct {
    uint8_t *lottie_buf;
    Tvg_Canvas *canvas;
    Tvg_Animation *animation;
    float f_total;
} lottie_player_t;

static const char *example_get_file_extension(const char* filename)
{
    const char* ext = strrchr(filename, '.');
    return (ext && ext != filename) ? ext + 1 : "";
}

esp_err_t lottie_player_init(lottie_player_config_t *config, lottie_palyer_handle_t *handle)
{
    esp_err_t ret = ESP_OK;
    Tvg_Result tvg_engine = TVG_RESULT_UNKNOWN;
    Tvg_Result tvg_res = TVG_RESULT_SUCCESS;

    lottie_player_t *player = (lottie_player_t *)calloc(1, sizeof(lottie_player_t));
    ESP_GOTO_ON_FALSE(player, ESP_ERR_NO_MEM, err, TAG, "no mem for palyer handle");

    tvg_engine = tvg_engine_init(TVG_ENGINE_SW, 0);
    ESP_GOTO_ON_FALSE(tvg_engine == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_engine_init failed");

    player->canvas = tvg_swcanvas_create();
    ESP_GOTO_ON_FALSE(player->canvas, ESP_ERR_INVALID_STATE, err, TAG, "tvg_engine_init failed");

    tvg_res = tvg_swcanvas_set_target(player->canvas, (uint32_t *)config->framebuf, config->player_width, config->player_width, config->player_height, TVG_COLORSPACE_ARGB8888);
    ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_engine_init failed");

    /* tvg Lottie */
    player->animation = tvg_animation_new();
    ESP_GOTO_ON_FALSE(player->animation, ESP_ERR_INVALID_STATE, err, TAG, "tvg_animation_new failed");

    Tvg_Paint *picture = tvg_animation_get_picture(player->animation);
    ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_animation_get_picture failed");

    player->lottie_buf = heap_caps_calloc(1, config->file_size + 1, MALLOC_CAP_SPIRAM);
    if (player->lottie_buf == NULL) {
        ESP_LOGE(TAG, "lottie_buf malloc failed");
    } else {
        memcpy(player->lottie_buf, config->file_data, config->file_size);
        player->lottie_buf[config->file_size] = '\0';
    }

    const char *ext = example_get_file_extension(config->file_name);
    tvg_res = tvg_picture_load_data(picture, (const char *)player->lottie_buf, config->file_size, ext, false);
    ESP_GOTO_ON_FALSE(picture, ESP_ERR_INVALID_STATE, err, TAG, "tvg_picture_load_data failed");

    tvg_res = tvg_picture_set_size(picture, config->player_width, config->player_height);
    ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_picture_set_size failed");

    tvg_res = tvg_canvas_push(player->canvas, picture);
    ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_canvas_push failed");

    /* Get player->animation total frame */
    float f_total;
    tvg_res = tvg_animation_get_total_frame(player->animation, &f_total);
    ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_animation_get_total_frame failed");
    ESP_GOTO_ON_FALSE((f_total != 0.0f), ESP_ERR_INVALID_STATE, err, TAG, "tvg_animation_get_total_frame failed");
    player->f_total = f_total;
    ESP_LOGI(TAG, "totalFrame:%f", f_total);

    *handle = (lottie_palyer_handle_t)player;

    ESP_LOGI(TAG, "new lottie player handle:@%p", player);

    return ESP_OK;

err:
    if (player->animation) {
        tvg_animation_del(player->animation);
    }

    if (player->canvas) {
        tvg_canvas_destroy(player->canvas);
    }

    if (TVG_RESULT_SUCCESS == tvg_engine) {
        tvg_engine_term(TVG_ENGINE_SW);
    }

    if (player->lottie_buf) {
        free(player->lottie_buf);
    }

    if (player) {
        free(player);
    }

    return ret;
}

esp_err_t lottie_player_deinit(lottie_palyer_handle_t handle)
{
    ESP_RETURN_ON_FALSE(handle, ESP_ERR_INVALID_ARG, TAG, "handle is invalid");
    lottie_player_t *player = (lottie_player_t *)(handle);

    if (player->animation) {
        tvg_animation_del(player->animation);
    }
    if (player->canvas) {
        tvg_canvas_destroy(player->canvas);
    }

    tvg_engine_term(TVG_ENGINE_SW);

    if (player->lottie_buf) {
        free(player->lottie_buf);
    }

    if (player) {
        free(player);
    }

    return ESP_OK;
}

esp_err_t lottie_player_update(lottie_palyer_handle_t handle)
{
    ESP_RETURN_ON_FALSE(handle, ESP_ERR_INVALID_ARG, TAG, "handle is invalid");
    lottie_player_t *player = (lottie_player_t *)(handle);

    esp_err_t ret = ESP_OK;
    Tvg_Result tvg_res = TVG_RESULT_SUCCESS;
    float f = 0;

    /* Get the frame and refresh it to screen */
    tvg_res = tvg_animation_get_frame(player->animation, &f);
    if (++f >= player->f_total) {
        f = 0;
    }
    ESP_LOGD(TAG, "frame set:%f", f);
    tvg_res = tvg_animation_set_frame(player->animation, f);
    ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_animation_set_frame failed");

    tvg_res = tvg_canvas_update(player->canvas);
    ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_canvas_update failed");

    tvg_res = tvg_canvas_draw(player->canvas);
    ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_canvas_draw failed");

    tvg_res = tvg_canvas_sync(player->canvas);
    ESP_GOTO_ON_FALSE(tvg_res == TVG_RESULT_SUCCESS, ESP_ERR_INVALID_STATE, err, TAG, "tvg_canvas_sync failed");

    return ESP_OK;

err:
    if (player->animation) {
        tvg_animation_del(player->animation);
    }
    if (player->canvas) {
        tvg_canvas_destroy(player->canvas);
    }

    tvg_engine_term(TVG_ENGINE_SW);

    if (player->lottie_buf) {
        free(player->lottie_buf);
    }

    if (player) {
        free(player);
    }

    return ret;
}