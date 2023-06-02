/*********************************************************************************************************************
 * @file            hf_encoder.c
 * @author          Fantastic Potato
 * @version         1.0
 * @Target core     CH32V307VCT6
 * @date            2022-3-5
 ********************************************************************************************************************/

#include "hf_encoder.h"

/* 引用外部变量 */
extern const uint32 TIMERN[];

//-------------------------------------------------------------------------------------------------------------------
//  @brief      编码器初始化
//  @param      TIMER_x     定时器
//  @param      phaseA      通道A以及引脚
//  @param      phaseB      通道B以及引脚
//  @return     void
//  Sample usage:           encoder_init(TIMER_2, TIMER2_CHA_A0, TIMER2_CHB_A1)
//                          //定时器选择TIM_2, A相接A0, B相接A1
//-------------------------------------------------------------------------------------------------------------------
void encoder_init(TIMERN_enum TIMER_x, ENCODER_PIN_enum phaseA, ENCODER_PIN_enum phaseB)
{
    /* GPIO引脚初始化 */
    encoder_gpio_init(phaseA, phaseB);

    /* 时基初始化 */
    timbase_init(TIMER_x, 65535, 0);

    /* 编码器初始化 */
    TIM_EncoderInterfaceConfig((TIM_TypeDef *)TIMERN[TIMER_x],              //编码器模式3
            TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);

    /* 输入捕获初始化 */
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1 | TIM_Channel_2;        //选择通道1,2
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;           //双边沿捕获
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;         //IC1、IC2直接映射映射到 TI1、TI2上
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;                   //配置输入分频,不分频
    TIM_ICInitStructure.TIM_ICFilter = 0;                                   //IC2F=0000 配置输入滤波器 不滤波
    TIM_ICInit((TIM_TypeDef *)TIMERN[TIMER_x], &TIM_ICInitStructure);

    TIM_ClearITPendingBit((TIM_TypeDef *)TIMERN[TIMER_x], TIM_IT_Update);   //清除TIM更新标志位
    TIM_ITConfig((TIM_TypeDef *)TIMERN[TIMER_x], TIM_IT_Update, ENABLE);    //使能开启TIM中断
    TIM_SetCounter((TIM_TypeDef *)TIMERN[TIMER_x], 0x7fff);                 //复位值选择0x7fff,以便计算正反转

    TIM_Cmd((TIM_TypeDef *)TIMERN[TIMER_x], ENABLE);                        //使能定时器
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取定时器计数值
//  @param      TIMER_x     定时器
//  @return     定时器计数值
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
int encoder_cnt_get(TIMERN_enum TIMER_x)
{
    int count = ((TIM_TypeDef*)(TIMERN[TIMER_x]))->CNT;
    return count;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      清除定时器计数值
//  @param      TIMER_x     定时器
//  @return
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void encoder_cnt_clear(TIMERN_enum TIMER_x)
{
    ((TIM_TypeDef*)(TIMERN[TIMER_x]))->CNT = 0x7fff;                        //复位值选择0x7fff,以便计算正反转
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      编码器引脚初始化
//  @param      phaseA      通道A以及引脚
//  @param      phaseB      通道B以及引脚
//  @return     void
//  Sample usage:           内部使用，用户无需关心
//-------------------------------------------------------------------------------------------------------------------
void encoder_gpio_init(ENCODER_PIN_enum phaseA, ENCODER_PIN_enum phaseB)
{
    if(TIMER1_CHA_A8 == phaseA)       gpio_init(A8, GPI, 0, IN_FLOAT);
    else if(TIMER2_CHA_A0 == phaseA)  gpio_init(A0, GPI, 0, IN_FLOAT);
    else if(TIMER3_CHA_A6 == phaseA)  gpio_init(A6, GPI, 0, IN_FLOAT);
    else if(TIMER4_CHA_B6 == phaseA)  gpio_init(B6, GPI, 0, IN_FLOAT);

    if(TIMER1_CHB_A9 == phaseB)       gpio_init(A9, GPI, 0, IN_FLOAT);
    else if(TIMER2_CHB_A1 == phaseB)  gpio_init(A1, GPI, 0, IN_FLOAT);
    else if(TIMER3_CHB_A7 == phaseB)  gpio_init(A7, GPI, 0, IN_FLOAT);
    else if(TIMER4_CHB_B7 == phaseB)  gpio_init(B7, GPI, 0, IN_FLOAT);
}
