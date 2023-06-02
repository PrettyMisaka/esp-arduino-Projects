#include "bmp_make_ex.h"
#include "bmp_font.h"
BMP_EX bmpBase(200,300);

void BMP_EX::drawLine(int x0, int y0, int x1, int y1, uint16_t color){
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, row, col;
    uint32_t i = 0;
    if(y0 == y1){
        for(i = 0; i < x1 - x0; i++){
            drawPixel( x0 + i,y0,color);
        }
        return;
    }
    delta_x = x1 - x0;
    delta_y = y1 - y0;
    row = x0;
    col = y0;
    if(delta_x > 0)incx = 1;
    else if(delta_x == 0)incx = 0;
    else{
        incx = -1;
        delta_x = -delta_x;
    }

    if(delta_y > 0)incy = 1;
    else if(delta_y == 0)incy = 0;
    else{
        incy = -1;
        delta_y = -delta_y;
    }

    if(delta_x > delta_y)distance = delta_x;
    else distance = delta_y;
    for(t = 0; t <= distance + 1; t++){
        drawPixel(row, col, color);
        xerr += delta_x ;
        yerr += delta_y ;

        if(xerr > distance){
            xerr -= distance;
            row += incx;
        }

        if(yerr > distance){
            yerr -= distance;
            col += incy;
        }
    }
};

void BMP_EX::drawRectangle(int x1, int y1, int x2, int y2, uint16_t color){
    drawLine(x1, y1, x2, y1, color);
    drawLine(x1, y1, x1, y2, color);
    drawLine(x1, y2, x2, y2, color);
    drawLine(x2, y1, x2, y2, color);
};

void BMP_EX::drawCircle(int x0, int y0, int r, uint16_t color){
    int a, b;
    int di;
    a = 0;
    b = r;
    di = 3 - (r << 1);

    while(a <= b){
        drawPixel(x0 - b, y0 - a, color);
        drawPixel(x0 + b, y0 - a, color);
        drawPixel(x0 - a, y0 + b, color);
        drawPixel(x0 - b, y0 - a, color);
        drawPixel(x0 - a, y0 - b, color);
        drawPixel(x0 + b, y0 + a, color);
        drawPixel(x0 + a, y0 - b, color);
        drawPixel(x0 + a, y0 + b, color);
        drawPixel(x0 - b, y0 + a, color);
        a++;

        if(di < 0)di += 4 * a + 6;
        else{
            di += 10 + 4 * (a - b);
            b--;
        }

        drawPixel(x0 + a, y0 + b, color);
    }
}

void BMP_EX::showChar(int x, int y, char chr, uint8_t size, uint16_t point_color, uint16_t back_color){
    int temp, t1, t;
    int csize;		
    int colortemp;
    int sta;

    chr = chr - ' '; 

    if((x > (img_width - size / 2)) || (y > (img_height - size)))	return;

    if((size == 16) || (size == 32) ){
        csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);

        for(t = 0; t < csize; t++)
        {
            if(size == 16)temp = asc2_1608[chr][t];	
            else if(size == 32)temp = asc2_3216[chr][t];	
            else return;			

            for(t1 = 0; t1 < 8; t1++)
            {
                if(temp & 0x80) colortemp = point_color;
                else colortemp = back_color;

                drawPixel( x + t1, y - t, colortemp);
                temp <<= 1;
            }
        }
    }
}

static uint32_t userPow(uint8_t m, uint8_t n){
    uint32_t result = 1;
    while(n--)result *= m;
    return result;
}

void BMP_EX::showString(int x, int y, int width, int height, uint16_t point_color, uint16_t back_color, uint8_t size, char *p)
{
    uint8_t x0 = x;
    width += x;
    height += y;

    while((*p <= '~') && (*p >= ' ')){
        if(x >= width){
            x = x0;
            y += size;
        }

        if(y >= height)break; //�˳�

        showChar(x, y, *p, size, point_color, back_color);
        x += size / 2;
        p++;
    }
}

//size 16/24/32
void BMP_EX::printf_bmpString(int x, int y, uint16_t point_color, uint16_t back_color, uint8_t size, const char* buf, ...){
  const char *p = buf;
  char str[255] = {0};
  va_list v;
  va_start(v, buf);
  vsprintf(str, buf, v); 
  showString( x,  y,  strlen(str)*size,  size, point_color, back_color, size, str);
  va_end(v);
}

void BMP_EX::showImage(int x, int y, int width, int height, const uint8_t *p){
    uint16_t color;
    for(int j = 0; j < height; j++){
        for (int i = 0; i < width; i++){
            color = (p[ i * j * 2]<< 8) | (p[ i * j * 2 + 1]);
            drawPixel( x + i , y + j, color);
        }
    }
}  