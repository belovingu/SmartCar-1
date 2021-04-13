#ifndef __TCHASIS__
#define __TCHASIS__
#include "headfile.h"

#define WHEEL_PID_MAX_OUT 1000
#define WHEEL_RADIUS 0.066 / 2 /* 半径0.033m,周长0.2m, 66mm=6.6cm=0.066m */
#define CAR_WIDTH 0.155        /* 155mm=15.5cm=0.155m,200 (n/min) / 60 * 0.20734533 = 0.69 (m/s) */

extern rt_mutex_t chassis_dmutex;
extern rt_bool_t is_chassis_debug;
extern float speed_now[5];

int chassis_thread_init_cmd();
void chassis_thread(void *parameter);
int set_velocity(float velocity, int steer_error);
int set_chassis_pid_param(float kp, float ki, float kd);

#endif