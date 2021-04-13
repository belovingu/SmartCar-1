#include "headfile.h"
#include <stdio.h>

#define REFER_VOLTAGE 3.3
#define CONVERT_BITS (1 << 12)
#define ADC_UPDATE_INTERVAL 3000

extern rt_mutex_t disp_dmutex;

void adc_bat_thread(void *parameters)
{
    char str[30];
    rt_uint32_t value;
    float vol;

    lcd_init();
    lcd_clear(WHITE);
    TFT_PENCOLOR = BLACK;
    lcd_showstr(0, 0, "lithiumice");
    adc_init(ADC_1, ADC1_CH3_B14, ADC_12BIT);

    while (1)
    {
        value = adc_mean_filter(ADC_1, ADC1_CH3_B14, 5);
        vol = (float)value * REFER_VOLTAGE / CONVERT_BITS * 5.7;
        sprintf(str, "BatVoltage:%.2fV", vol);

        TFT_PENCOLOR = BLACK;
        rt_mutex_take(disp_dmutex, RT_WAITING_FOREVER);
        lcd_showstr(0, 0, str);
        rt_mutex_release(disp_dmutex);
        rt_thread_mdelay(ADC_UPDATE_INTERVAL);
    }
}
