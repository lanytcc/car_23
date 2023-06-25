#include "headfile.h"
#include "message.h"

char buf[30];

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
    ips114_show_string(128, 118, str);
}