/*********************************************************************************************************************
 * @file            hf_uart.h
 * @brief           UART配置
 * @author          FengGuorong
 * @version         1.0
 * @Target core     CH32V307VCT6
 * @date            2022-3-10
 ********************************************************************************************************************/

#ifndef HF_UART_H_
#define HF_UART_H_

#include "common.h"
#include "ch32v30x.h"
#include "ch32v30x_misc.h"
#include "ch32v30x_gpio.h"
#include "ch32v30x_rcc.h"
#include "ch32v30x_usart.h"
#include "hf_gpio.h"
#include "hf_nvic.h"

/* 枚举类型定义 */
/* 枚举UART引脚 */
typedef enum        //初始化的时候，必须使用同一组引脚例如，UART1_TX_A9和UART1_RX_A10是一组引脚
{
    //----------UART1-----------//
    UART1_TX_A9  = 0x00,    UART1_RX_A10,   //  默认引脚
    UART1_TX_B6,            UART1_RX_B7,    //  重映射
    //UART1_TX_B15,
    //----------UART2-----------//
    UART2_TX_A2  = 0x10,    UART2_RX_A3,    //  默认引脚
    UART2_TX_D5,            UART2_RX_D6,    //  重映射

    //----------UART3-----------//
    UART3_TX_B10 = 0x20,    UART3_RX_B11,   //  默认引脚
    UART3_TX_C10,           UART3_RX_C11,   //  部分重映射
    UART3_TX_D8,            UART3_RX_D9,    //  完全重映射

    //----------UART4-----------//
    UART4_TX_C10 = 0x40,    UART4_RX_C11,   //  默认引脚
    UART4_TX_B0,            UART4_RX_B1,

    //----------UART5-----------//
    UART5_TX_C12 = 0x50,    UART5_RX_D2,    //  默认引脚
    UART5_TX_E8,            UART5_RX_E9,
    //----------UART6-----------//
    UART6_TX_C0  = 0x60,    UART6_RX_C1,    //  默认引脚
    UART6_TX_E10,           UART6_RX_E11,
    //----------UART7-----------//
    UART7_TX_C2  = 0x70,    UART7_RX_C3,    //  默认引脚
    UART7_TX_A6,            UART7_RX_A7,
    UART7_TX_E12,           UART7_RX_E13,
    //----------UART8-----------//
    UART8_TX_C4  = 0x80,    UART8_RX_C5,     //  默认引脚
    UART8_TX_E14,           UART8_RX_E15,
    //8个串口 只有123有硬件RTS CTS引脚 45678没有
}UARTPIN_enum;

/* 枚举UART */
typedef enum
{
    UART_1 = 0,
    UART_2,
    UART_3,
    UART_4,
    UART_5,
    UART_6,
    UART_7,
    UART_8
}UARTN_enum;

/* 枚举UART中断方式 */
typedef enum
{
    UARTINT_RX = 0,
    UARTINT_TX,
    UARTINT_BOTH
}UARTNINT_enum;

/* 函数声明 */

/* 外部函数，用户可自行调用 */
/* 串口初始化 */
void uart_init(UARTN_enum UART_x, uint32 baud, UARTPIN_enum tx_pin, UARTPIN_enum rx_pin);
/* 串口发送一个字节 */
void uart_putchar(UARTN_enum UART_x, uint8 dat);
/* 串口发送数组 */
void uart_putbuff(UARTN_enum UART_x, uint8 *buff, uint32 len);
/* 串口发送字符串 */
void uart_putstr(UARTN_enum UART_x, const uint8 *str);
/* 读取串口接收的数据（查询接收） */
uint8 uart_getchar(UARTN_enum UART_x);
/* 读取串口接收的数据（whlie等待） */
uint8 uart_query(UARTN_enum UART_x, uint8 *dat);
/* 打开串口发送接收中断 */
void uart_interrupt_init(UARTN_enum UART_x,uint8 status, UARTNINT_enum UARTINT_xx);

/* 内部函数，用户无需关注 */
/* 串口引脚初始化 */
void uart_gpio_init(UARTPIN_enum tx_pin, UARTPIN_enum rx_pin);


#endif /* HF_UART_H_ */
