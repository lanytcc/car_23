/*********************************************************************************************************************
 * @file            hf_tim.c
 * @brief           TIMER调用
 * @author          FengGuorong
 * @version         1.0
 * @Target core     CH32V307VCT6
 * @date            2022-3-2
 ********************************************************************************************************************/

#include "hf_tim.h"

/* 内部变量定义 */
const uint32_t TIMERN[] = {TIM1_BASE, TIM2_BASE, TIM3_BASE, TIM4_BASE,TIM5_BASE, TIM8_BASE, TIM9_BASE, TIM10_BASE};

// 该数组禁止修改，内部使用用户无需关心
static timer_function_enum timer_function_state[10] =
{
    TIMER_FUNCTION_INIT, TIMER_FUNCTION_INIT,
    TIMER_FUNCTION_INIT, TIMER_FUNCTION_INIT,
    TIMER_FUNCTION_INIT, TIMER_FUNCTION_INIT,
    TIMER_FUNCTION_INIT, TIMER_FUNCTION_INIT,
    TIMER_FUNCTION_INIT, TIMER_FUNCTION_INIT
};

//-------------------------------------------------------------------------------------------------------------------
//  @brief      时基初始化
//  @param      index     选择定时器(可选择范围由hf_tim.h内TIMERN_enum枚举值确定)
//  @param      arr         周期值
//  @param      pre         分频系数
//  @return     void
//  Sample usage:           timbase_init(TIMER_2, 44000, 1);
//                          //时基初始化为定时器2,周期值为44000,频率为1MHz
//-------------------------------------------------------------------------------------------------------------------
void timbase_init(TIMERN_enum index, uint16_t arr, uint16_t pre)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    if(TIM_1 == index)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);//使能定时器1的时钟
    else  if(TIM_2 == index)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//使能定时器2的时钟
    else  if(TIM_3 == index)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//使能定时器3的时钟
    else  if(TIM_4 == index)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//使能定时器4的时钟
    else  if(TIM_5 == index)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);//使能定时器5的时钟
    else  if(TIM_5 == index)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);//使能定时器6的时钟
    else  if(TIM_5 == index)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//使能定时器7的时钟
    else  if(TIM_8 == index)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);//使能定时器8的时钟
    else  if(TIM_9 == index)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);//使能定时器9的时钟
    else  if(TIM_10== index)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE);//使能定时器10的时钟


    TIM_TimeBaseStructure.TIM_Period = arr;                    //计数44000
    TIM_TimeBaseStructure.TIM_Prescaler = pre;                      //设置为1MHZ计数一次,也就是1us计数一次
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                 //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;             //TIM向上计数模式
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                        //重复计数器设置为0
    TIM_TimeBaseInit((TIM_TypeDef *)TIMERN[index], &TIM_TimeBaseStructure);//根据指定的参数初始化TIMx的时间基数单位
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       TIMER 外设确认功能状态 库内部调用
// @param       index           TIMER 外设模块号
// @param       mode            需要确认的功能模块
// @return      uint8_t         1-可以使用 0-不可以使用
// Sample usage:                zf_assert(timer_funciton_check(TIM_1, TIMER_FUNCTION_PWM);
//-------------------------------------------------------------------------------------------------------------------
uint8_t timer_funciton_check (TIMERN_enum index, timer_function_enum mode)
{
    if(timer_function_state[index] == TIMER_FUNCTION_INIT)
        timer_function_state[index] = mode;
    else if(timer_function_state[index] == mode)
        return 1;
    else
        return 0;
    return 1;
}


