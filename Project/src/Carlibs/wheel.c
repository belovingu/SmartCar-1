#include "wheel.h"

wheel_t wheel_create(motor_t w_motor, encoder_t w_encoder, controller_t w_controller, float radius)
{
    wheel_t new_wheel = (wheel_t)rt_malloc(sizeof(struct wheel));

    new_wheel->w_motor = w_motor;
    new_wheel->w_encoder = w_encoder;
    new_wheel->w_controller = w_controller;
    new_wheel->radius = radius;
    new_wheel->speed_to_rpm = 60.0f / (100 * radius * 2.0f * PI);

    return new_wheel;
}

/** speed: cm/s, rpm: vol/min**/
/** so : rpm = speed x 60.0f / (100 * radius x 2 x PT) */
/** speed_to_rpm_transform = 60.0f / (100 * radius x 2 x PT) **/
/** then : rpm = speed * speed_to_rpm_transform **/
void wheel_set_speed(wheel_t whl, float speed)
{
    RT_ASSERT(whl != RT_NULL);

    float rpm = (speed * whl->speed_to_rpm);
    controller_set_target(whl->w_controller, rpm);
}

void wheel_update(wheel_t whl)
{
    RT_ASSERT(whl != RT_NULL);

    whl->rpm = encoder_measure_rpm(whl->w_encoder);
    whl->speed_now[0] = whl->rpm / whl->speed_to_rpm;

    for (int i = 5; i >= 1; i--)
    {
        whl->speed_now[i] = whl->speed_now[i - 1];
    }

    controller_update(whl->w_controller, whl->rpm);
    motor_run(whl->w_motor, (int)whl->w_controller->output);
}
