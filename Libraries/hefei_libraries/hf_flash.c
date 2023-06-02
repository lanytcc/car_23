/*********************************************************************************************************************
 * @file            hf_flash.c
 * @brief           FLASH配置
 * @author          FengGuorong
 * @version         1.0
 * @Target core     CH32V307VCT6
 * @date            2022-3-5
 ********************************************************************************************************************/

#include "hf_flash.h"
#include "common.h"
flash_data_union flash_data_union_buffer[FLASH_DATA_BUFFER_SIZE];               // FLASH 操作的数据缓冲区
//-------------------------------------------------------------------------------------------------------------------
//  @brief      校验FLASH是否有数据
//  @param      sector_num      需要写入的扇区编号   参数范围 FLASH_SECTION_00-FLASH_SECTION_15
//  @param      page_num        当前扇区页的编号        参数范围 FLASH_PAGE_0-FLASH_PAGE_3
//  @return                     返回1有数据，返回0没有数据，如果需要对有数据的区域写入新的数据则应该对所在扇区进行擦除操作
//  @since      v1.0
//  Sample usage:               flash_check(FLASH_SECTION_00,FLASH_PAGE_0);
//-------------------------------------------------------------------------------------------------------------------
uint8 flash_check (FLASH_SECTION_enum FLASH_SECTION_x, FLASH_PAGE_enum FLASH_PAGE_x)
{
    uint16 temp_loop;
    uint32 flash_addr = ((FLASH_BASE_ADDR+FLASH_SECTION_SIZE * FLASH_SECTION_x + FLASH_PAGE_SIZE*FLASH_PAGE_x));     // 提取当前 Flash 地址

    for(temp_loop = 0; temp_loop < FLASH_PAGE_SIZE; temp_loop+=4)                                       // 循环读取 Flash 的值
    {
        if( (*(__IO u32*) (flash_addr+temp_loop)) != 0xffffffff )                                       // 如果不是 0xffffffff 那就是有值
            return 1;
    }
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      擦除扇区
//  @param      sector_num      需要写入的扇区编号   参数范围 FLASH_SECTION_00-FLASH_SECTION_15
//  @param      page_num        当前扇区页的编号     参数范围 FLASH_PAGE_0-FLASH_PAGE_3
//  @return                     返回1有表示失败  返回0表示成功
//  @since      v1.0
//  Sample usage:               flash_erase_page(FLASH_SECTION_00,FLASH_PAGE_0);
//-------------------------------------------------------------------------------------------------------------------
uint8 flash_erase_page (FLASH_SECTION_enum FLASH_SECTION_x, FLASH_PAGE_enum FLASH_PAGE_x)
{
    static volatile FLASH_Status gFlashStatus = FLASH_COMPLETE;
    uint32 flash_addr = ((FLASH_BASE_ADDR+FLASH_SECTION_SIZE*FLASH_SECTION_x+FLASH_PAGE_SIZE*FLASH_PAGE_x));     // 提取当前 Flash 地址

    FLASH_Unlock();                                                                                     // 解锁 Flash
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);                           // 清除操作标志
    gFlashStatus = FLASH_ErasePage(flash_addr);                                                         // 擦除
    FLASH_ClearFlag(FLASH_FLAG_EOP );                                                                   // 清楚操作标志
    FLASH_Lock();                                                                                       // 锁定 Flash
    if(gFlashStatus != FLASH_COMPLETE)                                                                  // 判断操作是否成功
        return 1;
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      读取一页
//  @param      sector_num      需要读取的扇区编号   参数范围 FLASH_SECTION_00-FLASH_SECTION_15
//  @param      page_num        需要读取的页编号        参数范围 FLASH_PAGE_0-FLASH_PAGE_3
//  @param      buf             需要读取的数据地址   传入的数组类型必须为uint32
//  @param      len             需要读取的数据长度   参数范围 1-256
//  @return                     返回1有表示失败  返回0表示成功
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void flash_page_read (FLASH_SECTION_enum FLASH_SECTION_x, FLASH_PAGE_enum FLASH_PAGE_x, uint32 *buf, uint16 len)
{
    uint16 temp_loop = 0;
    uint32 flash_addr = ((FLASH_BASE_ADDR+FLASH_SECTION_SIZE*FLASH_SECTION_x+FLASH_PAGE_SIZE*FLASH_PAGE_x));    // 提取当前 Flash 地址

    for(temp_loop = 0; temp_loop < len; temp_loop++)                                                    // 根据指定长度读取
    {
        *buf++ = *(__IO uint32_t*)(flash_addr+temp_loop*4);                                             // 循环读取 Flash 的值
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      编程一页
//  @param      sector_num      需要写入的扇区编号   参数范围 FLASH_SECTION_00-FLASH_SECTION_15
//  @param      page_num        需要写入的页编号        参数范围 FLASH_PAGE_0-FLASH_PAGE_3
//  @param      buf             需要写入的数据地址   传入的数组类型必须为uint32
//  @param      len             需要写入的数据长度   参数范围 1-256
//  @return                     返回1有表示失败  返回0表示成功
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
uint8 flash_page_program (FLASH_SECTION_enum FLASH_SECTION_x, FLASH_PAGE_enum FLASH_PAGE_x, const uint32 *buf, uint16 len)
{
    static volatile FLASH_Status gFlashStatus = FLASH_COMPLETE;
    uint32 flash_addr = 0;
    flash_addr = ((FLASH_BASE_ADDR+FLASH_SECTION_SIZE*FLASH_SECTION_x+FLASH_PAGE_SIZE*FLASH_PAGE_x));            // 提取当前 Flash 地址

    if(flash_check(FLASH_SECTION_x, FLASH_PAGE_x))                                                               // 判断是否有数据 这里是冗余的保护 防止有人没擦除就写入
        flash_erase_page(FLASH_SECTION_x, FLASH_PAGE_x);                                                         // 擦除这一页

    FLASH_Unlock();                                                                                     // 解锁 Flash
    while(len--)                                                                                        // 根据长度
    {
        gFlashStatus = FLASH_ProgramWord(flash_addr, *buf++);                                           // 按字 32bit 写入数据
        if(gFlashStatus != FLASH_COMPLETE)                                                              // 反复确认操作是否成功
            return 1;
        flash_addr += 4;                                                                                // 地址自增
    }
    FLASH_Lock();                                                                                       // 锁定 Flash
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       从指定 FLASH 的扇区的指定页码读取数据到缓冲区
// @param       sector_num      需要读取的扇区编号   参数范围 FLASH_SECTION_0-FLASH_SECTION_127
// @param       page_num        当前扇区页的编号     参数范围 FLASH_PAGE_0-FLASH_PAGE_3
// @return      void
// Sample usage:                flash_read_page_to_data_buffer(FLASH_SECTION_127, FLASH_PAGE_3);
//-------------------------------------------------------------------------------------------------------------------
void flash_read_page_to_data_buffer (FLASH_SECTION_enum sector_num, FLASH_PAGE_enum page_num)
{
    uint16 temp_loop;
    uint32 flash_addr = ((FLASH_BASE_ADDR+FLASH_SECTION_SIZE*sector_num+FLASH_PAGE_SIZE*page_num)); // 提取当前 Flash 地址

    for(temp_loop = 0; temp_loop < FLASH_DATA_BUFFER_SIZE; temp_loop++)                             // 根据指定长度读取
    {
        flash_data_union_buffer[temp_loop].uint32_type = *(__IO uint32*)(flash_addr+temp_loop*4); // 循环读取 Flash 的值
    }
}
//-------------------------------------------------------------------------------------------------------------------
// @brief       向指定 FLASH 的扇区的指定页码写入缓冲区的数据
// @param       sector_num      需要写入的扇区编号   参数范围 FLASH_SECTION_0-FLASH_SECTION_127
// @param       page_num        当前扇区页的编号     参数范围 FLASH_PAGE_0-FLASH_PAGE_3
// @return      uint8           1-表示失败 0-表示成功
// Sample usage:                flash_write_page_to_data_buffer(FLASH_SECTION_127, FLASH_PAGE_3);
//-------------------------------------------------------------------------------------------------------------------
uint8 flash_write_page_to_data_buffer (FLASH_SECTION_enum sector_num, FLASH_PAGE_enum page_num)
{
    static volatile FLASH_Status gFlashStatus = FLASH_COMPLETE;
    uint32 flash_addr = 0;
    uint16 len = 0;
    flash_addr = ((FLASH_BASE_ADDR+FLASH_SECTION_SIZE*sector_num+FLASH_PAGE_SIZE*page_num));            // 提取当前 Flash 地址

    if(flash_check(sector_num, page_num))                                                               // 判断是否有数据 这里是冗余的保护 防止有人没擦除就写入
        flash_erase_page(sector_num, page_num);                                                         // 擦除这一页

    FLASH_Unlock();                                                                                     // 解锁 Flash
    while(len < FLASH_DATA_BUFFER_SIZE)                                                                                        // 根据长度
    {
        gFlashStatus = FLASH_ProgramWord(flash_addr, flash_data_union_buffer[len].uint32_type);                                           // 按字 32bit 写入数据
        if(gFlashStatus != FLASH_COMPLETE)                                                              // 反复确认操作是否成功
            return 1;
        len++;                                                                                          // 长度自增
        flash_addr += 4;                                                                                // 地址自增
    }
    FLASH_Lock();                                                                                       // 锁定 Flash
    return 0;
}
//-------------------------------------------------------------------------------------------------------------------
// @brief       清空数据缓冲区
// @param       void
// @return      void
// Sample usage:                flash_data_buffer_clear();
//-------------------------------------------------------------------------------------------------------------------
void flash_data_buffer_clear (void)
{
    memset(flash_data_union_buffer, 0xFF, FLASH_PAGE_SIZE);
}
