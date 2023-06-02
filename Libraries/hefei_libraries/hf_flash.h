/*********************************************************************************************************************
 * @file            hf_flash.h
 * @brief           FLASH配置
 * @author          FengGuorong
 * @version         1.0
 * @Target core     CH32V307VCT6
 * @date            2022-3-5
 ********************************************************************************************************************/


#ifndef HF_FLASH_H_
#define HF_FLASH_H_

#include "ch32v30x_misc.h"
#include "ch32v30x_flash.h"
#include "common.h"

/* 宏定义 */
#define FLASH_BASE_ADDR             (0x08000000)                // FALSH首地址
#define FLASH_PAGE_SIZE             (0x00000400)                // 1K byte
#define FLASH_SECTION_SIZE          (FLASH_PAGE_SIZE*4)         // 4K byte
//一共16个扇区，一个扇区4K
#define FLASH_DATA_BUFFER_SIZE      (FLASH_PAGE_SIZE/sizeof(flash_data_union))  // 自动计算每个页能够存下多少个数据

/* 枚举定义声明 */
/* 枚举 Flash 扇区 */
typedef enum
{
    FLASH_SECTION_00,
    FLASH_SECTION_01,
    FLASH_SECTION_02,
    FLASH_SECTION_03,
    FLASH_SECTION_04,
    FLASH_SECTION_05,
    FLASH_SECTION_06,
    FLASH_SECTION_07,
    FLASH_SECTION_08,
    FLASH_SECTION_09,
    FLASH_SECTION_10,
    FLASH_SECTION_11,
    FLASH_SECTION_12,
    FLASH_SECTION_13,
    FLASH_SECTION_14,
    FLASH_SECTION_15,
    FLASH_SECTION_16,
    FLASH_SECTION_17,
    FLASH_SECTION_18,
    FLASH_SECTION_19,
    FLASH_SECTION_20,
    FLASH_SECTION_21,
    FLASH_SECTION_22,
    FLASH_SECTION_23,
    FLASH_SECTION_24,
    FLASH_SECTION_25,
    FLASH_SECTION_26,
    FLASH_SECTION_27,
    FLASH_SECTION_28,
    FLASH_SECTION_29,
    FLASH_SECTION_30,
    FLASH_SECTION_31,
    FLASH_SECTION_32,
    FLASH_SECTION_33,
    FLASH_SECTION_34,
    FLASH_SECTION_35,
    FLASH_SECTION_36,
    FLASH_SECTION_37,
    FLASH_SECTION_38,
    FLASH_SECTION_39,
    FLASH_SECTION_40,
    FLASH_SECTION_41,
    FLASH_SECTION_42,
    FLASH_SECTION_43,
    FLASH_SECTION_44,
    FLASH_SECTION_45,
    FLASH_SECTION_46,
    FLASH_SECTION_47,
    FLASH_SECTION_48,
    FLASH_SECTION_49,
    FLASH_SECTION_50,
    FLASH_SECTION_51,
    FLASH_SECTION_52,
    FLASH_SECTION_53,
    FLASH_SECTION_54,
    FLASH_SECTION_55,
    FLASH_SECTION_56,
    FLASH_SECTION_57,
    FLASH_SECTION_58,
    FLASH_SECTION_59,
    FLASH_SECTION_60,
    FLASH_SECTION_61,
    FLASH_SECTION_62,
    FLASH_SECTION_63
}FLASH_SECTION_enum;

/* 枚举 Flash 页索引 */
typedef enum
{
    FLASH_PAGE_0,
    FLASH_PAGE_1,
    FLASH_PAGE_2,
    FLASH_PAGE_3,
}FLASH_PAGE_enum;
typedef union                                                                   // 固定的数据缓冲单元格式
{
    float   float_type;                                                       // float  类型
    uint32  uint32_type;                                                      // uint32 类型
    int32   int32_type;                                                       // int32  类型
    uint16  uint16_type;                                                      // uint16 类型
    int16   int16_type;                                                       // int16  类型
    uint8   uint8_type;                                                       // uint8  类型
    int8    int8_type;                                                        // int8   类型
}flash_data_union;                                                              // 所有类型数据共用同一个 32bit 地址

extern flash_data_union flash_data_union_buffer[FLASH_DATA_BUFFER_SIZE];

/* 函数声明 */

/* 外部函数，用户可自行调用 */
/* 校验FLASH是否有数据 */
uint8 flash_check(FLASH_SECTION_enum FLASH_SECTION_x, FLASH_PAGE_enum FLASH_PAGE_x);
/* 擦除扇区 */
uint8 flash_erase_page(FLASH_SECTION_enum FLASH_SECTION_x, FLASH_PAGE_enum FLASH_PAGE_x);
/* 读取一页 */
void flash_page_read (FLASH_SECTION_enum FLASH_SECTION_x, FLASH_PAGE_enum FLASH_PAGE_x, uint32 *buf, uint16 len);
/* 编程一页 */
uint8 flash_page_program(FLASH_SECTION_enum FLASH_SECTION_x, FLASH_PAGE_enum FLASH_PAGE_x, const uint32 *buf, uint16 len);
/* 向指定 FLASH 的扇区的指定页码写入缓冲区的数据 */
uint8 flash_write_page_to_data_buffer (FLASH_SECTION_enum sector_num, FLASH_PAGE_enum page_num);
/* 从指定 FLASH 的扇区的指定页码读取数据到缓冲区 */
void flash_read_page_to_data_buffer (FLASH_SECTION_enum sector_num, FLASH_PAGE_enum page_num);
/* 清空数据缓冲区 */
void flash_data_buffer_clear (void);

#endif
/* HF_FLASH_H_ */

