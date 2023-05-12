#ifndef __CANVASAPI_H__
#define __CANVASAPI_H__

#include <iostream>
#include <string.h>
#include <Arduino.h>

class CanvasAPI
{
private:
    void canvasCmdPush(String cmdString);
public:
    String canvasCmdCode;
    CanvasAPI();
    void clearCanvas();
    void showCanvasCmd();
    void setFillColor(const char* color);
    void setStrokeColor(const char* color);

    void fillRect(int x, int y, int width, int height);
    void strokeRect(int x, int y, int width, int height);
    void clearRect(int x, int y, int width, int height);
    
    void drawLine(int x_begin, int y_begin, int x_end, int y_end);

    void setFont(const char* size_px,const char* font);
    void drawStr(int x, int y,const char* str);
};


extern CanvasAPI canvasAPI_draw;

#endif
