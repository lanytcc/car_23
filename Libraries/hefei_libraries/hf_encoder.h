/*********************************************************************************************************************
 * @file            hf_encoder.h
 * @author          Fantastic Potato
 * @version         1.0
 * @Target core     CH32V307VCT6
 * @date            2022-3-5
 ********************************************************************************************************************/

#ifndef _hf_encoder_h
#define _hf_encoder_h

#include "hf_gpio.h"
#include "hf_tim.h"

/* 枚举编码器引脚 */
typedef enum
{
    TIMER1_CHA_A8,
    TIMER1_CHB_A9,

    TIMER2_CHA_A0,
    TIMER2_CHB_A1,

    TIMER3_CHA_A6,
    TIMER3_CHB_A7,

    TIMER4_CHA_B6,
    TIMER4_CHB_B7,

}ENCODER_PIN_enum;

/* 函数声明 */

/* 外部函数，用户可自行调用  */
/* 编码器初始化 */
void encoder_init(TIMERN_enum TIMER_x, ENCODER_PIN_enum phaseA, ENCODER_PIN_enum phaseB);
/* 获取定时器计数值 */
int encoder_cnt_get(TIMERN_enum TIMER_x);
/* 清除定时器计数值 */
void encoder_cnt_clear(TIMERN_enum TIMER_x);

/* 内部函数，用户无需关注 */
/* 编码器引脚初始化 */
void encoder_gpio_init(ENCODER_PIN_enum phaseA, ENCODER_PIN_enum phaseB);

#endif
