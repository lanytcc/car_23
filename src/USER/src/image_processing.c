

#include "image_processing.h"

#include "headfile.h"

uint8_t getThreshold() {
    uint32_t t_u16i, t_u16j;

    /*灰度直方图参数*/
    uint32_t t_u16HistoGramAr[16];

    uint32_t t_u32Amount = 135*240;//像素点总数
    uint32_t t_u32PixelIntegral = 47*30;//灰度值总数

    uint32_t t_u32PixelBack = 0;//前景像素点总数
    uint32_t t_u32PixelIntegralBack = 0;//前景灰度值

    uint32_t  t_int32PixelFore = 0;//背景像素点总数
    uint32_t  t_int32PixelIntegralFore = 0;//背景灰度值

    float   t_floatOmegaBack, t_floatOmegaFore, t_floatMicroBack, t_floatMicroFore, t_floatSigmaB, t_floatSigma; //类间方差：浮点型更精确

    uint8_t t_u8Threshold = 0;

    for (t_u16j = 0; t_u16j < 16; ++t_u16j) t_u16HistoGramAr[t_u16j] = 0;

    for (t_u16j = 0; t_u16j < 135; t_u16j += 4){
        for (t_u16i = 0; t_u16i < 240; t_u16i += 4){
            t_u16HistoGramAr[(uint8_t)mt9v03x_image_dvp[t_u16j][t_u16i] / 16]++; //统计灰度级中每个像素在整幅图像中的个数

        }
    }

    t_floatSigmaB = -1;
    for (t_u16j = 0; t_u16j < 16; ++t_u16j){
        t_u32PixelBack += t_u16HistoGramAr[t_u16j];    //前景像素点数
        t_int32PixelFore = t_u32Amount - t_u32PixelBack;         //背景像素点数
        t_floatOmegaBack = (float)t_u32PixelBack / t_u32Amount;//前景像素百分比
        t_floatOmegaFore = (float)t_int32PixelFore / t_u32Amount;//背景像素百分比
        t_u32PixelIntegralBack += t_u16HistoGramAr[t_u16j] * t_u16j;  //前景灰度值
        t_int32PixelIntegralFore = t_u32PixelIntegral - t_u32PixelIntegralBack;//背景灰度值
        t_floatMicroBack = (float)t_u32PixelIntegralBack / t_u32PixelBack;   //前景灰度百分比
        t_floatMicroFore = (float)t_int32PixelIntegralFore / t_int32PixelFore;   //背景灰度百分比
        t_floatSigma = t_floatOmegaBack * t_floatOmegaFore * (t_floatMicroBack - t_floatMicroFore) * (t_floatMicroBack - t_floatMicroFore);//计算类间方差
        if (t_floatSigma > t_floatSigmaB){                    //遍历最大的类间方差g //找出最大类间方差以及对应的阈值
            t_floatSigmaB = t_floatSigma;
            t_u8Threshold = (uint8_t)t_u16j;
        }
    }
    return t_u8Threshold;                        //返回最佳阈值;
}
