/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __CUBETEXT_H__
#define __CUBETEXT_H__

void cube_dice_init(int xsize, int ysize, void *cbuf);

void cube_dice_deinit();

void cube_dice_update();

void cube_angle_set(float x_move, float y_move, float z_move);

#endif
