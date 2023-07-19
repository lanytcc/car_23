#include "delay.h"
#include "headfile.h"
#include "move.h"
#include "dis_camera.h"
#include "message.h"


#define BASE_SPEED 5500
#define MAX_SPEED 10000

uint8_t speed_show = 0;

int clamp(int value, int min, int max){
    if(value < min) return min;
    if(value > max) return max;
    return value;
}

static uint8_t last_x = 0;
static uint8_t last_y = 0;
#define _k 100
#define _d 160
void cal_speeds(int *left_speed, int *right_speed){

    int _x = dis_x < 0? -dis_x: dis_x;
    int _y = dis_y < 0? -dis_y: dis_y;
    int diff_x = _x - last_x;
    //int diff_y = _y - last_y;

    last_x = _x;
    last_y = _y;

    if(dis_x > 0){ 
        // x增大,向右倾斜
        *left_speed = BASE_SPEED + _x * _k + diff_x * _d;
        *right_speed = BASE_SPEED - _x * _k + diff_x * _d;
    }
    else if(dis_x < 0){
        // x减小,向左倾斜
        *left_speed = BASE_SPEED - _x * _k + diff_x * _d;
        *right_speed = BASE_SPEED + _x * _k + diff_x * _d;
    }


    if(_x < 6 && dis_y >= 12){
        motor_forward(left, 6000);
        motor_forward(right, 2000);
        Delay_Ms(400);
        motor_forward(left, 3000);
        motor_forward(right, 4000);
        Delay_Ms(500);
        motor_forward(left, 2000);
        motor_forward(right, 5500);
        Delay_Ms(400);
        motor_forward(left, 4000);
        motor_forward(right, 4000);
        Delay_Ms(200);
    }

    // 限制速度范围
    *left_speed = clamp(*left_speed, 0, MAX_SPEED);
    *right_speed = clamp(*right_speed, 0, MAX_SPEED);

}

void car_move_calculus(){
    
    int left_speed = BASE_SPEED; 
    int right_speed = BASE_SPEED;

    cal_speeds(&left_speed, &right_speed);

    if (speed_show) {
        sprintf(buf, " %d|%d|%d|%d ", dis_x, dis_y, left_speed, right_speed);
        show_right_top_message(buf);
    }
    motor_forward(left, left_speed);
    motor_forward(right, right_speed);

}


#define midline 59
void identify_garage(){
    uint8_t *one_line = mt9v03x_image_dvp[midline];
    uint8_t is_w = 1, b_cnt = 0, _buf, _b;
    _b = 0;

    for (int i = 0; i < c_w; ++i) {
        _buf = binarization_point(one_line[i]);
        if (is_w == 1 && _buf == 0) {
            is_w = 0;
            ++b_cnt;
        } else if (_buf == 1) {
            is_w = 1;
        }
    }

    if(b_cnt > 6){
        motor_forward(left, 0);
        motor_forward(right, 0);
        Delay_Ms(2000);
    }
}