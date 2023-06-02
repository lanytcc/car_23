/*********************************************************************************************************************
* @file            HF_HC_SR04.c
* @author          Fantastic Potato
* @version         1.0
* @Target core     CH32V307VCT6
* @date            2022-9-19
 ********************************************************************************************************************/

#include "HF_HC_SR04.h"

#define max     5000U
#define prescal 72U
#define speed   334.52

unsigned long g_overflow_times;
unsigned int g_current_time_value;
unsigned char g_flag_unhandler;

//-------------------------------------------------------------------------------------------------------------------
// @brief        超声波初始化
// @param        void
// @return       void
// Sample usage:        ultrasonic_init();
                        //初始化超声波功能
//-------------------------------------------------------------------------------------------------------------------
void ultra_init(void)
{
    /* TRIG引脚初始化 */
    gpio_init(TRIG, GPO, 0, SPEED_50MHZ | OUT_PP);                //PA0,初始化为GPIO功能、输出模式、输出高电平、速度50MHZ 推挽输出

    /* ECHO引脚初始化 */
    gpio_init(ECHO, GPI, 0, IN_FLOAT);
    gpio_bit_output(ECHO, 0);                                   //PA4 下拉

    timbase_init(TIM_2, max, prescal);
    TIM_Cmd(TIM2, DISABLE);

    g_overflow_times = 0;
    g_current_time_value = 0;
    g_flag_unhandler = 0;

}

//-------------------------------------------------------------------------------------------------------------------
// @brief        超声波发射一次并返回距离
// @param        void
// @return       unsigned int       测得的距离
// Sample usage:        ultrasonic_output();
                        //获取超声波测得的距离
//-------------------------------------------------------------------------------------------------------------------
unsigned int ultra_get_distance(void)
{
    unsigned int distance;

    distance = 0;
    while (gpio_get_input(ECHO) == 1)
    {
        ; //wait end
    }

    /* 超声波trig引脚置1 */
    gpio_bit_output(TRIG, 1);

    Delay_Us(20);

    gpio_bit_output(TRIG, 0);

    while (g_flag_unhandler)
    {
        g_flag_unhandler = 0;
        /* 计算单次求得的距离 */
        distance = (unsigned int)100 * ((g_overflow_times + (float)g_current_time_value / max) * max * prescal / (float)72000000 * speed * 100) / 2;
    }

    g_overflow_times = 0;
    g_current_time_value = 0;

    return distance;
}
