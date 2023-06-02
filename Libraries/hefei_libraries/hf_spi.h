/*********************************************************************************************************************
 * @file            hf_spi.h
 * @brief           硬件SPI配置
 * @author          FengGuorong
 * @version         1.0
 * @Target core     CH32V307VCT6
 * @date            2022-03-12
 ********************************************************************************************************************/


#ifndef HF_SPI_H_
#define HF_SPI_H_

#include "ch32v30x_spi.h"
#include "ch32v30x_rcc.h"
#include "ch32v30x_gpio.h"
#include "common.h"
#include "hf_gpio.h"
#include "stddef.h"

/* 枚举类型定义 */
/* 枚举SPI */
typedef enum
{
    SPI_1 = 0x00,
    SPI_2 = 0x01,
    SPI_3 = 0x02
}SPIN_enum;

/* 枚举SPI通道 */
typedef enum
{
    //----------SPI1----------
    SPI1_SCK_A5 = 0x00,
    SPI1_MISO_A6,
    SPI1_MOSI_A7,

    // 重映射
    SPI1_SCK_B3 = 0x10,
    SPI1_MISO_B4,
    SPI1_MOSI_B5,

    //----------SPI2----------
    SPI2_SCK_B13 = 0x20,
    SPI2_MISO_B14,
    SPI2_MOSI_B15,

    //----------SPI3----------
    SPI3_SCK_B3 = 0x30,
    SPI3_MISO_B4,
    SPI3_MOSI_B5,

    // 重映射
    SPI3_SCK_C10 = 0x40,
    SPI3_MISO_C11,
    SPI3_MOSI_C12,

    // 不需要MISO引脚就用此定义
    SPI_MISO_NULL = 0xFF
}SPI_PIN_enum;

/* 枚举SPI分频系数 */
typedef enum
{
    FPCLK_DIV_2 = 0x00,
    FPCLK_DIV_4,
    FPCLK_DIV_16,
    FPCLK_DIV_32,
    FPCLK_DIV_64,
    FPCLK_DIV_128,
    FPCLK_DIV_256,
}SPI_SPEED_enum;

/* 函数声明 */

/* 外部函数，用户可自行调用 */
/* SPI初始化 */
void spi_init(
        SPIN_enum SPI_x,
        SPI_PIN_enum sck_pin,
        SPI_PIN_enum mosi_pin,
        SPI_PIN_enum miso_pin,
        gpio_pin_enum cs_pin,
        uint8 mode,
        SPI_SPEED_enum BaudRatePrescaler);
/* SPI发送接收函数 */
void spi_mosi(SPIN_enum spi_n, uint8 *modata, uint8 *midata, uint32 len);

/* 内部函数，用户无需关注 */
/* SPI引脚初始化 */
void spi_gpio_init(SPI_PIN_enum sck_pin, SPI_PIN_enum mosi_pin, SPI_PIN_enum miso_pin, gpio_pin_enum cs_pin);
void        spi_write_8bit                  (SPIN_enum SPI_x, uint8 data);
void        spi_write_16bit                 (SPIN_enum SPI_x, uint16 data);

#endif /* HF_SPI_H_ */
