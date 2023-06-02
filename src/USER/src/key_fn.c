#include "headfile.h"
#include "key_fn.h"
#include "message.h"
#include "dis_camera.h"
#include "main.h"

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
                    show_left_top_message("display_status: off");
                }
                else {
                    display_status = 1;
                    show_left_top_message("display_status: on ");
                }
            } else if (key_value == 0x05) {
                if (frame_dis_status) {
                    frame_dis_status = 0;
                    show_left_top_message("frame_dis_status: off");
                }
                else {
                    frame_dis_status = 1;
                    show_left_top_message("frame_dis_status: on ");
                }
            } else if (key_value == 0x06) {
                if (white_value_status) {
                    white_value_status = 0;
                    show_left_top_message("white_value_status: off");
                }
                else {
                    white_value_status = 1;
                    show_left_top_message("white_value_status: on ");
                }
            }
            key_value = key_check();
        }
    }
}