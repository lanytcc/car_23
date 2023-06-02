/*********************************************************************************************************************
 * @file            hf_dvp.h
 * @brief           DVP≈‰÷√
 * @author          FengGuorong
 * @version         1.0
 * @Target core     CH32V307VCT6
 * @date            2022-3-18
 ********************************************************************************************************************/


#ifndef HF_DVP_H_
#define HF_DVP_H_

#include "ch32v30x.h"
#include "stdio.h"

#include "ch32v30x_rcc.h"
#include "ch32v30x_misc.h"
#include "ch32v30x_gpio.h"


void dvp_camera_init(uint32_t *image0, uint32_t *image1, uint16_t col_len, uint16_t row_len);
void dvp_gpio_init(
        uint8_t dvp_d0,uint8_t dvp_d1,uint8_t dvp_d2,uint8_t dvp_d3,
        uint8_t dvp_d4,uint8_t dvp_d5,uint8_t dvp_d6,uint8_t dvp_d7,
        uint8_t dvp_pclk,uint8_t dvp_vsync,uint8_t dvp_hsync);


#endif /* HF_DVP_H_ */
