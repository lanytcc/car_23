/*********************************************************************************************************************
 * @file            hf_uart.c
 * @brief           UART配置
 * @author          FengGuorong
 * @version         1.0
 * @Target core     CH32V307VCT6
 * @date            2022-3-10
 ********************************************************************************************************************/

#include "hf_uart.h"

/* 内部变量定义 */
const uint32 UARTN[] = {USART1_BASE, USART2_BASE, USART3_BASE, UART4_BASE, UART5_BASE, UART6_BASE, UART7_BASE, UART8_BASE};
const uint8_t uart_irq[] = {USART1_IRQn, USART2_IRQn, USART3_IRQn, UART4_IRQn, UART5_IRQn, UART6_IRQn, UART7_IRQn, UART8_IRQn};

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口初始化
//  @param      UART_x       串口通道
//  @param      baud        波特率
//  @param      tx_pin      串口发送引脚号
//  @param      rx_pin      串口接收引脚号
//  @return     void
//  Sample usage:           uart_init(UART_1, 115200, UART1_TX_A9, UART1_RX_A10); //串口1初始化引脚号,TX为A9,RX为A10
//-------------------------------------------------------------------------------------------------------------------
void uart_init(UARTN_enum UART_x, uint32 baud, UARTPIN_enum tx_pin, UARTPIN_enum rx_pin)
{
    USART_InitTypeDef USART_InitStructure;

    // AFIO总线开启
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    // 端口重映射
    if(UART1_TX_B6 == tx_pin && UART1_RX_B7 == rx_pin)
        GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
    else if(UART2_TX_D5 == tx_pin && UART2_RX_D6 == rx_pin)
        GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
    else if(UART3_TX_C10 == tx_pin && UART3_RX_C11 == rx_pin)
        GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);
    else if(UART3_TX_D8 == tx_pin && UART3_RX_D9 == rx_pin)
        GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);
    else if(UART4_TX_B0 == tx_pin && UART4_RX_B1 == rx_pin)
        GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);
    else if(UART5_TX_E8 == tx_pin && UART5_RX_E9 == rx_pin)
        GPIO_PinRemapConfig(GPIO_Remap_TIM5CH4_LSI, ENABLE);
    else if(UART6_TX_E10 == tx_pin && UART6_RX_E11 == rx_pin)
        GPIO_PinRemapConfig(GPIO_Remap_TIM67_DAC_DMA, ENABLE);

    else if(UART7_TX_A6 == tx_pin && UART7_RX_A7 == rx_pin)
        AFIO->PCFR2 |= 0x01<<22;                       //[23:22]
    else if(UART7_TX_E12 == tx_pin && UART7_RX_E13 == rx_pin)
        AFIO->PCFR2 |= 0x01<<23;                       //[23:22]

    else if(UART8_TX_E14 == tx_pin && UART8_RX_E15 == rx_pin)
        AFIO->PCFR2 |= 1<<25;    //[25:24]

    //  串口总线使能
    if(UART_1 == UART_x) RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    else if(UART_2 == UART_x) RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    else if(UART_3 == UART_x) RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    else if(UART_4 == UART_x) RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
    else if(UART_5 == UART_x) RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
    else if(UART_6 == UART_x) RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART6, ENABLE);
    else if(UART_7 == UART_x) RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7, ENABLE);
    else if(UART_8 == UART_x) RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8, ENABLE);

    //引脚初始化
    uart_gpio_init(tx_pin, rx_pin);

    //串口参数配置
    USART_InitStructure.USART_BaudRate = baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    //使能串口
    USART_Init((USART_TypeDef*)UARTN[UART_x], &USART_InitStructure);
    USART_Cmd((USART_TypeDef*)UARTN[UART_x], ENABLE);

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口发送一个字节
//  @param      UART_x       串口通道
//  @param      dat         串口数据
//  @return     void
//  Sample usage:           uart_putchar(UART_1, 0x43);        //串口1发送0x43。
//-------------------------------------------------------------------------------------------------------------------
void uart_putchar(UARTN_enum UART_x, uint8 dat)
{
    while((((USART_TypeDef*)UARTN[UART_x])->STATR & USART_FLAG_TXE)==0);
    ((USART_TypeDef*)UARTN[UART_x])->DATAR = dat;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口发送数组
//  @param      UART_x       串口通道
//  @param      buff        要发送的数组地址
//  @param      len         数据长度
//  @return     void
//  Sample usage:           uart_putbuff(UART_1, buff, 10);     //串口1发送10个buff数组。
//-------------------------------------------------------------------------------------------------------------------
void uart_putbuff(UARTN_enum UART_x, uint8 *buff, uint32 len)
{
    while(len--)
        uart_putchar(UART_x, *buff++);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口发送字符串
//  @param      UART_x       串口通道
//  @param      str         字符串首地址
//  @return     void
//  Sample usage:           uart_putstr(UART_1, (uint8 *)"12345")   //串口1发送12345这个字符串
//-------------------------------------------------------------------------------------------------------------------
void uart_putstr(UARTN_enum UART_x, const uint8 *str)
{
    while(*str)
    {
        uart_putchar(UART_x, *str++);
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      打开串口发送接收中断
//  @param      UART_x       串口通道
//  @param      status      使能或者失能
//  @param      UARTINT_xx  中断类型
//                  UARTINT_RX = 0,
//                  UARTINT_TX,
//                  UARTINT_BOTH
//  @return     void
//  Sample usage:           uart_tx_irq(UART_1, DISABLE);       //关闭串口1发送 中断
//-------------------------------------------------------------------------------------------------------------------
void uart_interrupt_init(UARTN_enum UART_x,uint8 status, UARTNINT_enum UARTINT_xx)
{
    if(UARTINT_RX == UARTINT_xx)
    {
        USART_ITConfig(((USART_TypeDef*)UARTN[UART_x]), USART_IT_RXNE, status);

        NVIC->IPRIOR[(uint32_t)((IRQn_Type)uart_irq[UART_x])] = 0;

        if(status)  interrupt_enable((IRQn_Type)uart_irq[UART_x]);
        else        interrupt_disable((IRQn_Type)uart_irq[UART_x]);
    }
    else if(UARTINT_TX == UARTINT_xx)
    {
        USART_ITConfig(((USART_TypeDef*)UARTN[UART_x]), USART_IT_TXE, status);

        NVIC->IPRIOR[(uint32_t)((IRQn_Type)uart_irq[UART_x])] = 0;

        if(status)  interrupt_enable((IRQn_Type)uart_irq[UART_x]);
        else        interrupt_disable((IRQn_Type)uart_irq[UART_x]);
    }
    else if (UARTINT_BOTH == UARTINT_xx)
    {
        USART_ITConfig(((USART_TypeDef*)UARTN[UART_x]), USART_IT_RXNE, ENABLE);
        nvic_init(USART1_IRQn + UART_x, 0, 0, status);
        USART_ITConfig(((USART_TypeDef*)UARTN[UART_x]), USART_IT_TXE, ENABLE);
        nvic_init(USART1_IRQn + UART_x, 0, 1, status);
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取串口接收的数据（whlie等待）
//  @param      UART_x           串口模块号(UART_1,UART_2,UART_3,UART_4)
//  @param      *dat            接收数据的地址
//  @return     void
//  Sample usage:               uint8 dat; uart_getchar(USART_1,&dat);       // 接收串口1数据  存在在dat变量里
//-------------------------------------------------------------------------------------------------------------------
uint8 uart_getchar(UARTN_enum UART_x)
{
    uint8 dat;
    while((((USART_TypeDef*)UARTN[UART_x])->STATR & USART_FLAG_RXNE) == 0);
    dat = (uint8_t)(((USART_TypeDef*)UARTN[UART_x])->DATAR & (uint16_t)0xFF);
    return dat;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取串口接收的数据（查询接收）
//  @param      UART_x           串口模块号(UART_1 - UART_8)
//  @param      *dat            接收数据的地址
//  @return     uint8           1：接收成功   0：未接收到数据
//  Sample usage:               uint8 dat; uart_query(USART_1,&dat);       // 接收串口1数据  存在在dat变量里
//-------------------------------------------------------------------------------------------------------------------
uint8 uart_query(UARTN_enum UART_x, uint8 *dat)
{
    if((((USART_TypeDef*)UARTN[UART_x])->STATR & USART_FLAG_RXNE) != 0)
    {
        *dat = (uint16_t)(((USART_TypeDef*)UARTN[UART_x])->DATAR & (uint16_t)0xFF);
        return 1;
    }
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      串口引脚初始化
//  @param      tx_pin      串口发送引脚号
//  @param      rx_pin      串口接收引脚号
//  @return     void
//  Sample usage:           内部使用，用户无需关心
//-------------------------------------------------------------------------------------------------------------------
void uart_gpio_init(UARTPIN_enum tx_pin, UARTPIN_enum rx_pin)
{
    if(tx_pin == UART1_TX_A9 && rx_pin == UART1_RX_A10)
    {
        gpio_init(A9, GPO, 0, OUT_AF_PP);
        gpio_init(A10, GPI, 0, IN_FLOAT);
    }
    else if(tx_pin == UART1_TX_B6)
    {
        gpio_init(B6, GPO, 0, OUT_AF_PP);
        gpio_init(B7, GPI, 0, IN_FLOAT);

    }
    else if(tx_pin == UART2_TX_A2)
    {
        gpio_init(A2, GPO, 0, OUT_AF_PP);
        gpio_init(A3, GPI, 0, IN_FLOAT);

    }
    else if(tx_pin == UART2_TX_D5)
    {
        gpio_init(D5, GPO, 0, OUT_AF_PP);
        gpio_init(D6, GPI, 0, IN_FLOAT);

    }
    else if(tx_pin == UART3_TX_B10)
    {
        gpio_init(B10, GPO, 0, OUT_AF_PP);
        gpio_init(B11, GPI, 0, IN_FLOAT);

    }
    else if(tx_pin == UART3_TX_C10)
    {
        gpio_init(C10, GPO, 0, OUT_AF_PP);
        gpio_init(C11, GPI, 0, IN_FLOAT);
    }
    else if(tx_pin == UART3_TX_D8)
    {
        gpio_init(D8, GPO, 0, OUT_AF_PP);
        gpio_init(D9, GPI, 0, IN_PULLUP);
    }
    else if(tx_pin == UART4_TX_C10)
    {
        gpio_init(C10, GPO, 0, OUT_AF_PP);
        gpio_init(C11, GPI, 0, IN_FLOAT);
    }
    else if(tx_pin == UART4_TX_B0)
    {
        gpio_init(B0, GPO, 0, OUT_AF_PP);
        gpio_init(B1, GPI, 0, IN_FLOAT);
    }
    else if(tx_pin == UART5_TX_C12)
    {
        gpio_init(C12, GPO, 0, OUT_AF_PP);
        gpio_init(D2, GPI, 0, IN_FLOAT);
    }
    else if(tx_pin == UART5_TX_E8)
    {
        gpio_init(E8, GPO, 0, OUT_AF_PP);
        gpio_init(E9, GPI, 0, IN_FLOAT);
    }
    else if(tx_pin == UART6_TX_C0)
    {
        gpio_init(C0, GPO, 0, OUT_AF_PP);
        gpio_init(C1, GPI, 0, IN_PULLUP);
    }
    else if(tx_pin == UART6_TX_E10)
    {
        gpio_init(E10, GPO, 0, OUT_AF_PP);
        gpio_init(E11, GPI, 0, IN_FLOAT);
    }
    else if(tx_pin == UART7_TX_C2)
    {
        gpio_init(C2, GPO, 0, OUT_AF_PP);
        gpio_init(C3, GPI, 0, IN_FLOAT);
    }
    else if(tx_pin == UART7_TX_A6)
    {
        gpio_init(A6, GPO, 0, OUT_AF_PP);
        gpio_init(C3, GPI, 0, IN_FLOAT);
    }
    else if(tx_pin == UART7_TX_E12)
    {
        gpio_init(E12, GPO, 0, OUT_AF_PP);
        gpio_init(E13, GPI, 0, IN_FLOAT);
    }
    else if(tx_pin == UART8_TX_C4)
    {
        gpio_init(C4, GPO, 0, OUT_AF_PP);
        gpio_init(C5, GPI, 0, IN_FLOAT);
    }
    else if(tx_pin == UART8_TX_E14)
    {
        gpio_init(E14, GPO, 0, OUT_AF_PP);
        gpio_init(E15, GPI, 0, IN_FLOAT);
    }

}
