#include "servo.h"

#define DBG_SECTION_NAME "servo"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>

void servo_init(void)
{
    pwm_init(SERVO1_PIN, 50, SERVO1_MID_POS);
    pwm_init(SERVO2_PIN, 50, SERVO2_MID_POS);
    pwm_init(SERVO3_PIN, 50, SERVO3_MID_POS);
}

/**
 * @param angle: left -, right +
 * @param angle: -1000 ~ 1000
 */
rt_err_t servo1_set_angle(int angle)
{
    float tmp = (float)limit_int(angle, 1000);
    int set_duty = SERVO1_MID_POS + (int)(tmp / 1000 * SERVO1_HALF_ONE);
    pwm_duty(SERVO1_PIN, set_duty);

    return RT_EOK;
}

/**
 * @param angle: left -, right +
 * @param angle: -1000 ~ 1000
 */
rt_err_t servo2_set_angle(int angle)
{
    float tmp = (float)limit_int(angle, 1000);
    int set_duty = SERVO2_MID_POS + (int)(tmp / 1000 * SERVO2_HALF_ONE);
    pwm_duty(SERVO2_PIN, set_duty);

    return RT_EOK;
}

/**
 * @param angle: left -, right +
 * @param angle: -1000 ~ 1000
 */
rt_err_t servo3_set_angle(int angle)
{
    float tmp = (float)limit_int(angle, 1000);
    int set_duty = SERVO3_MID_POS + (int)(tmp / 1000 * SERVO3_HALF_ONE);
    pwm_duty(SERVO3_PIN, set_duty);

    return RT_EOK;
}