#include "headfile.h"
#include "imageProcess.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

int str2bool(char *str)
{
    if (strcmp(str, "0") == 0)
    {
        return RT_FALSE;
    }
    else
    {
        return RT_TRUE;
    }
}

double myatof(const char *str)
{
    double s = 0.0;

    double d = 10.0;
    int jishu = 0;

    bool falg = false;

    while (*str == ' ')
    {
        str++;
    }

    if (*str == '-') //记录数字正负
    {
        falg = true;
        str++;
    }

    if (!(*str >= '0' && *str <= '9')) //假设一開始非数字则退出。返回0.0
        return s;

    while (*str >= '0' && *str <= '9' && *str != '.') //计算小数点前整数部分
    {
        s = s * 10.0 + *str - '0';
        str++;
    }

    if (*str == '.') //以后为小树部分
        str++;

    while (*str >= '0' && *str <= '9') //计算小数部分
    {
        s = s + (*str - '0') / d;
        d *= 10.0;
        str++;
    }

    if (*str == 'e' || *str == 'E') //考虑科学计数法
    {
        str++;
        if (*str == '+')
        {
            str++;
            while (*str >= '0' && *str <= '9')
            {
                jishu = jishu * 10 + *str - '0';
                str++;
            }
            while (jishu > 0)
            {
                s *= 10;
                jishu--;
            }
        }
        if (*str == '-')
        {
            str++;
            while (*str >= '0' && *str <= '9')
            {
                jishu = jishu * 10 + *str - '0';
                str++;
            }
            while (jishu > 0)
            {
                s /= 10;
                jishu--;
            }
        }
    }

    return s * (falg ? -1.0 : 1.0);
}

int limit_loop(int x, int a, int b)
{
    int i = 0;
    i = x + 1;
    if (i > b)
        i = i % (b - a + 1);

    return i;
}

int limit_int(int x, int y)
{
    if (x > y)
        return y;
    else if (x < -y)
        return -y;
    else
        return x;
}

float limit_float(float x, float y)
{
    if (x > y)
        return y;
    else if (x < -y)
        return -y;
    else
        return x;
}

int limit_ab_int(int x, int a, int b)
{
    if (x < a)
        x = a;
    if (x > b)
        x = b;
    return x;
}

float limit_ab_float(float x, float a, float b)
{
    if (x < a)
        x = a;
    if (x > b)
        x = b;
    return x;
}

void reboot(void)
{
    NVIC_SystemReset();
}
MSH_CMD_EXPORT(reboot, reset system);

void upload_my_bin()
{
    rt_kprintf("START");
    for (int i = 0; i < img_height; i++)
    {
        for (int j = 0; j < img_width; j++)
        {
            if (BinImage[i][j] != 0)
            {
                uart_putchar(USART_1, 0xFF);
            }
            else
            {
                uart_putchar(USART_1, 0x00);
            }
        }
    }
    rt_kprintf("END\n");
}
MSH_CMD_EXPORT(upload_my_bin, upload_my_bin);

void upload_my_gray()
{
    rt_kprintf("START");
    for (int i = 0; i < img_height; i++)
    {
        for (int j = 0; j < img_width; j++)
        {
            uart_putchar(USART_1, mt9v03x_csi_image[i][j]);
        }
    }
    rt_kprintf("END\n");
}
MSH_CMD_EXPORT(upload_my_gray, upload_my_gray);

//zf upload
void upload_zf_bin()
{

    uart_putchar(USART_1, 0x00);
    uart_putchar(USART_1, 0xff);
    uart_putchar(USART_1, 0x01);
    uart_putchar(USART_1, 0x01);
    for (int i = 0; i < img_height; i++)
    {
        for (int j = 0; j < img_width; j++)
        {
            if (BinImage[i][j] != 0)
            {
                uart_putchar(USART_1, 0xFF);
            }
            else
            {
                uart_putchar(USART_1, 0x00);
            }
        }
    }
}
MSH_CMD_EXPORT(upload_zf_bin, upload_zf_bin);

void upload_zf_gray()
{

    uart_putchar(USART_1, 0x00);
    uart_putchar(USART_1, 0xff);
    uart_putchar(USART_1, 0x01);
    uart_putchar(USART_1, 0x01);
    for (int i = 0; i < img_height; i++)
    {
        for (int j = 0; j < img_width; j++)
        {
            uart_putchar(USART_1, mt9v03x_csi_image[i][j]);
        }
    }
}
MSH_CMD_EXPORT(upload_zf_gray, upload_zf_gray);

#include "findLines.h"
void test_road_width()
{
    for (int i = 0; i < 120; i++)
    {
        int w = f1.rightline[i] - f1.leftline[i];
        rt_kprintf("%d,", w / 2);
    }
}
MSH_CMD_EXPORT(test_road_width, test_road_width);
