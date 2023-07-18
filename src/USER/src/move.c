#include "delay.h"
#include "headfile.h"
#include "move.h"
#include "dis_camera.h"
#include "message.h"

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#define BASE_SPEED 5500 
#define MAX_SPEED 10000

uint8_t speed_show = 0;
void car_move(int16_t f){

    int left_speed = BASE_SPEED - (f * 100);
    int right_speed = BASE_SPEED + (f * 100);

    if (speed_show) {
        sprintf(buf, " %d|%d|%d ", f, left_speed, right_speed);
        show_right_top_message(buf);
    }

    if(left_speed < 0){
        left_speed = 0;
    }
    if(right_speed < 0){
        right_speed = 0;
    }
    if(left_speed > 10000){
        left_speed = 10000;
    }
    if(right_speed > 10000){
        right_speed = 10000;
    }
    motor_forward(left, left_speed);
    motor_forward(right, right_speed);
}

int clamp(int value, int min, int max){
    if(value < min) return min;
    if(value > max) return max;
    return value;
}

static uint8_t last_x = 94;
static uint8_t last_y = 60;
static uint8_t _cnt = 0;
#define _k 100
#define _d 160
void cal_speeds(int *left_speed, int *right_speed){

    int _x = abs(dis_x);
    int _y = abs(dis_y);
    int diff_x = _x - last_x;
    int diff_y = _y - last_y;

    last_x = _x;
    last_y = _y;

    if(/*_cnt == 0 && */ dis_x > 0){ 
        // x增大,向右倾斜
        *left_speed = BASE_SPEED + _x * _k + diff_x * _d;
        *right_speed = BASE_SPEED - _x * _k + diff_x * _d;
    }
    else if(/*_cnt == 0 && */ dis_x < 0){
        // x减小,向左倾斜
        *left_speed = BASE_SPEED - _x * _k + diff_x * _d;
        *right_speed = BASE_SPEED + _x * _k + diff_x * _d;
    }

    // if(_cnt == 0 && _x < 6 && dis_y > 9 && dis_y < 15){
    //     *left_speed -= 1500;
    //     *right_speed -= 1500;
    // }

    if(/*_cnt == 0 && */_x < 6 && dis_y >= 12){
        // *left_speed = 10000;
        // *right_speed = 0;
        // _cnt = 56;
        motor_forward(left, 6000);
        motor_forward(right, 2000);
        Delay_Ms(400);
        motor_forward(left, 3000);
        motor_forward(right, 4000);
        Delay_Ms(500);
        motor_forward(left, 2000);
        motor_forward(right, 5000);
        Delay_Ms(550);
    }
    // if(_cnt == 28){
    //     *left_speed = 0;
    //     *right_speed = 10000;
    // }
    // if(_cnt != 0) {
    //     --_cnt;
    // }

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