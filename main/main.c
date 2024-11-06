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
#include "esp_lv_decoder.h"
#include "esp_lv_fs.h"

#define TAG "main"

mmap_assets_handle_t asset_handle;
static esp_lv_fs_handle_t fs_drive_handle;
static esp_lv_decoder_handle_t decoder_handle = NULL;
static const mmap_assets_config_t config = {
    .partition_label = "assets_A",
    .max_files = MMAP_ASSETS_FILES,
    .checksum = MMAP_ASSETS_CHECKSUM,
    .flags = {
        .mmap_enable = true,
        .app_bin_check = true,
    },
};
/*******************************************************************************
* Private functions
*******************************************************************************/

// *INDENT-OFF*

void app_mount_mmap_fs()
{

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

esp_err_t lv_fs_add(void)
{
    fs_cfg_t fs_cfg;

        fs_cfg.fs_letter = 'A';
        fs_cfg.fs_assets = asset_handle;
        fs_cfg.fs_nums = config.max_files;

        esp_err_t ret = esp_lv_fs_desc_init(&fs_cfg, &fs_drive_handle);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to initialize FS for %s", config.partition_label);
            return ret;
        }

    return ESP_OK;
}

int find_icon_index(char * data)
{
    int i,j;
    char icon_name [10];
    snprintf(icon_name, sizeof(icon_name), "%s.png", data);
    for(i = 0; i < mmap_assets_get_stored_files(asset_handle); i++){
        if(!strcmp(mmap_assets_get_name(asset_handle, i), icon_name))
            break;
    }
    if(i == mmap_assets_get_stored_files(asset_handle)){
        ESP_LOGE(TAG, "cannot find corresponding weather icon");
        for(j = 0; i < mmap_assets_get_stored_files(asset_handle); j++){
            if(!strcmp(mmap_assets_get_name(asset_handle, i), "999.png"))
                break;
        }
        i = j;
    }
    return i;

}

void updata_weather_icon(void)
{
    static lv_img_dsc_t img_weather_dsc;

    img_weather_dsc.data_size = mmap_assets_get_size(asset_handle, find_icon_index(weather_icon));
    img_weather_dsc.data = mmap_assets_get_mem(asset_handle, find_icon_index(weather_icon));
    lv_img_set_src(ui_weathershow, &img_weather_dsc);  
}

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

    //format hour,minute,second
    snprintf(hour_str, sizeof(hour_str), "%02d", timeinfo.tm_hour);
    snprintf(min_str, sizeof(min_str), "%02d", timeinfo.tm_min);
    snprintf(time_str, sizeof(time_str), "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);

    //format month and day(for example:10/18)
    snprintf(date_str, sizeof(date_str), "%02d/%02d", 
             timeinfo.tm_mon + 1, timeinfo.tm_mday);

    //get weekday
    const char *weekdays[] = {"周日", "周一", "周二", "周三", 
                              "周四", "周五", "周六"};
    snprintf(weekday_str, sizeof(weekday_str), "%s", weekdays[timeinfo.tm_wday]);

    //update text of each label
    lv_label_set_text(ui_hour, hour_str);
    lv_label_set_text(ui_min, min_str);
    lv_label_set_text(ui_date, date_str);
    lv_label_set_text(ui_weekday, weekday_str);
    lv_label_set_text(title_timestate, time_str);

    lv_label_set_text(ui_weather, weather_text);
    lv_label_set_text(ui_temp, weather_temp);
    if(wifi_connected_already() == WIFI_STATUS_CONNECTED_OK){
        lv_img_set_src(title_wifistate, &ui_img_wifi_png);   
    }else{
        lv_img_set_src(title_wifistate, &ui_img_wifi_disconnection_png); 
    }
    updata_weather_icon();
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

<<<<<<< HEAD
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

        // ESP_LOGI(TAG, "name:[%s], mem:[%p], size:[%d bytes], w:[%d], h:[%d]", name, mem, size, width, height);
    }
}

void perf_decoder_fs_esp(void)
{
    esp_lv_decoder_handle_t decoder_handle = NULL;
    esp_err_t ret_fs = esp_lv_decoder_init(&decoder_handle);
    if (ret_fs != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize SPNG decoder");
        return;
    }
    int i;
    char icon_name [10];
    snprintf(icon_name, sizeof(icon_name), "%s.png", weather_icon);
    for(i = 0; i < mmap_assets_get_stored_files(asset_handle); i++){
        //ESP_LOGI(TAG, "get_stored_files: %s", mmap_assets_get_name(asset_handle, i));
        if(!strcmp(mmap_assets_get_name(asset_handle, i), icon_name))
            break;
    }
    if(i == mmap_assets_get_stored_files(asset_handle)){
        ESP_LOGE(TAG, "cannot find corresponding weather icon");
        int j;
        for(j = 0; i < mmap_assets_get_stored_files(asset_handle); j++){
            if(!strcmp(mmap_assets_get_name(asset_handle, i), "999.png"))
                break;
        }
        i = j;
    }
    img_weather_dsc.data_size = mmap_assets_get_size(asset_handle, i);
    img_weather_dsc.data = mmap_assets_get_mem(asset_handle, i);
}

=======
>>>>>>> f45c91d (complete home page)
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
        .lvgl_port_cfg = ESP_LVGL_PORT_INIT_CONFIG(),
        .buffer_size = BSP_LCD_H_RES * BSP_LCD_V_RES,
        .trans_size = BSP_LCD_H_RES * 10, // in SRAM, DMA-capable
        .double_buffer = 0,
        .flags = {
            .buff_dma = false,
            .buff_spiram = true,
        }
    };
    custom_cfg.lvgl_port_cfg.task_stack = 1024*30,
    bsp_display_start_with_config(&custom_cfg);

    /* Turn on display backlight */
    bsp_display_backlight_on();

    app_mount_mmap_fs();

    esp_err_t ret_fs = lv_fs_add();
    if (ret_fs != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize flash FS");
    }
    ret_fs = esp_lv_decoder_init(&decoder_handle);
    if (ret_fs != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize SPNG decoder");
    }
    /* Add and show objects on display */
    app_lvgl_display();

    app_weather_start();
    app_network_start();

    app_imu_init();

    ESP_LOGI(TAG, "Example initialization done.");
}
// *INDENT-ON*
