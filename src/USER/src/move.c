#include "headfile.h"
#include "move.h"
#include "dis_camera.h"
#include "message.h"

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#define BASE_SPEED 5000 
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
void cal_speeds(int *left_speed, int *right_speed){

    int _x = abs(dis_x);
    int _y = abs(dis_y);
    int diff_x = _x - last_x;
    int diff_y = _y - last_y;

    last_x = _x;
    last_y = _y;

    if(dis_x > 0){ 
        // x增大,向右倾斜
        *right_speed = BASE_SPEED - _x * 100;
        *left_speed = BASE_SPEED + _x * 100;
    }
    else if(dis_x < 0){
        // x减小,向左倾斜
        *left_speed = BASE_SPEED + _x * 100;
        *right_speed = BASE_SPEED - _x * 100;
    }

    if(dis_y > 0){
        // y增大
        *left_speed += _y * _y * 5;
        *right_speed += _y * _y * 5; 
    }
    else if(dis_y < 0){
        // y减小
        *left_speed -= _y * _y * 5;
        *right_speed -= _y * _y * 5;
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
        sprintf(buf, " %d|%d|%d ", dis_x, left_speed, right_speed);
        show_right_top_message(buf);
    }
    motor_forward(left, left_speed);
    motor_forward(right, right_speed);

}