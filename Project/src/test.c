#include "headfile.h"

motor_t left_motor;
motor_t right_motor;
encoder_t left_encoder;
encoder_t right_encoder;

void openctrl_init()
{
    servo_init();
    servo1_set_angle(0);

    left_encoder = encoder_create(1, 50);
    right_encoder = encoder_create(2, 50);

    left_motor = motor_create(1);
    right_motor = motor_create(2);
}

int openctrl_motor(int argc, char **argv)
{
    int motor_pwm = atoi(argv[1]);

    motor_run(left_motor, motor_pwm);
    motor_run(right_motor, motor_pwm);

    // rt_thread_mdelay(100);
    // motor_run(left_motor, 0);
    // motor_run(right_motor, 0);

    return RT_EOK;
}

int openctrl_servo(int argc, char **argv)
{
    int servo_value = atoi(argv[1]);
    servo1_set_angle(servo_value);

    return RT_EOK;
}

int openctrl_encoder()
{
    while (1)
    {
        int a = (int)encoder_measure_rpm(left_encoder);
        int b = (int)encoder_measure_rpm(right_encoder);
        rt_kprintf("left: %d,right: %d\r\n", a, b);
        rt_thread_mdelay(50);
    }
    return RT_EOK;
}

MSH_CMD_EXPORT(openctrl_encoder, openctrl_encoder);
MSH_CMD_EXPORT(openctrl_init, openctrl_init);
MSH_CMD_EXPORT(openctrl_motor, openctrl_motor(-1000 ~1000));
MSH_CMD_EXPORT(openctrl_servo, openctrl_servo(-1000 ~1000));