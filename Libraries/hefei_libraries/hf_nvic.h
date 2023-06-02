/*********************************************************************************************************************
 * @file            hf_nvic.h
 * @brief           NVIC配置
 * @author          Fantastic Potato
 * @version         1.0
 * @Target core     CH32V307VCT6
 * @date            2022-3-4
 ********************************************************************************************************************/

#ifndef HF_NVIC_H_
#define HF_NVIC_H_

#include "common.h"
#include "stdint.h"
#include "ch32v30x.h"
#include "ch32v30x_misc.h"


/* 函数声明*/

/* 外部函数，用户可自行调用 */
/* 中断初始化 */
void        interrupt_init              (void);
/* 打开总中断 */
void        interrupt_global_enable     (void);
/* 关闭总中断 */
void        interrupt_global_disable    (void);
/* 使能中断 */
void        interrupt_enable            (IRQn_Type irqn);
/* 失能中断 */
void        interrupt_disable           (IRQn_Type irqn);
/* 设置中断优先级 */
void        interrupt_set_priority      (IRQn_Type irqn, uint8_t priority);

void nvic_init(IRQn_Type irqn, uint8_t preemption_priority, uint8_t sub_priority, uint8_t status);

#endif /* HF_NVIC_H_ */
