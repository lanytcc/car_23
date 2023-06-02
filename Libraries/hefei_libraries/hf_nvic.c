/*********************************************************************************************************************
 * @file            hf_nvic.c
 * @brief           NVIC配置
 * @author          Fantastic Potato
 * @version         1.0
 * @Target core     CH32V307VCT6
 * @date            2022-3-4
 ********************************************************************************************************************/

#include "hf_nvic.h"
//static uint32 interrupt_nest_count = 0;
//-------------------------------------------------------------------------------------------------------------------
// @brief       指定中断使能
// @param       irqn            指定中断号 可查看 isr.c 对应中断服务函数的标注
// @return      void
// Sample usage:                interrupt_enable(UART1_IRQn);
//-------------------------------------------------------------------------------------------------------------------
void interrupt_enable (IRQn_Type irqn)
{
    NVIC_EnableIRQ(irqn);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       指定中断屏蔽
// @param       irqn            指定中断号 可查看 isr.c 对应中断服务函数的标注
// @return      void
// Sample usage:                interrupt_disable(UART1_IRQn);
//-------------------------------------------------------------------------------------------------------------------
void interrupt_disable (IRQn_Type irqn)
{
    NVIC_DisableIRQ(irqn);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       指定中断设置优先级
// @param       irqn            指定中断号 可查看 isr.c 对应中断服务函数的标注
// @param       priority        中断优先级 0-7 越低越高
// @return      void
// Sample usage:                interrupt_enable(UART1_IRQn, 0);
//-------------------------------------------------------------------------------------------------------------------
void interrupt_set_priority (IRQn_Type irqn, uint8_t priority)
{
    NVIC_SetPriority(irqn, priority);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       中断组初始化 clock_init 内部调用
// @param       void
// @return      void
// Sample usage:                interrupt_init();
//-------------------------------------------------------------------------------------------------------------------
void interrupt_init (void)
{
    //NVIC_SetPriorityGrouping(4);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       全局中断使能
// @param       void
// @return      void
// Sample usage:                interrupt_global_enable ();
//-------------------------------------------------------------------------------------------------------------------
void interrupt_global_enable ()
{
//    extern void en_interrupt();
//    en_interrupt();

}

////-------------------------------------------------------------------------------------------------------------------
//// @brief       全局中断屏蔽
//// @param       void
//// @return      void
//// Sample usage:                interrupt_disable_all();
////-------------------------------------------------------------------------------------------------------------------
//uint32_t interrupt_disable_all (void)
//{
//    uint32_t temp;
//    extern void dis_interrupt();
//    dis_interrupt();
//    temp = __get_MSTATUS();
//    //返回当前mstatus寄存器的值
//    return temp;
//}
//-------------------------------------------------------------------------------------------------------------------
// @brief       全局中断屏蔽
// @param       void
// @return      void
// Sample usage:                interrupt_global_disable ();
//-------------------------------------------------------------------------------------------------------------------
void interrupt_global_disable ()
{
//    if(!interrupt_nest_count)
//    {
//        extern void dis_interrupt();
//        dis_interrupt();
//    }
//    interrupt_nest_count++;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      中断初始化
//  @param      irqn                        中断号，可以查看CH32V30x.h文件中的IRQn_Type枚举体定义
//  @param      preemption_priority         抢占优先级值越小，优先级越高 (可选参数0-7，最高0，最低7)
//  @param      sub_priority                次优先级值越小，优先级越高     (可选参数0-7，最高0，最低7)
//  @param      status                      使能或者失能
//  @return     void
//  Sample usage:                           nvic_init(EXTI0_IRQn,0, 0, ENABLE); //外部中断0使能,抢占优先级最高，次优先级最高。
//-------------------------------------------------------------------------------------------------------------------
void nvic_init(IRQn_Type irqn, uint8_t preemption_priority, uint8_t sub_priority, uint8_t status)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = irqn;                                  //中断号设置
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=preemption_priority;   //抢占优先级值越小，优先级越高
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub_priority;               //次优先级值越小，优先级越高
    NVIC_InitStructure.NVIC_IRQChannelCmd = status;                             //使能
    NVIC_Init(&NVIC_InitStructure);

}

