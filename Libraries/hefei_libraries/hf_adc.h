/*********************************************************************************************************************
 * @file            hf_adc.h
 * @brief           ADC配置
 * @author          FengGuorong
 * @version         1.0
 * @Target core     CH32V307VCT6
 * @date            2022-3-8
 ********************************************************************************************************************/


#ifndef HF_ADC_H_
#define HF_ADC_H_

#include "ch32v30x.h"
#include "ch32v30x_adc.h"
#include "ch32v30x_rcc.h"
#include "hf_gpio.h"

/* 枚举定义声明 */
/* 枚举ADC通道 */
typedef enum        //CH32V307只有一个ADC，可以分时复用在很多个通道上面
{
    ADC_IN0_A0 = 0,
    ADC_IN1_A1,
    ADC_IN2_A2,
    ADC_IN3_A3,
    ADC_IN4_A4,
    ADC_IN5_A5,
    ADC_IN6_A6,
    ADC_IN7_A7,
    ADC_IN8_B0,
    ADC_IN9_B1,
    ADC_IN10_C0,
    ADC_IN11_C1,
    ADC_IN12_C2,
    ADC_IN13_C3,
    ADC_IN14_C4,
    ADC_IN15_C5,
}ADCCH_enum;

/* 枚举ADC通道分辨率 */
typedef enum
{
    ADC_8BIT = 4,       //8位分辨率
    ADC_10BIT = 2,      //10位分辨率
    ADC_12BIT = 0,      //12位分辨率
}ADCRES_enum;

/* 函数声明 */

/* 外部函数，用户可自行调用 */
/* ADC初始化 */
void adc_init(ADCCH_enum ADC_INx_xx);
/* ADC转换一次 */
uint16 adc_convert(ADCCH_enum ADC_INx_xx, ADCRES_enum resolution);
/* ADC转换N次 */
uint16 adc_mean_filter(ADCCH_enum ADC_INx_xx, ADCRES_enum resolution, uint8 count);

/* 内部函数，用户无需关注 */
/* ADC引脚初始化 */
void adc_gpio_init(ADCCH_enum ADC_INx_xx);

#endif /* HF_ADC_H_ */
