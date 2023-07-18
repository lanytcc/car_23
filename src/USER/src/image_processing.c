

#include <stdint.h>

#include "image_processing.h"

#include "dis_camera.h"
#include "message.h"
#include "HF_Double_DC_Motor.h"


uint8_t flag_show_status = 0;
#define  _avg_fre 4
static uint8_t _threshold_avg[_avg_fre] = {0};
static uint8_t _pos = 0;
uint8_t threshold_avg(uint8_t threshold){
    _threshold_avg[_pos] = threshold;
    _pos += 1;
    if (_pos >= _avg_fre)_pos = 0;
    uint8_t pos = _pos + _avg_fre;
    uint16_t ans = 0;
    for (uint8_t i = 1; i <= _avg_fre; ++i) {
        ans += _threshold_avg[(pos-i)%_avg_fre];
    }
    return ans/_avg_fre;
}

/**
直接计算类间方差,没有遍历所有阈值,需要提前指定阈值的范围(min到max)。
*/
double ostu(uint8_t* hist, int min, int max, int ht, int wth){

	double sum = 0.0, mean_bg = 0.0, var_bg = 0.0;

    for (uint8_t i = min; i <= max; ++i) {
        sum += hist[i];
        mean_bg += i * hist[i];
    }

    if (sum != 0.0) {
        mean_bg /= sum;
        for (uint8_t j = min; j <= max; ++j) {
            double diff = j - mean_bg;
            var_bg += pow(diff, 2) * hist[j];
        }
        return sum / (ht * wth) * pow(var_bg / sum, 2);
    }

	return 0;
}


uint8_t get_ostu(){

    int16_t i, j;
    uint32_t pixel_count = 0, pixel_sum = 0;
    uint32_t pixel_back_count = 0, pixel_back_sum = 0;
    uint32_t pixel_fore_count = 0, pixel_fore_sum = 0;
    double omega_back, omega_fore, micro_back, micro_fore, sigma_b, sigma;
    int16_t min_value, max_value;
    uint8_t threshold = 0;
    uint8_t histogram[256] = {0};

    // Calculate histogram and total pixel count and total intensity
    for (j = 0; j < c_h; j++) { 
        for (i = 0; i < c_w; i++) { 
            histogram[mt9v03x_image_dvp[j][i]]++;
            pixel_count++;
            pixel_sum += mt9v03x_image_dvp[j][i];
        } 
    }

    min_value = 0;
    while (histogram[min_value] == 0) min_value++;
    
    max_value = 255;
    while (histogram[max_value] == 0) max_value--;

    if (max_value == min_value) return max_value;
    if (min_value + 1 == max_value) return min_value;

    sigma_b = -1;
    for (j = min_value; j < max_value; j++) {
        
        pixel_back_count += histogram[j];
        pixel_back_sum += j * histogram[j];
        
        pixel_fore_count = pixel_count - pixel_back_count;
        pixel_fore_sum = pixel_sum - pixel_back_sum;
        
        omega_back = (double)pixel_back_count / pixel_count;
        omega_fore = (double)pixel_fore_count / pixel_count;
        
        micro_back = pixel_back_count ? (double)pixel_back_sum / pixel_back_count : 0.0;
        micro_fore = pixel_fore_count ? (double)pixel_fore_sum / pixel_fore_count : 0.0;
        
        sigma = omega_back * omega_fore * pow(micro_back - micro_fore, 2.0);
        
        if (sigma > sigma_b) {
            sigma_b = sigma;
            threshold = j;
        }
    }

    if(flag_show_status){
        sprintf(buf, " %d ", threshold);
        show_right_bottom_message(buf);
    }

    return threshold;
}

uint8_t get_threshold() {
    // uint8_t histogram[256] = {0};

    // for (int i = 0; i < c_h; ++i) {
    //     for (int j = 0; j < c_w; ++j) {
    //         ++histogram[mt9v03x_image_dvp[i][j]];
    //     }
    // }

    return get_ostu();
    //return get_ostu(histogram, c_h*c_w);
    //return ostu_threshold(histogram, c_h * c_w);
}