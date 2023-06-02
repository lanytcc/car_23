/*********************************************************************************************************************
* @file            board_led.c
* @author          Davis Klay HF
* @Target core     CH32V307VCT6
* @revisions       -2022.09.07, V1.0
* @modify          1. 修改了板载LED熄灭函数和板载LED电平翻转函数的注释
*                  2. 删除了板载LED点亮函数中使另一个LED熄灭的函数
********************************************************************************************************************/
#include "board_led.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief        板载LED初始化
// @param        led_num            选择LED：LEDx(x = 1.2)
// @return       void
// Sample usage:       board_led_init(LED1);//板载LED1初始化
//-------------------------------------------------------------------------------------------------------------------
void board_led_init(unsigned char led_num)
{
    if(led_num == LED1)
    {
        gpio_init(LED1_PIN, GPO, 1, SPEED_50MHZ|LED1_MODE);
    }
    if(led_num == LED2)
    {
        gpio_init(LED2_PIN, GPO, 1, SPEED_50MHZ|LED2_MODE);//根据stm32教程p69，此处是否应该改为2MHz
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief        板载LED点亮
// @param        led_num            选择LED：LEDx(x = 1.2)
// @return       void
// Sample usage:       board_led_on(LED1);//点亮板载LED1
//-------------------------------------------------------------------------------------------------------------------
void board_led_on(unsigned char led_num)
{
    if(led_num == LED1)
    {
        gpio_bit_output(LED1_PIN,ON);
    }
    if(led_num == LED2)
    {
        gpio_bit_output(LED2_PIN,ON);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief        板载LED熄灭
// @param        led_num            选择LED：LEDx(x = 1.2)
// @return       void
// Sample usage:       board_led_off(LED1);//板载LED1熄灭
//-------------------------------------------------------------------------------------------------------------------
void board_led_off(unsigned char led_num)
{
    if(led_num == LED1)
    {
        gpio_bit_output(LED1_PIN,OFF);
    }
    if(led_num == LED2)
    {
        gpio_bit_output(LED2_PIN,OFF);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief        板载LED电平翻转
// @param        led_num            选择LED：LEDx(x = 1.2)
// @return       void
// Sample usage:       board_led_toggle(LED1);//板载LED1翻转
//-------------------------------------------------------------------------------------------------------------------
void board_led_toggle(unsigned char led_num)
{
    if(led_num == LED1)
    {
        gpio_toggle(LED1_PIN);
    }
    if(led_num == LED2)
    {
        gpio_toggle(LED2_PIN);
    }
}
