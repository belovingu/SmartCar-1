/*
 * Copyright (c) 2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-26     sogwms       The first version
 */

#ifndef __ENCODER_H__
#define __ENCODER_H__

#include <rtthread.h>
#include "zf_qtimer.h"

typedef struct encoder *encoder_t;

struct encoder
{
    int index;
    float rpm;
    int sample_time;
    int last_time;
};

encoder_t encoder_create(int index, int sample_time);
float encoder_measure_rpm(encoder_t enc); /* revolutions per minute */

#endif
