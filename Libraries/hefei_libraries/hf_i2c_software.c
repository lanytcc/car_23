/*********************************************************************************************************************
 * @file            hf_i2c_software.c
 * @brief           软件IIC配置
 * @author          Fantastic Potato
 * @version         1.0
 * @Target core     CH32V307VCT6
 * @date            2022-3-5
 ********************************************************************************************************************/

#include "hf_i2c_software.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      主机写一个字节
//  @return     void
//  Sample usage:       i2c_sim_send_byte();
//-------------------------------------------------------------------------------------------------------------------
void i2c_sim_send_byte(uint8 i2c_byte)
{
    uint8 i;
    for( i = 0; i < 8; i++)
    {
        scl_clr();          //SCL为低电平
        if((i2c_byte & 0x80) >> 7) //高位在前
        {
            sda_set();      //SDA为高电平
        }
        else
        {
            sda_clr();      //SDA为低电平
        }
        Delay_Us(delay_time);
        i2c_byte = i2c_byte << 1;
        scl_set();          //SCL为高电平
        Delay_Us(delay_time);
        scl_clr();          //SCL为低电平

        if (i == 7)
        {
            sda_set();    //控制SDA线输出高电平，释放总线，等待接收方应答信号
        }

        Delay_Us(delay_time);
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      主机读一个字节
//  @return     void
//  Sample usage:       i2c_sim_read_byte();
//-------------------------------------------------------------------------------------------------------------------
uint8 i2c_sim_read_byte(void)
{
    uint8 i;
    uint8 value = 0;

    sda_set();    //控制SDA线输出高电平，释放总线，等待接收方应答信号
    scl_clr();              //SCL为低电平
    Delay_Us(delay_time);

    //读到第1个bit为数据的bit7
    for( i = 0; i < 8; i++)
    {
        value <<= 1;
        scl_set();          //SCL为高电平
        Delay_Us(delay_time);

        if (sda_get())      //利用延时函数在SCL为高电平期间读取SDA线电平逻辑
         {
             value++;
         }

        scl_clr();          //SCL为低电平
        Delay_Us(delay_time);
    }

    return value;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief        I2C读取单字节
// @param        write_add  : 从机写入地址
// @param        read_add   : 从机读取地址
// @param        reg_add    : 从机寄存器地址
// @return       从寄存器中读取到的数据
// Sample usage:        i2c_sim_read_reg_byte(0x02, 0x03, 0x10)
//                      //向从机读取地址0x03的寄存器地址0x10读取地址
//-------------------------------------------------------------------------------------------------------------------
uint8 i2c_sim_read_reg_byte(uint8 write_add, uint8 read_add,uint8 reg_add)
{
    uint8 ReData = 0;                 //定义变量接收数据

    i2c_sim_start();                          //启动IIC

    i2c_sim_send_byte(write_add);//发送写命令


    if(!(i2c_sim_waitack()))  //发送写命令并检查应答位
    {
        i2c_sim_send_byte(reg_add);          //确定要操作的寄存器
        i2c_sim_waitack();

        i2c_sim_start();                      //重启总线

        i2c_sim_send_byte(read_add);     //发送读取命令
        i2c_sim_waitack();

        ReData = i2c_sim_read_byte();        //读取数据
        i2c_sim_nack();                       //发送非应答信号结束数据传送
    }
    i2c_sim_stop();                           //关闭IIC
    return ReData;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief        I2C向寄存器写入一个数据
// @param        write_add  : 从机写入地址
// @param        reg_add    : 从机寄存器地址
// @param        dat        : 写入的数据
// @return       void
// Sample usage:    i2c_sim_write_reg_byte(0x02, 0x03, 0x10)
//                  //向从机写入地址0x02的寄存器地址0x03写入数据0x10
//-------------------------------------------------------------------------------------------------------------------
void i2c_sim_write_reg_byte(uint8 write_add, uint8 reg_add, uint8 dat)
{
    i2c_sim_start();                          //启动IIC
    i2c_sim_send_byte(write_add);//发送写命令
    if(!(i2c_sim_waitack()))                //检查应答位
    {
        i2c_sim_send_byte(reg_add);          //写入地址
        i2c_sim_waitack();
        i2c_sim_send_byte(dat);              //发送数据
        i2c_sim_waitack();
    }
    i2c_sim_stop();                           //关闭IIC
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      SDA,SCL置空闲状态
//  @return     void
//  Sample usage:       内部函数，用户无需关注
//-------------------------------------------------------------------------------------------------------------------
void i2c_sim_idle(void)
{
    scl_set();          //SCL为高电平
    sda_set();          //SDA为高电平
    Delay_Us(delay_time);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      发送起始信号
//  @return     void
//  Sample usage:       内部函数，用户无需关注
//-------------------------------------------------------------------------------------------------------------------
void i2c_sim_start(void)
{
    scl_set();          //SCL为高电平
    sda_set();          //SDA为高电平
    Delay_Us(delay_time);

    sda_clr();          //SDA为低电平
    Delay_Us(delay_time);
    scl_clr();          //SCL为低电平
    Delay_Us(delay_time);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      发送停止信号
//  @return     void
//  Sample usage:       内部函数，用户无需关注
//-------------------------------------------------------------------------------------------------------------------
void i2c_sim_stop()
{
    scl_set();          //SCL为高电平
    sda_clr();          //SDA为低电平

    Delay_Us(delay_time);

    sda_set();          //SDA为高电平
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      主机等待应答信号
//  @return     1:NACK信号, 0:ACK信号
//  Sample usage:       内部函数，用户无需关注
//-------------------------------------------------------------------------------------------------------------------
uint8 i2c_sim_waitack(void)
{
    uint8 ack_flag;

    sda_set();          //SDA为高电平,释放SDA总线,由接收端控制SDA线
    Delay_Us(delay_time);
    scl_set();          //SCL为高电平,等待SDA响应,若SDA线为高电平，表示NACK信号，反之则为ACK信号
    Delay_Us(delay_time);

    if(sda_get())       //读取SDA线状态判断响应类型，高电平,为NACK信号，反之则为ACK信号
    {
        ack_flag = 1;   //NACK信号
    }
    else
    {
        ack_flag = 0;   //ACK信号
    }

    scl_clr();          //SCL为低电平
    Delay_Us(delay_time);

    return ack_flag;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      主机产生应答信号ACK
//  @return     void
//  Sample usage:       内部函数，用户无需关注
//-------------------------------------------------------------------------------------------------------------------
void i2c_sim_ack(void)
{
    sda_clr();          //SDA为低电平
    Delay_Us(delay_time);
    scl_set();          //SCL为高电平期间读取SDA线为低电平，则为ACK响应
    Delay_Us(delay_time);
    scl_clr();          //SCL为低电平
    Delay_Us(delay_time);
    sda_set();          //SDA为高电平
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      主机产生非应答信号NACK
//  @return     void
//  Sample usage:       内部函数，用户无需关注
//-------------------------------------------------------------------------------------------------------------------
void i2c_sim_nack(void)
{
    sda_set();          //SDA为低电平
    Delay_Us(delay_time);
    scl_set();          //SCL线为高电平期间读取SDA线为高电平,则为NACK响应
    Delay_Us(delay_time);
    scl_clr();          //SCL为低电平
    Delay_Us(delay_time);
}
