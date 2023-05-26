#include <bmp_make.h>
#include <Arduino.h>

BMP_BASE::BMP_BASE(int width, int height){
    file_header_length = 14;
    info_header_length = 40;
    if(width > BMPIMG_MAXWIDTH) width = BMPIMG_MAXWIDTH;
    if(height > BMPIMG_MAXHEIGHT) height = BMPIMG_MAXHEIGHT;
    data_length = file_header_length + info_header_length + width * height*2;
    img_width = width;
    img_height = height;
    // BMP文件头
    file_header.type = 0x4D42; // BM
    file_header.size = data_length;
    file_header.reserved1 = 0;
    file_header.reserved2 = 0;
    file_header.offset = file_header_length + info_header_length;
    // BMP位图信息头
    info_header.size = info_header_length;
    info_header.width = width;
    info_header.height = height;
    info_header.planes = 1;
    info_header.bit_count = 16;//RGB565
    info_header.compression = 0;
    info_header.size_image = width * height * 2;
    info_header.x_pels_per_meter = 0;
    info_header.y_pels_per_meter = 0;
    info_header.clr_used = 0;
    info_header.clr_important = 0;
}

void BMP_BASE::drawPixel(int x, int y, uint16_t color){
    // Serial.printf("file_size%d,info_size%d",sizeof(file_header),sizeof(info_header));
    bmp_data[file_header.offset + y * info_header.width + x] = color >> 8;
    bmp_data[file_header.offset + y * info_header.width + x + 1] = color&0x00ff;
}
void BMP_BASE::clear(uint16_t color){
    clear( 0, 0, img_width, img_height, color);
};
int BMP_BASE::clear(int x0, int y0, int x1, int y1, uint16_t color){
    if(x1 < x0 || y1 < y0) return 0;
    if(x1 > img_width) x1 = img_width;
    if(y1 > img_height) x1 = img_height;
    for (size_t x = x0; x < x1; x++){
        for (size_t y = y0; y < y1; y++){
            bmp_data[file_header.offset + y * info_header.width + x] = color >> 8;
            bmp_data[file_header.offset + y * info_header.width + x + 1] = color&0x00ff;
        }
    }
    return 1;
};

void BMP_BASE::resetHeaderData(int width, int height){
    if(width > BMPIMG_MAXWIDTH) width = BMPIMG_MAXWIDTH;
    if(height > BMPIMG_MAXHEIGHT) height = BMPIMG_MAXHEIGHT;
    img_width = width;
    img_height = height;
    data_length = file_header_length + info_header_length + width * height*2;
    // BMP文件头
    file_header.size = data_length;
    // BMP位图信息头
    info_header.width = width;
    info_header.height = height;
    info_header.size_image = width * height * 2;
};
void BMP_BASE::pushHeader2data(){
    int offset = 0;
    // 文件类型，必须为"BM"
    bmp_data[offset + 0] = file_header.type & 0x00ff;
    bmp_data[offset + 1] = file_header.type >> 8;
    offset = 2;
    // 文件大小，单位为字节
    bmp_data[offset + 0] = file_header.size;
    bmp_data[offset + 1] = file_header.size >> 8;
    bmp_data[offset + 2] = file_header.size >> 16;
    bmp_data[offset + 3] = file_header.size >> 24;
    offset = 6;
    // 保留字段，必须为0
    bmp_data[offset + 0] = 0;
    bmp_data[offset + 1] = 0;
    bmp_data[offset + 2] = 0;
    bmp_data[offset + 3] = 0;
    offset = 10;
    // 像素数据起始位置，单位为字节
    bmp_data[offset + 0] = file_header.offset;
    bmp_data[offset + 1] = file_header.offset >> 8;
    bmp_data[offset + 2] = file_header.offset >> 16;
    bmp_data[offset + 3] = file_header.offset >> 24;
    offset = 14;
    // 信息头大小，必须为40
    bmp_data[offset + 0] = info_header.size;
    bmp_data[offset + 1] = info_header.size >> 8;
    bmp_data[offset + 2] = info_header.size >> 16;
    bmp_data[offset + 3] = info_header.size >> 24;
    offset += 4;
    // 图像宽度，单位为像素
    bmp_data[offset + 0] = info_header.width;
    bmp_data[offset + 1] = info_header.width >> 8;
    bmp_data[offset + 2] = info_header.width >> 16;
    bmp_data[offset + 3] = info_header.width >> 24;
    offset += 4;
    // 图像高度，单位为像素
    bmp_data[offset + 0] = info_header.height;
    bmp_data[offset + 1] = info_header.height >> 8;
    bmp_data[offset + 2] = info_header.height >> 16;
    bmp_data[offset + 3] = info_header.height >> 24;
    offset += 4;
    // 颜色平面数，必须为1
    bmp_data[offset + 0] = info_header.planes ;
    bmp_data[offset + 1] = info_header.planes >> 8;
    offset += 2;
    // 每个像素的位数，必须为24
    bmp_data[offset + 0] = info_header.bit_count ;
    bmp_data[offset + 1] = info_header.bit_count >> 8;
    offset += 2;
    // 压缩方式，必须为0
    bmp_data[offset + 0] = info_header.compression;
    bmp_data[offset + 1] = info_header.compression >> 8;
    bmp_data[offset + 2] = info_header.compression >> 16;
    bmp_data[offset + 3] = info_header.compression >> 24;
    offset += 4;
    // 像素数据大小，单位为字节
    bmp_data[offset + 0] = info_header.size_image;
    bmp_data[offset + 1] = info_header.size_image >> 8;
    bmp_data[offset + 2] = info_header.size_image >> 16;
    bmp_data[offset + 3] = info_header.size_image >> 24;
    offset += 4;
    // X方向像素数/米 水平分辨率，用象素/米表示。一般为0 (38-41字节)
    bmp_data[offset + 0] = info_header.x_pels_per_meter;
    bmp_data[offset + 1] = info_header.x_pels_per_meter >> 8;
    bmp_data[offset + 2] = info_header.x_pels_per_meter >> 16;
    bmp_data[offset + 3] = info_header.x_pels_per_meter >> 24;
    offset += 4;
    // Y方向像素数/米 垂直分辨率，用象素/米表示。一般为0 (42-45字节)
    bmp_data[offset + 0] = info_header.y_pels_per_meter;
    bmp_data[offset + 1] = info_header.y_pels_per_meter >> 8;
    bmp_data[offset + 2] = info_header.y_pels_per_meter >> 16;
    bmp_data[offset + 3] = info_header.y_pels_per_meter >> 24;
    offset += 4;
    // 使用的颜色数，必须为0
    bmp_data[offset + 0] = info_header.clr_used;
    bmp_data[offset + 1] = info_header.clr_used >> 8;
    bmp_data[offset + 2] = info_header.clr_used >> 16;
    bmp_data[offset + 3] = info_header.clr_used >> 24;
    offset += 4;
    // 重要的颜色数，必须为0
    bmp_data[offset + 0] = info_header.clr_important;
    bmp_data[offset + 1] = info_header.clr_important >> 8;
    bmp_data[offset + 2] = info_header.clr_important >> 16;
    bmp_data[offset + 3] = info_header.clr_important >> 24;
    offset += 4;
};
