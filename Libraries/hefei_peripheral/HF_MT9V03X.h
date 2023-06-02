/*********************************************************************************************************************
* @file            HF_MT9V03X.h
* @author          Fantastic Potato
* @version         1.0
* @Target core     CH32V307VCT6
* @date            20220905
 ********************************************************************************************************************/

#include "headfile.h"

#ifndef HF_MT9V03X_H_
#define HF_MT9V03X_H_

// 配置摄像头参数
#define MT9V03X_DVP_W               188                                             // 图像宽度  范围1-752
#define MT9V03X_DVP_H               120                                             // 图像高度 范围1-480
#define MT9V03X_IMAGE_SIZE_DVP      (MT9V03X_DVP_W*MT9V03X_DVP_H)

//--------------------------------------------------------------------------------------------------
// 引脚配置
//--------------------------------------------------------------------------------------------------
#define MT9V03X_COF_UART_DVP        UART_5                                          // 配置摄像头所使用到的串口
#define MT9V03X_COF_BAUR_DVP        9600                                            // 总钻风配置串口波特率 禁止修改
#define MT9V03X_COF_UART_RX_DVP     UART5_TX_C12                                    // 总钻风 UART-RX 引脚 要接在单片机 TX 上
#define MT9V03X_COF_UART_TX_DVP     UART5_RX_D2                                     // 总钻风 UART-TX 引脚 要接在单片机 RX 上

//--------------------------------------------------------------------------------------------------
// 摄像头数据引脚，DVP专用引脚，禁止用户修改引脚
//--------------------------------------------------------------------------------------------------
#define MT9V03X_D0_PIN_DVP          A9
#define MT9V03X_D1_PIN_DVP          A10
#define MT9V03X_D2_PIN_DVP          C8
#define MT9V03X_D3_PIN_DVP          C9
#define MT9V03X_D4_PIN_DVP          C11
#define MT9V03X_D5_PIN_DVP          B6
#define MT9V03X_D6_PIN_DVP          B8
#define MT9V03X_D7_PIN_DVP          B9

#define MT9V03X_PCLK_PIN_DVP        A6
#define MT9V03X_VSY_PIN_DVP         A5
#define MT9V03X_HERF_PIN_DVP        A4

// 超时设置
#define MT9V03X_INIT_TIMEOUT                  0x0080

// 摄像头命令枚举
typedef enum
{
    INIT = 0,                                                                   // 摄像头初始化命令
    AUTO_EXP,                                                                   // 自动曝光命令
    EXP_TIME,                                                                   // 曝光时间命令
    FPS,                                                                        // 摄像头帧率命令
    SET_COL,                                                                    // 图像列命令
    SET_ROW,                                                                    // 图像行命令
    LR_OFFSET,                                                                  // 图像左右偏移命令
    UD_OFFSET,                                                                  // 图像上下偏移命令
    GAIN,                                                                       // 图像偏移命令
    PCLK_MODE,                                                                  // 像素时钟模式命令(仅总钻风MT9V034 V1.5以及以上版本支持该命令)
    CONFIG_FINISH,                                                              // 非命令位，主要用来占位计数

    COLOR_GET_WHO_AM_I = 0xEF,
    SET_EXP_TIME = 0XF0,                                                        // 单独设置曝光时间命令
    GET_STATUS,                                                                 // 获取摄像头配置命令
    GET_VERSION,                                                                // 固件版本号命令

    SET_ADDR = 0XFE,                                                            // 寄存器地址命令
    SET_DATA                                                                    // 寄存器数据命令
}m9v03x_cmd_enum;

extern uint8_t    mt9v03x_finish_flag_dvp;//一场图像采集完成标志位
extern uint8_t    mt9v03x_image_dvp[MT9V03X_DVP_H][MT9V03X_DVP_W];

void     mt9v03x_uart_callback_dvp      (void);
void     mt9v03x_handler_dvp            (void);
uint16_t mt9v03x_get_version_dvp        (void);
uint8_t  mt9v03x_set_exposure_time_dvp  (uint16_t light);
uint8_t  mt9v03x_set_reg_dvp            (uint8_t addr, uint16_t data);
uint8_t  mt9v03x_init_dvp               (void);



#endif /* HF_MT9V03X_H_ */
