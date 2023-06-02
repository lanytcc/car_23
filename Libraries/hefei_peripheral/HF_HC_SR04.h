/*********************************************************************************************************************
* @file            HF_HC_SR04.h
* @author          Fantastic Potato
* @version         1.0
* @Target core     CH32V307VCT6
* @date            2022-9-19
 ********************************************************************************************************************/

#ifndef _HF_HC_SR04_H
#define _HF_HC_SR04_H

#include "headfile.h"

/* 资源定义 */
#define TRIG         B4
#define ECHO         D9
#define EXTI_NUM     EXTI_9


//函数声明
/* 超声波初始化 */
void ultra_init(void);
/* 超声波发射一次并返回距离 */
unsigned int ultra_get_distance(void);

//全局变量声明

#endif
