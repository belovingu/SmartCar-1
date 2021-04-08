#include "headfile.h"
#include <encoder.h>

#define DBG_SECTION_NAME "encoder"
#define DBG_LEVEL DBG_LOG
#include <rtdbg.h>

#define ENCODER1_QTIMER QTIMER_1
#define ENCODER1_A QTIMER1_TIMER0_C0
#define ENCODER1_B QTIMER1_TIMER1_C1

#define ENCODER2_QTIMER QTIMER_1
#define ENCODER2_A QTIMER1_TIMER2_C2
#define ENCODER2_B QTIMER1_TIMER3_C24

//encoder:30, motor:68
#define PULSE_REVOL (512 * 68 / 30)

encoder_t encoder_create(int index, int sample_time)
{
    encoder_t enc = (encoder_t)rt_malloc(sizeof(struct encoder));

    pit_start(PIT_CH1);
    pit_start(PIT_CH2);
    enc->index = index;
    enc->sample_time = sample_time;
    enc->rpm = 0;

    switch (enc->index)
    {
    case 1:
        qtimer_quad_init(QTIMER_1, QTIMER1_TIMER0_C0, QTIMER1_TIMER1_C1);
        pit_start(PIT_CH1);
        break;
    case 2:
        qtimer_quad_init(QTIMER_1, QTIMER1_TIMER2_C2, QTIMER1_TIMER3_C24);
        pit_start(PIT_CH2);
        break;

    default:
        break;
    }

    return enc;
}

float encoder_measure_rpm(encoder_t enc)
{
    RT_ASSERT(enc != RT_NULL);
    float encoder_pul_num;

    if (rt_tick_get() - enc->last_time < rt_tick_from_millisecond(enc->sample_time))
    {
        LOG_W("waiting for encoder sample time");
        return RT_EOK;
    }

    switch (enc->index)
    {
    case 1:
        encoder_pul_num = (float)qtimer_quad_get(QTIMER_1, QTIMER1_TIMER0_C0);
        qtimer_quad_clear(QTIMER_1, QTIMER1_TIMER0_C0);
        enc->rpm = -encoder_pul_num * 1000 * 60 / pit_get_ms(PIT_CH1) / PULSE_REVOL;
        pit_close(PIT_CH1);
        pit_start(PIT_CH1);
        break;
    case 2:
        encoder_pul_num = (float)qtimer_quad_get(QTIMER_1, QTIMER1_TIMER2_C2);
        qtimer_quad_clear(QTIMER_1, QTIMER1_TIMER2_C2);
        enc->rpm = encoder_pul_num * 1000 * 60 / pit_get_ms(PIT_CH2) / PULSE_REVOL;
        pit_close(PIT_CH2);
        pit_start(PIT_CH2);
        break;
    default:
        break;
    }
    return enc->rpm;
}
