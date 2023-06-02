/*********************************************************************************************************************
* @file            car_light.h
* @author          Andreas HF
* @Target core     CH32V307VCT6
* @revisions       -2022.10.22, V1.0
* @modify          none
 ********************************************************************************************************************/

#ifndef __CAR_LIGHT_H
#define __CAR_LIGHT_H

#include "headfile.h"

//全局变量声明


/* 类型定义 */
typedef enum
{
    red,
    yellow,
    white,
    green,
    blue,
    cyan,
    purple
}rgb_color;

//函数声明(外部函数，用户可自行调用)
/* 智能车尾灯同时亮起 */
void car_both_rgb_on(rgb_color color);
/* 智能车尾灯熄灭 */
void car_both_rgb_off();
/* 智能车尾灯左灯闪烁 */
void car_left_rgb_flash(rgb_color color, uint16_t flash_count, uint16_t time);
/* 智能车尾灯右灯闪烁 */
void car_right_rgb_flash(rgb_color color, uint16_t flash_count, uint16_t time);
/* 智能车尾灯同时闪烁 */
void car_both_rgb_flash(rgb_color color, uint16_t flash_count, uint16_t time);

#endif
