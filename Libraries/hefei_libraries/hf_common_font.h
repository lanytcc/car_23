/*********************************************************************************************************************
* @file            hf_common_font.h
* @author
* @version         1.0
* @Target core     CH32V307VCT6
* @date            20220907
********************************************************************************************************************/
#include "common.h"
#ifndef _HF_COMMON_FONT_H_
#define _HF_COMMON_FONT_H_



//-------常用颜色----------
#define RGB565_WHITE           0xFFFF  // 白色
#define RGB565_BLACK           0x0000  // 黑色
#define RGB565_BLUE            0x001F  // 蓝色
#define RGB565_PURPLE          0XF81F  // 紫色
#define RGB565_PINK            0XFE19  // 粉色
#define RGB565_RED             0xF800  // 红色
#define RGB565_MAGENTA         0xF81F  // 品红
#define RGB565_GREEN           0x07E0  // 绿色
#define RGB565_CYAN            0x07FF  // 青色
#define RGB565_YELLOW          0xFFE0  // 黄色
#define RGB565_BROWN           0XBC40  // 棕色
#define RGB565_GRAY            0X8430  // 灰色

//-------------------------------------------------------------------------------------------------------------------
// @brief       绝对值函数 数据范围是 [-32767,32767]
// @param       dat             需要求绝对值的数
// @return      int             返回绝对值
// Sample usage:                dat = myabs(dat);                               // 将dat变成正数
//-------------------------------------------------------------------------------------------------------------------
#define     myabs(x)            (x>=0? x: -(x))

extern const uint8      tft_ascii[95][16];
extern const uint8      oled_6x8[][6];
extern const uint8      oled_8x16[];
#endif
