/*********************************************************************************************************************
* @file            HF_MT9V03X.c
* @author          Fantastic Potato
* @version         1.0
* @Target core     CH32V307VCT6
* @date            20220905
 ********************************************************************************************************************/

#include "HF_MT9V03X.h"

uint8_t *camera_buffer_addr;                                                    // 摄像头缓冲区地址指针

uint8_t mt9v03x_finish_flag_dvp = 0;                                                // 一场图像采集完成标志位
uint8_t mt9v03x_image_dvp[MT9V03X_DVP_H][MT9V03X_DVP_W];

static          uint8_t     receive_dvp[3];
static          uint8_t     receive_num_dvp = 0;
static volatile uint8_t     uart_receive_flag_dvp;

//需要配置到摄像头的数据
int16_t mt9v03x_set_confing_buffer_dvp[CONFIG_FINISH][2]=
{
    {AUTO_EXP,          0},                                                     // 自动曝光设置       范围1-63 0为关闭 如果自动曝光开启  EXP_TIME命令设置的数据将会变为最大曝光时间，也就是自动曝光时间的上限
                                                                                // 一般情况是不需要开启这个功能，因为比赛场地光线一般都比较均匀，如果遇到光线非常不均匀的情况可以尝试设置该值，增加图像稳定性
    {EXP_TIME,          450},                                                   // 曝光时间         摄像头收到后会自动计算出最大曝光时间，如果设置过大则设置为计算出来的最大曝光值
    {FPS,               50},                                                    // 图像帧率         摄像头收到后会自动计算出最大FPS，如果过大则设置为计算出来的最大FPS
    {SET_COL,           MT9V03X_DVP_W},                                         // 图像列数量        范围1-752     K60采集不允许超过188
    {SET_ROW,           MT9V03X_DVP_H},                                         // 图像行数量        范围1-480
    {LR_OFFSET,         0},                                                     // 图像左右偏移量  正值 右偏移   负值 左偏移  列为188 376 752时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
    {UD_OFFSET,         0},                                                     // 图像上下偏移量  正值 上偏移   负值 下偏移  行为120 240 480时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
    {GAIN,              20},                                                    // 图像增益         范围16-64     增益可以在曝光时间固定的情况下改变图像亮暗程度
    {PCLK_MODE,         1},                                                     // 仅总钻风MT9V034 V1.5以及以上版本支持该命令，
                                                                                // 像素时钟模式命令 PCLK模式     默认：0     可选参数为：0 1。        0：不输出消隐信号，1：输出消隐信号。(通常都设置为0，如果使用CH32V307的DVP接口或STM32的DCMI接口采集需要设置为1)


    {INIT,              0}                                                      // 摄像头开始初始化
};

//从摄像头内部获取到的配置数据
int16_t mt9v03x_get_confing_buffer_dvp[CONFIG_FINISH-1][2]=
{
    {AUTO_EXP,          0},                                                     // 自动曝光设置
    {EXP_TIME,          0},                                                     // 曝光时间
    {FPS,               0},                                                     // 图像帧率
    {SET_COL,           0},                                                     // 图像列数量
    {SET_ROW,           0},                                                     // 图像行数量
    {LR_OFFSET,         0},                                                     // 图像左右偏移量
    {UD_OFFSET,         0},                                                     // 图像上下偏移量
    {GAIN,              0},                                                     // 图像增益
    {PCLK_MODE,         0},                                                     //像素时钟模式(仅总钻风MT9V034 V1.5以及以上版本支持该命令)
};

//-------------------------------------------------------------------------------------------------------------------
// @brief       配置摄像头内部配置信息 内部调用
// @param       uartn           选择使用的串口
// @param       buff            发送配置信息的地址
// @return      uint8_t         1-失败 0-成功
// Sample usage:                调用该函数前请先初始化串口
//-------------------------------------------------------------------------------------------------------------------
static uint8_t mt9v03x_set_config_dvp (int16_t buff[CONFIG_FINISH-1][2])
{
    uint16_t temp, i;
    uint8_t  send_buffer[4];
    int16_t timeout = MT9V03X_INIT_TIMEOUT;

    uart_receive_flag_dvp = 0;

    // 设置参数  具体请参看问题锦集手册
    // 开始配置摄像头并重新初始化
    for(i=0; i<CONFIG_FINISH; i++)
    {
        send_buffer[0] = 0xA5;
        send_buffer[1] = buff[i][0];
        temp = buff[i][1];
        send_buffer[2] = temp>>8;
        send_buffer[3] = (uint8_t)temp;
        uart_putbuff(MT9V03X_COF_UART_DVP,send_buffer,4);

        Delay_Ms(2);
    }

    while(!uart_receive_flag_dvp && timeout-- > 0)                                   // 等待接受回传数据
    {
        Delay_Ms(1);
    }
    timeout = MT9V03X_INIT_TIMEOUT;

    uart_receive_flag_dvp = 0;
    while(((0xff != receive_dvp[1]) || (0xff != receive_dvp[2])) && timeout-- > 0)   // 判断数据是否截取到对应内容
    {
        Delay_Ms(1);
    }

    // 以上部分对摄像头配置的数据全部都会保存在摄像头上51单片机的eeprom中
    // 利用set_exposure_time函数单独配置的曝光数据不存储在eeprom中
    if(timeout <= 0)                                                         // 超时
        return 1;
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       获取摄像头内部配置信息 内部调用
// @param       uartn           选择使用的串口
// @param       buff            接收配置信息的地址
// @return      uint8_t         1-失败 0-成功
// Sample usage:                调用该函数前请先初始化串口
//-------------------------------------------------------------------------------------------------------------------
static uint8_t mt9v03x_get_config_dvp (int16_t buff[CONFIG_FINISH-1][2])
{
    uint16_t temp, i;
    uint8_t  send_buffer[4];
    int16_t timeout = MT9V03X_INIT_TIMEOUT;

    for(i=0; i<(CONFIG_FINISH-1); i++)
    {
        send_buffer[0] = 0xA5;
        send_buffer[1] = GET_STATUS;
        temp = buff[i][0];
        send_buffer[2] = temp>>8;
        send_buffer[3] = (uint8_t)temp;
        uart_putbuff(MT9V03X_COF_UART_DVP,send_buffer,4);

        timeout = MT9V03X_INIT_TIMEOUT;
        while(!uart_receive_flag_dvp && timeout-- > 0)                              // 等待接受回传数据
        {
            Delay_Ms(1);
        }
        if(timeout <= 0 && i==0)                                                     // 超时
            return 1;

        uart_receive_flag_dvp = 0;
        buff[i][1] = receive_dvp[1]<<8 | receive_dvp[2];
    }
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       MT9V03X摄像头串口中断函数
// @param       void
// @return      void
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void mt9v03x_uart_callback_dvp (void)
{
//    receive_dvp[receive_num_dvp] = ((USART_TypeDef*)uart_index[MT9V03X_COF_UART_DVP])->DATAR & 0xFF;
//    receive_num_dvp++;

    if(uart_query(MT9V03X_COF_UART_DVP, &receive_dvp[receive_num_dvp]))
        receive_num_dvp++;

    if(1 == receive_num_dvp && 0XA5!=receive_dvp[0])
        receive_num_dvp = 0;
    if(3 == receive_num_dvp)
    {
        receive_num_dvp = 0;
        uart_receive_flag_dvp = 1;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       获取摄像头固件版本
// @param       void            选择使用的串口
// @return      uint16_t        0-获取错误 N-版本号
// Sample usage:                调用该函数前请先初始化串口
//-------------------------------------------------------------------------------------------------------------------
uint16_t mt9v03x_get_version_dvp (void)
{
    uint16_t temp;
    uint8_t  send_buffer[4];
    int16_t  timeout = MT9V03X_INIT_TIMEOUT;

    send_buffer[0] = 0xA5;
    send_buffer[1] = GET_STATUS;
    temp = GET_VERSION;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8_t)temp;
    uart_putbuff(MT9V03X_COF_UART_DVP,send_buffer,4);

    while(!uart_receive_flag_dvp && timeout-- > 0)                                  // 等待接受回传数据
    {
        Delay_Ms(1);
    }
    uart_receive_flag_dvp = 0;

    if(timeout <= 0)                                                            // 超时
        return 0;
    return ((uint16_t)(receive_dvp[1]<<8) | receive_dvp[2]);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       单独设置摄像头曝光时间
// @param       uartn           选择使用的串口
// @param       light           设置曝光时间越大图像越亮，摄像头收到后会根据分辨率及FPS计算最大曝光时间如果设置的数据过大，那么摄像头将会设置这个最大值
// @return      uint8_t         1-失败 0-成功
// Sample usage:                调用该函数前请先初始化串口
//-------------------------------------------------------------------------------------------------------------------
uint8_t mt9v03x_set_exposure_time_dvp (uint16_t light)
{
    uint16_t temp;
    uint8_t  send_buffer[4];
    int16_t timeout = MT9V03X_INIT_TIMEOUT;

    send_buffer[0] = 0xA5;
    send_buffer[1] = SET_EXP_TIME;
    temp = light;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8_t)temp;
    uart_putbuff(MT9V03X_COF_UART_DVP,send_buffer,4);

    while(!uart_receive_flag_dvp && timeout-- > 0)                                  // 等待接受回传数据
    {
        Delay_Ms(1);
    }
    uart_receive_flag_dvp = 0;

    temp = receive_dvp[1]<<8 | receive_dvp[2];

    if(timeout <= 0 || temp != light)
        return 0;
    return 1;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       对摄像头内部寄存器进行写操作
// @param       uartn           选择使用的串口
// @param       addr            摄像头内部寄存器地址
// @param       data            需要写入的数据
// @return      uint8_t         1-失败 0-成功
// Sample usage:                调用该函数前请先初始化串口
//-------------------------------------------------------------------------------------------------------------------
uint8_t mt9v03x_set_reg_dvp (uint8_t addr, uint16_t data)
{
    uint16_t temp;
    uint8_t  send_buffer[4];
    int16_t timeout = MT9V03X_INIT_TIMEOUT;

    send_buffer[0] = 0xA5;
    send_buffer[1] = SET_ADDR;
    temp = addr;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8_t)temp;
    uart_putbuff(MT9V03X_COF_UART_DVP,send_buffer,4);

    Delay_Ms(10);
    send_buffer[0] = 0xA5;
    send_buffer[1] = SET_DATA;
    temp = data;
    send_buffer[2] = temp>>8;
    send_buffer[3] = (uint8_t)temp;
    uart_putbuff(MT9V03X_COF_UART_DVP,send_buffer,4);

    while(!uart_receive_flag_dvp && timeout-- > 0)                                  // 等待接受回传数据
    {
        Delay_Ms(1);
        timeout--;
    }
    uart_receive_flag_dvp = 0;

    temp = receive_dvp[1]<<8 | receive_dvp[2];
    if(timeout <= 0 || temp != data)
        return 0;
    return 1;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      MT9V03X摄像头场中断
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:               在isr.c里面先创建对应的中断函数，然后调用该函数(之后别忘记清除中断标志位)
//-------------------------------------------------------------------------------------------------------------------
void mt9v03x_handler_dvp(void)
{
    DVP->DMA_BUF0 = (uint32_t)camera_buffer_addr;       // 恢复DMA地址
    mt9v03x_finish_flag_dvp = 1;                        // 摄像头采集完成标志位置1
    DVP->CR0 |= RB_DVP_ENABLE;                          // 重新打开DVP，采集下一副图像
}



//-------------------------------------------------------------------------------------------------------------------
// @brief       MT9V03X摄像头初始化
// @param       void
// @return      uint8_t         1-失败 0-成功
// Sample usage:                使用FLEXIO接口采集摄像头
//-------------------------------------------------------------------------------------------------------------------
uint8_t mt9v03x_init_dvp (void)
{
    //串口 配置摄像头
    uart_init(UART_5, 9600, UART5_TX_C12, UART5_RX_D2);
    //uart_rx_interrupt(MT9V03X_COF_UART_DVP, ENABLE);
    uart_interrupt_init(UART_5,ENABLE, UARTINT_RX);

    interrupt_global_enable();

    uart_receive_flag_dvp = 0;
    if(mt9v03x_get_config_dvp(mt9v03x_get_confing_buffer_dvp))
    {
        assert(0);
        return 1;
    }

    uart_receive_flag_dvp = 0;
    if(mt9v03x_set_config_dvp(mt9v03x_set_confing_buffer_dvp))
    {
        assert(0);
        return 1;
    }

    uart_receive_flag_dvp = 0;
    if(mt9v03x_get_config_dvp(mt9v03x_get_confing_buffer_dvp))
    {
        assert(0);
        return 1;
    }

    interrupt_global_disable();

    // DVP引脚初始化
    dvp_gpio_init(
            MT9V03X_D0_PIN_DVP, MT9V03X_D1_PIN_DVP, MT9V03X_D2_PIN_DVP, MT9V03X_D3_PIN_DVP,
            MT9V03X_D4_PIN_DVP, MT9V03X_D5_PIN_DVP, MT9V03X_D6_PIN_DVP, MT9V03X_D7_PIN_DVP,
            MT9V03X_PCLK_PIN_DVP, MT9V03X_VSY_PIN_DVP, MT9V03X_HERF_PIN_DVP);
    // 设置地址
    camera_buffer_addr = mt9v03x_image_dvp[0];

    // DVP接口初始化
    dvp_camera_init((uint32_t *)camera_buffer_addr, NULL, MT9V03X_DVP_W*MT9V03X_DVP_H, 1);
    return 0;
}
