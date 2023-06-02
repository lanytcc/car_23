#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "common.h"

extern char buf[30];

void show_message(const char *str, uint8_t pos);

void show_left_top_message(const char *str);

void show_right_top_message(const char *str);

void show_left_bottom_message(const char *str);

void show_right_bottom_message(const char *str);
#endif