#ifndef MOVE_H_
#define MOVE_H_
#include "common.h"
#include <stdint.h>

extern int _speed;
extern uint8_t speed_show;

void car_move(int16_t f);
void car_move_calculus();

#endif