/*********************************************************************************************************************
 * @file            hf_pwm.c
 * @brief           PWM配置
 * @author          Fantastic Potato
 * @version         1.0
 * @Target core     CH32V307VCT6
 * @date            2022-3-11
 ********************************************************************************************************************/

#include "hf_pwm.h"

/* 引用外部变量 */
extern const uint32 TIMERN[];

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWM初始化
//  @param      pwmch           PWM通道号及引脚
//  @param      freq            PWM频率
//  @param      duty            PWM占空比
//  @param      TIM_OCPreload_Enable_Disable     默认设置为TIM_OCPreload_Disable,后期可删除此形参只是为了适配RGB灯做的修改
//  @return     void
//  Sample usage:               pwm_init(PWM1_CH1_A8, 50, 5000);   //初始化PWM1 通道1 使用引脚A8  输出PWM频率50HZ   占空比为百分之 5000/PWM_DUTY_MAX*100
//                              PWM_DUTY_MAX在wh_pwm.h文件中 默认为10000
//-------------------------------------------------------------------------------------------------------------------
void pwm_init(PWMCH_enum pwmch, uint32 freq, uint32 duty, uint16_t TIM_OCPreload_Enable_Disable)
{
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    uint16 match_temp;                                          //占空比值
    uint16 period_temp;                                         //周期值
    uint16 freq_div = 0;                                        //分频值

    pwm_gpio_init(pwmch);                                       //PWM引脚初始化

    /* 获取系统主频 */
    freq_div = (uint16)((SystemCoreClock / freq) >> 16);                            //多少分频
    period_temp = (uint16)(SystemCoreClock/(freq*(freq_div + 1)));                  //周期
    match_temp = period_temp * duty / PWM_DUTY_MAX;                         //占空比

    /* 时基初始化 */
    timbase_init((pwmch >> 8), period_temp - 1, freq_div);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);        //使能AFIO复用功能模块时钟

    //引脚重映射


    if((pwmch >> 4) == 0x01)      GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);
    else if((pwmch >> 4) == 0x11) GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
    else if((pwmch >> 4) == 0x21) GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);
    else if((pwmch >> 4) == 0x22) GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
    else if((pwmch >> 4) == 0x31) GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);
    else if((pwmch >> 4) == 0x71) AFIO->PCFR2 |= 0x01<<2;
    else if((pwmch >> 4) == 0x81) AFIO->PCFR2 |= 0x02<<3;
    else if((pwmch >> 4) == 0x91) AFIO->PCFR2 |= 0x02<<5;
    else if((pwmch >> 4) == 0x92) AFIO->PCFR2 |= 0x01<<5;


    //初始化TIM PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                       //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;           //比较输出使能
    TIM_OCInitStructure.TIM_Pulse = match_temp;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;               //输出极性:TIM输出比较极性高
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;

    if((pwmch & 0x03) == 0x00)                                              //通道选择
    {
        TIM_OC1Init(((TIM_TypeDef *) TIMERN[pwmch>>8]), &TIM_OCInitStructure );                         //定时器通道1初始化
        TIM_CtrlPWMOutputs(((TIM_TypeDef *) TIMERN[pwmch>>8]), ENABLE );
        TIM_OC1PreloadConfig(((TIM_TypeDef *) TIMERN[pwmch>>8]), TIM_OCPreload_Enable_Disable);         //定时器预装载配置
    }
    else if((pwmch & 0x03) == 0x01)
    {
        TIM_OC2Init(((TIM_TypeDef *) TIMERN[pwmch>>8]), &TIM_OCInitStructure );
        TIM_CtrlPWMOutputs(((TIM_TypeDef *) TIMERN[pwmch>>8]), ENABLE );
        TIM_OC2PreloadConfig(((TIM_TypeDef *) TIMERN[pwmch>>8]), TIM_OCPreload_Enable_Disable);
    }
    else if((pwmch & 0x03) == 0x02)
    {
        TIM_OC3Init(((TIM_TypeDef *) TIMERN[pwmch>>8]), &TIM_OCInitStructure );
        TIM_CtrlPWMOutputs(((TIM_TypeDef *) TIMERN[pwmch>>8]), ENABLE );
        TIM_OC3PreloadConfig(((TIM_TypeDef *) TIMERN[pwmch>>8]), TIM_OCPreload_Enable_Disable);
    }
    else if((pwmch & 0x03) == 0x03)
    {
        TIM_OC4Init(((TIM_TypeDef *) TIMERN[pwmch>>8]), &TIM_OCInitStructure );
        TIM_CtrlPWMOutputs(((TIM_TypeDef *) TIMERN[pwmch>>8]), ENABLE );
        TIM_OC4PreloadConfig(((TIM_TypeDef *) TIMERN[pwmch>>8]), TIM_OCPreload_Enable_Disable);
    }

    TIM_Cmd(((TIM_TypeDef *) TIMERN[pwmch>>8]), ENABLE);                                              //定时器使能
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWM占空比设定
//  @param      pwmch           PWM通道号及引脚
//  @param      duty            PWM占空比
//  @return     void
//  Sample usage:               pwm_duty(PWM1_CH1_A8, 5000);   //PWM1 通道1 使用引脚A8 占空比为百分之 5000/PWM_DUTY_MAX*100
//                              PWM_DUTY_MAX在wh_pwm.h文件中 默认为10000
//-------------------------------------------------------------------------------------------------------------------
void pwm_duty(PWMCH_enum pwmch, uint32 duty)
{
    uint16 match_temp;
    uint16 period_temp;

    //获取周期值
    //计算占空比

    period_temp = ((TIM_TypeDef *) TIMERN[pwmch>>8])->ATRLR;    //获取定时器的周期值
    match_temp = period_temp * duty / PWM_DUTY_MAX;             //占空比

    (*(volatile uint32_t*)(TIMERN[pwmch>>8] + 0x34 + (pwmch&0x03) * 0x04)) = match_temp;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWM频率设定
//  @param      pwmch           PWM通道号及引脚
//  @param      freq            PWM频率
//  @param      duty            PWM占空比
//  @return     void
//  Sample usage:               pwm_freq(PWM1_CH1_A8, 60, 5000);   //使用引脚A8  输出PWM频率60HZ   占空比为百分之 5000/PWM_DUTY_MAX*100
//                              PWM_DUTY_MAX在wh_pwm.h文件中 默认为10000
//-------------------------------------------------------------------------------------------------------------------
void pwm_freq(PWMCH_enum pwmch, uint32 freq,uint32_t duty)
{
    uint16 period_temp = 0;                                     //周期值
    uint16 freq_div = 0;                                        //分频值
    uint16 match_temp;

    freq_div = (uint16)((sys_clk / freq) >> 16);                //计算多少分频
    period_temp = (uint16)(sys_clk/(freq*(freq_div + 1)));      //计算周期

    ((TIM_TypeDef *) TIMERN[pwmch>>8])->ATRLR = period_temp - 1 ;
    ((TIM_TypeDef *) TIMERN[pwmch>>8])->PSC = freq_div;
    match_temp = period_temp * duty / PWM_DUTY_MAX;             //占空比

    (*(volatile uint32_t*)(TIMERN[pwmch>>8] + 0x34 + (pwmch&0x03) * 0x04)) = match_temp;
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWM停止输出
//  @param      pwmch           PWM通道号及引脚
//  @return     void
//  Sample usage:               pwm_freq(PWM1_CH1_A8, 60, 5000);   //使用引脚A8  输出PWM频率60HZ   占空比为百分之 5000/PWM_DUTY_MAX*100
//                              PWM_DUTY_MAX在wh_pwm.h文件中 默认为10000
//-------------------------------------------------------------------------------------------------------------------
void pwm_stop(PWMCH_enum pwmch)
{
    TIM_Cmd(((TIM_TypeDef *) TIMERN[pwmch>>8]), DISABLE);         //定时器失能
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      PWM引脚初始化
//  @param      pwmch       PWM通道(可选择范围由wh_pwm.h内PWMCH_enum枚举值确定)
//  @return     void
//  Sample usage:           内部使用  用户无需关心
//-------------------------------------------------------------------------------------------------------------------
void pwm_gpio_init(PWMCH_enum pwmch)
{
    //-------------------------------PWM1-------------------------------//
         if(pwmch == TIM1_PWM_CH1_A8 )   gpio_init(A8 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM1_PWM_CH2_A9 )   gpio_init(A9 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM1_PWM_CH3_A10)   gpio_init(A10, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM1_PWM_CH4_A11)   gpio_init(A11, GPO, 0, SPEED_50MHZ|OUT_AF_PP);

    else if(pwmch == TIM1_PWM_CH1_E9 )   gpio_init(E9 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM1_PWM_CH2_E11)   gpio_init(E11, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM1_PWM_CH3_E13)   gpio_init(E13, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM1_PWM_CH4_E14)   gpio_init(E14, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    //-------------------------------PWM2-------------------------------//
    else if(pwmch == TIM2_PWM_CH1_A0 )    gpio_init(A0 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM2_PWM_CH2_A1 )    gpio_init(A1 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM2_PWM_CH3_A2 )    gpio_init(A2 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM2_PWM_CH4_A3 )    gpio_init(A3 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);

    else if(pwmch == TIM2_PWM_CH1_A15)    gpio_init(A15, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM2_PWM_CH2_B3 )    gpio_init(B3 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM2_PWM_CH3_B10)    gpio_init(B10, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM2_PWM_CH4_B11)    gpio_init(B11, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    //-------------------------------PWM3-------------------------------//
    else if(pwmch == TIM3_PWM_CH1_A6 )    gpio_init(A6,  GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM3_PWM_CH2_A7 )    gpio_init(A7,  GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM3_PWM_CH3_B0 )    gpio_init(B0,  GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM3_PWM_CH4_B1 )    gpio_init(B1,  GPO, 0, SPEED_50MHZ|OUT_AF_PP);

    else if(pwmch == TIM3_PWM_CH1_C6 )    gpio_init(C6,  GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM3_PWM_CH2_C7 )    gpio_init(C7,  GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM3_PWM_CH3_C8 )    gpio_init(C8,  GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM3_PWM_CH4_C9 )    gpio_init(C9,  GPO, 0, SPEED_50MHZ|OUT_AF_PP);

    else if(pwmch == TIM3_PWM_CH1_B4 )    gpio_init(B4,  GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM3_PWM_CH2_B5 )    gpio_init(B5,  GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    //-------------------------------PWM4-------------------------------//
    else if(pwmch == TIM4_PWM_CH1_B6 )    gpio_init(B6,  GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM4_PWM_CH2_B7 )    gpio_init(B7,  GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM4_PWM_CH3_B8 )    gpio_init(B8,  GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM4_PWM_CH4_B9 )    gpio_init(B9,  GPO, 0, SPEED_50MHZ|OUT_AF_PP);

    else if(pwmch == TIM4_PWM_CH1_D12)    gpio_init(D12, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM4_PWM_CH2_D13)    gpio_init(D13, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM4_PWM_CH3_D14)    gpio_init(D14, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM4_PWM_CH4_D15)    gpio_init(D15, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    //-------------------------------PWM5-------------------------------//
    else if(pwmch == TIM5_PWM_CH1_A0 )    gpio_init(A0 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM5_PWM_CH2_A1 )    gpio_init(A1 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM5_PWM_CH3_A2 )    gpio_init(A2 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM5_PWM_CH4_A3 )    gpio_init(A3 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    //-------------------------------PWM8-------------------------------//
    else if(pwmch == TIM8_PWM_CH1_C6 )    gpio_init(C6 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM8_PWM_CH2_C7 )    gpio_init(C7 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM8_PWM_CH3_C8 )    gpio_init(C8 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM8_PWM_CH4_C9 )    gpio_init(C9 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);

    else if(pwmch == TIM8_PWM_CH1_B6 )    gpio_init(B6 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM8_PWM_CH2_B7 )    gpio_init(B7 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM8_PWM_CH3_B8 )    gpio_init(B8 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM8_PWM_CH4_C13)    gpio_init(C13, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    //-------------------------------PWM9-------------------------------//
    else if(pwmch == TIM9_PWM_CH1_A2 )    gpio_init(A2 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM9_PWM_CH2_A3 )    gpio_init(A3 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM9_PWM_CH3_A4 )    gpio_init(A4 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM9_PWM_CH4_C4 )    gpio_init(C4 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);

    else if(pwmch == TIM9_PWM_CH1_D9 )    gpio_init(D9 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM9_PWM_CH2_D11)    gpio_init(D11, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM9_PWM_CH3_D13)    gpio_init(D13, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM9_PWM_CH4_D15)    gpio_init(D15, GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    //-------------------------------PWM10-------------------------------//
    else if(pwmch == TIM10_PWM_CH1_B8 )   gpio_init(B8 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM10_PWM_CH2_B9 )   gpio_init(B9 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM10_PWM_CH3_C3 )   gpio_init(C3 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM10_PWM_CH4_C11)   gpio_init(C11, GPO, 0, SPEED_50MHZ|OUT_AF_PP);

    else if(pwmch == TIM10_PWM_CH1_D1 )   gpio_init(D1 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM10_PWM_CH2_D3 )   gpio_init(D3 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM10_PWM_CH3_D5 )   gpio_init(D5 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM10_PWM_CH4_D7 )   gpio_init(D7 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);

    else if(pwmch == TIM10_PWM_CH1_B3 )   gpio_init(B3 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM10_PWM_CH2_B4 )   gpio_init(B4 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM10_PWM_CH3_B5 )   gpio_init(B5 , GPO, 0, SPEED_50MHZ|OUT_AF_PP);
    else if(pwmch == TIM10_PWM_CH4_C15)   gpio_init(C15, GPO, 0, SPEED_50MHZ|OUT_AF_PP);

}
