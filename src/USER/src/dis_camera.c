#include "headfile.h"
#include "dis_camera.h"
#include "message.h"
#include "image_processing.h"
#include <stdint.h>


const uint8_t c_w = 188, c_h = 120;
const uint8_t d_w = 240, d_h = 135;
enum camera_stauts camera_stauts;
uint8_t flag = 0;
uint8_t frame_cnt = 0;

void change_flag(){
    flag = get_threshold();
}

void display() {
    if(camera_stauts == ok){ //判断是否完成采集
        ips114_show_gray_image(flag);
        frame_cnt += 1;
    }
}

static uint8_t _frame_avg[6] = {0};
static uint8_t _pos = 0;
uint8_t frame_avg(){
    _frame_avg[_pos] = frame_cnt;
    _pos += 1;
    if (_pos >= 6)_pos = 0;
    uint8_t pos = _pos + 6;
    return (_frame_avg[(pos-1)%6] + _frame_avg[(pos-2)%6] + _frame_avg[(pos-3)%6])/3;
}

void camera_check() {
    if(mt9v03x_finish_flag_dvp){
        camera_stauts = ok;
    } else {
        camera_stauts = no;
    }
}

void sample(){
    mt9v03x_finish_flag_dvp = 0;
}

uint8_t binarization_point(uint8_t p){
    if (flag == 0) return p;
    if (p > flag) {
        return 1;
    } else {
        return 0;
    }
}


uint8_t center_value_status = 0;
void cal_center(int *pair){
    if (flag == 0) return;

    int x_sum = 0;
    int y_sum = 0;
    int pixel_count = 0;

    for(int i = 0; i < c_h; ++i) {
        for(int j = 0; j < c_w; ++j) {
            if( binarization_point(mt9v03x_image_dvp[i][j]) == 1 ) {
                x_sum += j;
                y_sum += i; 
                ++pixel_count;
            }
        }
    }

    pair[0] = x_sum / pixel_count;
    pair[1] = y_sum / pixel_count;

    if (center_value_status){
        sprintf(buf, " %d|%d ", pair[0], pair[1]);
        show_right_top_message(buf);
    }
}


uint8_t s_x = 94;
uint8_t s_y = 60;
int dis_x = 0;
int dis_y = 0;
int16_t cal_factor(){

    int16_t ans = 0;

    if(flag == 0){
        return ans;
    }

    int xy[2] = {0};
    cal_center(xy);

    dis_x = xy[0] - s_x;
    dis_y = xy[1] - s_y;

    ans = dis_x;

    return ans;
}