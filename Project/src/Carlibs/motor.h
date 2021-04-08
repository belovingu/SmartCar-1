/*
 * Copyright (c) 2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-08-26     sogwms       The first version
 */

#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <rtthread.h>

typedef struct motor *motor_t;
struct motor
{
    int index;
};

motor_t motor_create(int index);
rt_err_t motor_run(motor_t motor, int thousands);

#endif
