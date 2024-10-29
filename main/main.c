/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "esp_log.h"
//#include "bsp/esp32_s3_eye.h"
#include "bsp/esp-box.h"
#include "lvgl.h"
#include "ui/ui.h"
#include "lv_examples.h"
#include "esp_log.h"
#include "app_wifi.h"
#include "app_weather.h"
#include "nvs_flash.h"
#include <time.h>
#include <sys/time.h>
#include "ui/components/ui_comp_panel2.h"
#include "app/app_weather.h"

#define TAG "ESP-EXAMPLE"

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
    lv_label_set_text(ui_comp_get_child(ui_Panel2,UI_COMP_PANEL2_TIMESTATE), time_str);
    lv_label_set_text(ui_comp_get_child(ui_Panel5,UI_COMP_PANEL2_TIMESTATE), time_str);
    lv_label_set_text(ui_comp_get_child(ui_Panel6,UI_COMP_PANEL2_TIMESTATE), time_str);
    lv_label_set_text(ui_weather, item_text);
    lv_label_set_text(ui_temp, item_temp);
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

void app_main(void)
{
    /* Initialize NVS. */
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    /* Initialize display and LVGL */
    //bsp_display_start();
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

    /* Add and show objects on display */
    app_lvgl_display();

    app_weather_start();
    app_network_start();

    ESP_LOGI(TAG, "Example initialization done.");
}
// *INDENT-ON*
