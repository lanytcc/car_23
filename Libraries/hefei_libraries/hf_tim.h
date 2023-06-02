/*********************************************************************************************************************
 * @file            hf_tim.h
 * @brief           TIMER调用
 * @author          FengGuorong
 * @version         1.0
 * @Target core     CH32V307VCT6
 * @date            2022-3-18
 ********************************************************************************************************************/


#ifndef HF_TIM_H_
#define HF_TIM_H_

#include "ch32v30x_rcc.h"
#include "ch32v30x_gpio.h"
#include "ch32v30x_tim.h"
#include "ch32v30x_misc.h"

/* 枚举类型声明 */

/* 枚举TIMER */
typedef enum
{
    TIM_1,
    TIM_2,
    TIM_3,
    TIM_4,
    TIM_5,
    TIM_6,
    TIM_7,
    TIM_8,
    TIM_9,
    TIM_10,
}TIMERN_enum;


/* 枚举TIMER的功能 */
typedef enum
{
    TIMER_FUNCTION_INIT = 0,                                                    // 功能未初始化
    TIMER_FUNCTION_TIMER,                                                       // 用作 TIMER 计时
    TIMER_FUNCTION_PIT,                                                         // 用作 PIT
    TIMER_FUNCTION_PWM,                                                         // 用作 PWM
    TIMER_FUNCTION_ENCODER,                                                     // 用作 ENCODER
    TIMER_FUNCTION_CAMERA,                                                      // 用作 CAMERA
}timer_function_enum;



/* 枚举TIMER通道 */
typedef enum
{
    CHANNEL_1 =0,
    CHANNEL_2,
    CHANNEL_3,
    CHANNEL_4,
}TIMERN_CHANNEL_enum;

/* 函数声明 */

/* 外部函数，用户可自行调用 */
/* 时基初始化 */
void timbase_init(TIMERN_enum TIMER_x, uint16_t arr, uint16_t pre);
/* 外设确认功能状态 */
uint8_t     timer_funciton_check    (TIMERN_enum index, timer_function_enum mode);


#endif /* HF_TIM_H_ */
