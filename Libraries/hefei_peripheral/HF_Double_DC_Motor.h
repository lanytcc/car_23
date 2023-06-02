
/*********************************************************************************************************************
* @file            HF_L9110S_2.h
* @author
* @version         1.0
* @Target core     CH32V307VCT6
* @date            20220905
********************************************************************************************************************/

#ifndef HF_DOUBLE_DC_MOTOR_H_
#define HF_DOUBLE_DC_MOTOR_H_


#include "headfile.h"

/* 资源定义 */
#define R_PWM_F        TIM2_PWM_CH2_A1
#define R_PWM_B        TIM2_PWM_CH1_A0
#define L_PWM_F        TIM3_PWM_CH4_B1
#define L_PWM_B        TIM3_PWM_CH3_B0

/* 类型定义 */
typedef enum
{
    left,
    right
}left_or_right;

//函数定义
/* 单电机初始化 */
void motor_init(left_or_right object);
/* 电机正转 */
void motor_forward(left_or_right object, uint16_t value);
/* 电机反转 */
void motor_back(left_or_right object, uint16_t value);

#endif /* HF_DOUBLE_DC_MOTOR_H_ */
