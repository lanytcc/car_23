/*********************************************************************************************************************
* @file            car_reed.h
* @author          Fantastic Potato
* @Target core     CH32V307VCT6
* @revisions       -2022.09.18
* @modify          none
********************************************************************************************************************/

#ifndef CAR_REED_H_
#define CAR_REED_H_

#include "headfile.h"


//函数声明
/* 智能车干簧管初始化 */
void reed_init(void);
/* 智能车磁标检测 */
void reed_check(void);


//全局变量声明
extern unsigned char g_reed_flag;

#endif /* CAR_REED_H_ */
