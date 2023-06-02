/********************************************************************************************************************
* @file            HF_Wireless_uart.h
* @author
* @version         1.0
* @Target core     CH32V307VCT6
* @date            20220905
********************************************************************************************************************/

#ifndef HF_WIRELESS_UART_H_
#define HF_WIRELESS_UART_H_


#include "headfile.h"

#define WIRELESS_UART_NUMBER       1                                           // 定义接入的无线串口的数量 默认最大就三个
#define WIRELESS_UART_INDEX        UART_2                                      // 无线串口 1 对应使用的串口号
#define WIRELESS_UART_BUAD_RATE    115200                                      // 无线串口 1 对应使用的串口波特率
#define A3                         UART2_RX_A3                                 // 无线串口 1 对应模块的 TX 要接到单片机的 RX
#define A2                         UART2_TX_A2                                 // 无线串口 1 对应模块的 RX 要接到单片机的 TX
#define WIRELESS_UART_RTS_PIN      E10                                         // 无线串口 1 对应模块的 RTS 引脚


#define WIRELESS_UART_AUTO_BAUD_RATE    0
// ------------------------------------ 自动波特率 ------------------------------------

#define WIRELESS_UART_BUFFER_SIZE       64
#define WIRELESS_UART_TIMEOUT_COUNT     0x64


//函数定义

/*无线串口发送一个字节*/
uint32 wireless_uart_putchar (uint8 data);
/*无线串口读取一个字节*/
uint32 wireless_uart_getchar (uint8 data);
/*无线串口发送字符串*/
uint32 wireless_uart_putbuff ( uint8 *buff, uint32 len);
/*无线串口发送字符串*/
uint32 wireless_uart_putstr ( uint8 *str);
/*无线串口发送图像*/
uint8 wireless_uart_putimage ( uint8 *image_addr, uint32 image_size);
/*无线串口初始化*/
uint8 wireless_uart_init (void);


#endif
