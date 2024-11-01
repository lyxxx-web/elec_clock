/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
/*
 * Memory allocator for TinyGL
 */
#include "zgl.h"

/* modify these functions so that they suit your needs */

void gl_free(void *p)
{
    free(p);
}

void *gl_malloc(int size)
{
    return malloc(size);
}

void *gl_zalloc(int size)
{
    return calloc(1, size);
}
