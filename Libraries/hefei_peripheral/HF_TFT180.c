/*********************************************************************************************************************
* @file            HF_TFT180.c
* @author
* @version         1.0
* @Target core     CH32V307VCT6
* @date            20220905
********************************************************************************************************************/

#include "HF_TFT180.h"

static uint16_t tft180_pencolor = TFT180_DEFAULT_PENCOLOR;
static uint16_t tft180_bgcolor = TFT180_DEFAULT_BGCOLOR;

static tft180_dir_enum tft180_display_dir = TFT180_DEFAULT_DISPLAY_DIR;
static uint8_t tft180_x_max = 160;
static uint8_t tft180_y_max = 128;

#if TFT180_USE_SOFT_SPI
static soft_spi_info_struct             tft180_spi;
#define tft180_write_8bit_data(data)    (soft_spi_write_8bit(&TFT180_spi, (data)))
#define tft180_write_16bit_data(data)   (soft_spi_write_16bit(&TFT180_spi, (data)))
#else
#define tft180_write_8bit_data(data)    (spi_write_8bit(TFT180_SPI, (data)))
#define tft180_write_16bit_data(data)   (spi_write_16bit(TFT180_SPI, (data)))
#endif

//-------------------------------------------------------------------------------------------------------------------
// @brief       写命令 内部调用
// @note        内部调用 用户无需关心
//-------------------------------------------------------------------------------------------------------------------
static void tft180_write_index (const uint8_t dat)
{
    TFT180_CS(1);
    TFT180_CS(0);
    TFT180_DC(0);
    tft180_write_8bit_data(dat);
    TFT180_CS(1);
    TFT180_DC(1);
    TFT180_CS(0);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       设置显示区域 内部调用
// @param       x1              起始x轴坐标
// @param       y1              起始y轴坐标
// @param       x2              结束x轴坐标
// @param       y2              结束y轴坐标
// @return      void
// @note        内部调用 用户无需关心
//-------------------------------------------------------------------------------------------------------------------
static void tft180_set_region (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    if(tft180_display_dir == TFT180_PORTAIT || tft180_display_dir == TFT180_PORTAIT_180)
    {
        tft180_write_index(0x2a);
        tft180_write_8bit_data(0x00);
        tft180_write_8bit_data(x1 + 2);
        tft180_write_8bit_data(0x00);
        tft180_write_8bit_data(x2 + 2);

        tft180_write_index(0x2b);
        tft180_write_8bit_data(0x00);
        tft180_write_8bit_data(y1 + 1);
        tft180_write_8bit_data(0x00);
        tft180_write_8bit_data(y2 + 1);
    }
    else
    {
        tft180_write_index(0x2a);
        tft180_write_8bit_data(0x00);
        tft180_write_8bit_data(x1 + 1);
        tft180_write_8bit_data(0x0);
        tft180_write_8bit_data(x2 + 1);

        tft180_write_index(0x2b);
        tft180_write_8bit_data(0x00);
        tft180_write_8bit_data(y1 + 2);
        tft180_write_8bit_data(0x00);
        tft180_write_8bit_data(y2 + 2);
    }
    tft180_write_index(0x2c);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       液晶清屏函数 将屏幕清空成背景颜色
// @param       void
// @return      void
// Sample usage:                tft180_clear();
//-------------------------------------------------------------------------------------------------------------------
void tft180_clear (void)
{
    uint32_t i = tft180_x_max * tft180_y_max;

    tft180_set_region(0, 0, tft180_x_max - 1, tft180_y_max - 1);
    for( ; i > 0; i --)
    {
        tft180_write_16bit_data(tft180_bgcolor);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       液晶清屏函数
// @param       color           颜色格式 RGB565 或者可以使用 hf_common_font.h 内常用颜色宏定义
// @return      void
// Sample usage:                tft180_full(RGB565_YELLOW);
//-------------------------------------------------------------------------------------------------------------------
void tft180_full (const uint16_t color)
{
    uint32 i = tft180_x_max * tft180_y_max;

    tft180_set_region(0, 0, tft180_x_max - 1, tft180_y_max - 1);
    for( ; i > 0; i --)
    {
        tft180_write_16bit_data(color);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       设置显示方向 这个函数只有在初始化屏幕之前调用才生效
// @param       dir             显示方向  参照 hf_device_ips114.h 内 tft180_dir_enum 枚举体定义
// @return      void
// Sample usage:                tft180_set_dir(TFT180_CROSSWISE);
//-------------------------------------------------------------------------------------------------------------------
void tft180_set_dir (tft180_dir_enum dir)
{
    tft180_display_dir = dir;
    if(dir < 2)
    {
        tft180_x_max = 128;
        tft180_y_max = 160;
    }
    else
    {
        tft180_x_max = 160;
        tft180_y_max = 128;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       设置显示颜色
// @param       pen             颜色格式 RGB565 或者可以使用 hf_common_font.h 内常用颜色宏定义
// @param       bgcolor         颜色格式 RGB565 或者可以使用 hf_common_font.h 内常用颜色宏定义
// @return      void
// Sample usage:                tft180_set_color(RGB565_WHITE, RGB565_BLACK);
//-------------------------------------------------------------------------------------------------------------------
void tft180_set_color (uint16_t pen, const uint16_t bgcolor)
{
    tft180_pencolor = pen;
    tft180_bgcolor = bgcolor;
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       液晶画点
// @param       x               坐标x方向的起点 参数范围 [0, tft180_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, tft180_y_max-1]
// @param       dat             需要显示的颜色
// @return      void
// Sample usage:                tft180_draw_point(0, 0, RGB565_RED);            // 坐标 0,0 画一个红色的点
//-------------------------------------------------------------------------------------------------------------------
void tft180_draw_point (uint16_t x, uint16_t y, const uint16_t color)
{

    tft180_set_region(x, y, x, y);
    tft180_write_16bit_data(color);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       液晶显示字符
// @param       x               坐标x方向的起点 参数范围 [0, tft180_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, tft180_y_max-1]
// @param       dat             需要显示的字符
// @return      void
// Sample usage:                tft180_show_char(0, 0, 'x');                    // 坐标 0,0 写一个字符 x
//-------------------------------------------------------------------------------------------------------------------
void tft180_show_char (uint16_t x, uint16_t y, const char dat)
{

    uint8_t i,j;
    uint8_t temp;

    for(i = 0; i < 16; i ++)
    {
        tft180_set_region(x, y + i, x + 7, y + i);
        temp = tft_ascii[dat - 32][i];                                          // 减 32 因为是取模是从空格开始取得 空格在 ascii 中序号是 32
        for(j = 0; j < 8; j ++)
        {
            if(temp & 0x01)
                tft180_write_16bit_data(tft180_pencolor);
            else
                tft180_write_16bit_data(tft180_bgcolor);
            temp >>= 1;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       液晶显示字符串
// @param       x               坐标x方向的起点 参数范围 [0, tft180_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, tft180_y_max-1]
// @param       dat             需要显示的字符串
// @return      void
// Sample usage:                tft180_show_string(0, 0, "ABCD");
//-------------------------------------------------------------------------------------------------------------------
void tft180_show_string (uint16_t x, uint16_t y, const char dat[])
{

    uint16_t j = 0;
    while(dat[j] != '\0')
    {
        tft180_show_char(x + 8*j, y, dat[j]);
        j++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       液晶显示32位有符号(去除整数部分无效的0)
// @param       x               坐标x方向的起点 参数范围 [0, tft180_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, tft180_y_max-1]
// @param       dat             需要显示的变量 数据类型 int32
// @param       num             需要显示的位数 最高10位  不包含正负号
// @return      void
// Sample usage:                tft180_show_int(0, 0, x, 3);                    // x 可以为 int32 int16 int8 类型
// note:                        负数会显示一个 ‘-’号   正数显示一个空格
//-------------------------------------------------------------------------------------------------------------------
void tft180_show_int (uint16_t x, uint16_t y, const int32 dat, uint8_t num)
{

    int32_t dat_temp = dat;
    int32_t offset = 1;
    char data_buffer[12];

    memset(data_buffer, 0, 12);
    memset(data_buffer, ' ', num+1);

    if(num < 10)
    {
        for(; num > 0; num--)
            offset *= 10;
        dat_temp %= offset;
    }
    int_to_str(data_buffer, dat_temp);
    tft180_show_string(x, y, (const char *)&data_buffer);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       液晶显示32位有符号(去除整数部分无效的0)
// @param       x               坐标x方向的起点 参数范围 [0, tft180_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, tft180_y_max-1]
// @param       dat             需要显示的变量 数据类型 uint32
// @param       num             需要显示的位数 最高10位  不包含正负号
// @return      void
// Sample usage:                tft180_show_uint(0, 0, x, 3);                   // x 可以为 uint32 uint16 uint8 类型
// note:                        负数会显示一个 ‘-’号   正数显示一个空格
//-------------------------------------------------------------------------------------------------------------------
void tft180_show_uint (uint16_t x, uint16_t y, const uint32_t dat, uint8_t num)
{

    uint32_t dat_temp = dat;
    int32_t offset = 1;
    char data_buffer[12];
    memset(data_buffer, 0, 12);
    memset(data_buffer, ' ', num);

    if(num < 10)
    {
        for(; num>0; num--)
            offset *= 10;
        dat_temp %= offset;
    }
    uint_to_str(data_buffer, dat_temp);
    tft180_show_string(x, y, (const char *)&data_buffer);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       液晶显示浮点数(去除整数部分无效的0)
// @param       x               坐标x方向的起点 参数范围 [0, tft180_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, tft180_y_max-1]
// @param       dat             需要显示的变量，数据类型float或double
// @param       num             整数位显示长度   最高10位
// @param       pointnum        小数位显示长度   最高6位
// @return      void
// Sample usage:                tft180_show_float(0, 0, x, 2, 3);               // 显示浮点数   整数显示2位   小数显示三位
// @note                        特别注意当发现小数部分显示的值与你写入的值不一样的时候，
//                              可能是由于浮点数精度丢失问题导致的，这并不是显示函数的问题，
//                              有关问题的详情，请自行百度学习   浮点数精度丢失问题。
//                              负数会显示一个 ‘-’号   正数显示一个空格
//-------------------------------------------------------------------------------------------------------------------
void tft180_show_float (uint16_t x, uint16_t y, const float dat, uint8_t num, uint8_t pointnum)
{

    float dat_temp = dat;
    float offset = 1.0;
    char data_buffer[17];
    memset(data_buffer, 0, 17);
    memset(data_buffer, ' ', num + pointnum + 2);

    if(num < 10)
    {
        for(; num > 0; num--)
            offset *= 10;
        dat_temp = dat_temp - ((int)dat_temp / (int)offset) * offset;
    }
    float_to_str(data_buffer, dat_temp, pointnum);
    tft180_show_string(x, y, data_buffer);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       TFT180 显示二值图像 数据每八个点组成一个字节数据
// @param       x               坐标x方向的起点 参数范围 [0, tft180_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, tft180_y_max-1]
// @param       *image          图像数组指针
// @param       width           图像实际宽度
// @param       height          图像实际高度
// @param       dis_width       图像显示宽度 参数范围 [0, tft180_x_max]
// @param       dis_height      图像显示高度 参数范围 [0, tft180_y_max]
// @return      void
//-------------------------------------------------------------------------------------------------------------------
void tft180_show_binary_image (uint16_t x, uint16_t y, const uint8_t *image, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height)
{
    uint32_t i = 0, j = 0;
    uint8_t temp = 0;
    uint32_t width_index = 0, height_index = 0;

    tft180_set_region(x, y, x+dis_width - 1, y+dis_height-1);             // 设置显示区域

    for(j = 0; j<dis_height; j++)
    {
        height_index = j * height / dis_height;
        for(i=0; i<dis_width; i++)
        {
            width_index = i * width / dis_width;
            temp = *(image + height_index * width / 8 + width_index / 8);       // 读取像素点
            if(0x80 & (temp << (width_index % 8)))
                tft180_write_16bit_data(RGB565_WHITE);
            else
                tft180_write_16bit_data(RGB565_BLACK);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       TFT180 显示 8bit 灰度图像 带二值化阈值
// @param       x               坐标x方向的起点 参数范围 [0, tft180_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, tft180_y_max-1]
// @param       *image          图像数组指针
// @param       width           图像实际宽度
// @param       height          图像实际高度
// @param       dis_width       图像显示宽度 参数范围 [0, tft180_x_max]
// @param       dis_height      图像显示高度 参数范围 [0, tft180_y_max]
// @param       threshold       二值化显示阈值 0-不开启二值化
// @return      void
//-------------------------------------------------------------------------------------------------------------------
void tft180_show_gray_image (uint16_t x, uint16_t y, const uint8_t *image, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height, uint8_t threshold)
{

    uint32 i = 0, j = 0;
    uint16 color = 0,temp = 0;
    uint32 width_index = 0, height_index = 0;

    tft180_set_region(x, y, x+dis_width-1, y+dis_height-1);             // 设置显示区域

    for(j = 0; j<dis_height; j ++)
    {
        height_index = j * height / dis_height;
        for(i = 0; i < dis_width; i ++)
        {
            width_index = i * width / dis_width;
            temp = *(image + height_index * width + width_index);               // 读取像素点
            if(threshold == 0)
            {
                color = (0x001f & ((temp) >> 3)) << 11;
                color = color | (((0x003f) & ((temp) >> 2)) << 5);
                color = color | (0x001f & ((temp) >> 3));
                tft180_write_16bit_data(color);
            }
            else if(temp < threshold)
                tft180_write_16bit_data(RGB565_BLACK);
            else
                tft180_write_16bit_data(RGB565_WHITE);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       TFT180 显示 RGB565 彩色图像
// @param       x               坐标x方向的起点 参数范围 [0, tft180_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, tft180_y_max-1]
// @param       *image          图像数组指针
// @param       width           图像实际宽度
// @param       height          图像实际高度
// @param       dis_width       图像显示宽度 参数范围 [0, tft180_x_max]
// @param       dis_height      图像显示高度 参数范围 [0, tft180_y_max]
// @param       color_mode      色彩模式 0-低位在前 1-高位在前
// @return      void
//-------------------------------------------------------------------------------------------------------------------
void tft180_show_rgb565_image (uint16_t x, uint16_t y, const uint16_t *image, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height, uint8_t color_mode)
{

    uint32_t i = 0, j = 0;
    uint16_t color = 0;
    uint32_t width_index = 0, height_index = 0;

    tft180_set_region(x, y, x+dis_width-1, y+dis_height-1);             // 设置显示区域

    for(j = 0; j<dis_height; j++)
    {
        height_index = j * height / dis_height;
        for(i = 0; i<dis_width; i++)
        {
            width_index = i * width / dis_width;
            color = *(image + height_index * width + width_index);              // 读取像素点
            if(color_mode)
                color = ((color & 0xff) << 8) | (color >> 8);
            tft180_write_16bit_data(color);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       TFT180 显示波形
// @param       x               坐标x方向的起点 参数范围 [0, tft180_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, tft180_y_max-1]
// @param       *wave           波形数组指针
// @param       width           波形实际宽度
// @param       value_max       波形实际最大值
// @param       dis_width       波形显示宽度 参数范围 [0, tft180_x_max]
// @param       dis_value_max   波形显示最大值 参数范围 [0, tft180_y_max]
// @return      void
// Sample usage:                tft180_show_wave(32, 64, data, 128, 64, 64, 32);
//-------------------------------------------------------------------------------------------------------------------
void tft180_show_wave (uint16_t x, uint16_t y, const uint16_t *wave, uint16_t width, uint16_t value_max, uint16_t dis_width, uint16_t dis_value_max)
{

    uint32_t i = 0, j = 0;
    uint32_t width_index = 0, value_max_index = 0;

    tft180_set_region(x, y, x+dis_width-1, y+dis_value_max-1);          // 设置显示区域
    for(i=0; i<dis_value_max; i++)
    {
        for(j=0; j<dis_width; j++)
        {
            tft180_write_16bit_data(tft180_bgcolor);
        }
    }

    for(i=0; i<dis_width; i++)
    {
        width_index = i * width / dis_width;
        value_max_index = *(wave + width_index) * (dis_value_max - 1) / value_max;
        tft180_draw_point(i+x, (dis_value_max-1)-value_max_index+y, tft180_pencolor);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       汉字显示
// @param       x               坐标x方向的起点 参数范围 [0, tft180_x_max-1]
// @param       y               坐标y方向的起点 参数范围 [0, tft180_y_max-1]
// @param       size            取模的时候设置的汉字字体大小 也就是一个汉字占用的点阵长宽为多少个点 取模的时候需要长宽是一样的
// @param       *chinese_buffer 需要显示的汉字数组
// @param       number          需要显示多少位
// @param       color           显示颜色
// @return      void
// @Note                        使用PCtoLCD2002软件取模           阴码、逐行式、顺向   16*16
//-------------------------------------------------------------------------------------------------------------------
void tft180_show_chinese (uint16_t x, uint16_t y, uint8_t size, const uint8_t *chinese_buffer, uint8_t number, const uint16_t color)
{

    int i, j, k;
    uint8_t temp, temp1, temp2;
    const uint8_t *p_data;

    temp2 = size / 8;

    tft180_set_region(x, y, number*size-1+x, y+size-1);

    for(i=0; i<size; i++)
    {
        temp1 = number;
        p_data = chinese_buffer + i * temp2;
        while(temp1 --)
        {
            for(k=0; k<temp2; k++)
            {
                for(j=8; j>0; j--)
                {
                    temp = (*p_data >> (j-1)) & 0x01;
                    if(temp)    tft180_write_16bit_data(color);
                    else        tft180_write_16bit_data(tft180_bgcolor);
                }
                p_data ++;
            }
            p_data = p_data - temp2 + temp2 * size;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief       1.14寸 IPS液晶初始化
// @return      void
// Sample usage:                tft180_init();
//-------------------------------------------------------------------------------------------------------------------
void tft180_init (void)
{
#if TFT180_USE_SOFT_SPI
    soft_spi_init(&tft180_spi, 0, TFT180_SOFT_SPI_DELAY, TFT180_SCL_PIN, TFT180_SDA_PIN, SOFT_SPI_PIN_NULL, SOFT_SPI_PIN_NULL);
#else
    spi_init(TFT180_SPI, TFT180_SCL_PIN, TFT180_SDA_PIN, SPI_MISO_NULL, SPI_CS_NULL, 0, TFT180_SPI);
#endif

    gpio_init(TFT180_DC_PIN, GPO, GPIO_LOW, OUT_PP);
    gpio_init(TFT180_RES_PIN, GPO, GPIO_LOW, OUT_PP);
    gpio_init(TFT180_CS_PIN, GPO, GPIO_LOW, OUT_PP);
    gpio_init(TFT180_BL_PIN, GPO, GPIO_HIGH, OUT_PP);

    tft180_set_dir(tft180_display_dir);
    tft180_set_color(tft180_pencolor, tft180_bgcolor);

    TFT180_RST(0);
    Delay_Ms(10);

    TFT180_RST(1);
    Delay_Ms(120);

    tft180_write_index(0x11);
    Delay_Ms(120);

    tft180_write_index(0xB1);
    tft180_write_8bit_data(0x01);
    tft180_write_8bit_data(0x2C);
    tft180_write_8bit_data(0x2D);

    tft180_write_index(0xB2);
    tft180_write_8bit_data(0x01);
    tft180_write_8bit_data(0x2C);
    tft180_write_8bit_data(0x2D);

    tft180_write_index(0xB3);
    tft180_write_8bit_data(0x01);
    tft180_write_8bit_data(0x2C);
    tft180_write_8bit_data(0x2D);
    tft180_write_8bit_data(0x01);
    tft180_write_8bit_data(0x2C);
    tft180_write_8bit_data(0x2D);

    tft180_write_index(0xB4);
    tft180_write_8bit_data(0x07);

    tft180_write_index(0xC0);
    tft180_write_8bit_data(0xA2);
    tft180_write_8bit_data(0x02);
    tft180_write_8bit_data(0x84);
    tft180_write_index(0xC1);
    tft180_write_8bit_data(0xC5);

    tft180_write_index(0xC2);
    tft180_write_8bit_data(0x0A);
    tft180_write_8bit_data(0x00);

    tft180_write_index(0xC3);
    tft180_write_8bit_data(0x8A);
    tft180_write_8bit_data(0x2A);
    tft180_write_index(0xC4);
    tft180_write_8bit_data(0x8A);
    tft180_write_8bit_data(0xEE);

    tft180_write_index(0xC5);
    tft180_write_8bit_data(0x0E);

    tft180_write_index(0x36);
    switch(tft180_display_dir)                                                  // y x v
    {
        case 0: tft180_write_8bit_data(1<<7 | 1<<6 | 0<<5);  break;             // 竖屏模式
        case 1: tft180_write_8bit_data(0<<7 | 0<<6 | 0<<5);  break;             // 竖屏模式  旋转180
        case 2: tft180_write_8bit_data(1<<7 | 0<<6 | 1<<5);  break;             // 横屏模式
        case 3: tft180_write_8bit_data(0<<7 | 1<<6 | 1<<5);  break;             // 横屏模式  旋转180
    }

    tft180_write_index(0xe0);
    tft180_write_8bit_data(0x0f);
    tft180_write_8bit_data(0x1a);
    tft180_write_8bit_data(0x0f);
    tft180_write_8bit_data(0x18);
    tft180_write_8bit_data(0x2f);
    tft180_write_8bit_data(0x28);
    tft180_write_8bit_data(0x20);
    tft180_write_8bit_data(0x22);
    tft180_write_8bit_data(0x1f);
    tft180_write_8bit_data(0x1b);
    tft180_write_8bit_data(0x23);
    tft180_write_8bit_data(0x37);
    tft180_write_8bit_data(0x00);
    tft180_write_8bit_data(0x07);
    tft180_write_8bit_data(0x02);
    tft180_write_8bit_data(0x10);

    tft180_write_index(0xe1);
    tft180_write_8bit_data(0x0f);
    tft180_write_8bit_data(0x1b);
    tft180_write_8bit_data(0x0f);
    tft180_write_8bit_data(0x17);
    tft180_write_8bit_data(0x33);
    tft180_write_8bit_data(0x2c);
    tft180_write_8bit_data(0x29);
    tft180_write_8bit_data(0x2e);
    tft180_write_8bit_data(0x30);
    tft180_write_8bit_data(0x30);
    tft180_write_8bit_data(0x39);
    tft180_write_8bit_data(0x3f);
    tft180_write_8bit_data(0x00);
    tft180_write_8bit_data(0x07);
    tft180_write_8bit_data(0x03);
    tft180_write_8bit_data(0x10);

    tft180_write_index(0x2a);
    tft180_write_8bit_data(0x00);
    tft180_write_8bit_data(0x00 + 2);
    tft180_write_8bit_data(0x00);
    tft180_write_8bit_data(0x80 + 2);

    tft180_write_index(0x2b);
    tft180_write_8bit_data(0x00);
    tft180_write_8bit_data(0x00 + 3);
    tft180_write_8bit_data(0x00);
    tft180_write_8bit_data(0x80 + 3);

    tft180_write_index(0xF0);
    tft180_write_8bit_data(0x01);
    tft180_write_index(0xF6);
    tft180_write_8bit_data(0x00);

    tft180_write_index(0x3A);
    tft180_write_8bit_data(0x05);
    tft180_write_index(0x29);

    tft180_clear();
}
//-------------------------------------------------------------------------------------------------------------------
// @brief       浮点数字转字符串
// @param       *str            字符串指针
// @param       number          传入的数据
// @param       point_bit       小数点精度
// @return      NULL
// Sample usage:                float_to_str(data_buffer, 3.1415, 2);           // 结果输出 data_buffer = "3.14"
//-------------------------------------------------------------------------------------------------------------------
void float_to_str (char *str, float number, uint8_t point_bit)
{
    int data_int = 0;
    int data_float = 0.0;
    int data_temp[5];
    int data_temp_point[5];
    uint8_t bit = 0;

    data_int = (int)number;
    if(0 > number)
    {
        *str++ = '-';
    }
    else if(0.0 == number)
    {
        *str++ = '0';
        *str++ = '.';
        *str = '0';
        return;
    }

    number = number - data_int;
    while(point_bit--)
    {
        number = number*10;
    }
    data_float = (int)number;

    while(0 != data_int)
    {
        data_temp[bit++] = data_int%10;
        data_int /= 10;
    }
    while(0 != bit)
    {
        *str++ = (myabs(data_temp[bit-1])+0x30);
        bit--;
    }
    *str++ = '.';
    if(0 == data_float)
        *str = '0';
    else
    {
        while(0 != data_float)
        {
            data_temp_point[bit++] = data_float%10;
            data_float /= 10;
        }
        while(0 != bit)
        {
            *str++ = (myabs(data_temp_point[bit-1])+0x30);
            bit--;
        }
    }
}
//-------------------------------------------------------------------------------------------------------------------
// @brief       整形数字转字符串 数据范围是 [0,65535]
// @param       *str            字符串指针
// @param       number          传入的数据
// @return      NULL
// Sample usage:                uint_to_str(data_buffer, 300);
//-------------------------------------------------------------------------------------------------------------------
void uint_to_str (char *str, uint32_t number)
{
    int8_t data_temp[16];
    uint8_t bit = 0;

    if(0 == number)
    {
        *str = '0';
        return;
    }

    while(0 != number)
    {
        data_temp[bit++] = number%10;
        number /= 10;
    }
    while(0 != bit)
    {
        *str++ = (myabs(data_temp[bit-1])+0x30);
        bit--;
    }
}
//-------------------------------------------------------------------------------------------------------------------
// @brief       整形数字转字符串 数据范围是 [-32768,32767]
// @param       *str            字符串指针
// @param       number          传入的数据
// @return      NULL
// Sample usage:                int32o_str(data_buffer, -300);
//-------------------------------------------------------------------------------------------------------------------
void int_to_str (char *str, int32_t number)
{
    uint8_t data_temp[16];
    uint8_t bit = 0;

    if(0 > number)
    {
        *str++ = '-';
    }
    else if(0 == number)
    {
        *str = '0';
        return;
    }

    // int [-32768,32767]
    while(0 != number)
    {
        data_temp[bit++] = myabs(number%10);
        number /= 10;
    }
    while(0 != bit)
    {
        *str++ = (data_temp[bit-1]+0x30);
        bit--;
    }
}
