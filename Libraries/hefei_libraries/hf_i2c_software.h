/*********************************************************************************************************************
 * @file            hf_i2c_software.h
 * @brief           软件IIC配置
 * @author          Fantastic Potato
 * @version         1.0
 * @Target core     CH32V307VCT6
 * @date            2022-3-5
 ********************************************************************************************************************/

#ifndef _hf_i2csoftware_h
#define _hf_i2csoftware_h

#include <stdio.h>
#include "hf_gpio.h"
#include "delay.h"

/* 宏定义 */

/* IIC总线规定，IIC空闲时，SCL、SDA都为高电平，最好外部上拉（一定不能下拉）
 * SDA/SCL的GPIO初始化必须为开漏输出 */
#define sim_scl A8
#define sim_sda A11

#define scl_clr() gpio_bit_output(sim_scl, 0)
#define scl_set() gpio_bit_output(sim_scl, 1)

#define sda_clr() gpio_bit_output(sim_sda, 0)
#define sda_set() gpio_bit_output(sim_sda, 1)

#define sda_get()   gpio_get_input(sim_sda)
#define scl_get()   gpio_get_input(sim_scl)

#define delay_time 20                         //等待时间由芯片数据手册时序图确定

/* 函数声明 */

/* 外部函数，用户可自行调用 */
/* 主机写一个字节 */
void i2c_sim_send_byte(uint8 i2c_byte);      //默认高位在前
/* 主机读一个字节 */
uint8 i2c_sim_read_byte(void);               //默认高位在前
/* I2C读取单字节 */
uint8 i2c_sim_read_reg_byte(uint8 write_add, uint8 read_add,uint8 reg_add);
/* I2C向寄存器写入一个字节 */
void i2c_sim_write_reg_byte(uint8 write_add, uint8 reg_add, uint8 dat);

/* 内部函数，用户无需关注 */
/* SDA,SCL置空闲状态 */
void i2c_sim_idle(void);
/* 发送起始信号 */
void i2c_sim_start(void);
/* 发送停止信号 */
void i2c_sim_stop(void);
/* 主机等待应答信号 */
uint8 i2c_sim_waitack(void);
/* 主机产生应答信号ACK */
void i2c_sim_ack(void);
/* 主机产生非应答信号NACK */
void i2c_sim_nack(void);

#endif
