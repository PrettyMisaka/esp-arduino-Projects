#ifndef __CANVASAPI_H__
#define __CANVASAPI_H__

#include <iostream>
#include <string.h>
#include <Arduino.h>

#define CANVAS_COORDINATE_ARR_LENGTH 100

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

class CoordinateAPI : public CanvasAPI
{
private:
    int length;
    int array[2][CANVAS_COORDINATE_ARR_LENGTH];
    char color[10];
    int zero_x;
    int zero_y;
public:
    CoordinateAPI(int x, int y);
    void setZeroPoint(int x, int y);
    void setLineColor(const char* color_str);
    int pushArrayValue(int x, int y);
    int delArrayValue(int x, int y);
    void clearArrayValue();
    void printArray();
    void showCoordinate();
};

extern CanvasAPI canvasAPI_draw;
extern CoordinateAPI CoordinateAPI_Coordinate1;

#endif
