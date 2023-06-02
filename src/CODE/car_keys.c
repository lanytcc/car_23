
/*********************************************************************************************************************
* @file            car_keys.c
* @author          Andreas HF
* @Target core     CH32V307VCT6
* @revisions       -2022.09.07, V1.0
* @modify          none
********************************************************************************************************************/

#include "car_keys.h"

//-------------------------------------------------------------------------------------------------------------------
// @brief        按键初始化
// @param        key_num            选择按键：Kx(x = 1, 2, 3, 4, 5, 6)
// @return       void
// Sample usage:       key_init(K1);//按键初始化
//-------------------------------------------------------------------------------------------------------------------
void key_init(unsigned char key_num)
{
    if(key_num == K1)
    {
        gpio_init(K1_PIN, GPI, 1, SPEED_50MHZ|K1_MODE);
    }
    if(key_num == K2)
    {
        gpio_init(K2_PIN, GPI, 1, SPEED_50MHZ|K2_MODE);
    }
    if(key_num == K3)
    {
        gpio_init(K3_PIN, GPI, 1, SPEED_50MHZ|K3_MODE);
    }
    if(key_num == K4)
    {
        gpio_init(K4_PIN, GPI, 1, SPEED_50MHZ|K4_MODE);
    }
    if(key_num == K5)
    {
        gpio_init(K5_PIN, GPI, 1, SPEED_50MHZ|K5_MODE);
    }
    if(key_num == K6)
    {
        gpio_init(K6_PIN, GPI, 1, SPEED_50MHZ|K6_MODE);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief        检测键值
// @param        key_num            选择按键：Kx(x = 1, 2, 3, 4)
// @return       key_value
// Sample usage:       key_check(K1);//检测K1键值
//-------------------------------------------------------------------------------------------------------------------
unsigned char key_check(void)
{
    unsigned char key_value;

    if(KEYDOWN == gpio_get_input(K1_PIN))
    {
        key_value = 0x01;

        while(KEYUP != gpio_get_input(K1_PIN))
        {
            ;
        }
    }
    else if(KEYDOWN == gpio_get_input(K2_PIN))
    {
        key_value = 0x02;

        while(KEYUP != gpio_get_input(K2_PIN))
        {
            ;
        }
    }
    else if(KEYDOWN== gpio_get_input(K3_PIN))
    {
        key_value = 0x03;

        while(KEYUP != gpio_get_input(K3_PIN))
        {
            ;
        }
    }
    else if(KEYDOWN == gpio_get_input(K4_PIN))
    {
        key_value = 0x04;

        while(KEYUP != gpio_get_input(K4_PIN))
        {
            ;
        }
    }
    else if(KEYDOWN == gpio_get_input(K5_PIN))
    {
        key_value = 0x05;

        while(KEYUP != gpio_get_input(K5_PIN))
        {
            ;
        }
    }
    else if(KEYDOWN == gpio_get_input(K6_PIN))
    {
        key_value = 0x06;

        while(KEYUP != gpio_get_input(K6_PIN))
        {
            ;
        }
    }
    else
    {
        key_value = 0xFF;
    }
    return key_value;
}


