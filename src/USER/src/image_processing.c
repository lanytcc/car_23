

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

/**
*args:
*histogram：长度为256的整型数组，表示图像中0~255出现的像素值的个数。
*pixel_total：整型变量，表示图像中像素的总数。
*
*https://www.ipol.im/pub/art/2016/158/article_lr.pdf
*
*/
uint8_t ostu_threshold( uint8_t* histogram, int pixel_total ){
    //用于计算均值
    unsigned int sumB = 0;
    unsigned int sum1 = 0;
    //用于计算类间方差
    float wB = 0.0f;
    float wF = 0.0f;
    float mF = 0.0f;
    //用于记录最大的类间方差
    float max_var = 0.0f;
    //用于计算类间方差
    float inter_var = 0.0f;
    //返回值：表示计算得到的阈值
    uint8_t threshold = 0;
    //索引buf
    int index_histo = 0;

    for ( index_histo = 0; index_histo < 256; ++index_histo ){
        sum1 += index_histo * histogram[ index_histo ];
    }

    for (index_histo = 0; index_histo < 256; ++index_histo){
        wB += histogram[ index_histo ];
        wF = pixel_total - wB;
        if ( wB == 0 || wF == 0 ){
            continue;
        }
        sumB += index_histo * histogram[ index_histo ];
        mF = ( sum1 - sumB ) / wF;
        inter_var = wB * wF * pow( ( sumB / wB ) - mF, 2);
        if ( inter_var >= max_var ){
            threshold = index_histo;
            max_var = inter_var;
        }
    }

    //threshold = threshold_avg(threshold);

    if(flag_show_status){
        sprintf(buf, " %d ", threshold);
        show_right_bottom_message(buf);
    }
    
    return threshold;
}

uint8_t get_ostu( uint8_t* histogram, int pixel_total ) {

    int threshold = 0;
    double max_inter_var = -1.0;

    int pixel_integral = 0;
    for (int i = 0; i < 256; ++i) {
        pixel_integral += histogram[i] * i; 
    }

    for (int t = 0; t < 255; ++t) {
    
        int back_pixels = 0;
        int front_pixels = pixel_total;

        double back_omega, front_omega, back_mean, front_mean, inter_var;

        int back_pixel_integral = 0;
        for (int i = 0; i <= t; ++i) {
            back_pixels += histogram[i];
            back_pixel_integral += histogram[i] * i;
        }

        front_pixels = pixel_total - back_pixels;
        int front_pixel_integral = pixel_integral - back_pixel_integral;

        back_omega = (double)back_pixels / pixel_total;
        front_omega = (double)front_pixels / pixel_total;

        back_mean = (double)back_pixel_integral / back_pixels;  
        front_mean = (double)front_pixel_integral / front_pixels;

        inter_var = back_omega * front_omega * (back_mean - front_mean) * (back_mean - front_mean);

        if (inter_var > max_inter_var) {
            max_inter_var = inter_var;
            threshold = t;
        }
    }

    return threshold;
}


uint8_t get_threshold() {
    uint8_t histogram[256] = {0};

    for (int i = 0; i < c_h; ++i) {
        for (int j = 0; j < c_w; ++j) {
            ++histogram[mt9v03x_image_dvp[i][j]];
        }
    }

    return get_ostu(histogram, c_h*c_w);
    //return ostu_threshold(histogram, c_h * c_w);
}