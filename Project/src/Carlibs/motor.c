#include "util.h"
#include "motor.h"
#include "zf_pwm.h"

#define DBG_SECTION_NAME "motor"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>

motor_t motor_create(int index)
{
    motor_t new_motor = (motor_t)rt_malloc(sizeof(struct motor));
    new_motor->index = index;

    switch (index)
    {
    case 1:
        pwm_init(PWM1_MODULE3_CHA_D0, 20000, 0);
        pwm_init(PWM1_MODULE3_CHB_D1, 20000, 0);
        break;
    case 2:
        pwm_init(PWM2_MODULE3_CHA_D2, 20000, 0);
        pwm_init(PWM2_MODULE3_CHB_D3, 20000, 0);
        break;
    default:
        break;
    }

    return new_motor;
}

rt_err_t motor_run(motor_t mot, int thousands)
{
    RT_ASSERT(mot != RT_NULL);

    thousands = 10 * limit_int(thousands, 1000);

    switch (mot->index)
    {
    case 1:
        if (thousands >= 0)
        {
            pwm_duty(PWM1_MODULE3_CHA_D0, thousands);
            pwm_duty(PWM1_MODULE3_CHB_D1, 0);
        }
        else
        {
            thousands = -thousands;
            pwm_duty(PWM1_MODULE3_CHA_D0, 0);
            pwm_duty(PWM1_MODULE3_CHB_D1, thousands);
        }

        break;
    case 2:
        if (thousands >= 0)
        {
            pwm_duty(PWM2_MODULE3_CHA_D2, thousands);
            pwm_duty(PWM2_MODULE3_CHB_D3, 0);
        }
        else
        {
            thousands = -thousands;
            pwm_duty(PWM2_MODULE3_CHA_D2, 0);
            pwm_duty(PWM2_MODULE3_CHB_D3, thousands);
        }

        break;
    default:
        break;
    }

    return RT_EOK;
}