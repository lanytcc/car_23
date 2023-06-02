/*********************************************************************************************************************
 * @file            hf_pit.h
 * @brief           PIT配置
 * @author          Fantastic Potato
 * @version         1.0
 * @Target core     CH32V307VCT6
 * @date            2022-3-17
 ********************************************************************************************************************/

#ifndef HF_PIT_H_
#define HF_PIT_H_

#include "hf_nvic.h"
#include "hf_tim.h"


/* 函数声明 */

/* 外部函数，用户可自行调用 */
/* 定时器周期中断(ms) */
void timer_pit_interrupt_ms(TIMERN_enum TIMER_x, uint16 ms);
/* 定时器周期中断(us) */
void timer_pit_interrupt_us(TIMERN_enum TIMER_x, uint16 us);
/* 关闭定时器 */
void timer_pit_close(TIMERN_enum TIMER_x);

#endif /* HF_PIT_H_ */
