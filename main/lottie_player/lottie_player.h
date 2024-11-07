/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "esp_err.h"

typedef bool (*lottie_player_end_cb_t)(void *user_data);
typedef struct {
    uint16_t width;
    uint16_t height;
    uint8_t *framebuf;

    const char *file_name;
    void *file_data;
    uint32_t file_size;

    lottie_player_end_cb_t on_end;
    void *target_obj;
} lottie_player_cfg_t;

typedef struct lottie_player_t *lottie_palyer_handle_t;

esp_err_t lottie_player_init(lottie_player_cfg_t *config, lottie_palyer_handle_t *handle);

esp_err_t lottie_player_deinit(lottie_palyer_handle_t handle);

esp_err_t lottie_player_update(lottie_palyer_handle_t handle);
