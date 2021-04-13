#ifndef __WHEEL_H__
#define __WHEEL_H__

#include <rtthread.h>
#include <motor.h>
#include <encoder.h>
#include <controller.h>

#define PI 3.1415926f

typedef struct wheel *wheel_t;

struct wheel
{
    motor_t w_motor;
    encoder_t w_encoder;
    controller_t w_controller;

    float rpm;
    float radius;
    float speed_to_rpm;
    float speed_now[5];
};

wheel_t wheel_create(motor_t w_motor, encoder_t w_encoder, controller_t w_controller, float radius);
void wheel_set_speed(wheel_t whl, float speed);
void wheel_update(wheel_t whl);

#endif
