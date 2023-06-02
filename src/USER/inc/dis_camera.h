#ifndef CAMERA_H_
#define CAMERA_H_
#include "common.h"

const uint8_t c_w = 188, c_h = 120;
const uint8_t d_w = 240, d_h = 135;

enum camera_stauts{
    ok,no,
};
extern enum camera_stauts camera_stauts;

extern uint8_t white_value_status;

extern uint8_t flag;
extern uint8_t frame_cnt;

void display();

int frame_avg();

void camera_check();

void sample();

uint8_t binarization_point(uint8_t p);

void cal_area(int *area);

int8_t cal_factor();

#endif