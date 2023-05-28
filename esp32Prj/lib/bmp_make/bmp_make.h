#ifndef __BMP_MAKE_H__
#define __BMP_MAKE_H__

#include <iostream>
#include <Arduino.h>

#define BMPIMG_MAXWIDTH 250
#define BMPIMG_MAXHEIGHT 250

#define BMP_ENCODING_FORMAT_USE_BASE64
//0 原始格式 1 base64

// BMP文件头结构体
typedef struct {
    uint16_t type; // 文件类型，必须为"BM"
    uint32_t size; // 文件大小，单位为字节
    uint16_t reserved1; // 保留字段，必须为0
    uint16_t reserved2; // 保留字段，必须为0
    uint32_t offset; // 像素数据起始位置，单位为字节
}  BMPFileHeaderTypedef;

// BMP位图信息头结构体
typedef struct {
    uint32_t size; // 信息头大小，必须为40
    int32_t width; // 图像宽度，单位为像素
    int32_t height; // 图像高度，单位为像素
    uint16_t planes; // 颜色平面数，必须为1
    uint16_t bit_count; // 每个像素的位数，必须为24
    uint32_t compression; // 压缩方式，必须为0
    uint32_t size_image; // 像素数据大小，单位为字节
    int32_t x_pels_per_meter; // X方向像素数/米 水平分辨率，用象素/米表示。一般为0 (38-41字节)
    int32_t y_pels_per_meter; // Y方向像素数/米 垂直分辨率，用象素/米表示。一般为0 (42-45字节)
    uint32_t clr_used; // 使用的颜色数，必须为0
    uint32_t clr_important; // 重要的颜色数，必须为0
} BMPInfoHeaderTypedef;

class BMP_BASE
{
private:
    BMPFileHeaderTypedef file_header;
    BMPInfoHeaderTypedef info_header;
    int file_header_length, info_header_length;
public:
    int data_length;
    int base64_length;
    int base64_offset;
    int img_width, img_height;
#ifndef BMP_ENCODING_FORMAT_USE_BASE64
    //bmp length 
    uint8_t bmp_data[14 + 40 + BMPIMG_MAXWIDTH*BMPIMG_MAXHEIGHT*2 + 1];
#else
    //base64 length
    uint8_t bmp_data[((14 + 40 + BMPIMG_MAXWIDTH*BMPIMG_MAXHEIGHT*2)+3-(14 + 40 + BMPIMG_MAXWIDTH*BMPIMG_MAXHEIGHT*2)%3)*4/3+1];
#endif
    BMP_BASE(int width, int height);
    void resetHeaderData(int width, int height);
    void pushHeader2data();
    
    void drawPixel(int x, int y, uint16_t color);
    void clear(uint16_t color);
    int clear(int x0, int y0, int x1, int y1, uint16_t color);
};


#endif