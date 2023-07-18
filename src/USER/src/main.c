
#include <stdint.h>
#include <stdio.h>

#include "HF_Double_DC_Motor.h"

#include "init.h"
#include "message.h"
#include "time.h"
#include "dis_camera.h"
#include "move.h"
#include "key_fn.h"

uint8_t display_status = 1;
uint8_t frame_dis_status = 1;

int main() {

    init();

    unsigned char key_value;
    uint16_t frame = 30;
    uint16_t frame_time = 1000/frame;
    uint8_t frame_show = 0;
    int ms;
    int ms_new;
    uint16_t s_cnt = 0;

    int16_t factor = 0;

    while(1) {


        time_begin();
        ms = get_time();
        if(frame_dis_status){
            sprintf(buf, "%d", frame_show);
            show_left_bottom_message(buf);
        }

        key_value = key_check();

        camera_check();
        change_flag();
        if(display_status)display();
        factor = cal_factor();
        car_move_calculus();
        sample();

        ms_new = get_time();
        time_end();
        int t = ms_new - ms;
        if (t > 0 && t < frame_time) {
            Delay_Ms(frame_time - t);
            t += frame_time - t;
        }
        s_cnt += t;
        if (s_cnt >= 1000) {
            frame_show = frame_avg();
            s_cnt = 0;
            frame_cnt = 0;
        }

        key_fn(key_value);
    }
}