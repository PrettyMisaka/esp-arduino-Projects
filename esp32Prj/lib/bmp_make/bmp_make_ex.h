#ifndef __BMP_MAKE_EX_H__
#define __BMP_MAKE_EX_H__

#include "bmp_make.h"

#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 
#define BRRED 			 0XFC07 
#define GRAY  			 0X8430 

class BMP_EX : public BMP_BASE{
    public:
        BMP_EX(int width, int height):BMP_BASE( width, height){}
        void drawLine(int x0, int y0, int x1, int y1, uint16_t color);
        void drawRectangle(int x1, int y1, int x2, int y2, uint16_t color);
        void drawCircle(int x0, int y0, int r, uint16_t color);
        void showChar(int x, int y, char chr, uint8_t size, uint16_t point_color, uint16_t back_color);
        void showString(int x, int y, int width, int height, uint16_t point_color, uint16_t back_color, uint8_t size, char *p);
        void printf_bmpString(int x, int y, uint16_t point_color, uint16_t back_color, uint8_t size, const char* buf, ...);
};

extern BMP_EX bmpBase;

#endif
