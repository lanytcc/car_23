/*********************************************************************************************************************
 * @file            common.h
 * @author          Fantastic Potato
 * @version         1.0
 * @Target core     CH32V307VCT6
 * @date            2022-3-2
 ********************************************************************************************************************/

#ifndef _common_h
#define _common_h

#include <math.h>
#include <assert.h>
#include <stddef.h>

#include <string.h>
#include "ch32v30x.h"

/* 数据类型声明 */
typedef unsigned char       uint8;   //  8 bits
typedef unsigned short int  uint16;  // 16 bits
typedef unsigned long int   uint32;  // 32 bits
typedef unsigned long long  uint64;  // 64 bits

//typedef uint32  u32;
typedef uint16 u16;
typedef uint8  u8;
typedef uint64 u64;

typedef char                int8;    //  8 bits
typedef short int           int16;   // 16 bits
typedef long  int           int32;   // 32 bits
typedef long  long          int64;   // 64 bits

typedef volatile int8       vint8;   //  8 bits
typedef volatile int16      vint16;  // 16 bits
typedef volatile int32      vint32;  // 32 bits
typedef volatile int64      vint64;  // 64 bits

typedef volatile uint8      vuint8;  //  8 bits
typedef volatile uint16     vuint16; // 16 bits
typedef volatile uint32     vuint32; // 32 bits
typedef volatile uint64     vuint64; // 64 bits

/* 结构体声明 */
/* 定义PFIC_IRQ结构体 */
typedef struct
{
  uint32 ienr0;
  uint32 ienr1;
  uint8 flag;
}PFIC_IRQ_TypeDef;

/* 外界变量引用 */
extern uint32 sys_clk;
extern PFIC_IRQ_TypeDef irq_status;

//函数声明


#endif
