/*********************************************************************************************************************
 * @file            hf_adc.c
 * @brief           ADC配置
 * @author          FengGuorong
 * @version         1.0
 * @Target core     CH32V307VCT6
 * @date            2022-3-8
 ********************************************************************************************************************/

#include "hf_adc.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC初始化
//  @param      ADC_INx_Ax              选择ADC通道，见hf_adc.h内关于ADC通道的定义
//  @return     void
//  Sample usage:               adc_init(ADC_IN0_A0);//初始化A0引脚为ADC功能
//-------------------------------------------------------------------------------------------------------------------
void adc_init(ADCCH_enum ADC_INx_xx)
{
    /* 初始化ADC结构体 */
    ADC_InitTypeDef ADC_InitStructure;

    /* 开启GPIO、ADC1的时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE );   //使能ADC1通道时钟，默认采用ADC1

    /* 根据ADC通道初始化GPIO */
    adc_gpio_init(ADC_INx_xx);

    /* 设置ADC分频因子，根据APB2的时钟进行设置 */
    //RCC_ADCCLKConfig(RCC_PCLK2_Div2);
    if(sys_clk > 84000000)  RCC_ADCCLKConfig(RCC_PCLK2_Div8);           //设置ADC分频因子，ADC最大速率不能超过14M
    else RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    ADC_DeInit(ADC1);                                                   //复位ADC1

    /* 配置ADC初始化参数 */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                  //ADC工作模式:ADC1工作在独立模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                       //模数转换工作在单通道模式
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                 //模数转换工作在单次转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //转换由软件而不是外部触发启动
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              //ADC数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1;                             //顺序进行规则转换的ADC通道的数目
    ADC_Init(ADC1, &ADC_InitStructure);                                 //根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器

    ADC_Cmd(ADC1, ENABLE);                                              //使能指定的ADC1
//    ADC_BufferCmd(ADC1, DISABLE);                                       //disable buffer

    ADC_ResetCalibration(ADC1);                                         //使能复位校准
    while(ADC_GetResetCalibrationStatus(ADC1));                         //等待复位校准结束
    ADC_StartCalibration(ADC1);                                         //开启AD校准
    while(ADC_GetCalibrationStatus(ADC1));                              //等待校准结束
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC转换一次
//  @param      ADC_INx_Ax      选择ADC通道
//  @param      resolution      分辨率(8位 10位 12位)
//  @return     void
//  Sample usage:               adc_convert(ADC_IN0_A0, ADC_8BIT);  //采集A0端口返回8位分辨率的AD值
//-------------------------------------------------------------------------------------------------------------------
uint16 adc_convert(ADCCH_enum ADC_INx_xx, ADCRES_enum resolution)
{
    /* 设置指定ADC的规则组通道，一个序列，采样时间 */
    ADC_RegularChannelConfig(ADC1, (uint8)ADC_INx_xx, 1, ADC_SampleTime_239Cycles5);  //ADC1,ADC通道,采样时间为41.5周期
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);                                          //使能指定的ADC1的软件转换启动功能
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));                                 //等待转换结束
    return ((ADC1->RDATAR)>>resolution);                                            //返回最近一次ADC1规则组的转换结果
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC转换N次
//  @param      ADC_IN0_Ax      选择ADC通道
//  @param      resolution      分辨率(8位 10位 12位)
//  @param      count           转换次数
//  @return     void
//  Sample usage:               adc_convert(ADC_IN0_A0, ADC_8BIT,5);  //采集A0端口返回8位分辨率的AD值，采集五次取平均值
//-------------------------------------------------------------------------------------------------------------------
uint16 adc_mean_filter(ADCCH_enum ADC_INx_xx, ADCRES_enum resolution, uint8 count)
{
    uint8 i;
    uint32 sum;

    assert(count);                                      //断言次数不能为0

    sum = 0;
    for(i=0; i<count; i++)
    {
        sum += adc_convert(ADC_INx_xx, resolution);
    }

    sum = sum/count;
    return sum;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ADC引脚初始化
//  @param      ADC_INx_Ax          可选择范围由hf_adc.h内ADCCH_enum枚举值确定
//  @return     void
//  Sample usage:           内部使用  用户无需关心
//-------------------------------------------------------------------------------------------------------------------
void adc_gpio_init(ADCCH_enum ADC_INx_xx)
{
    if(ADC_IN0_A0 == ADC_INx_xx)            gpio_init(A0, GPI, 0, IN_AIN);
    else if(ADC_IN1_A1 == ADC_INx_xx)       gpio_init(A1, GPI, 0, IN_AIN);
    else if(ADC_IN2_A2 == ADC_INx_xx)       gpio_init(A2, GPI, 0, IN_AIN);
    else if(ADC_IN3_A3 == ADC_INx_xx)       gpio_init(A3, GPI, 0, IN_AIN);
    else if(ADC_IN4_A4 == ADC_INx_xx)       gpio_init(A4, GPI, 0, IN_AIN);
    else if(ADC_IN5_A5 == ADC_INx_xx)       gpio_init(A5, GPI, 0, IN_AIN);
    else if(ADC_IN6_A6 == ADC_INx_xx)       gpio_init(A6, GPI, 0, IN_AIN);
    else if(ADC_IN7_A7 == ADC_INx_xx)       gpio_init(A7, GPI, 0, IN_AIN);
    else if(ADC_IN8_B0 == ADC_INx_xx)       gpio_init(B0, GPI, 0, IN_AIN);
    else if(ADC_IN9_B1 == ADC_INx_xx)       gpio_init(B1, GPI, 0, IN_AIN);
    else if(ADC_IN10_C0 == ADC_INx_xx)      gpio_init(C0, GPI, 0, IN_AIN);
    else if(ADC_IN11_C1 == ADC_INx_xx)      gpio_init(C1, GPI, 0, IN_AIN);
    else if(ADC_IN12_C2 == ADC_INx_xx)      gpio_init(C2, GPI, 0, IN_AIN);
    else if(ADC_IN13_C3 == ADC_INx_xx)      gpio_init(C3, GPI, 0, IN_AIN);
    else if(ADC_IN14_C4 == ADC_INx_xx)      gpio_init(C4, GPI, 0, IN_AIN);
    else if(ADC_IN15_C5 == ADC_INx_xx)      gpio_init(C5, GPI, 0, IN_AIN);
}


