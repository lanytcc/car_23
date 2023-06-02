
/*********************************************************************************************************************
* @file            car_4keys.h
* @author          Andreas HF
* @Target core     CH32V307VCT6
* @revisions       -2022.10.22, V1.0
* @modify          none
********************************************************************************************************************/

#ifndef CAR_KEYS_H_
#define CAR_KEYS_H_

#include "headfile.h"

//宏定义
#define KEYDOWN    0
#define KEYUP      1

#define K1         1
#define K2         2
#define K3         3
#define K4         4
#define K5         5
#define K6         6

#define K1_PIN     C0
#define K1_MODE    IN_PULLUP

#define K2_PIN     C1
#define K2_MODE    IN_PULLUP

#define K3_PIN     C2
#define K3_MODE    IN_PULLUP

#define K4_PIN     C3
#define K4_MODE    IN_PULLUP

#define K5_PIN     C4
#define K5_MODE    IN_PULLUP

#define K6_PIN     C5
#define K6_MODE    IN_PULLUP

//函数声明
/*按键初始化*/
void key_init(unsigned char key_num);
/*检测键值*/
unsigned char key_check(void);

//全局变量声明

#endif /* CAR_KEYS_H_ */
