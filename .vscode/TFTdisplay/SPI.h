#ifndef _SPI_H_
#define _SPI_H_
#include <rtthread.h>
#include <drv_gpio.h>

// TFTSPI_CS  ------ 管脚D11
// TFTSPI_SCK ------ 管脚D12
// TFTSPI_SDI ------ 管脚E12
// TFTSPI_DC  ------ 管脚E11
// TFTSPI_RST ------ 管脚B13

#define SPI_MOSI_SET rt_pin_write(E12, PIN_HIGH)
#define SPI_SCLK_SET rt_pin_write(D12, PIN_HIGH)

#define SPI_MOSI_CLR rt_pin_write(E12, PIN_LOW)
#define SPI_SCLK_CLR rt_pin_write(D12, PIN_LOW)

#define LCD_CS_SET rt_pin_write(D11, PIN_HIGH)
#define LCD_RS_SET rt_pin_write(E11, PIN_HIGH)
#define LCD_RST_SET rt_pin_write(B13, PIN_HIGH)

#define LCD_CS_CLR rt_pin_write(D11, PIN_LOW)
#define LCD_RS_CLR rt_pin_write(E11, PIN_LOW)
#define LCD_RST_CLR rt_pin_write(B13, PIN_LOW)

typedef rt_uint16_t u16;
typedef rt_uint8_t u8;
typedef rt_uint32_t u32;

#define delay_ms(x) rt_thread_mdelay(x)

void SPIv_WriteData(u8 Data);
void SPIv_Init(void);

#endif
