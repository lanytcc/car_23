#ifndef CAMERA_H_
#define CAMERA_H_
#include "common.h"

extern const uint8_t c_w, c_h;
extern const uint8_t d_w, d_h;

enum camera_stauts{
    ok,no,
};
extern enum camera_stauts camera_stauts;

extern uint8_t white_value_status;

extern uint8_t flag;
extern uint8_t frame_cnt;

void change_flag();

void display();

uint8_t frame_avg();

void camera_check();

void sample();

uint8_t binarization_point(uint8_t p);

void cal_area(int *area);

int8_t cal_factor();

#endif