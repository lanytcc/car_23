#include "headfile.h"
#include "init.h"
#include "message.h"

void init() {
    interrupt_global_disable();
    board_init();
    Delay_Init();

    ips114_init();     
    show_message("--car_23-Maye--", 0);
    show_message("version:0.3.6.22", 0);
    show_message("init_begin...", 0);
    Delay_Ms(2000);

    show_message("init_motor...", 0);
    motor_init(left);
    motor_init(right);
    show_message("  ok", strlen("init_motor"));
    Delay_Ms(200);

    show_message("init_key...", 0);
    key_init(K1);
    key_init(K2);
    key_init(K3);
    key_init(K4);
    key_init(K5);
    key_init(K6);

    adc_init(ADC_IN2_A2);
    adc_init(ADC_IN3_A3);
    adc_init(ADC_IN4_A4);
    adc_init(ADC_IN5_A5);
    adc_init(ADC_IN6_A6);
    show_message("  ok", strlen("init_key"));
    Delay_Ms(200);

    show_message("init_led...", 0);
    board_led_init(LED1);
    board_led_init(LED2);
    show_message("  ok", strlen("init_led"));
    Delay_Ms(200);

    show_message("init_rgb...", 0);
    ws2812b_init();
    show_message("  ok", strlen("init_rgb"));
    Delay_Ms(200);

    show_message("init_camera...", 0);
    mt9v03x_init_dvp();
    show_message("  ok", strlen("init_camera"));
    Delay_Ms(200);

    show_message("All system is already", 0);
    Delay_Ms(1000);
}