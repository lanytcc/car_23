#include "headfile.h"
#include "key_fn.h"
#include "message.h"
#include "dis_camera.h"
#include "image_processing.h"
#include "main.h"
#include "move.h"

void key_fn(unsigned char key_value){
    if (key_value != 0xFF){
        while(key_value == 0x02){
            key_value = key_check();
        }
        while (key_value != 0x02) {
            if (key_value == 0x01) {
                flag += 10;
                sprintf(buf, "%d:flag + 10  ", flag);
                show_left_top_message(buf);
            } else if (key_value == 0x03) {
                flag -= 10;
                sprintf(buf, "%d::flag - 10  ", flag);
                show_left_top_message(buf);
            } else if (key_value == 0x04) {
                if (display_status) {
                    display_status = 0;
                    frame_dis_status = 0;
                    show_left_top_message("display_status: off");
                    show_left_top_message("frame_dis_status: off");
                }
                else {
                    display_status = 1;
                    frame_dis_status = 1;
                    show_left_top_message("display_status: on ");
                    show_left_top_message("frame_dis_status: on ");
                }
            } else if (key_value == 0x05) {
                if (center_value_status) {
                    center_value_status = 0;
                    show_left_top_message("center_value_status: off");
                }
                else {
                    center_value_status = 1;
                    show_left_top_message("center_value_status: on ");
                }
            } else if (key_value == 0x06) {
                // if (flag_show_status) {
                //     flag_show_status = 0;
                //     show_left_top_message("flag_show_status: off ");
                // }
                // else {
                //     flag_show_status = 1;
                //     show_left_top_message("flag_show_status: on ");
                // }
                if (speed_show) {
                    speed_show = 0;
                    show_left_top_message("speed_show: off");
                }
                else {
                    speed_show = 1;
                    show_left_top_message("speed_show: on ");
                }
            }
            key_value = key_check();
        }
    }
}
