/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		headfile
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		��ɿƼ�(QQ3184284598)
 * @version    		�鿴doc��version�ļ� �汾˵��
 * @Software 		IAR 8.3 or MDK 5.28
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 ********************************************************************************************************************/

#ifndef _headfile_h
#define _headfile_h

#include <stdint.h>
#include "fsl_common.h"

#include "fsl_debug_console.h"
#include "fsl_iomuxc.h"
#include "fsl_pit.h"

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_cache.h"
#include "common.h"
#include "zf_vector.h"

//------�ļ�ϵͳ���ͷ�ļ�
#include "ff.h"
#include "diskio.h"
#include "fsl_sd_disk.h"

#include "SEEKFREE_PRINTF.h"

//------��ɿƼ���Ƭ����������ͷ�ļ�
#include "zf_gpio.h"
#include "zf_iomuxc.h"
#include "zf_pit.h"
#include "zf_pwm.h"
#include "zf_uart.h"
#include "zf_spi.h"
#include "zf_systick.h"
#include "zf_qtimer.h"
#include "zf_adc.h"
#include "zf_iic.h"
#include "zf_flash.h"
#include "zf_camera.h"
#include "zf_csi.h"
#include "zf_rom_api.h"
#include "zf_usb_cdc.h"
#include "zf_sdcard.h"

//------RTTͷ�ļ�
#include "rtthread.h"

//------��ɿƼ���Ʒ����ͷ�ļ�
#include "SEEKFREE_FONT.h"
#include "SEEKFREE_18TFT.h"
#include "SEEKFREE_IPS114_SPI.h"
#include "SEEKFREE_IIC.h"
#include "SEEKFREE_VIRSCO.h"
#include "SEEKFREE_MPU6050.h"
#include "SEEKFREE_NRF24L01.h"
#include "SEEKFREE_MMA8451.h"
#include "SEEKFREE_L3G4200D.h"
#include "SEEKFREE_ICM20602.h"
#include "SEEKFREE_MT9V03X.h"
#include "SEEKFREE_WIRELESS.h"
#include "SEEKFREE_MT9V03X_CSI.h"
#include "SEEKFREE_W25QXXJV.h"

#include "util.h"
#include "servo.h"
#include "gpio_port.h"
#include "easyblink.h"
#include "agile_button.h"
// #include "imageProcess.h"
// #include "buttonThread.h"
// #include "chassisThread.h"
// #include "findLines.h"
// #include "distinguish.h"

#include "motor.h"
#include "wheel.h"
#include "encoder.h"
#include "controller.h"
#include "pos_pid_controller.h"
#include "inc_pid_controller.h"

#define img_width MT9V03X_CSI_W
#define img_height MT9V03X_CSI_H

#define white_point 255
#define black_point 0

#define Image BinImage
extern ebled_t errno_led;
extern ebled_t info_beep;

#endif
