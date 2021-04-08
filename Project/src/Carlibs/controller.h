/*
 * Copyright (c) 2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-26     sogwms       The first version
 */

#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <rtthread.h>

struct controller
{
    float target;
    float output;
    rt_uint16_t sample_time; // unit:ms

    rt_err_t (*update)(void *controller, float current_point);
    rt_err_t (*set_param)(void *controller, float Kp, float Ki, float Kd);
};

typedef struct controller *controller_t;

controller_t controller_create(rt_size_t size, rt_uint16_t sample_time);
rt_err_t controller_update(controller_t controller, float current_point);

rt_err_t controller_set_target(controller_t controller, float target);
rt_err_t controller_set_param(controller_t controller, float Kp, float Ki, float Kd);

#endif // __CONTROLLER_H__
