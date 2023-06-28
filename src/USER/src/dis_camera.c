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
    if(camera_stauts == ok){ //�ж��Ƿ���ɲ���
        ips114_show_gray_image(0, 0,
            mt9v03x_image_dvp,
            c_w, c_h,
            d_w, d_h,
            flag);
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

uint8_t white_value_status = 0;
void cal_area(int *area){
    if (flag == 0) return;
    uint8_t w = c_w / 2;
    int left = 0, right = 0;
    for (uint8_t i = 0; i < c_h; ++i){
        uint8_t *one_w = mt9v03x_image_dvp[i];
        for (uint8_t j = 0; j < w; ++j) {
            left += binarization_point(one_w[j]);
        }
        for (uint8_t j = w; j < c_w; ++j) {
            right += binarization_point(one_w[j]);
        }
    }
    if (white_value_status){
        sprintf(buf, " %d|%d ", left, right);
        show_right_top_message(buf);
    }
    area[0] = left;
    area[1] = right;
}

int8_t cal_factor(){

    int8_t ans = 0;

    if(flag == 0){
        return ans;
    }
    int area[2] = {0};
    cal_area(area);
    int diff = area[0] - area[1];

    ans = diff / 10;

    return ans;
}