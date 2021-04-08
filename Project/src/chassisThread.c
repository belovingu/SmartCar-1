#include "headfile.h"

#include "inc_pid_controller.h"
#include "pos_pid_controller.h"
#include "encoder.h"
#include "wheel.h"
#include "motor.h"

#include "util.h"
#include "chassisThread.h"

#define DBG_TAG "car"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

wheel_t left_wheel = RT_NULL;
wheel_t right_wheel = RT_NULL;
rt_bool_t is_chassis_debug = RT_FALSE;
rt_bool_t is_chassis_running = RT_TRUE;

float left_target_rpm = 0, right_target_rpm = 0;
float current_velocity = 0, last_velocity = 0;
float wheel_kp = 0.6, wheel_ki = 0.2, wheel_kd = 0;
float wheel_diff_p = 0.002; //80*1

extern float steer_error;
extern rt_mutex_t chassis_dmutex;

void chassis_thread(void *parameter)
{
    // 1.1 Create two motors
    motor_t left_motor = motor_create(1);
    motor_t right_motor = motor_create(2);

    // 1.2 Create two encoders
    encoder_t left_encoder = encoder_create(1, WHEEL_SAMPLE_TIME);
    encoder_t right_encoder = encoder_create(2, WHEEL_SAMPLE_TIME);

    // 1.3 Create two pid contollers
    inc_pid_controller_t left_pid = inc_pid_controller_create(wheel_kp, wheel_ki, wheel_kd,
                                                              WHEEL_PID_MAX_OUT,
                                                              -WHEEL_PID_MAX_OUT,
                                                              WHEEL_SAMPLE_TIME);
    inc_pid_controller_t right_pid = inc_pid_controller_create(wheel_kp, wheel_ki, wheel_kd,
                                                               WHEEL_PID_MAX_OUT,
                                                               -WHEEL_PID_MAX_OUT,
                                                               WHEEL_SAMPLE_TIME);

    // 1.4 Add two wheels
    left_wheel = wheel_create((motor_t)left_motor,
                              (encoder_t)left_encoder,
                              (controller_t)left_pid,
                              WHEEL_RADIUS);
    right_wheel = wheel_create((motor_t)right_motor,
                               (encoder_t)right_encoder,
                               (controller_t)right_pid,
                               WHEEL_RADIUS);

    rt_thread_mdelay(100);
    while (1)
    {
        if (is_chassis_running)
        {
            rt_mutex_take(chassis_dmutex, RT_WAITING_FOREVER);
            wheel_update(left_wheel);
            wheel_update(right_wheel);
            rt_mutex_release(chassis_dmutex);
        }
        else
        {
            motor_run(left_wheel->w_motor, 0);
            motor_run(right_wheel->w_motor, 0);
        }

        if (is_chassis_debug)
        {
            rt_kprintf("wheel:%d,%d,%d,%d\r\n",
                       (int)left_wheel->rpm, (int)left_wheel->w_controller->output,
                       (int)right_wheel->rpm, (int)right_wheel->w_controller->output);
        }
        rt_thread_mdelay(WHEEL_SAMPLE_TIME);
    }
}

int set_velocity(float velocity)
{
    if (left_wheel == RT_NULL || right_wheel == RT_NULL)
    {
        return RT_ERROR;
    }

    float left_speed = 0, right_speed = 0, computed = 0;
    rt_mutex_take(chassis_dmutex, RT_WAITING_FOREVER);
    if (ABS(steer_error) > 200)
    {
        computed = wheel_diff_p * steer_error * velocity;
    }
    else
    {
        computed = 0;
    }

    left_speed = velocity + computed;
    right_speed = velocity - computed;

    wheel_set_speed(left_wheel, left_speed);
    wheel_set_speed(right_wheel, right_speed);

    rt_mutex_release(chassis_dmutex);
    return RT_EOK;
}

int set_chassis_pid_param(float kp, float ki, float kd)
{
    if (left_wheel == RT_NULL || right_wheel == RT_NULL)
    {
        return RT_ERROR;
    }
    rt_mutex_take(chassis_dmutex, RT_WAITING_FOREVER);
    controller_set_param(left_wheel->w_controller, kp, ki, kd);
    controller_set_param(right_wheel->w_controller, kp, ki, kd);
    rt_mutex_release(chassis_dmutex);
    return RT_EOK;
}

int set_chassis_debug_cmd(int argc, char **argv)
{
    if (argc == 1)
    {
        return RT_ERROR;
    }
    is_chassis_debug = str2bool(argv[1]);

    return RT_EOK;
}
MSH_CMD_EXPORT(set_chassis_debug_cmd, set_chassis_debug_cmd 0 or 1);

int chassis_thread_init_cmd()
{
    rt_thread_t tid_chassis = rt_thread_create("chassis_thread",
                                               chassis_thread, RT_NULL,
                                               2048, 30, 5);

    rt_thread_startup(tid_chassis);

    return RT_EOK;
}
MSH_CMD_EXPORT(chassis_thread_init_cmd, chassis_thread_init_cmd Kp Ki kd);

int set_chassis_pid_param_cmd(int argc, char **argv)
{
    wheel_kp = myatof(argv[1]);
    wheel_ki = myatof(argv[2]);
    wheel_kd = myatof(argv[3]);
    wheel_diff_p = myatof(argv[4]);

    set_chassis_pid_param(wheel_kp, wheel_ki, wheel_kd);
    return RT_EOK;
}
MSH_CMD_EXPORT(set_chassis_pid_param_cmd, set_chassis_pid_param_cmd Kp Ki Kd wheel_diff_p)

int set_velocity_cmd(int argc, char **argv)
{
    if (argc == 1)
    {
        return RT_ERROR;
    }
    set_velocity(myatof(argv[1]));
    return RT_EOK;
}
MSH_CMD_EXPORT(set_velocity_cmd, set_velocity 0.5(m / s));

int set_chassis_enable_cmd(int argc, char **argv)
{
    if (argc == 1)
    {
        return RT_ERROR;
    }
    is_chassis_running = myatof(argv[1]);
    return RT_EOK;
}
MSH_CMD_EXPORT(set_chassis_enable_cmd, set_chassis_enable_cmd 0 or 1);
