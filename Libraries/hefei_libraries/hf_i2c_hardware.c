/*********************************************************************************************************************
 * @file            hf_i2c_hardware.c
 * @brief           硬件IIC配置
 * @author          Fantastic Potato
 * @version         1.0
 * @Target core     CH32V307VCT6
 * @date            2022-3-5
 ********************************************************************************************************************/

#include "hf_i2c_hardware.h"

/* 宏定义 */
#define SIZE sizeof(TEXT_Buffer)

/* 内部变量声明 */
const u8 TEXT_Buffer[]={""};

//-------------------------------------------------------------------------------------------------------------------
//  @brief      硬件I2C初始化
// @param        I2Cx(x=0,1)                选择I2C通道
//  @param      address                     I2C设备自身地址
//  @return     void
//  Sample usage:                           i2c_hardware_init(I2C1,1000, 0x02); //I2C1,设置为1KHz,地址为0x02
//-------------------------------------------------------------------------------------------------------------------
void i2c_hardware_init(I2C_TypeDef* I2Cx, u16 address)
{
    I2C_InitTypeDef I2C_InitTSturcture;

    uint32_t i2c_rcc;

    /* GPIO时钟初始化 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE );

    /* I2C参数初始化 */
    if (I2Cx == I2C1)           //选择I2C1
    {
        i2c_rcc = RCC_APB1Periph_I2C1;

        /* GPIO初始化 */
        gpio_init(B6, GPO, 1, SPEED_50MHZ | OUT_AF_OD);
        gpio_init(B7, GPO, 1, SPEED_50MHZ | OUT_AF_OD);
    }
    else   //选择I2C2
    {
        i2c_rcc = RCC_APB1Periph_I2C2;

        /* GPIO初始化 */
        gpio_init(B10, GPO, 1, SPEED_50MHZ | OUT_AF_OD);
        gpio_init(B11, GPO, 1, SPEED_50MHZ | OUT_AF_OD);
    }

    /* I2C时钟初始化 */
    RCC_APB1PeriphClockCmd( i2c_rcc, ENABLE );

    /* I2C1初始化 */
    I2C_InitTSturcture.I2C_ClockSpeed = 400000;              //波特率
    I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;             //配置为普通I2C工作模式
    I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_2;     //指定I2C快速模式占空比
    I2C_InitTSturcture.I2C_OwnAddress1 = address;           //指定I2C自身设备地址
    I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;            //使能或者关闭响应 (一般都是使能)
    I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;  //指定地址的长度,可以是7位或10位
    I2C_Init( I2Cx, &I2C_InitTSturcture );
    I2C_Cmd( I2Cx, ENABLE );

    I2C_AcknowledgeConfig( I2Cx, ENABLE );
}

//-------------------------------------------------------------------------------------------------------------------
// @brief        I2C发送单字节
// @param        I2Cx(x=0,1)                选择I2C通道
// @param        slave_address              从机地址
// @param        data                       发送的字节
// @return        void
// Sample usage:        i2c_send_byte(I2C1, 0x21);
//                      I2C1开始通信,发送数据0x21
//-------------------------------------------------------------------------------------------------------------------
void i2c_send_byte(I2C_TypeDef * I2Cx, uint8_t slave_address, uint8_t data)
{
    /* 等待I2C总线空闲 */
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));

    /* 发送起始信号 */
    I2C_GenerateSTART(I2Cx, ENABLE);

    /* 选择I2C作为主机模式 */
    while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_MODE_SELECT ) );

    /* 发送从机地址 */
    I2C_Send7bitAddress( I2Cx, slave_address, I2C_Direction_Transmitter );

    /* 等待地址发送完毕 */
    while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );

    /* 发送字节数据 */
    if( I2C_GetFlagStatus( I2Cx, I2C_FLAG_TXE ) !=  RESET )
    {
        I2C_SendData( I2Cx, data );
    }

    /* 等待发送完毕 */
    while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

    /* I2C通信停止信号  */
    I2C_GenerateSTOP( I2Cx, ENABLE );

    /* 检查停止信号是否发送完毕 */
//    while( !I2C_CheckEvent( I2Cx, I2C_EVENT_SLAVE_STOP_DETECTED ) );
}

//-------------------------------------------------------------------------------------------------------------------
// @brief        I2C向寄存器地址写入单字节
// @param        I2Cx(x=0,1)           选择I2C通道
// @param        write_address         写入从机地址
// @param        reg_address           寄存器地址
// @param        data                  发送的字节
// @return       void
// Sample usage:        i2c_send_reg_byte(0x34, 0x21);
//                      //I2C_PERIPH开始通信,发送寄存器地址0x34,发送数据0x21
//-------------------------------------------------------------------------------------------------------------------
void i2c_send_reg_byte(I2C_TypeDef * I2Cx, uint8_t write_address, uint8_t reg_address, uint8_t data)
{
    /* 等待I2C总线空闲 */
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));

    /* 发送起始信号 */
    I2C_GenerateSTART(I2Cx, ENABLE);

    /* 选择I2C作为主机模式 */
    while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_MODE_SELECT ) );

    /* 发送从机地址 */
    I2C_Send7bitAddress( I2Cx, write_address, I2C_Direction_Transmitter );

    /* 等待地址发送完毕 */
    while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );

    /* 8位数据地址 */
#if (Address_Lenth  == Address_8bit)

    I2C_SendData( I2Cx, (uint8_t)(reg_address&0x00FF) );
    while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

    /* 16位数据地址 */
#elif (Address_Lenth  == Address_16bit)

    I2C_SendData( I2Cx, (u8)(reg_address>>8) );
    while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );
    I2C_SendData( I2Cx, (u8)(reg_address&0x00FF) );
    while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );
#endif

    /* 发送字节数据 */
    if( I2C_GetFlagStatus( I2Cx, I2C_FLAG_TXE ) !=  RESET )
    {
        I2C_SendData( I2Cx, data );
    }

    /* 等待发送完毕 */
    while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );


    /* I2C通信停止信号  */
    I2C_GenerateSTOP( I2Cx, ENABLE );

    /* 检查停止信号是否发送完毕 */
//    while( !I2C_CheckEvent( I2Cx, I2C_EVENT_SLAVE_STOP_DETECTED ) );
}

//-------------------------------------------------------------------------------------------------------------------
// @brief        I2C读取单字节
// @param        I2Cx(x=0,1)                选择I2C通道
// @param        write_address              写入从机地址，由数据手册确定，write_address可以与read_address一致
// @param        read_address               读取从机地址
// @param        reg_address                从机寄存器地址
// @param        data_buffer                数据存放地址
// @return        void
// Sample usage:        i2c_send_byte(I2C1, 0x21);
//                      I2C1开始通信,发送数据0x21
//-------------------------------------------------------------------------------------------------------------------
void i2c_read_reg_byte(I2C_TypeDef * I2Cx, uint8_t write_address, uint8_t read_address, uint8_t reg_address, uint8_t *data_buffer)
{
    /* 等待I2C总线空闲 */
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));

    /* 发送起始信号 */
    I2C_GenerateSTART(I2Cx, ENABLE);
    /* 选择I2C作为主机模式 */
    while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_MODE_SELECT ) );

    /* 发送从机地址、写  */
#if (Hard_Device == Hard_Rtc)
    I2C_Send7bitAddress( I2Cx, write_address, I2C_Direction_Transmitter );
    /* 等待地址发送完毕 */
    while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );
#endif

    /* 发送数据地址 */
#if (Address_Lenth  == Address_8bit)

        I2C_SendData( I2Cx, (uint8_t)(reg_address&0x00FF) );
        while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

#elif (Address_Lenth  == Address_16bit)

        I2C_SendData( I2Cx, (u8)(reg_address>>8) );
        while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );
        I2C_SendData( I2Cx, (u8)(reg_address&0x00FF) );
        while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

#endif

    /* 重新开始 */
    /* 发送起始信号 */
    I2C_GenerateSTART(I2Cx, ENABLE);
    /* 选择I2C作为主机模式 */
    while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_MODE_SELECT ) );

#if (Hard_Device == Hard_Rtc)
    /* 发送从机地址、读  */
    I2C_Send7bitAddress( I2Cx, read_address, I2C_Direction_Receiver );
    /* 等待地址发送完毕 */
    while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED ) );
#endif

    /* 读一字节数据 */
    I2C_AcknowledgeConfig(I2Cx, DISABLE);   //单字节读和多字节读的应答位，不连续读，产生非应答
    while(!I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE));
    *data_buffer = I2C_ReceiveData(I2Cx);          //读取数据

    /* I2C通信停止信号  */
    I2C_GenerateSTOP( I2Cx, ENABLE );
}

//-------------------------------------------------------------------------------------------------------------------
// @brief        I2C向寄存器连续写入
// @param        I2Cx(x=0,1)           选择I2C通道
// @param        write_address         写入从机地址
// @param        reg_address           从机寄存器地址
// @param        p_buffer              写入的字节存放的数组地址
// @param        length                写入的字节数量
// @return       void
// Sample usage:        uint8_t data[3]; i2c_send_reg_bytes(I2C1,0x75, 0x22, &data, 3);
//                      //I2C1向0x75的从机地址的0x22寄存器写入data数组，数组中有3个字节
//-------------------------------------------------------------------------------------------------------------------
void i2c_send_reg_bytes(I2C_TypeDef * I2Cx, uint8_t write_address, uint8_t reg_address, uint8_t *p_buffer, uint16_t length)
{
    /* 等待I2C总线空闲 */
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));

    /* 发送起始信号 */
    I2C_GenerateSTART(I2Cx, ENABLE);
    /* 选择I2C作为主机模式 */
    while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_MODE_SELECT ) );

    /* 发送从机地址、写  */
    I2C_Send7bitAddress( I2Cx, write_address, I2C_Direction_Transmitter );
    /* 等待地址发送完毕 */
    while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );

    /* 发送数据地址 */
#if (Address_Lenth  == Address_8bit)

        I2C_SendData( I2Cx, (uint8_t)(reg_address&0x00FF) );
        while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

#elif (Address_Lenth  == Address_16bit)

        I2C_SendData( I2Cx, (u8)(reg_address>>8) );
        while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );
        I2C_SendData( I2Cx, (u8)(reg_address&0x00FF) );
        while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

#endif

    /* 连续写数据 */
    for(uint16_t cnt=0; cnt<(length-1); cnt++)
    {
        /* 发送字节数据 */
        if( I2C_GetFlagStatus( I2Cx, I2C_FLAG_TXE ) !=  RESET )
        {
            I2C_SendData( I2Cx, *p_buffer );
        }
        /* 等待发送完毕 */
        while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );
        p_buffer++;
    }
    //最后一个字节独立出来，防止HardFault_Handler
    if( I2C_GetFlagStatus( I2Cx, I2C_FLAG_TXE ) !=  RESET )
    {
        I2C_SendData( I2Cx, *p_buffer );
    }
    while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

    /* I2C通信停止信号  */
    I2C_GenerateSTOP( I2Cx, ENABLE );
}



//-------------------------------------------------------------------------------------------------------------------
// @brief        I2C写入数组，连续写入
// @param        I2Cx(x=0,1)
// @param        write_address         写入从机地址
// @param        reg_address           从机寄存器地址
// @param        p_buffer              写入的字节存放的数组地址
// @param        length                写入的字节数量
// @return       void
// Sample usage:        uint8_t data[3]; i2c_send_reg_bytes(I2C1,0x75, 0x22, &data, 3);
//                      //I2C1向0x75的从机地址的0x22寄存器写入data数组，数组中有3个字节
//-------------------------------------------------------------------------------------------------------------------
/*
 * 页写的意思是在指向从机地址的页写数据，也就是EEPROM内部“地址指针”指向的地址所在页。
 * 每次写之前我们都要将“地址指针”指向一个地址（见下面源程序）写的过程中。
 * 一旦写到最后一个字节，将会回到该页首地址继续写下去，因此，写完该页，我们需要重新将“地址指针”指向下一页首地址 */
void i2c_send_reg_buffer(I2C_TypeDef * I2Cx, uint8_t write_address, uint8_t reg_address, uint8_t *p_buffer, uint16_t length)
{
    uint8_t number_of_page, number_of_single, address, count;
    number_of_page = 0;
    number_of_single = 0;
    address = 0;
    count = 0;

    address = reg_address % I2C_PAGE_SIZE;
    count = I2C_PAGE_SIZE - address;
    number_of_page = length / I2C_PAGE_SIZE;
    number_of_single = length % I2C_PAGE_SIZE;

    /* 如果写入地址与页大小对齐  */
    if (0 == address)
    {
        while (number_of_page--)
        {
            i2c_send_reg_bytes(I2Cx, write_address, reg_address, p_buffer, I2C_PAGE_SIZE);
            write_address += I2C_PAGE_SIZE;
            p_buffer += I2C_PAGE_SIZE;
        }
        if (0 != number_of_single)
        {
            i2c_send_reg_bytes(I2Cx, write_address, reg_address, p_buffer, I2C_PAGE_SIZE);
        }
    }
    else
    {
        /* 如果写入地址与页大小不对齐 */
        if (length < count)
        {
            i2c_send_reg_bytes(I2Cx, write_address, reg_address, p_buffer, count);
        }
        else
        {
            length -= count;
            number_of_page = length / I2C_PAGE_SIZE;
            number_of_single = length % I2C_PAGE_SIZE;
            if (0 != count)
            {
                i2c_send_reg_bytes(I2Cx, write_address, reg_address, p_buffer, count);
                write_address += count;
                p_buffer += count;
            }
            /* 写页面 */
            while (number_of_page--)
            {
                i2c_send_reg_bytes(I2Cx, write_address, reg_address, p_buffer, count);
                write_address += I2C_PAGE_SIZE;
                p_buffer += I2C_PAGE_SIZE;
            }
            /* 写单个字节 */
            if (0 != number_of_single)
            {
                i2c_send_reg_bytes(I2Cx, write_address, reg_address, p_buffer, count);
            }
        }
    }
}


//-------------------------------------------------------------------------------------------------------------------
// @brief        I2C读取数组
// @param        I2Cx(x=0,1)
// @param        write_address              写入从机地址，由数据手册确定，write_address可以与read_address一致
// @param        read_address               读取从机地址
// @param        reg_address                从机寄存器地址
// @param        data_buffer                数据存放地址
// @param        length                     读取的字节数量
// @return       void
// Sample usage:        uint8_t data[3]; i2c_read_buffer(I2C1, 0x68, 0x75, &data, 3);
//                      //I2C1读取从机地址为0x68的外设内部地址为0x70的寄存器，将其存放在data中
//-------------------------------------------------------------------------------------------------------------------
void i2c_read_reg_buffer(I2C_TypeDef * I2Cx, uint8_t write_address, uint8_t read_address, uint8_t reg_address, uint8_t *data_buffer, uint16_t length)
{
    /* 等待I2C总线空闲 */
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));

    /* 发送起始信号 */
    I2C_GenerateSTART(I2Cx, ENABLE);
    /* 选择I2C作为主机模式 */
    while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_MODE_SELECT ) );

    /* 发送从机地址、写  */
    I2C_Send7bitAddress( I2Cx, write_address, I2C_Direction_Transmitter );
    /* 等待地址发送完毕 */
    while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) );

    /* 发送数据地址 */
#if (Address_Lenth  == Address_8bit)

        I2C_SendData( I2Cx, (uint8_t)(reg_address&0x00FF) );
        while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

#elif (Address_Lenth  == Address_16bit)

        I2C_SendData( I2Cx, (u8)(reg_address>>8) );
        while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );
        I2C_SendData( I2Cx, (u8)(reg_address&0x00FF) );
        while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) );

#endif

    /* 重新开始 */
    /* 发送起始信号 */
    I2C_GenerateSTART(I2Cx, ENABLE);
    /* 选择I2C作为主机模式 */
    while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_MODE_SELECT ) );

    /* 发送从机地址、读  */
    I2C_Send7bitAddress( I2Cx, read_address, I2C_Direction_Receiver );
    /* 等待地址发送完毕 */
    while( !I2C_CheckEvent( I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED ) );

    /* 读多字节数据 */
    for(uint16_t cnt=0; cnt<(length-1); cnt++)
    {
        I2C_AcknowledgeConfig(I2Cx, ENABLE);    //单字节读和多字节读的应答位，连续读，产生应答
        while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE) == RESET);
        *data_buffer = I2C_ReceiveData(I2Cx);
    }
    I2C_AcknowledgeConfig(I2Cx, DISABLE);    //单字节读和多字节读的应答位，不连续读，不产生应答
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE) == RESET);
    *data_buffer = I2C_ReceiveData(I2Cx);

    /* I2C通信停止信号  */
    I2C_GenerateSTOP( I2Cx, ENABLE );
}
