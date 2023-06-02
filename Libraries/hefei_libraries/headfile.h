/*********************************************************************************************************************
 * @file            headfile.h
 * @brief           头文件
 * @author          Fantastic Potato
 * @version         1.0
 * @Target core     CH32V307VCT6
 * @date            2022-3-2
 ********************************************************************************************************************/
#ifndef HEADFILE_H_
#define HEADFILE_H_

#include "ch32v30x.h"
#include <stdio.h>
#include <stdint.h>
#include "isr.h"
#include "board.h"

#include "hf_adc.h"
#include "hf_gpio.h"
#include "hf_tim.h"
#include "hf_encoder.h"
#include "hf_flash.h"
#include "hf_i2c_hardware.h"
#include "hf_i2c_software.h"
#include "hf_nvic.h"
#include "hf_pwm.h"
#include "hf_spi.h"
#include "hf_uart.h"
#include "hf_pit.h"
#include "hf_dma.h"
#include "hf_dvp.h"
#include "hf_exti.h"
#include "hf_common_font.h"

//------外设驱动头文件
#include "HF_MT9V03X.h"
#include "HF_WS2812b.h"
#include "HF_TFT180.h"
#include "HF_Wireless_uart.h"
#include "HF_IPS114.h"
#include "HF_HC_SR04.h"
#include "HF_Double_DC_Motor.h"

//------智能车驱动头文件
#include "car_init.h"
#include "board_led.h"
#include "car_adc.h"
#include "car_uart.h"
#include "car_value_process.h"
#include "car_light.h"
#include "car_motion.h"
#include "car_reed.h"
#include <car_keys.h>


#endif /* HEADFILE_H_ */
