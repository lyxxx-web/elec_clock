/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __CUBETEXT_H__
#define __CUBETEXT_H__

typedef struct {
    void *image;
    uint16_t width;
    uint16_t height;
} Texture_t;

typedef struct {
    uint16_t width;
    uint16_t height;
    uint8_t *framebuf;
    Texture_t texture[6];
} tinyGL_config_t;

typedef struct tibyGL_modle_t *tinyGL_modle_handle_t;

esp_err_t cube_dice_init(tinyGL_config_t *config, tinyGL_modle_handle_t *handle);

esp_err_t cube_dice_deinit(tinyGL_modle_handle_t handle);

esp_err_t cube_dice_update(tinyGL_modle_handle_t handle);

esp_err_t cube_angle_set(tinyGL_modle_handle_t handle, float x_move, float y_move, float z_move);

#endif
