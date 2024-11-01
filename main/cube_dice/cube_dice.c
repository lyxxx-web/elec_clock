/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <string.h>
#include "esp_log.h"

#include <stdlib.h>
#include <math.h>
#include "zbuffer.h"
#include "gl.h"
#include "glu.h"
#include "mmap_generate_assets.h"

typedef struct {
    float v1;
    float v2;
    float v3;
} V3;

typedef struct {
    float t1;
    float t2;
} T2;

// 初始化一个八个顶点的立方体
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

// 初始化纹理坐标
static T2 ptrt[4] = {
    {0.0, 0.0},
    {1.0, 0.0},
    {1.0, 1.0},
    {0.0, 1.0}
};

static ZBuffer *frameBuffer;
static GLuint TexObj[6];

static float dice_x_set = 0.0f;
static float dice_y_set = 0.0f;
static float dice_z_set = 0.0f;

extern mmap_assets_handle_t asset_handle;

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

void cube_angle_set(float x_move, float y_move, float z_move)
{
    dice_x_set = x_move;
    dice_y_set = y_move;
    dice_z_set = z_move;
}

static void applyDiceInertia()
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

    dice_x_rotation += dice_x_set;
    dice_y_rotation += dice_y_set;

    // Check if the velocities are close to zero, and stop the rotation
    if (fabs(dice_x_set - stop_x) < 0.5f && fabs(dice_y_set - stop_y) < 0.5f) {
        if (fabs(dice_x_set - stop_x) < 0.5f) {
            if (dice_x_set) {
                left = fmod(dice_x_rotation, 90.0f);
                if (dice_x_rotation > 0) {
                    dice_dst_x = fabs(left) < 45 ? dice_x_rotation - left : dice_x_rotation + (90 - fabs(left));
                } else {
                    dice_dst_x = fabs(left) < 45 ? dice_x_rotation + fabs(left) : dice_x_rotation - (90 - fabs(left));
                }
            }
        }

        if (fabs(dice_y_set - stop_y) < 0.5f) {
            if (dice_y_set) {
                left = fmod(dice_y_rotation, 90.0f);
                if (dice_y_rotation > 0) {
                    dice_dst_y = fabs(left) < 45 ? dice_y_rotation - left : dice_y_rotation + (90 - fabs(left));
                } else {
                    dice_dst_y = fabs(left) < 45 ? dice_y_rotation + fabs(left) : dice_y_rotation - (90 - fabs(left));
                }
            }
        }

        dice_x_set = 0.0f;
        dice_y_set = 0.0f;
        stop_x = 0.0f;
        stop_y = 0.0f;

    } else {
        stop_x = dice_x_set;
        stop_y = dice_y_set;
    }

    if (dice_dst_x) {
        if (fabs(dice_dst_x - dice_x_rotation) < 0.1f) {
            dice_x_rotation = dice_dst_x;
            dice_dst_x = 0;
        } else {
            dice_x_rotation += (dice_dst_x > dice_x_rotation ? 1 : -1) * fmin(8.05f, fabs(dice_dst_x - dice_x_rotation) * 0.15f);
        }
    }

    if (dice_dst_y) {
        if (fabs(dice_dst_y - dice_y_rotation) < 0.1f) {
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

GLuint LoadTexture(uint8_t image_index)
{
    int width, height;
    void *image;

    image = (void *)mmap_assets_get_mem(asset_handle, MMAP_ASSETS_DICE1_BMP + image_index);
    image += 54;
    width = mmap_assets_get_width(asset_handle, MMAP_ASSETS_DICE1_BMP + image_index);
    height = mmap_assets_get_height(asset_handle, MMAP_ASSETS_DICE1_BMP + image_index);

    ESP_LOGI("Load", "width=%d, height=%d, %p", width, height, image);

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

void cube_dice_init(int xsize, int ysize, void *cbuf)
{
    frameBuffer = ZB_open(xsize, ysize, ZB_MODE_RGB24, 0, NULL, NULL, cbuf);

    glInit(frameBuffer);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glViewport(0, 0, xsize, ysize);
    glEnable(GL_DEPTH_TEST);
    GLfloat h = (GLfloat)xsize / (GLfloat)ysize;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -h, h, 5.0, 60.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -45.0);

    TexObj[0] = LoadTexture(0);
    TexObj[1] = LoadTexture(1);
    TexObj[2] = LoadTexture(2);
    TexObj[3] = LoadTexture(3);
    TexObj[4] = LoadTexture(4);
    TexObj[5] = LoadTexture(5);
    reshape();
}

void cube_dice_deinit()
{
    glDisable(GL_TEXTURE_2D);

    glDeleteTextures(sizeof(TexObj) / sizeof(GLuint), TexObj); // Delete the texture object

    if (frameBuffer != NULL) {
        ZB_close(frameBuffer);
        frameBuffer = NULL;
    }

    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void cube_dice_update()
{
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glTranslatef(0.0, 0.0, -6.0);

    applyDiceInertia();

    DrawQUADS((V3 *)&ptrv, 0, 1, 2, 3, (T2 *)&ptrt, 3, 0, 1, 2, TexObj[0]); //front
    DrawQUADS((V3 *)&ptrv, 0, 3, 7, 4, (T2 *)&ptrt, 1, 2, 3, 0, TexObj[1]); //left
    DrawQUADS((V3 *)&ptrv, 4, 7, 6, 5, (T2 *)&ptrt, 2, 3, 0, 1, TexObj[2]); //back
    DrawQUADS((V3 *)&ptrv, 5, 6, 2, 1, (T2 *)&ptrt, 3, 0, 1, 2, TexObj[3]); //right
    DrawQUADS((V3 *)&ptrv, 7, 3, 2, 6, (T2 *)&ptrt, 3, 0, 1, 2, TexObj[4]); //top
    DrawQUADS((V3 *)&ptrv, 5, 1, 0, 4, (T2 *)&ptrt, 3, 0, 1, 2, TexObj[5]); //bottom
}
