#include "headfile.h"
#include "move.h"
#include "dis_camera.h"
#include "message.h"

#include <math.h>
#include <stdlib.h>

int _speed = 4000;

uint8_t speed_show = 0;
void car_move(int16_t f){

    int left_speed = _speed - (f * 100);
    int right_speed = _speed + (f * 100);

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

#define BASE_SPEED 5000 
#define MAX_SPEED 10000

int clamp(int value, int min, int max){
    if(value < min) return min;
    if(value > max) return max;
    return value;
}

void cal_speeds(int *left_speed, int *right_speed){

    if(diff_x > 0){ 
        // x增大,向右倾斜
        *right_speed = BASE_SPEED + pow(abs(diff_x), 3.0);
        *left_speed = BASE_SPEED - pow(abs(diff_x), 3.0);
    }
    else if(diff_x < 0){
        // x减小,向左倾斜
        *left_speed = BASE_SPEED - pow(abs(diff_x), 3.0);
        *right_speed = BASE_SPEED + pow(abs(diff_x), 3.0);
    }

    if(diff_y > 0){
        // y增大
        *left_speed += pow(abs(diff_y), 2.0);
        *right_speed += pow(abs(diff_y), 2.0); 
    }
    else if(diff_y < 0){
        // y减小
        *left_speed -= pow(abs(diff_y), 2.0);
        *right_speed -= pow(abs(diff_y), 2.0);
    }

    // 限制速度范围
    *left_speed = clamp(*left_speed, 0, MAX_SPEED);
    *right_speed = clamp(*right_speed, 0, MAX_SPEED);

}

void car_move_calculus(){
    
    int left_speed = BASE_SPEED; 
    int right_speed = BASE_SPEED;

    cal_speeds(&left_speed, &right_speed);

    motor_forward(left, left_speed);
    motor_forward(right, right_speed);

}