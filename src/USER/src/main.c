
#include "headfile.h"
#include <stdio.h>

char buf[30];

void time_begin(){
    SysTick->CTLR = 0;
    SysTick->CNT = 0;
    SysTick->CTLR = 1;
}

void time_end(){
    SysTick->CTLR = 0;
}

int get_time(){
    return SysTick->CNT / (SystemCoreClock/9000);
}

uint8_t rows = 0;
void show_message(const char *str, uint8_t pos) {
    if(rows >= 7){
        rows = 1;
    }
    if(pos == 0){
        ips114_show_string(0, rows * 18, str);
        rows += 1;
    } else {
        uint8_t _rows = rows;
        if(_rows == 1)_rows += 1;
        ips114_show_string(pos * 8, (_rows - 1) * 18, str);
    }
}

void show_left_top_message(const char *str) {
    if(rows >= 3){
        rows = 0;
    }
    ips114_show_string(0, rows * 17, str);
    rows += 1;
}

void show_right_top_message(const char *str) {
    ips114_show_string(108, 0, str);
}

void show_left_bottom_message(const char *str) {
    ips114_show_string(0, 118, str);
}

void show_right_bottom_message(const char *str) {
    ips114_show_string(231, 118, str);
}


void init() {
    interrupt_global_disable();
    board_init();
    Delay_Init();

    ips114_init();     //初始化屏幕
    show_message("car_23-Maye", 0);
    show_message("version:0.1.0.1  f=30", 0);
    show_message("init_begin...", 0);
    Delay_Ms(2000);

    /* 初始化电机 */
    show_message("init_motor...", 0);
    motor_init(left);
    motor_init(right);
    show_message("  ok", strlen("init_motor"));
    Delay_Ms(200);

    /* 初始化按键 */
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

    /* 初始化板载LED */
    show_message("init_led...", 0);
    board_led_init(LED1);
    board_led_init(LED2);
    show_message("  ok", strlen("init_led"));
    Delay_Ms(200);

    /* 初始化RGB引脚 */
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


enum {
    ok,no,
}camera_stauts;

void camera_check() {
    if(mt9v03x_finish_flag_dvp){
        camera_stauts = ok;
    } else {
        camera_stauts = no;
    }
}

void sample(){
    mt9v03x_finish_flag_dvp = 0;
}

const uint8_t c_w = 188, c_h = 120;
const uint8_t d_w = 240, d_h = 135;
uint8_t flag = 0;
uint8_t frame_cnt = 0;

void display() {
    if(camera_stauts == ok){ //判断是否完成采样
        ips114_show_gray_image(0, 0,
            mt9v03x_image_dvp,
            c_w, c_h,
            d_w, d_h,
            flag);
        frame_cnt += 1;
    }
}

uint8_t binarization_point(uint8_t p){
    if (flag == 0) return p;
    if (p > flag) {
        return 1;
    } else {
        return 0;
    }
}

uint8_t _frame_avg[6] = {0};
uint8_t _pos = 0;
int frame_avg(){
    if (frame_cnt >= _frame_avg[_pos]){
        _frame_avg[_pos] = frame_cnt;
    }
    else {
        _pos += 1;
        if (_pos >= 6)_pos = 0;
        _frame_avg[_pos] = 0;
    }
    uint8_t pos = _pos + 6;
    return (_frame_avg[(pos-1)%6] + _frame_avg[(pos-2)%6] + _frame_avg[(pos-3)%6])/3;
}

uint8_t white_value_status = 0;
void cal_area(int *area){
    if (flag == 0) return;
    uint8_t w = c_w / 2;
    int left = 0, right = 0;
    for (uint16_t i = 0; i < c_h; i += 4){
        for (uint16_t j = 0; j < w; j += 4) {
            left += binarization_point(mt9v03x_image_dvp[i][j]);
        }
        for (uint16_t j = w; j < c_w; j += 4) {
            right += binarization_point(mt9v03x_image_dvp[i][j]);
        }
    }
    if (white_value_status){
        sprintf(buf, " %d|%d ", left, right);
        show_right_top_message(buf);
    }
    area[0] = left;
    area[1] = right;
}

int8_t cal_factor(){

    int8_t ans = 0;

    if(flag == 0){
        return ans;
    }
    int area[2] = {0};
    cal_area(area);
    int diff = area[0] - area[1];

    ans = diff / 10;

    return ans;
}

int _speed = 3000;

void car_move(int8_t f){

    int left_speed = _speed - f * 30;
    int right_speed = _speed + f * 30;
    if(left_speed < 0){
        left_speed = 0;
    }
    if(right_speed < 0){
        right_speed = 0;
    }
    motor_forward(left, left_speed);
    motor_back(right, right_speed);

}

int main() {

    init();

    unsigned char key_value;
    uint8_t display_status = 1;
    uint8_t frame_dis_status = 1;
    uint16_t frame = 30;
    uint16_t frame_time = 1000/frame;
    int ms;
    int ms_new;
    uint16_t s_cnt = 0;

    int8_t factor = 0;

    while(1) {

        time_begin();
        ms = get_time();

        key_value = key_check();

        if(frame_dis_status){
            sprintf(buf, "%d", frame_avg());
            show_left_bottom_message(buf);
        }

        camera_check();
        if(display_status)display();
        factor = cal_factor();
        car_move(factor);
        sample();

        ms_new = get_time();
        time_end();
        int t = ms_new - ms;
        if (t > 0 && t < frame_time) {
            Delay_Ms(frame_time - t);
            t += frame_time - t;
        }
        s_cnt += t;
        if (s_cnt >= 1000) {
            s_cnt = 0;
            frame_cnt = 0;
        }

        if (key_value != 0xFF){
            while(key_value == 0x02){
                key_value = key_check();
            }
            while (key_value != 0x02) {
                if (key_value == 0x01) {
                    flag += 10;
                    sprintf(buf, "%d:flag + 10  ", flag);
                    show_left_top_message(buf);
                } else if (key_value == 0x03) {
                    flag -= 10;
                    sprintf(buf, "%d::flag - 10  ", flag);
                    show_left_top_message(buf);
                } else if (key_value == 0x04) {
                    if (display_status) {
                        display_status = 0;
                        show_left_top_message("display_status: off");
                    }
                    else {
                        display_status = 1;
                        show_left_top_message("display_status: on ");
                    }
                } else if (key_value == 0x05) {
                    if (frame_dis_status) {
                        frame_dis_status = 0;
                        show_left_top_message("frame_dis_status: off");
                    }
                    else {
                        frame_dis_status = 1;
                        show_left_top_message("frame_dis_status: on ");
                    }
                } else if (key_value == 0x06) {
                    if (white_value_status) {
                        white_value_status = 0;
                        show_left_top_message("white_value_status: off");
                    }
                    else {
                        white_value_status = 1;
                        show_left_top_message("white_value_status: on ");
                    }
                }
                key_value = key_check();
            }

        }
    }
}

