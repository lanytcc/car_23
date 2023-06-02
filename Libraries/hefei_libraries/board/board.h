/*********************************************************************************************************************
* @file             board.h
* @brief            学习板基本参数初始化
* @author           Fantastic Potato
* @version          1.0
* @Target core      CH32V307VCT6
* @date             2022-3-3
********************************************************************************************************************/

#ifndef BOARD_BOARD_H_
#define BOARD_BOARD_H_

#include "headfile.h"

/* 宏定义 */
#define PRINTF_ENABLE           1                   //printf使能
#define DEBUG_UART              UART_3              //DEBUG串口
#define DEBUG_UART_BAUD         115200              //DEBUG串口波特率
#define DEBUG_UART_TX_PIN       UART3_TX_B10        //DEBUG串口TX引脚
#define DEBUG_UART_RX_PIN       UART3_RX_B11        //DEBUG串口RX引脚


/* 函数声明 */

/* 外部函数，用户可自行调用 */
void board_init(void);

#endif /* BOARD_BOARD_H_ */
