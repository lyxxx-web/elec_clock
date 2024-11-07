/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "esp_log.h"
#include "esp_err.h"
#include "esp_check.h"

#include "zbuffer.h"
#include "gl.h"
#include "glu.h"

#include "cube_dice.h"

static const char *TAG = "tintgl_dice";

typedef struct {
    float v1;
    float v2;
    float v3;
} V3;

typedef struct {
    float t1;
    float t2;
} T2;

static V3 ptrv[8] = {
    {-1.0f, 1.0f, 1.0f},
    {-1.0f, -1.0f, 1.0f},
    {1.0f, -1.0f, 1.0f},
    {1.0f, 1.0f, 1.0f},
    {-1.0f, 1.0f, -1.0f},
    {-1.0f, -1.0f, -1.0f},
    {1.0f, -1.0f, -1.0f},
    {1.0f, 1.0f, -1.0f}
};

static T2 ptrt[4] = {
    {0.0, 0.0},
    {1.0, 0.0},
    {1.0, 1.0},
    {0.0, 1.0}
};

typedef struct {
    ZBuffer *frameBuffer;
    GLuint TexObj[6];

    float dice_x_set;
    float dice_y_set;
    float dice_z_set;
} tiny_texure_dice_t;

void DrawQUADS(V3 *ptr, int iv1, int iv2, int iv3, int iv4, T2 *ptrt, int it1, int it2, int it3, int it4, GLuint texture)
{
    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_QUADS);
    glTexCoord2fv((float *)&ptrt[it1]);
    glVertex3fv((float *)&ptr[iv1]);

    glTexCoord2fv((float *)&ptrt[it2]);
    glVertex3fv((float *)&ptr[iv2]);

    glTexCoord2fv((float *)&ptrt[it3]);
    glVertex3fv((float *)&ptr[iv3]);

    glTexCoord2fv((float *)&ptrt[it4]);
    glVertex3fv((float *)&ptr[iv4]);

    glEnd();
}

esp_err_t cube_angle_set(tinyGL_modle_handle_t handle, float x_move, float y_move, float z_move)
{
    ESP_RETURN_ON_FALSE(handle, ESP_ERR_INVALID_ARG, TAG, "handle is invalid");
    tiny_texure_dice_t *tinygl_dice = (tiny_texure_dice_t *)(handle);

    tinygl_dice->dice_x_set = x_move;
    tinygl_dice->dice_y_set = y_move;
    tinygl_dice->dice_z_set = z_move;

    return ESP_OK;
}

static void applyDiceInertia(tiny_texure_dice_t *tinygl_dice)
{
    float left;

    static float stop_x = 0;
    static float stop_y = 0;

    static float dice_x_rotation = 0.0f;
    static float dice_y_rotation = 0.0f;
    static float dice_z_rotation = 0.0f;

    static float dice_dst_x = 0.0f;
    static float dice_dst_y = 0.0f;
// static float dice_dst_z = 0.0f;

    dice_x_rotation += tinygl_dice->dice_x_set;
    dice_y_rotation += tinygl_dice->dice_y_set;

    // Check if the velocities are close to zero, and stop the rotation
    if (fabs(tinygl_dice->dice_x_set - stop_x) < 0.5f && fabs(tinygl_dice->dice_y_set - stop_y) < 0.5f) {
        if (fabs(tinygl_dice->dice_x_set - stop_x) < 0.5f) {
            if (tinygl_dice->dice_x_set) {
                left = fmod(dice_x_rotation, 90.0f);
                if (dice_x_rotation > 0) {
                    dice_dst_x = fabs(left) < 45 ? dice_x_rotation - left : dice_x_rotation + (90 - fabs(left));
                } else {
                    dice_dst_x = fabs(left) < 45 ? dice_x_rotation + fabs(left) : dice_x_rotation - (90 - fabs(left));
                }
            }
        }

        if (fabs(tinygl_dice->dice_y_set - stop_y) < 0.5f) {
            if (tinygl_dice->dice_y_set) {
                left = fmod(dice_y_rotation, 90.0f);
                if (dice_y_rotation > 0) {
                    dice_dst_y = fabs(left) < 45 ? dice_y_rotation - left : dice_y_rotation + (90 - fabs(left));
                } else {
                    dice_dst_y = fabs(left) < 45 ? dice_y_rotation + fabs(left) : dice_y_rotation - (90 - fabs(left));
                }
            }
        }

        tinygl_dice->dice_x_set = 0.0f;
        tinygl_dice->dice_y_set = 0.0f;
        stop_x = 0.0f;
        stop_y = 0.0f;

    } else {
        stop_x = tinygl_dice->dice_x_set;
        stop_y = tinygl_dice->dice_y_set;
    }

    if (dice_dst_x) {
        if (fabs(dice_dst_x - dice_x_rotation) < 0.5f) {
            dice_x_rotation = dice_dst_x;
            dice_dst_x = 0;
        } else {
            dice_x_rotation += (dice_dst_x > dice_x_rotation ? 1 : -1) * fmin(8.05f, fabs(dice_dst_x - dice_x_rotation) * 0.15f);
        }
    }

    if (dice_dst_y) {
        if (fabs(dice_dst_y - dice_y_rotation) < 0.5f) {
            dice_y_rotation = dice_dst_y;
            dice_dst_y = 0;
        } else {
            dice_y_rotation += (dice_dst_y > dice_y_rotation ? 1 : -1) * fmin(8.05f, fabs(dice_dst_y - dice_y_rotation) * 0.15f);
        }
    }

    glRotatef(dice_x_rotation, 1.0f, 0.0f, 0.0f);
    glRotatef(dice_y_rotation, 0.0f, 1.0f, 0.0f);
    glRotatef(dice_z_rotation, 0.0f, 0.0f, 1.0f);
}

void reshape()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0, 1.0, 1.0, 300.0);
    glMatrixMode(GL_MODELVIEW);
}

GLuint LoadTexture(Texture_t *texture)
{
    void *image = texture->image;
    int width = texture->width;
    int height = texture->height;

    ESP_LOGD("Load", "width=%d, height=%d, %p", width, height, image);

    glEnable(GL_DEPTH_TEST);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    GLuint TexObj;
    glGenTextures(1, &TexObj);
    glBindTexture(GL_TEXTURE_2D, TexObj);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0,  GL_RGB, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, TexObj);
    glEnable(GL_TEXTURE_2D);

    return TexObj;
}

esp_err_t cube_dice_init(tinyGL_config_t *config, tinyGL_modle_handle_t *handle)
{
    tiny_texure_dice_t *tiny_dice = (tiny_texure_dice_t *)calloc(1, sizeof(tiny_texure_dice_t));
    ESP_RETURN_ON_FALSE(tiny_dice, ESP_ERR_NO_MEM, TAG, "no mem for tiny_texure_dice handle");

    tiny_dice->frameBuffer = ZB_open(config->width, config->height, ZB_MODE_RGB24, 0, NULL, NULL, config->framebuf);

    glInit(tiny_dice->frameBuffer);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glViewport(0, 0, config->width, config->height);
    glEnable(GL_DEPTH_TEST);
    GLfloat h = (GLfloat)config->width / (GLfloat)config->height;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -h, h, 5.0, 60.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -45.0);

    for (int i = 0; i < 6; i++) {
        tiny_dice->TexObj[i] = LoadTexture(&config->texture[i]);
    }
    reshape();

    *handle = (tinyGL_modle_handle_t)tiny_dice;

    ESP_LOGI(TAG, "new tiny_dice handle:@%p", tiny_dice);

    return ESP_OK;
}

esp_err_t cube_dice_deinit(tinyGL_modle_handle_t handle)
{
    ESP_RETURN_ON_FALSE(handle, ESP_ERR_INVALID_ARG, TAG, "handle is invalid");
    tiny_texure_dice_t *tinygl_dice = (tiny_texure_dice_t *)(handle);

    glDisable(GL_TEXTURE_2D);

    glDeleteTextures(sizeof(tinygl_dice->TexObj) / sizeof(GLuint), tinygl_dice->TexObj); // Delete the texture object

    if (tinygl_dice->frameBuffer != NULL) {
        ZB_close(tinygl_dice->frameBuffer);
        tinygl_dice->frameBuffer = NULL;
    }

    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    return ESP_OK;
}

esp_err_t cube_dice_update(tinyGL_modle_handle_t handle)
{
    ESP_RETURN_ON_FALSE(handle, ESP_ERR_INVALID_ARG, TAG, "handle is invalid");
    tiny_texure_dice_t *tinygl_dice = (tiny_texure_dice_t *)(handle);

    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glTranslatef(0.0, 0.0, -6.0);

    applyDiceInertia(tinygl_dice);

    DrawQUADS((V3 *)&ptrv, 0, 1, 2, 3, (T2 *)&ptrt, 3, 0, 1, 2, tinygl_dice->TexObj[0]); //front
    DrawQUADS((V3 *)&ptrv, 0, 3, 7, 4, (T2 *)&ptrt, 1, 2, 3, 0, tinygl_dice->TexObj[1]); //left
    DrawQUADS((V3 *)&ptrv, 4, 7, 6, 5, (T2 *)&ptrt, 2, 3, 0, 1, tinygl_dice->TexObj[2]); //back
    DrawQUADS((V3 *)&ptrv, 5, 6, 2, 1, (T2 *)&ptrt, 3, 0, 1, 2, tinygl_dice->TexObj[3]); //right
    DrawQUADS((V3 *)&ptrv, 7, 3, 2, 6, (T2 *)&ptrt, 3, 0, 1, 2, tinygl_dice->TexObj[4]); //top
    DrawQUADS((V3 *)&ptrv, 5, 1, 0, 4, (T2 *)&ptrt, 3, 0, 1, 2, tinygl_dice->TexObj[5]); //bottom

    return ESP_OK;
}
