/*********************************************************************************************************************
 * @file            hf_dma.c
 * @author          Fantastic Potato
 * @version         1.0
 * @Target core     CH32V307VCT6
 * @date            2022-3-19
 ********************************************************************************************************************/

#include "hf_dma.h"

/* 内部变量声明 */
const uint32 DMA_CHN_BASE[18] = {DMA1_Channel1_BASE, DMA1_Channel2_BASE, DMA1_Channel3_BASE
        , DMA1_Channel4_BASE, DMA1_Channel5_BASE, DMA1_Channel6_BASE, DMA1_Channel7_BASE
        , DMA2_Channel1_BASE, DMA2_Channel2_BASE, DMA2_Channel3_BASE
        , DMA2_Channel4_BASE, DMA2_Channel5_BASE, DMA2_Channel6_BASE, DMA2_Channel7_BASE
        , DMA2_Channel8_BASE, DMA2_Channel9_BASE, DMA2_Channel10_BASE, DMA2_Channel11_BASE};



//-------------------------------------------------------------------------------------------------------------------
// @brief        配置DMA功能
// @param        DMA_CHx                dma通道,见common.h内定义的枚举类型
// @param        ppaddr                 外设基地址配置
// @param        memaddr                存储器基地址配置
// @param        num                    配置数据长度
// @param        directions             配置DMA传输方向
// @return       void
// Sample usage:        dma_struct_init(&init_struct, (uint32_t)(&ADC_RDATA), rx_buffer, P2M);
//-------------------------------------------------------------------------------------------------------------------
void dma_init(DMA_CH_enum DMA_CHx, uint32_t ppaddr, uint32_t memaddr, uint32_t buffer_size, DMADIR_enum directions)
{
    /* 配置DMA结构体 */
    DMA_InitTypeDef DMA_InitStruct;

    /* DMA时钟使能 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    /* DMA初始化 */
    DMA_DeInit((DMA_Channel_TypeDef *)DMA_CHN_BASE[DMA_CHx]);

    /* 配置DMA结构体参数 */
    DMA_InitStruct.DMA_PeripheralBaseAddr = ppaddr;
    DMA_InitStruct.DMA_MemoryBaseAddr = memaddr;
    if(directions == 0)
    {
        DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
    }
    else
    {
        DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;
    }
    DMA_InitStruct.DMA_BufferSize = buffer_size;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;           //DMA模式使能
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;             //存储器到存储器DMA传输禁能

    /* DMA通道x初始化 */
    DMA_Init((DMA_Channel_TypeDef *)DMA_CHN_BASE[DMA_CHx], &DMA_InitStruct);

    /* DMA通道x传输使能 */
    DMA_Cmd((DMA_Channel_TypeDef *)DMA_CHN_BASE[DMA_CHx], ENABLE);
}
