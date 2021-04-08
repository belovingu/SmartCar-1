#ifndef __SERVO_H__
#define __SERVO_H__
#include "headfile.h"

#define SERVO1_PIN PWM4_MODULE2_CHA_C30 //定义车模寻迹舵机引脚
#define SERVO2_PIN PWM1_MODULE0_CHA_D12 //定义云台舵机1引脚
#define SERVO3_PIN PWM1_MODULE0_CHB_D13 //定义云台舵机2引脚

#define SERVO1_MID_POS 770
#define SERVO1_HALF_ONE 80 // 770-80, 770, 770+80

#define SERVO2_MID_POS 770
#define SERVO2_HALF_ONE 80 // 770-80, 770, 770+80

#define SERVO3_MID_POS 770
#define SERVO3_HALF_ONE 80 // 770-80, 770, 770+80

void servo_init(void);
rt_err_t servo1_set_angle(int angle);
rt_err_t servo2_set_angle(int angle);
rt_err_t servo3_set_angle(int angle);

#endif //  __SERVO_H__
