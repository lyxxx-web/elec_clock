/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <time.h>
#include <sys/time.h>
#include "esp_log.h"
#include "ui/ui.h"
#include "app_wifi.h"
#include "app_weather.h"
#include "app_imu.h"
#include "nvs_flash.h"

#include "bsp/esp-bsp.h"
#include "mmap_generate_assets.h"

#define TAG "main"

/*******************************************************************************
* Private functions
*******************************************************************************/

// *INDENT-OFF*

struct timeval tv_now =
{
   .tv_sec = 0,
   .tv_usec = 0 
};

void ui_clock_update(lv_timer_t *timer)
{
    struct tm timeinfo;
    static char hour_str[3], min_str[3],time_str[6];
    char date_str[16], weekday_str[16];

    gettimeofday(&tv_now, NULL);
    localtime_r(&tv_now.tv_sec, &timeinfo);

    // 格式化小时、分钟和秒
    snprintf(hour_str, sizeof(hour_str), "%02d", timeinfo.tm_hour);
    snprintf(min_str, sizeof(min_str), "%02d", timeinfo.tm_min);
    snprintf(time_str, sizeof(time_str), "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);

    // 格式化月和日（如：10月18日）
    snprintf(date_str, sizeof(date_str), "%02d/%02d", 
             timeinfo.tm_mon + 1, timeinfo.tm_mday);

    // 获取星期几
    const char *weekdays[] = {"周日", "周一", "周二", "周三", 
                              "周四", "周五", "周六"};
    snprintf(weekday_str, sizeof(weekday_str), "%s", weekdays[timeinfo.tm_wday]);

    // 更新各个label的文本
    lv_label_set_text(ui_hour, hour_str);
    lv_label_set_text(ui_min, min_str);
    lv_label_set_text(ui_date, date_str);
    lv_label_set_text(ui_weekday, weekday_str);
    lv_label_set_text(title_timestate, time_str);

    lv_label_set_text(ui_weather, item_text);
    lv_label_set_text(ui_temp, item_temp);
    if(wifi_connected_already() == WIFI_STATUS_CONNECTED_OK){
        lv_img_set_src(title_wifistate, &ui_img_wifi_png);   
    }else{
        lv_img_set_src(title_wifistate, &ui_img_wifi_disconnection_png); 
    } 
}

void ui_init_timer()
{
    lv_timer_t * timer_clock = lv_timer_create(ui_clock_update, 1000,  NULL);
    ui_clock_update(timer_clock);
}

void app_lvgl_display(void)
{
    bsp_display_lock(0);

    ui_init();
    ui_init_timer();

    bsp_display_unlock();
}

mmap_assets_handle_t asset_handle;

void app_mount_mmap_fs()
{
    const mmap_assets_config_t config = {
        .partition_label = "assets_A",
        .max_files = MMAP_ASSETS_FILES,
        .checksum = MMAP_ASSETS_CHECKSUM,
        .flags = {
            .mmap_enable = true,
            .app_bin_check = true,
        },
    };

    mmap_assets_new(&config, &asset_handle);

    int stored_files = mmap_assets_get_stored_files(asset_handle);
    ESP_LOGI(TAG, "stored_files:%d", stored_files);

    for (int i = 0; i < MMAP_ASSETS_FILES; i++) {
        const char *name = mmap_assets_get_name(asset_handle, i);
        const uint8_t *mem = mmap_assets_get_mem(asset_handle, i);
        int size = mmap_assets_get_size(asset_handle, i);
        int width = mmap_assets_get_width(asset_handle, i);
        int height = mmap_assets_get_height(asset_handle, i);

        ESP_LOGI(TAG, "name:[%s], mem:[%p], size:[%d bytes], w:[%d], h:[%d]", name, mem, size, width, height);
    }
}

void app_main(void)
{
    /* Initialize NVS. */
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    /* Initialize display and LVGL */
    bsp_display_cfg_t custom_cfg = {
        .lvgl_port_cfg = 
        {
        .task_priority = 4,       
        .task_stack = 30000,
        .task_affinity = -1,
        .task_max_sleep_ms = 500,
        .timer_period_ms = 5,
    },
        .buffer_size = BSP_LCD_H_RES * CONFIG_BSP_LCD_DRAW_BUF_HEIGHT,
#if CONFIG_BSP_LCD_DRAW_BUF_DOUBLE
        .double_buffer = 1,
#else
        .double_buffer = 0,
#endif
        .flags = {
            .buff_dma = true,
            .buff_spiram = false,
        }
    };
    bsp_display_start_with_config(&custom_cfg);

    /* Turn on display backlight */
    bsp_display_backlight_on();

    app_mount_mmap_fs();

    /* Add and show objects on display */
    app_lvgl_display();

    app_weather_start();
    app_network_start();

    app_imu_init();

    ESP_LOGI(TAG, "Example initialization done.");
}
// *INDENT-ON*
