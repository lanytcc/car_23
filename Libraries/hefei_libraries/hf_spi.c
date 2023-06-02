/*********************************************************************************************************************
 * @file            hf_spi.c
 * @brief           硬件SPI配置
 * @author          FengGuorong
 * @version         1.0
 * @Target core     CH32V307VCT6
 * @date            2022-03-12
 ********************************************************************************************************************/

#include "hf_spi.h"

/* 内部变量声明 */
const uint32 SPIN[3] = {SPI1_BASE, SPI2_BASE,SPI3_BASE};

//-------------------------------------------------------------------------------------------------------------------
//  @brief      SPI初始化
//  @param      spi_n               选择SPI模块   (SPI_1-SPI_2)
//  @param      sck_pin             SCK引脚(可选择范围由hf_spi.h内SPI_PIN_enum枚举值确定)
//  @param      mosi_pin            MOSI引脚(可选择范围由hf_spi.h内SPI_PIN_enum枚举值确定)
//  @param      miso_pin            MISO引脚(可选择范围由hf_spi.h内SPI_PIN_enum枚举值确定)
//  @param      cs_pin              片选引脚(可选择范围由common.h内PIN_enum枚举值确定) 使用软件CS引脚
//  @param      mode                SPI模式 0：CPOL=0 CPHA=0    1：CPOL=0 CPHA=1   2：CPOL=1 CPHA=0   3：CPOL=1 CPHA=1 //具体细节可自行百度
//  @param      BaudRatePrescaler   SPI的速率设置为系统时钟的多少分频
//  @return     void
//  Sample usage:               spi_init(SPI_1, SPI1_SCK_PA5, SPI1_MOSI_PA7, SPI1_MISO_PA6, A0, 0, FPCLK_DIV_2);//硬件SPI初始化  模式0 波特率为系统时钟频率/2
//                              PWM_DUTY_MAX在wh_pwm.h文件中 默认为50000
//-------------------------------------------------------------------------------------------------------------------
void spi_init(
        SPIN_enum SPI_x,
        SPI_PIN_enum sck_pin,
        SPI_PIN_enum mosi_pin,
        SPI_PIN_enum miso_pin,
        gpio_pin_enum cs_pin,
        uint8 mode,
        SPI_SPEED_enum BaudRatePrescaler)
{

    SPI_InitTypeDef  SPI_InitStructure;

    if(SPI_1 == SPI_x)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO, ENABLE);                //SPI1时钟使能
    }
    else if(SPI_2 == SPI_x)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 | RCC_APB2Periph_AFIO, ENABLE);                //SPI2时钟使能
    }
    else if(SPI_3 == SPI_x)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3 | RCC_APB2Periph_AFIO, ENABLE);                //SPI2时钟使能
    }

    spi_gpio_init(sck_pin, mosi_pin, miso_pin, cs_pin);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;      //SPI设置为双线双向全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                           //设置SPI工作模式:设置为主SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                       //设置SPI的数据大小:SPI发送接收8位帧结构

    switch(mode)
    {
        case 0:
        {
            SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                      //串行同步时钟的空闲状态为低电平
            SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                    //串行同步时钟的第一个跳变沿（上升或下降）数据被采样
        }break;
        case 1:
        {
            SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                      //串行同步时钟的空闲状态为低电平
            SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                    //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
        }break;
        case 2:
        {
            SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                     //串行同步时钟的空闲状态为高电平
            SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                    //串行同步时钟的第一个跳变沿（上升或下降）数据被采样
        }break;
        case 3:
        {
            SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                     //串行同步时钟的空闲状态为高电平
            SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                    //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
        }break;
    }

    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                               //主机模式，使用软件控制NSS引脚
    SPI_InitStructure.SPI_BaudRatePrescaler = BaudRatePrescaler << 3;       //定义波特率预分频的值
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                      //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI_InitStructure.SPI_CRCPolynomial = 7;                                //CRC值计算的多项式
    SPI_Init((SPI_TypeDef *)(SPIN[SPI_x]), &SPI_InitStructure);             //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

    if(SPI1_SCK_B3 == sck_pin) GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);
    else if(SPI3_SCK_C10 == sck_pin) GPIO_PinRemapConfig(GPIO_Remap_SPI3, ENABLE);

    SPI_Cmd(((SPI_TypeDef *)(SPIN[SPI_x])), ENABLE);                         //使能SPI外设

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      SPI发送接收函数
//  @param      spi_n           选择SPI模块   (SPI_1-SPI_2)
//  @param      modata          发送的数据缓冲区地址
//  @param      midata          发送数据时接收到的数据的存储地址(不需要接收则传 NULL)
//  @param      len             发送的字节数
//  @return     void
//  @since      v2.0
//  Sample usage:               spi_mosi(SPI_1, buf, buf, 1);    //发送modata的内容，并接收到midata里，长度为1字节
//-------------------------------------------------------------------------------------------------------------------
void spi_mosi(SPIN_enum spi_n, uint8 *modata, uint8 *midata, uint32 len)
{
    while(len--)
    {
        while((((SPI_TypeDef *)(SPIN[spi_n]))->STATR & SPI_I2S_FLAG_TXE) == RESET);
        ((SPI_TypeDef *)(SPIN[spi_n]))->DATAR = *(modata++);
        if(NULL != midata)
        {
            while((((SPI_TypeDef *)(SPIN[spi_n]))->STATR & SPI_I2S_FLAG_RXNE) == RESET);
            *(midata++) = ((SPI_TypeDef *)(SPIN[spi_n]))->DATAR;
        }
    }
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      SPI引脚初始化
//  @param      sck_pin     SCK引脚(可选择范围由hf_spi.h内SPI_PIN_enum枚举值确定)
//  @param      mosi_pin    MOSI引脚(可选择范围由hf_spi.h内SPI_PIN_enum枚举值确定)
//  @param      miso_pin    MISO引脚(可选择范围由hf_spi.h内SPI_PIN_enum枚举值确定)
//  @param      cs_pin      片选引脚(可选择范围由common.h内PIN_enum枚举值确定)
//  @return     void
//  Sample usage:           内部使用  用户无需关心
//-------------------------------------------------------------------------------------------------------------------
void spi_gpio_init(SPI_PIN_enum sck_pin, SPI_PIN_enum mosi_pin, SPI_PIN_enum miso_pin, gpio_pin_enum cs_pin)
{
    if(SPI1_SCK_A5        == sck_pin)    gpio_init(A5,  GPO, 1, SPEED_50MHZ|OUT_AF_PP);
    else if(SPI1_SCK_B3   == sck_pin)    gpio_init(B3,  GPO, 1, SPEED_50MHZ|OUT_AF_PP);
    else if(SPI2_SCK_B13  == sck_pin)    gpio_init(B13, GPO, 1, SPEED_50MHZ|OUT_AF_PP);
    else if(SPI3_SCK_B3   == sck_pin)    gpio_init(B3,  GPO, 1, SPEED_50MHZ|OUT_AF_PP);
    else if(SPI3_SCK_C10  == sck_pin)    gpio_init(C10, GPO, 1, SPEED_50MHZ|OUT_AF_PP);

    if(SPI1_MOSI_A7       == mosi_pin)   gpio_init(A7,  GPO, 1, SPEED_50MHZ|OUT_AF_PP);
    else if(SPI1_MOSI_B5  == mosi_pin)   gpio_init(B5,  GPO, 1, SPEED_50MHZ|OUT_AF_PP);
    else if(SPI2_MOSI_B15 == mosi_pin)   gpio_init(B15, GPO, 1, SPEED_50MHZ|OUT_AF_PP);
    else if(SPI3_MOSI_B5  == mosi_pin)   gpio_init(B5,  GPO, 1, SPEED_50MHZ|OUT_AF_PP);
    else if(SPI3_MOSI_C12 == mosi_pin)   gpio_init(C12, GPO, 1, SPEED_50MHZ|OUT_AF_PP);

    if(SPI1_MISO_A6       == miso_pin)   gpio_init(A6,  GPI, 1, SPEED_50MHZ|OUT_AF_PP);
    else if(SPI1_MISO_B4  == miso_pin)   gpio_init(B4,  GPI, 1, SPEED_50MHZ|OUT_AF_PP);
    else if(SPI2_MISO_B14 == miso_pin)   gpio_init(B14, GPI, 1, SPEED_50MHZ|OUT_AF_PP);
    else if(SPI3_MISO_C11 == miso_pin)   gpio_init(C11, GPI, 1, SPEED_50MHZ|OUT_AF_PP);
    else if(SPI3_MISO_B4  == miso_pin)   gpio_init(B4,  GPI, 1, SPEED_50MHZ|OUT_AF_PP);
    else if(SPI_MISO_NULL == miso_pin)   miso_pin = SPI_MISO_NULL;

    //初始化CS引脚
    gpio_init(cs_pin, GPO, 0, SPEED_50MHZ|OUT_PP);

}
//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI 接口写 8bit 数据
// @param       spi_n           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// @param       data            数据
// @return      void
// Sample usage:                spi_write_8bit(SPI_1,0x11);
//-------------------------------------------------------------------------------------------------------------------
void spi_write_8bit (SPIN_enum SPI_x, uint8 dat)
{
    ((SPI_TypeDef *)(SPIN[SPI_x]))->DATAR = dat;                                  // 发送数据
    while((((SPI_TypeDef *)(SPIN[SPI_x]))->STATR & SPI_I2S_FLAG_BSY) != RESET);    // 发送为空
}
//-------------------------------------------------------------------------------------------------------------------
// @brief       SPI 接口写 16bit 数据
// @param       spi_n           SPI 模块号 参照 zf_driver_spi.h 内 spi_index_enum 枚举体定义
// @param       data            数据
// @return      void
// Sample usage:                spi_write_16bit(SPI_1,0x1101);
//-------------------------------------------------------------------------------------------------------------------

void spi_write_16bit(SPIN_enum SPI_x, uint16 dat) {
    SPI_TypeDef *spi = (SPI_TypeDef *)(SPIN[SPI_x]);

    // 发送高 8 位数据
    spi->DATAR = (uint8_t)((dat >> 8) & 0xFF);
    while ((spi->STATR & SPI_I2S_FLAG_BSY) != RESET);

    // 发送低 8 位数据
    spi->DATAR = (uint8_t)(dat & 0xFF);
    while ((spi->STATR & SPI_I2S_FLAG_BSY) != RESET);
}
