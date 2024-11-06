
/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
#include <string.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_attr.h"
#include "esp_sleep.h"
#include "nvs_flash.h"

#include "bsp/esp-bsp.h"
#include "icm42670.h"
#include "cube_dice.h"

static const char *TAG = "imu";

static i2c_master_bus_handle_t i2c_bus_handle;
static icm42670_value_t gyro_val_last;
static icm42670_handle_t imu = NULL;

extern tinyGL_modle_handle_t tinygl_handle;

static void app_imu_read(void)
{
    icm42670_value_t gyro_offset = {0};

    icm42670_value_t gyro_val;
    icm42670_get_gyro_value(imu, &gyro_val);
    // ESP_LOGI(TAG, "GYRO val: %.2f, %.2f, %.2f", gyro_val.x, gyro_val.y, gyro_val.z);

    if (fabs(gyro_val_last.x - gyro_val.x) / 1 > 5.0f) {
        gyro_offset.y = (gyro_val.x - gyro_val_last.x) / 1;
        gyro_val_last.x = gyro_val.x;
        // ESP_LOGI(TAG, "x OFFSET: %.2f", gyro_offset.y);
        if(tinygl_handle){
            cube_angle_set(tinygl_handle, gyro_offset.x, gyro_offset.y, gyro_offset.z);
        }
    }

    if (fabs(gyro_val_last.y - gyro_val.y) / 1 > 5.0f) {
        gyro_offset.x = (gyro_val.y - gyro_val_last.y) / 1;
        gyro_val_last.y = gyro_val.y;
        // ESP_LOGI(TAG, "y OFFSET: %.2f", gyro_offset.x);
        if(tinygl_handle){
            cube_angle_set(tinygl_handle, gyro_offset.x, gyro_offset.y, gyro_offset.z);
        }
    }

    if (fabs(gyro_val_last.z - gyro_val.z) / 5 > 5.0f) {
        gyro_offset.z = (gyro_val.z - gyro_val_last.z) / 5;
        gyro_val_last.z = gyro_val.z;
    }
}

static void app_imu_task(void *arg)
{
    while (1) {
        app_imu_read();
        vTaskDelay(pdMS_TO_TICKS(30));
    }
    vTaskDelete(NULL);
}

void app_imu_init(void)
{
    bsp_get_i2c_bus_handle(&i2c_bus_handle);
    imu = icm42670_create(i2c_bus_handle, ICM42670_I2C_ADDRESS);
    if (imu) {
        /* Configuration of the acceleremeter and gyroscope */
        const icm42670_cfg_t imu_cfg = {
            .acce_fs = ACCE_FS_2G,
            .acce_odr = ACCE_ODR_400HZ,
            .gyro_fs = GYRO_FS_2000DPS,
            .gyro_odr = GYRO_ODR_400HZ,
        };
        ESP_ERROR_CHECK(icm42670_config(imu, &imu_cfg));

        /* Set accelerometer and gyroscope to ON */
        icm42670_acce_set_pwr(imu, ACCE_PWR_LOWNOISE);
        icm42670_gyro_set_pwr(imu, GYRO_PWR_LOWNOISE);
    }

    BaseType_t res;

    res = xTaskCreate(app_imu_task, "imu task", 4 * 1024, NULL, 5, NULL);
    if (res != pdPASS) {
        ESP_LOGE(TAG, "Create imu task fail!");
    }
}