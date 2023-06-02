/*********************************************************************************************************************
 * @file            hf_dvp.c
 * @brief           DVP配置
 * @author          FengGuorong
 * @version         1.0
 * @Target core     CH32V307VCT6
 * @date            2022-3-18
 ********************************************************************************************************************/

#include "delay.h"
#include "hf_gpio.h"
#include "hf_dvp.h"
#include "hf_nvic.h"


//-------------------------------------------------------------------------------------------------------------------
//  @brief      dvp接口引脚初始化
//  @param      dvp_d0到dvp_d0       摄像头D0-D7引脚
//  @param      dvp_pclk            摄像头PCLK引脚
//  @param      dvp_vsync           摄像头vysnc引脚
//  @param      dvp_herf            摄像头herf引脚
//  @return     void
//  Sample usage:                   内部使用，用户无需关心
//-------------------------------------------------------------------------------------------------------------------
void dvp_gpio_init(
        uint8_t dvp_d0,uint8_t dvp_d1,uint8_t dvp_d2,uint8_t dvp_d3,
        uint8_t dvp_d4,uint8_t dvp_d5,uint8_t dvp_d6,uint8_t dvp_d7,
        uint8_t dvp_pclk,uint8_t dvp_vsync,uint8_t dvp_hsync)
{
    //DVP_D0 到 DVP_ D7
    gpio_init(dvp_d0, GPI, 0, IN_PULLUP);
    gpio_init(dvp_d1, GPI, 0, IN_PULLUP);
    gpio_init(dvp_d2, GPI, 0, IN_PULLUP);
    gpio_init(dvp_d3, GPI, 0, IN_PULLUP);
    gpio_init(dvp_d4, GPI, 0, IN_PULLUP);
    gpio_init(dvp_d5, GPI, 0, IN_PULLUP);
    gpio_init(dvp_d6, GPI, 0, IN_PULLUP);
    gpio_init(dvp_d7, GPI, 0, IN_PULLUP);
    //DVP_VSYNC
    gpio_init(dvp_vsync, GPI, 0, IN_PULLUP);
    //DVP_PCLK
    gpio_init(dvp_pclk,  GPI, 0, IN_PULLUP);
    //DVP_HSYNC
    gpio_init(dvp_hsync, GPI, 0, IN_PULLUP);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      dvp接口初始化
//  @param      *image0                数据buff0的地址
//  @param      *image1                数据buff1的地址
//  @param      col_len                列长度
//  @param      row_len                行长度
//  @return     void
//  Sample usage:                      内部使用，用户无需关心
//-------------------------------------------------------------------------------------------------------------------
void dvp_camera_init(uint32_t *image0, uint32_t *image1, uint16_t col_len, uint16_t row_len)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DVP, ENABLE);

    DVP->CR0 &= ~RB_DVP_MSK_DAT_MOD;

    /* VSYNC、HSYNC:High level active */
    DVP->CR0 |= RB_DVP_D8_MOD | RB_DVP_JPEG | RB_DVP_V_POLAR ;
    DVP->CR1 &= ~((RB_DVP_ALL_CLR)| RB_DVP_RCV_CLR);
    DVP->ROW_NUM = row_len;                         // rows行数
    DVP->COL_NUM = col_len;                         // cols列数

    DVP->DMA_BUF0 = (uint32_t)image0;               //DMA addr0
    DVP->DMA_BUF1 = (uint32_t)image1;               //DMA addr1


    /* Set frame capture rate */
    DVP->CR1 &= ~RB_DVP_FCRC;
    DVP->CR1 |= DVP_RATE_100P;  //100%
    DVP->CR1 |= RB_DVP_CM;



    //Interupt Enable
    DVP->IER |= RB_DVP_IE_FRM_DONE;


    DVP->CR1 |= RB_DVP_DMA_EN;  //enable DMA
    DVP->CR0 |= RB_DVP_ENABLE;  //enable DVP

    interrupt_set_priority(DVP_IRQn, 0x03);
    interrupt_enable(DVP_IRQn);

}



