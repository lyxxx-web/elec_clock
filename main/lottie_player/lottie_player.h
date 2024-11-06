/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "esp_err.h"

typedef struct {
    uint16_t player_width;
    uint16_t player_height;
    uint8_t *framebuf;

    const char *file_name;
    void *file_data;
    uint32_t file_size;
} lottie_player_config_t;

typedef struct lottie_player_t *lottie_palyer_handle_t;

esp_err_t lottie_player_init(lottie_player_config_t *config, lottie_palyer_handle_t *handle);

esp_err_t lottie_player_deinit(lottie_palyer_handle_t handle);

esp_err_t lottie_player_update(lottie_palyer_handle_t handle);
