/*********************************************************************************************************************
 * @file            hf_exti.c
 * @brief           EXTI配置
 * @author          Fantastic Potato
 * @version         1.0
 * @Target core     CH32V307VCT6
 * @date            2022-3-27
 ********************************************************************************************************************/

#include "hf_exti.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief        开启引脚外部中断
// @param        pin         选择的引脚 (可选择范围由 common.h 内PIN_enum枚举值确定)
// @param        exti_mode              中断的模式，只能选择以下参数：
//  @arg          CHANGE:                电平改变触发
//  @arg          RISING:                上升沿触发
//  @arg          FALLING:               下降沿触发
// @return       void
// Sample usage:            exti_enable(EXTI_Line0,A0, RISING);//开启PA0外部中断，模式为电平上升沿触发
                            //注：无法同时开启同一引脚数的外部中断（如PA0和PB0）;;;用于中断的IO需初始化为输入模式
//-------------------------------------------------------------------------------------------------------------------
void exti_enable(uint32 exti_line_num, gpio_pin_enum pin,TRIGGER_enum trigger)
{
    //GPIO 外部中断初始化

    EXTI_InitTypeDef EXTI_InitStructure={0};  //EXTI初始化结构体
//    NVIC_InitTypeDef NVIC_InitStructure={0};  //NVIC初始化结构体
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);  //使能AFIO总线时钟

    gpio_init(pin, GPI, 1, IN_PULLUP);
    GPIO_EXTILineConfig(pin >> 5, pin & 0x1F);


    EXTI_InitStructure.EXTI_Line= 1 << (pin & 0x1F);     //配置EXTI结构体
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = trigger;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    if((pin&0x1F) < 1)
       interrupt_enable(EXTI0_IRQn);                                             // 使能 Line0 的中断响应
    else if((pin&0x1F) < 2)
       interrupt_enable(EXTI1_IRQn);                                             // 使能 Line1 的中断响应
    else if((pin&0x1F) < 3)
       interrupt_enable(EXTI2_IRQn);                                             // 使能 Line2 的中断响应
    else if((pin&0x1F) < 4)
       interrupt_enable(EXTI3_IRQn);                                             // 使能 Line3 的中断响应
    else if((pin&0x1F) < 5)
       interrupt_enable(EXTI4_IRQn);                                             // 使能 Line4 的中断响应
    else if((pin&0x1F) < 10)
       interrupt_enable(EXTI9_5_IRQn);                                           // 使能 Line5-9 的中断响应
    else
       interrupt_enable(EXTI15_10_IRQn);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief        屏蔽引脚外部中断
// @param        pin         选择的引脚 (可选择范围由 common.h 内PIN_enum枚举值确定)
// @return       void
// Sample usage:        exti_disable(A0);
                        //关闭引脚PX0的外部中断（X=A、B、C、F）
//-------------------------------------------------------------------------------------------------------------------
void exti_disable(gpio_pin_enum pin)
{
    EXTI->INTENR &= ~(1 << (pin & 0x1F));
}

