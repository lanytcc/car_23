/*********************************************************************************************************************
* @file            HF_IPS114.h
* @author          FantasticPotato
* @version         1.0
* @Target core     CH32V307VCT6
* @date            20220919
*********************************************************************************************************************/

#ifndef HF_IPS114_H_
#define HF_IPS114_H_

#include "headfile.h"

#define IPS114_USE_SOFT_SPI             0                                       // Ĭ��ʹ��Ӳ�� SPI ��ʽ���� ����ʹ��Ӳ�� SPI ��ʽ����
#if IPS114_USE_SOFT_SPI                                                         // ������ ��ɫ�����Ĳ�����ȷ�� ��ɫ�ҵľ���û���õ�
//====================================================���� SPI ����====================================================
#define IPS114_SOFT_SPI_DELAY           1                                       // ���� SPI ��ʱ����ʱ���� ��ֵԽС SPI ͨ������Խ��
#define IPS114_SCL_PIN                  D4                                      // ���� SPI SCK ����
#define IPS114_SDA_PIN                  D6                                      // ���� SPI MOSI ����
//====================================================���� SPI ����====================================================
#else
//====================================================Ӳ�� SPI ����====================================================
#define IPS114_SPI_SPEED                SystemCoreClock                         // Ӳ�� SPI ����
#define IPS114_SPI                      SPI_2                                   // Ӳ�� SPI ��
#define IPS114_SCL_PIN                  SPI2_SCK_B13                            // Ӳ�� SPI SCK ����
#define IPS114_SDA_PIN                  SPI2_MOSI_B15                           // Ӳ�� SPI MOSI ����
//====================================================Ӳ�� SPI ����====================================================
#endif

#define IPS114_RST_PIN                  B7                                      // Һ����λ���Ŷ���
#define IPS114_DC_PIN                   D7                                      // Һ������λ���Ŷ���
#define IPS114_CS_PIN                   D4                                      // CS Ƭѡ����
#define IPS114_BLK_PIN                  D0                                      // Һ���������Ŷ���

#define IPS114_DC(x)                    (x? (gpio_high(IPS114_DC_PIN)): (gpio_low(IPS114_DC_PIN)))
#define IPS114_RST(x)                   (x? (gpio_high(IPS114_RST_PIN)): (gpio_low(IPS114_RST_PIN)))
#define IPS114_CS(x)                    (x? (gpio_high(IPS114_CS_PIN)): (gpio_low(IPS114_CS_PIN)))
#define IPS114_BLK(x)                   (x? (gpio_high(IPS114_BLK_PIN)): (gpio_low(IPS114_BLK_PIN)))

typedef enum
{
    IPS114_PORTAIT                      = 0,                                    // ����ģʽ
    IPS114_PORTAIT_180                  = 1,                                    // ����ģʽ  ��ת180
    IPS114_CROSSWISE                    = 2,                                    // ����ģʽ
    IPS114_CROSSWISE_180                = 3,                                    // ����ģʽ  ��ת180
}ips114_dir_enum;

#define IPS114_DEFAULT_DISPLAY_DIR      (IPS114_CROSSWISE)                  // Ĭ�ϵ���ʾ����
#define IPS114_DEFAULT_PENCOLOR         (RGB565_RED)                            // Ĭ�ϵĻ�����ɫ
#define IPS114_DEFAULT_BGCOLOR          (RGB565_WHITE)                          // Ĭ�ϵı�����ɫ

void    ips114_clear                    (void);
void    ips114_full                     (const uint16 color);
void    ips114_set_dir                  (ips114_dir_enum dir);
void    ips114_set_color                (const uint16 pen, const  uint16 bgcolor);
void    ips114_draw_point               (uint16 x, uint16 y, const uint16 color);
void    ips114_draw_line                (uint16 x_start, uint16 y_start, uint16 x_end, uint16 y_end, const uint16 color);

void    ips114_show_char                (uint16 x, uint16 y, const char dat);
void    ips114_show_string              (uint16 x, uint16 y, const char dat[]);
void    ips114_show_int                 (uint16 x,uint16 y, const int32 dat, uint8 num);
void    ips114_show_uint                (uint16 x,uint16 y, const uint32 dat, uint8 num);
void    ips114_show_float               (uint16 x,uint16 y, const float dat, uint8 num, uint8 pointnum);

void    ips114_show_binary_image        (uint16 x, uint16 y, const uint8 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height);
void    ips114_show_gray_image          (uint8 threshold);
void    ips114_show_rgb565_image        (uint16 x, uint16 y, const uint16 *image, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height, uint8 color_mode);

void    ips114_show_wave                (uint16 x, uint16 y, const uint16 *wave, uint16 width, uint16 value_max, uint16 dis_width, uint16 dis_value_max);
void    ips114_show_chinese             (uint16 x, uint16 y, uint8 size, const uint8 *chinese_buffer, uint8 number, const uint16 color);

#define ips114_displayimage7725(p, width, height)                                       (ips114_show_binary_image(0, 0, (p), (width), (height), (width), (height)))

#define ips114_displayimage032(p, width, height)                                        (ips114_show_gray_image(0, 0, (p), (width), (height), (width), (height), 0))
#define ips114_displayimage032_zoom(p, width, height, dis_width, dis_height)            (ips114_show_gray_image(0, 0, (p), (width), (height), (dis_width), (dis_height), 0))
#define ips114_displayimage032_zoom1(x, y, p, width, height, dis_width, dis_height)     (ips114_show_gray_image((x), (y), (p), (width), (height), (dis_width), (dis_height), 0))

#define ips114_displayimage8660(p, width, height)                                       (ips114_show_rgb565_image(0, 0, (p), (width), (height), (width), (height), 1))
#define ips114_displayimage8660_zoom(p, width, height, dis_width, dis_height)           (ips114_show_rgb565_image(0, 0, (p), (width), (height), (dis_width), (dis_height), 1))
#define ips114_displayimage8660_zoom1(x, y, p, width, height, dis_width, dis_height)    (ips114_show_rgb565_image((x), (y), (p), (width), (height), (dis_width), (dis_height), 1))

void    ips114_init                     (void);

#endif /* HF_IPS114_H_ */
