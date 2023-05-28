#include <bmp_make.h>
#include <Arduino.h>
static char ch64[] = {
'A','B','C','D','E','F','G','H','I','J','K','L','M','N',
'O','P','Q','R','S','T','U','V','W','X','Y','Z',
'a','b','c','d','e','f','g','h','i','j','k','l','m','n',
'o','p','q','r','s','t','u','v','w','x','y','z',
'0','1','2','3','4','5','6','7','8','9','+','/','='
};
static uint8_t base64decode(char p){
        if ( p >= 'A' && p <= 'Z' )
                return (p - 'A');
        else if ( p >= 'a' && p <= 'z' )
                return (p - 'a' + 26);
        else if ( p >= '0' && p <= '9' )
                return (p - '0' + 26 + 26);
        else if ( p == '+' )
                return 62;
        else if ( p == '/' )
                return 63;
        else if ( p == '=' )
                return 64;
        return -1;
}
BMP_BASE::BMP_BASE(int width, int height){
    file_header_length = 14;
    info_header_length = 40;
    if(width*height > BMPIMG_MAXWIDTH*BMPIMG_MAXHEIGHT){
        width = BMPIMG_MAXWIDTH;
        height = BMPIMG_MAXHEIGHT;
    }
    this->data_length = file_header_length + info_header_length + width * height*2;
    this->img_width = width;
    this->img_height = height;
    this->base64_length = (data_length + 3 - data_length % 3) * 4 / 3;
    this->base64_offset = 54*4/3;
    this->bmp_data[base64_length] = 0;
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
    if(!((0 <= x && x < img_width)&&( 0 <= y && y < img_height))) return;
#ifndef BMP_ENCODING_FORMAT_USE_BASE64
    bmp_data[file_header.offset + (y * info_header.width + x)*2] = color;
    bmp_data[file_header.offset + (y * info_header.width + x)*2 + 1] = color >> 8;
#else
    int pix_index = y * info_header.width + x;
    int pix_offset = pix_index%3;
    int base64_index = this->base64_offset + (pix_index/3)*8/*base64*/;
    char ch[4];
    uint8_t decodeVal;
    switch (pix_offset)
    {
    case 0:
        decodeVal = base64decode((char)bmp_data[base64_index+2]);
        ch[0] = (char)((color&0x00FC) >> 2 );
        ch[1] = (char)((color&0x0003) << 4 | ((color>>8)&0xF0) >> 4 );
        ch[2] = (char)((((color>>8)&0x000F) << 2 )|(decodeVal&0x03)&0xcf);
        //查询编码数组获取编码后的字符
        bmp_data[base64_index]   = ch64[ch[0]];
        bmp_data[base64_index+1] = ch64[ch[1]];
        bmp_data[base64_index+2] = ch64[ch[2]];
        // if(base64_index == base64_offset) Serial.printf("|%d %d|",ch[2],ch64[ch[2]]);
        break;
    case 1:
        decodeVal = base64decode((char)bmp_data[base64_index+2]);
        ch[0] = (char)(( (decodeVal&0x3c )| ((color&0x00c0) >> 6)));
        // if(base64_index == base64_offset) Serial.printf("|%d %d %d %d %d|",color,decodeVal,decodeVal&0x3c,(color&0x00c0 >> 6),ch[0]);
        ch[1] = (char)((color&0x003f));
        ch[2] = (char)(((color>>8)&0x00fc) >> 2 );
        decodeVal = base64decode((char)bmp_data[base64_index+5]);
        ch[3] = (char)((((color>>8)&0x0003) << 4 )|(decodeVal&0x0f));
        //查询编码数组获取编码后的字符
        bmp_data[base64_index+2]   = ch64[ch[0]];
        bmp_data[base64_index+2+1] = ch64[ch[1]];
        bmp_data[base64_index+2+2] = ch64[ch[2]];
        bmp_data[base64_index+2+3] = ch64[ch[3]];
        break;
    case 2:
        decodeVal = base64decode((char)bmp_data[base64_index+5]);
        ch[0] = (char)(((color&0x00F0) >> 4 )| (decodeVal&0x30));
        ch[1] = (char)((color&0x000f) << 2 | ((color>>8)&0x00c0) >> 6 );
        ch[2] = (char)(((color>>8)&0x003F));
        //查询编码数组获取编码后的字符
        bmp_data[base64_index+5] = ch64[ch[0]];
        bmp_data[base64_index+6] = ch64[ch[1]];
        bmp_data[base64_index+7] = ch64[ch[2]];
        break;
    default:
        break;
    }
    if(base64_index == 4096 && 0)
            Serial.printf("%d|%d|%d|%d\n",ch[0],ch[1],ch[2],ch[3]);
#endif
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
            drawPixel( x, y, color);
        }
    }
    return 1;
};

void BMP_BASE::resetHeaderData(int width, int height){
    if(width*height > BMPIMG_MAXWIDTH*BMPIMG_MAXHEIGHT){
        width = BMPIMG_MAXWIDTH;
        height = BMPIMG_MAXHEIGHT;
    }
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
    // 转换为base64 从后往前 offset == 54
    char ch[4];
    for (int i = file_header_length + info_header_length - 3 ,j = base64_offset - 4; i >= 0;  i -= 3,j -= 4 ){
        ch[0] = (char)((bmp_data[i]&0xFC) >> 2 );
        ch[1] = (char)((bmp_data[i]&0x03) << 4 | (bmp_data[i+1]&0xF0) >> 4 );
        ch[2] = (char)((bmp_data[i+1]&0x0F) << 2 | (bmp_data[i+2]&0xC0) >> 6 );
        ch[3] = (char)((bmp_data[i+2]&0x3F));
        //查询编码数组获取编码后的字符
        bmp_data[j]   = ch64[ch[0]];
        bmp_data[j+1] = ch64[ch[1]];
        bmp_data[j+2] = ch64[ch[2]];
        bmp_data[j+3] = ch64[ch[3]];
    }
};
