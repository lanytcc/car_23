/*********************************************************************************************************************
 * @file            hf_dma.h
 * @author          Fantastic Potato
 * @version         1.0
 * @Target core     CH32V307VCT6
 * @date            2022-3-19
 ********************************************************************************************************************/

#ifndef _hf_dma_h
#define _hf_dma_h

#include "common.h"
#include "ch32v30x_dma.h"
#include "ch32v30x_rcc.h"

/* 枚举定义声明 */
/* 枚举DMA方向 */
typedef enum
{
    P2M = 0,        //外设作为数据传输的来源
    M2P             //外设作为数据传输的目的地
}DMADIR_enum;

/* 枚举DMA通道 */
typedef enum
{
    DMA1_CH1 = 0,
    DMA1_CH2,
    DMA1_CH3,
    DMA1_CH4,
    DMA1_CH5,
    DMA1_CH6,
    DMA1_CH7,

    DMA2_CH1,
    DMA2_CH2,
    DMA2_CH3,
    DMA2_CH4,
    DMA2_CH5,
    DMA2_CH6,
    DMA2_CH7,
    DMA2_CH8,
    DMA2_CH9,
    DMA2_CH10,
    DMA2_CH11,
}DMA_CH_enum;

/* 函数声明 */

/* 外部函数，用户可自行调用  */
/* 配置DMA功能 */
void dma_init(DMA_CH_enum DMA_CHx, uint32_t ppaddr, uint32_t memaddr, uint32_t num, DMADIR_enum directions);


#endif
