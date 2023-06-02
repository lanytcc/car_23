/*********************************************************************************************************************
* @file            car_light.c
* @author          Andreas HF
* @Target core     CH32V307VCT6
* @revisions       -2022.09.07, V1.0
* @modify          none
 ********************************************************************w************************************************/

#include "car_light.h"

/* rgb颜色定义 */
static unsigned char rgb_off[3] = {0x00,0x00,0x00};
static unsigned char rgb_blue[3] = {0x00,0x00,0xFF};
static unsigned char rgb_green[3] = {0x00,0xFF,0x00};
static unsigned char rgb_cyan[3] = {0x00,0xFF,0xFF};
static unsigned char rgb_red[3] = {0xFF,0x00,0x00};
static unsigned char rgb_purple[3] = {0xFF,0x00,0xFF};
static unsigned char rgb_yellow[3] = {0xFF,0xFF,0x00};
static unsigned char rgb_white[3] = {0xFF,0xFF,0xFF};

//-------------------------------------------------------------------------------------------------------------------
// @brief        智能车尾灯同时亮起
// @param        color          尾灯颜色
// @param        time           延时时间(ms)
// @return       void
// Sample usage:        car_both_rgb_on(red);//尾灯同时显示红色，延时1s
//-------------------------------------------------------------------------------------------------------------------
void car_both_rgb_on(rgb_color color)
{
    if(color == red)
    {
        ws2812b_write(rgb_red);
        ws2812b_write(rgb_red);
    }
    else if(color == yellow)
    {
        ws2812b_write(rgb_yellow);
        ws2812b_write(rgb_yellow);
    }
    else if(color == white)
    {
        ws2812b_write(rgb_white);
        ws2812b_write(rgb_white);
    }
    else if(color == blue)
    {
        ws2812b_write(rgb_blue);
        ws2812b_write(rgb_blue);
    }
    else if(color == green)
    {
        ws2812b_write(rgb_green);
        ws2812b_write(rgb_green);
    }
    else if(color == cyan)
    {
        ws2812b_write(rgb_cyan);
        ws2812b_write(rgb_cyan);
    }
    else if(color == purple)
    {
        ws2812b_write(rgb_purple);
        ws2812b_write(rgb_purple);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief        智能车尾灯熄灭
// @param        void
// @return       void
// Sample usage:        car_both_rgb_off();//尾灯熄灭
//-------------------------------------------------------------------------------------------------------------------
void car_both_rgb_off()
{
    ws2812b_write(rgb_off);
    ws2812b_write(rgb_off);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief        智能车尾灯左灯闪烁
// @param        color          尾灯颜色
// @param        flash_count    闪烁次数
// @param        time           间隔时间(ms)
// @return       void
// Sample usage:        car_left_rgb_flash(red, 3, 500);//左灯闪烁红色三次，间隔0.5s
//-------------------------------------------------------------------------------------------------------------------
void car_left_rgb_flash(rgb_color color, uint16_t flash_count, uint16_t time)
{
    unsigned int i;

    if(color == red)
    {
        for(i=0; i<flash_count; i++)
        {
            ws2812b_write(rgb_red);
            ws2812b_write(rgb_off);

            Delay_Ms(time);

            ws2812b_write(rgb_off);
            ws2812b_write(rgb_off);
            Delay_Ms(time);
        }
    }
    else if(color == yellow)
    {
        for(i=0; i<flash_count; i++)
        {
            ws2812b_write(rgb_yellow);
            ws2812b_write(rgb_off);

            Delay_Ms(time);

            ws2812b_write(rgb_off);
            ws2812b_write(rgb_off);
            Delay_Ms(time);
        }
    }
    else if(color == white)
    {
        for(i=0; i<flash_count; i++)
        {
            ws2812b_write(rgb_white);
            ws2812b_write(rgb_off);

            Delay_Ms(time);

            ws2812b_write(rgb_off);
            ws2812b_write(rgb_off);
            Delay_Ms(time);
        }
    }
    else if(color == green)
    {
        for(i=0; i<flash_count; i++)
        {
            ws2812b_write(rgb_green);
            ws2812b_write(rgb_off);

            Delay_Ms(time);

            ws2812b_write(rgb_off);
            ws2812b_write(rgb_off);
            Delay_Ms(time);
        }
    }
    else if(color == blue)
    {
        for(i=0; i<flash_count; i++)
        {
            ws2812b_write(rgb_blue);
            ws2812b_write(rgb_off);

            Delay_Ms(time);

            ws2812b_write(rgb_off);
            ws2812b_write(rgb_off);
            Delay_Ms(time);
        }
    }
    else if(color == cyan)
    {
        for(i=0; i<flash_count; i++)
        {
            ws2812b_write(rgb_cyan);
            ws2812b_write(rgb_off);

            Delay_Ms(time);

            ws2812b_write(rgb_off);
            ws2812b_write(rgb_off);
            Delay_Ms(time);
        }
    }
    else if(color == purple)
    {
        for(i=0; i<flash_count; i++)
        {
            ws2812b_write(rgb_purple);
            ws2812b_write(rgb_off);

            Delay_Ms(time);

            ws2812b_write(rgb_off);
            ws2812b_write(rgb_off);
            Delay_Ms(time);
        }
    }
    else
    {
        for(i=0; i<flash_count; i++)
        {
            ws2812b_write(rgb_off);
            ws2812b_write(rgb_off);

            Delay_Ms(time);

            ws2812b_write(rgb_off);
            ws2812b_write(rgb_off);
            Delay_Ms(time);
        }
    }

}

//-------------------------------------------------------------------------------------------------------------------
// @brief        智能车尾灯右灯闪烁
// @param        color          尾灯颜色
// @param        flash_count    闪烁次数
// @param        time           间隔时间(ms)
// @return       void
// Sample usage:        car_right_rgb_flash(red, 3, 500);//右灯闪烁红色三次，间隔0.5s
//-------------------------------------------------------------------------------------------------------------------
void car_right_rgb_flash(rgb_color color, uint16_t flash_count, uint16_t time)
{
    unsigned int i;

    if(color == red)
    {
        for(i=0; i<flash_count; i++)
        {
            ws2812b_write(rgb_off);
            ws2812b_write(rgb_red);

            Delay_Ms(time);

            ws2812b_write(rgb_off);
            ws2812b_write(rgb_off);
            Delay_Ms(time);
        }
    }
    else if(color == yellow)
    {
        for(i=0; i<flash_count; i++)
        {
            ws2812b_write(rgb_off);
            ws2812b_write(rgb_yellow);

            Delay_Ms(time);

            ws2812b_write(rgb_off);
            ws2812b_write(rgb_off);
            Delay_Ms(time);
        }
    }
    else if(color == white)
    {
        for(i=0; i<flash_count; i++)
        {
            ws2812b_write(rgb_off);
            ws2812b_write(rgb_white);

            Delay_Ms(time);

            ws2812b_write(rgb_off);
            ws2812b_write(rgb_off);
            Delay_Ms(time);
        }
    }
    else if(color == green)
    {
        for(i=0; i<flash_count; i++)
        {
            ws2812b_write(rgb_off);
            ws2812b_write(rgb_green);

            Delay_Ms(time);

            ws2812b_write(rgb_off);
            ws2812b_write(rgb_off);
            Delay_Ms(time);
        }
    }
    else if(color == blue)
    {
        for(i=0; i<flash_count; i++)
        {
            ws2812b_write(rgb_off);
            ws2812b_write(rgb_blue);

            Delay_Ms(time);

            ws2812b_write(rgb_off);
            ws2812b_write(rgb_off);
            Delay_Ms(time);
        }
    }
    else if(color == cyan)
    {
        for(i=0; i<flash_count; i++)
        {
            ws2812b_write(rgb_off);
            ws2812b_write(rgb_cyan);

            Delay_Ms(time);

            ws2812b_write(rgb_off);
            ws2812b_write(rgb_off);
            Delay_Ms(time);
        }
    }
    else if(color == purple)
    {
        for(i=0; i<flash_count; i++)
        {
            ws2812b_write(rgb_off);
            ws2812b_write(rgb_purple);

            Delay_Ms(time);

            ws2812b_write(rgb_off);
            ws2812b_write(rgb_off);
            Delay_Ms(time);
        }
    }
    else
    {
        for(i=0; i<flash_count; i++)
        {
            ws2812b_write(rgb_off);
            ws2812b_write(rgb_off);

            Delay_Ms(time);

            ws2812b_write(rgb_off);
            ws2812b_write(rgb_off);
            Delay_Ms(time);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief        智能车尾灯同时闪烁
// @param        color          尾灯颜色
// @param        flash_count    闪烁次数
// @param        time           间隔时间(ms)
// @return       void
// Sample usage:        car_both_rgb_flash(red, 3, 500);//尾灯同时闪烁红色三次，间隔0.5s
//-------------------------------------------------------------------------------------------------------------------
void car_both_rgb_flash(rgb_color color, uint16_t flash_count, uint16_t time)
{
    unsigned int i;

    if(color == red)
    {
        for(i=0; i<flash_count; i++)
        {
            ws2812b_write(rgb_red);
            ws2812b_write(rgb_red);

            Delay_Ms(time);

            ws2812b_write(rgb_off);
            ws2812b_write(rgb_off);
            Delay_Ms(time);
        }
    }
    else if(color == yellow)
    {
        for(i=0; i<flash_count; i++)
        {
            ws2812b_write(rgb_yellow);
            ws2812b_write(rgb_yellow);

            Delay_Ms(time);

            ws2812b_write(rgb_off);
            ws2812b_write(rgb_off);
            Delay_Ms(time);
        }
    }
    else if(color == white)
    {
        for(i=0; i<flash_count; i++)
        {
            ws2812b_write(rgb_white);
            ws2812b_write(rgb_white);

            Delay_Ms(time);

            ws2812b_write(rgb_off);
            ws2812b_write(rgb_off);
            Delay_Ms(time);
        }
    }
    else if(color == green)
    {
        for(i=0; i<flash_count; i++)
        {
            ws2812b_write(rgb_green);
            ws2812b_write(rgb_green);

            Delay_Ms(time);

            ws2812b_write(rgb_off);
            ws2812b_write(rgb_off);
            Delay_Ms(time);
        }
    }
    else if(color == blue)
    {
        for(i=0; i<flash_count; i++)
        {
            ws2812b_write(rgb_blue);
            ws2812b_write(rgb_blue);

            Delay_Ms(time);

            ws2812b_write(rgb_off);
            ws2812b_write(rgb_off);
            Delay_Ms(time);
        }
    }
    else if(color == cyan)
    {
        for(i=0; i<flash_count; i++)
        {
            ws2812b_write(rgb_cyan);
            ws2812b_write(rgb_cyan);

            Delay_Ms(time);

            ws2812b_write(rgb_off);
            ws2812b_write(rgb_off);
            Delay_Ms(time);
        }
    }
    else if(color == purple)
    {
        for(i=0; i<flash_count; i++)
        {
            ws2812b_write(rgb_purple);
            ws2812b_write(rgb_purple);

            Delay_Ms(time);

            ws2812b_write(rgb_off);
            ws2812b_write(rgb_off);
            Delay_Ms(time);
        }
    }
    else
    {
        for(i=0; i<flash_count; i++)
        {
            ws2812b_write(rgb_off);
            ws2812b_write(rgb_off);

            Delay_Ms(time);

            ws2812b_write(rgb_off);
            ws2812b_write(rgb_off);
            Delay_Ms(time);
        }
    }
}
