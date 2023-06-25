

#include "image_processing.h"

#include "dis_camera.h"
#include "message.h"
#include "HF_Double_DC_Motor.h"
#include <stdint.h>


uint8_t flag_show_status = 0;
#define  _avg_fre 6
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
args:

histogram：长度为256的整型数组，表示图像中0~255出现的像素值的个数。

pixel_total：整型变量，表示图像中像素的总数。

*/
uint8_t otsu_threshold( uint8_t* histogram, int pixel_total ){
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
    uint16_t index_histo = 0;

    for ( index_histo = 1; index_histo < 256; ++index_histo ){
        sum1 += index_histo * histogram[ index_histo ];
    }

    for (index_histo = 1; index_histo < 256; ++index_histo){
        wB = wB + histogram[ index_histo ];
        wF = pixel_total - wB;
        if ( wB == 0 || wF == 0 ){
            continue;
        }
        sumB = sumB + index_histo * histogram[ index_histo ];
        mF = ( sum1 - sumB ) / wF;
        inter_var = wB * wF * ( ( sumB / wB ) - mF ) * ( ( sumB / wB ) - mF );
        if ( inter_var >= max_var ){
            threshold = index_histo;
            max_var = inter_var;
        }
    }

    threshold = threshold_avg(threshold);

    if(flag_show_status){
        sprintf(buf, " %d ", threshold);
        show_right_bottom_message(buf);
    }
    
    return threshold;
}

uint8_t get_threshold() {
    uint8_t histogram[256];

    for (int i = 0; i < c_h; ++i) {
        uint8_t *one_h = mt9v03x_image_dvp[i];
        for (int j = 0; j < c_w; ++j) {
            histogram[one_h[j]]++;
        }
    }

    return otsu_threshold(histogram, c_h * c_w);
}
