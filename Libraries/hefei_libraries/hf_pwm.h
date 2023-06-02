/*********************************************************************************************************************
 * @file            hf_pwm.h
 * @brief           PWM配置
 * @author          Fantastic Potato
 * @version         1.0
 * @Target core     CH32V307VCT6
 * @date            2022-3-11
 ********************************************************************************************************************/

#ifndef HF_PWM_H_
#define HF_PWM_H_

#include "ch32v30x_rcc.h"
#include "ch32v30x_gpio.h"
#include "ch32v30x_tim.h"
#include "hf_gpio.h"
#include "hf_tim.h"

/* 宏定义 */
#define PWM_DUTY_MAX 10000

/* 枚举类型定义 */
/* 枚举PWM通道 */
typedef enum
{
    //同一个PWMP模块不同通道只能输出相同频率的PWM，占空比可设置不同
    //例如PWM1_CH1_PA8与PWM1_CH3_PA10属于同一个子模块，频率只能一样，但是占空比可以不一样

    //----------------PWM1----------------
    //以下为TIM1的PWM的一组通道，不允许通道混用。
    //例如：PWM1_PWM_CH2_PA9与PWM1_PWM_CH1N_PB13不能同时使用。
    TIM1_PWM_CH1_A8 = 0x0000,
    TIM1_PWM_CH2_A9,
    TIM1_PWM_CH3_A10,
    TIM1_PWM_CH4_A11,

    //重映射
    TIM1_PWM_CH1_E9 = 0x0010,
    TIM1_PWM_CH2_E11,
    TIM1_PWM_CH3_E13,
    TIM1_PWM_CH4_E14,

    //----------------PWM2----------------
    //以下为TIM2的PWM的一组通道，不允许通道混用。
    //例如：PWM2_PWM_CH1_A15与PWM2_PWM_CH3_A2不能同时使用。
    TIM2_PWM_CH1_A0 = 0x0100,
    TIM2_PWM_CH2_A1,
    TIM2_PWM_CH3_A2,
    TIM2_PWM_CH4_A3,

    //以下为TIM2的PWM的一组通道，不允许通道混用。
    TIM2_PWM_CH1_A15 = 0x0110,     //完全映射
    TIM2_PWM_CH2_B3,               //完全映射
    TIM2_PWM_CH3_B10,              //完全映射,下载器串口
    TIM2_PWM_CH4_B11,              //完全映射,下载器串口

    //----------------PWM3----------------
    ///以下为TIM3的PWM的一组通道，不允许通道混用。
    //例如：PWM3_PWM_CH1_PA6与PWM3_PWM_CH4_C9不能同时使用。
    TIM3_PWM_CH1_A6 = 0x0200,
    TIM3_PWM_CH2_A7,
    TIM3_PWM_CH3_B0,
    TIM3_PWM_CH4_B1,

    //以下为TIM3的PWM的一组通道，不允许通道混用。
    TIM3_PWM_CH1_C6 = 0x0210,      //完全映射
    TIM3_PWM_CH2_C7,               //完全映射
    TIM3_PWM_CH3_C8,               //完全映射
    TIM3_PWM_CH4_C9,               //完全映射

    //以下为TIM3的PWM的一组通道，不允许通道混用。
    TIM3_PWM_CH1_B4 = 0x0220,      //部分映射
    TIM3_PWM_CH2_B5,               //部分映射

    //----------------PWM4----------------
    //以下为TIM4的PWM的一组通道。
    TIM4_PWM_CH1_B6 = 0x0300,
    TIM4_PWM_CH2_B7,
    TIM4_PWM_CH3_B8,
    TIM4_PWM_CH4_B9,

    //重映射
    TIM4_PWM_CH1_D12 = 0x0310,
    TIM4_PWM_CH2_D13,
    TIM4_PWM_CH3_D14,
    TIM4_PWM_CH4_D15,


    //定时器5
    TIM5_PWM_CH1_A0 = 0x0400,
    TIM5_PWM_CH2_A1,
    TIM5_PWM_CH3_A2,
    TIM5_PWM_CH4_A3,

    //定时器8
    TIM8_PWM_CH1_C6 = 0x0700,
    TIM8_PWM_CH2_C7,
    TIM8_PWM_CH3_C8,
    TIM8_PWM_CH4_C9,

    //重映射
    TIM8_PWM_CH1_B6 = 0x0710,
    TIM8_PWM_CH2_B7,
    TIM8_PWM_CH3_B8,
    TIM8_PWM_CH4_C13,

    //定时器9
    TIM9_PWM_CH1_A2 = 0x0800,
    TIM9_PWM_CH2_A3,
    TIM9_PWM_CH3_A4,
    TIM9_PWM_CH4_C4,

    //重映射
    TIM9_PWM_CH1_D9 = 0x0810,
    TIM9_PWM_CH2_D11,
    TIM9_PWM_CH3_D13,
    TIM9_PWM_CH4_D15,

    //定时器10
    TIM10_PWM_CH1_B8 = 0x0900,
    TIM10_PWM_CH2_B9,
    TIM10_PWM_CH3_C3,
    TIM10_PWM_CH4_C11,

    //重定向一
    TIM10_PWM_CH1_D1 = 0x0910,
    TIM10_PWM_CH2_D3,
    TIM10_PWM_CH3_D5,
    TIM10_PWM_CH4_D7,

    //重定向二
    TIM10_PWM_CH1_B3 = 0x0920,
    TIM10_PWM_CH2_B4,
    TIM10_PWM_CH3_B5,
    TIM10_PWM_CH4_C15,


}PWMCH_enum;

/* 函数声明 */

/* 外部函数，用户可自行调用 */
/* PWM初始化 */
void pwm_init(PWMCH_enum pwmch, uint32 freq, uint32 duty, uint16_t TIM_OCPreload_Enable_Disable);
/* PWM占空比设定 */
void pwm_duty(PWMCH_enum pwmch, uint32 duty);
/* PWM频率设定 */
void pwm_freq(PWMCH_enum pwmch, uint32 freq,uint32_t duty);
/* PWM停止输出 */
void pwm_stop(PWMCH_enum pwmch);

/* 内部函数，用户无需关注 */
/* PWM引脚初始化 */
void pwm_gpio_init(PWMCH_enum pwmch);

#endif /* HF_PWM_H_ */
