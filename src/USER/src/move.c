#include "headfile.h"
#include "move.h"
#include "message.h"

int _speed = 5000;

uint8_t speed_show = 0;
void car_move(int16_t f){

    int left_speed = _speed - (f * 30);
    int right_speed = _speed + (f * 30);

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
    motor_forward(left, left_speed);
    motor_forward(right, right_speed);
}