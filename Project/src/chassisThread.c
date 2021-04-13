#include "headfile.h"

#include "inc_pid_controller.h"
#include "pos_pid_controller.h"
#include "encoder.h"
#include "wheel.h"
#include "motor.h"

#include "util.h"
#include "chassisThread.h"
#include "buttonThread.h"

wheel_t left_wheel = RT_NULL;
wheel_t right_wheel = RT_NULL;
rt_bool_t is_chassis_debug = RT_FALSE;
rt_bool_t is_chassis_running = RT_TRUE;

float wheel_kp = 0.4;
float wheel_ki = 0.1;
float wheel_kd = 0;
float wheel_diff_p = 0.2;
float speed_now[5] = {0};

void chassis_thread(void *parameter)
{
    // 1.1 Create two motors
    motor_t left_motor = motor_create(1);
    motor_t right_motor = motor_create(2);

    // 1.2 Create two encoders
    encoder_t left_encoder = encoder_create(1, 30);
    encoder_t right_encoder = encoder_create(2, 30);

    // 1.3 Create two pid contollers
    inc_pid_controller_t left_pid = inc_pid_controller_create(wheel_kp, wheel_ki, wheel_kd,
                                                              WHEEL_PID_MAX_OUT,
                                                              -WHEEL_PID_MAX_OUT,
                                                              30);
    inc_pid_controller_t right_pid = inc_pid_controller_create(wheel_kp, wheel_ki, wheel_kd,
                                                               WHEEL_PID_MAX_OUT,
                                                               -WHEEL_PID_MAX_OUT,
                                                               30);

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
        if (SWITCH_ON == switch_get(2))
        {
            wheel_update(left_wheel);
            wheel_update(right_wheel);
        }
        else
        {
            motor_run(left_wheel->w_motor, 0);
            motor_run(right_wheel->w_motor, 0);
        }

        for (int i = 0; i < 5; i++)
        {
            speed_now[i] = (left_wheel->speed_now[i] + right_wheel->speed_now[i]) / 2;
        }

        if (is_chassis_debug)
        {
            rt_kprintf("wheel:%d,%d,%d,%d\r\n",
                       (int)left_wheel->rpm, (int)left_wheel->w_controller->output,
                       (int)right_wheel->rpm, (int)right_wheel->w_controller->output);
        }
        rt_thread_mdelay(50);
    }
}

//paramset
int set_velocity(float velocity, int steer_error)
{
    if (left_wheel == RT_NULL || right_wheel == RT_NULL)
    {
        return RT_ERROR;
    }

    float left_speed = 0, right_speed = 0, computed = 0;

    computed = ABS(steer_error) > 10 ? wheel_diff_p * steer_error * velocity : 0;

    left_speed = velocity + computed;
    right_speed = velocity - computed;

    wheel_set_speed(left_wheel, left_speed);
    wheel_set_speed(right_wheel, right_speed);

    return RT_EOK;
}

int set_chassis_pid_param(float kp, float ki, float kd)
{
    if (left_wheel == RT_NULL || right_wheel == RT_NULL)
    {
        return RT_ERROR;
    }
    controller_set_param(left_wheel->w_controller, kp, ki, kd);
    controller_set_param(right_wheel->w_controller, kp, ki, kd);
    return RT_EOK;
}

//cmd
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
    // rt_timer_t timer = rt_timer_create("chassis_timer", chassis_thread, RT_NULL, 50, RT_TIMER_FLAG_PERIODIC);
    // if (RT_NULL != timer)
    // {
    //     rt_timer_start(timer);
    // }
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
    set_velocity(myatof(argv[1]), 0);
    return RT_EOK;
}
MSH_CMD_EXPORT(set_velocity_cmd, set_velocity 500(cm / s));

int set_chassis_enable_cmd(int argc, char **argv)
{
    if (argc == 1)
    {
        return RT_ERROR;
    }
    is_chassis_running = str2bool(argv[1]);
    return RT_EOK;
}
MSH_CMD_EXPORT(set_chassis_enable_cmd, set_chassis_enable_cmd 0 or 1);
