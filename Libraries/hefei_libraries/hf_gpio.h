/*********************************************************************************************************************
 * @file            hf_gpio.h
 * @brief           GPIO配置
 * @author          Fantastic Potato
 * @version         1.0
 * @Target core     CH32V307VCT6
 * @date            2022-3-8
 ********************************************************************************************************************/

#ifndef HF_GPIO_H_
#define HF_GPIO_H_

#include "ch32v30x_gpio.h"
#include "ch32v30x_rcc.h"
#include "ch32v30x_exti.h"
#include "common.h"


/* 宏定义 */
#define GPIOx_BASE      GPIOA_BASE//偏移0x0400
#define GPIO_PIN_CONFIG (SPEED_50MHZ | OUT_PP)              //宏定义GPIO引脚的默认配置，便于初始化GPIO时快速填写参数，如果需要其他参数可自行修改
#define GPIO_INT_CONFIG (IN_PULLUP)                         //宏定义GPIO中断引脚的默认配置，便于初始化GPIO中断时快速填写参数，如果需要其他参数可自行修改

/* GPIO地址 */
#define gpio_odr_addr(X)        (0x4004000C + ((X&0xf0)>>5)*0x400 + ((X&0x0f)/8))

/* GPIO复位 */
#define gpio_low(x)             ((GPIO_TypeDef*)GPION[(x>>5)])->BCR   = (uint16_t)(1 << (x & 0x0F))

/* GPIO置位 */
#define gpio_high(x)            ((GPIO_TypeDef*)GPION[(x>>5)])->BSHR  = (uint16_t)(1 << (x & 0x0F))

/* 中断标志位清除    */
#define CLEAR_GPIO_FLAG(pin) GPIO_ClearPinsInterruptFlags(PORTPTR[pin>>5], 1<<(pin&0x1f));

/* 引用外部变量 */
extern  const uint32_t GPION[5];

/* 枚举定义声明 */
/* 枚举GPIO端口 */
typedef enum                                                                    // 枚举 GPIO 引脚  此枚举定义不允许用户修改
{
    A0 = 32* 0, A1 ,    A2 ,    A3 ,    A4 ,    A5 ,    A6 ,    A7 ,
    A8 ,        A9 ,    A10,    A11,    A12,    A13,    A14,    A15,

    B0 = 32* 1, B1 ,    B2 ,    B3 ,    B4 ,    B5 ,    B6 ,    B7 ,
    B8 ,        B9 ,    B10,    B11,    B12,    B13,    B14,    B15,

    C0 = 32* 2, C1 ,    C2 ,    C3 ,    C4 ,    C5 ,    C6 ,    C7 ,
    C8 ,        C9 ,    C10,    C11,    C12,    C13,    C14,    C15,

    D0 = 32* 3, D1 ,    D2 ,    D3 ,    D4 ,    D5 ,    D6 ,    D7 ,
    D8 ,        D9 ,    D10,    D11,    D12,    D13,    D14,    D15,

    E0 = 32* 4, E1 ,    E2 ,    E3 ,    E4 ,    E5 ,    E6 ,    E7 ,
    E8 ,        E9 ,    E10,    E11,    E12,    E13,    E14,    E15,

    SPI_CS_NULL = 0xFF,
    PIN_NULL = 0xFF
}gpio_pin_enum;

/* 枚举GPIO端口方向 */
typedef enum
{
    GPI = 0,        //  定义管脚输入方向
    GPO = 1,        //  定义管脚输出方向
}gpio_dir_enum;

/* 枚举GPIO配置 */
typedef enum
{
    IN_AIN = 0x00,              //模拟输入模式
    IN_FLOAT = 0x04,            //浮空输入模式
    IN_PULLDOWN = 0x28,         //下拉输入
    IN_PULLUP = 0x48,           //上拉输入

    OUT_PP = 0x10,              //通用推挽输出模式
    OUT_OD = 0x14,              //通用开漏输出模式
    OUT_AF_PP = 0x18,           //复用功能推挽输出模式
    OUT_AF_OD = 0x1C,           //复用功能开漏输出模式

}gpio_mode_enum;

/* 枚举GPIO输出速度 */
typedef enum
{
    SPEED_10MHZ = (0x01 << 8),
    SPEED_2MHZ  = (0x02 << 8),
    SPEED_50MHZ = (0x03 << 8)
}gpio_speed_enum;

/* 枚举GPIO输出电平 */
typedef enum
{
    GPIO_LOW  = 0,  //  低电平
    GPIO_HIGH = 1,  //  高电平
}gpio_level_enum;


/* 函数声明 */
/* 外部函数，用户可自行调用 */
/* GPIO初始化 */
void gpio_init(gpio_pin_enum gpio_pin, gpio_dir_enum gpio_dir, gpio_level_enum dat, uint32_t pinconf);
/* GPIO输出设置 */
void gpio_bit_output(gpio_pin_enum pin, uint8_t dat);
/* GPIO状态获取状态 */
uint8 gpio_get_input(gpio_pin_enum pin);
uint8 gpio_get_output(gpio_pin_enum pin);
/* GPIO 翻转 */
void gpio_toggle(gpio_pin_enum pin);


#endif /* HF_GPIO_H_ */
