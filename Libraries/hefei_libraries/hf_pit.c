/*********************************************************************************************************************
 * @file            hf_pit.c
 * @brief           PIT配置
 * @author          Fantastic Potato
 * @version         1.0
 * @Target core     CH32V307VCT6
 * @date            2022-3-17
 ********************************************************************************************************************/

#include "hf_pit.h"

/* 引用外部变量 */
extern const uint32 TIMERN[];

//-------------------------------------------------------------------------------------------------------------------
//  @brief      定时器周期中断(ms)
//  @param      timern      定时器通道
//  @param      ms          定时周期(1-32767)
//  @return     void
//  Sample usage:           timer_pit_interrupt(TIMER_1, 5); 使用定时器1作为5ms一次的周期中断
//-------------------------------------------------------------------------------------------------------------------
void timer_pit_interrupt_ms(TIMERN_enum TIMER_x, uint16 ms)
{
    const uint32_t  irq_index[10] =
    {
        TIM1_UP_IRQn, TIM2_IRQn, TIM3_IRQn, TIM4_IRQn,
        TIM5_IRQn, TIM6_IRQn, TIM7_IRQn, TIM8_UP_IRQn,
        TIM9_UP_IRQn, TIM10_UP_IRQn
    };
    /* 定时器TIM初始化,计数器 */
    assert(ms);

    uint16 arr, pre;
    arr = sys_clk/1640 - 1;
    pre = 2*ms - 1;

    timbase_init(TIMER_x, arr, pre);

    TIM_ITConfig((TIM_TypeDef *)TIMERN[TIMER_x],TIM_IT_Update,ENABLE );      //使能指定的TIM中断,允许更新中断
    TIM_ClearITPendingBit((TIM_TypeDef *)TIMERN[TIMER_x], TIM_IT_Update);

    interrupt_set_priority((IRQn_Type)irq_index[(uint8_t)TIMER_x], 0x03);                    // 设置中断优先级
    interrupt_enable((IRQn_Type)irq_index[TIMER_x]);                                // 使能中断

    TIM_Cmd((TIM_TypeDef*)TIMERN[TIMER_x], ENABLE);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      定时器周期中断(us)
//  @param      timern      定时器通道
//  @param      us          定时周期(1-65535)
//  @return     void
//  Sample usage:           timer_pit_interrupt(TIMER_1, 5); 使用定时器1作为5ms一次的周期中断
//-------------------------------------------------------------------------------------------------------------------
void timer_pit_interrupt_us(TIMERN_enum TIMER_x, uint16 us)
{
    const uint32_t  irq_index[10] =
    {
        TIM1_UP_IRQn, TIM2_IRQn, TIM3_IRQn, TIM4_IRQn,
        TIM5_IRQn, TIM6_IRQn, TIM7_IRQn, TIM8_UP_IRQn,
        TIM9_UP_IRQn, TIM10_UP_IRQn
    };
    /* 定时器TIM初始化,计数器 */
    assert(us);

    uint16 arr, pre;
    arr = sys_clk/1000000 - 1;
    pre = us - 1;

    timbase_init(TIMER_x, arr, pre);

    TIM_ITConfig((TIM_TypeDef *)TIMERN[TIMER_x],TIM_IT_Update,ENABLE );      //使能指定的TIM中断,允许更新中断
    TIM_ClearITPendingBit((TIM_TypeDef *)TIMERN[TIMER_x], TIM_IT_Update);

    interrupt_set_priority((IRQn_Type)irq_index[(uint8_t)TIMER_x], 0x03);                    // 设置中断优先级
    interrupt_enable((IRQn_Type)irq_index[TIMER_x]);

    TIM_Cmd((TIM_TypeDef *)TIMERN[TIMER_x], ENABLE);  //使能TIMx

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      关闭PIT定时器
//  @param      timern      选择模块的通道 (选择范围 由TIMERN_enum枚举值的内容确定)
//  @return     void
//  Sample usage:           关闭定时器并清除标志位
//-------------------------------------------------------------------------------------------------------------------
void timer_pit_close(TIMERN_enum TIMER_x)
{
    TIM_DeInit((TIM_TypeDef *)TIMERN[TIMER_x]);
    TIM_Cmd((TIM_TypeDef *)TIMERN[TIMER_x], DISABLE);  //失能TIMx
}
