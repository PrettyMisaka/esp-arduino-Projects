#include <uart_ring_ex.h>
#include "bmp_radar.h"

uartRingParameterTypedef uartRingDebugParam;
uartRingParameterTypedef uartRingSerial2Param;
/*******************canvas cmd*******************/
static void setCanvasSize(callBackFun_EventTypedef e){
    webWidth = e.intData[0];
    webHeight = e.intData[1];
    Web_CanvasSetSize();

    canvasAPI_draw.setFillColor(e.stringData[0]);
    canvasAPI_draw.fillRect(0,0,webWidth,webHeight);
}
static void clearCanvas(callBackFun_EventTypedef e){
    canvasAPI_draw.clearCanvas();
}
static void setFillColor(callBackFun_EventTypedef e){
    canvasAPI_draw.setFillColor(e.stringData[0]);
}
static void setStrokeColor(callBackFun_EventTypedef e){
    canvasAPI_draw.setStrokeColor(e.stringData[0]);
}

static void fillRect(callBackFun_EventTypedef e){
    canvasAPI_draw.fillRect(e.intData[0],e.intData[1],e.intData[2],e.intData[3]);
}
static void strokeRect(callBackFun_EventTypedef e){
    canvasAPI_draw.strokeRect(e.intData[0],e.intData[1],e.intData[2],e.intData[3]);
}
static void clearRect(callBackFun_EventTypedef e){
    canvasAPI_draw.clearRect(e.intData[0],e.intData[1],e.intData[2],e.intData[3]);
}
static void drawLine(callBackFun_EventTypedef e){
    canvasAPI_draw.drawLine(e.intData[0],e.intData[1],e.intData[2],e.intData[3]);
}
static void setFont(callBackFun_EventTypedef e){
    canvasAPI_draw.setFont(e.stringData[0],e.stringData[1]);
}
static void drawStr(callBackFun_EventTypedef e){
    canvasAPI_draw.drawStr(e.intData[0],e.intData[1],e.stringData[0]);
}
static void showCanvasCmd(callBackFun_EventTypedef e){
    canvasAPI_draw.showCanvasCmd();
}
static void setWebReFlashTime(callBackFun_EventTypedef e){
    Web_SetReFlashTime(e.intData[0]);
}
/*******************Coordinate cmd*******************/
static void setZero(callBackFun_EventTypedef e){
    CoordinateAPI_Coordinate1.setZeroPoint(e.intData[0],e.intData[1]);
}
static void setLineColor(callBackFun_EventTypedef e){
    CoordinateAPI_Coordinate1.setLineColor(e.stringData[0]);
}
static void pushValue(callBackFun_EventTypedef e){
    int val = CoordinateAPI_Coordinate1.pushArrayValue(e.intData[0],e.intData[1]);
    if(val == 0) Serial.println("push err");
    else Serial.println("push success");
}
static void delValue(callBackFun_EventTypedef e){
    int val = CoordinateAPI_Coordinate1.delArrayValue(e.intData[0],e.intData[1]);
    if(val == 0) Serial.println("del err");
    else Serial.println("del success");
}
static void clearValue(callBackFun_EventTypedef e){
    CoordinateAPI_Coordinate1.clearArrayValue();
}
static void printArray(callBackFun_EventTypedef e){
    CoordinateAPI_Coordinate1.printArray();
}

void uartRingDebugTotalInit(void){
    ringBuff_Total_Init(&uartRingDebugParam);
    cmdBuff_Push(&uartRingDebugParam, (unsigned char*)"setWebReFlashTime INT\n", setWebReFlashTime);
/*******************canvas cmd*******************/
    cmdBuff_Push(&uartRingDebugParam, (unsigned char*)"setCanvasSize INT INT STRING\n", setCanvasSize);
    cmdBuff_Push(&uartRingDebugParam, (unsigned char*)"clearCanvas\n", clearCanvas);
    cmdBuff_Push(&uartRingDebugParam, (unsigned char*)"setFillColor STRING\n", setFillColor);
    cmdBuff_Push(&uartRingDebugParam, (unsigned char*)"setStrokeColor STRING\n", setStrokeColor);

    cmdBuff_Push(&uartRingDebugParam, (unsigned char*)"fillRect INT INT INT INT\n", fillRect);
    cmdBuff_Push(&uartRingDebugParam, (unsigned char*)"strokeRect INT INT INT INT\n", strokeRect);
    cmdBuff_Push(&uartRingDebugParam, (unsigned char*)"clearRect INT INT INT INT\n", clearRect);

    cmdBuff_Push(&uartRingDebugParam, (unsigned char*)"drawLine INT INT INT INT\n", drawLine);
    
    cmdBuff_Push(&uartRingDebugParam, (unsigned char*)"setFont STRING STRING\n", setFont);
    cmdBuff_Push(&uartRingDebugParam, (unsigned char*)"drawStr INT INT STRING\n", drawStr);
    cmdBuff_Push(&uartRingDebugParam, (unsigned char*)"showCanvasCmd\n", showCanvasCmd);
/*******************Coordinate cmd*******************/
    cmdBuff_Push(&uartRingDebugParam, (unsigned char*)"setZero INT INT\n", setZero);
    cmdBuff_Push(&uartRingDebugParam, (unsigned char*)"setLineColor STRING\n", setLineColor);
    cmdBuff_Push(&uartRingDebugParam, (unsigned char*)"pushValue INT INT\n", pushValue);
    cmdBuff_Push(&uartRingDebugParam, (unsigned char*)"delValue INT INT\n", delValue);
    cmdBuff_Push(&uartRingDebugParam, (unsigned char*)"clearValue\n", clearValue);
    cmdBuff_Push(&uartRingDebugParam, (unsigned char*)"printArray\n", printArray);
}
/********************Serial2 callback*********************/
static uint16_t point_color = BLACK;
static uint16_t back_color = WHITE;
static void setBmpSize(callBackFun_EventTypedef e){
    bmpBase.resetHeaderData(e.intData[0],e.intData[1]);
}
static void setPointCol(callBackFun_EventTypedef e){
    point_color = ((e.intData[0]&0xF8)<<8)|((e.intData[1]&0xFC)<<3)|((e.intData[2]&0xF8)>>3);
}
static void setBackCol(callBackFun_EventTypedef e){
    back_color = ((e.intData[0]&0xF8)<<8)|((e.intData[1]&0xFC)<<3)|((e.intData[2]&0xF8)>>3);
}
static void clearBmp(callBackFun_EventTypedef e){
    bmpBase.clear(point_color);
}
static void clearBmpSize(callBackFun_EventTypedef e){
    bmpBase.clear(e.intData[0],e.intData[1],e.intData[2],e.intData[3],point_color);
}
static void bmpDrawLine(callBackFun_EventTypedef e){
    bmpBase.drawLine(e.intData[0],e.intData[1],e.intData[2],e.intData[3],point_color);
}
static void bmpDrawRectangle(callBackFun_EventTypedef e){
    bmpBase.drawRectangle(e.intData[0],e.intData[1],e.intData[2],e.intData[3],point_color);
}
static void bmpDrawCircle(callBackFun_EventTypedef e){
    bmpBase.drawCircle(e.intData[0],e.intData[1],e.intData[2],point_color);
}
static void bmpShowString(callBackFun_EventTypedef e){
    bmpBase.printf_bmpString(e.intData[0],e.intData[1],point_color,back_color,16,e.stringData[0]);
}
static void pushRaderData(callBackFun_EventTypedef e){
    Radar_Push(e.intData[0],e.intData[1]);
    Serial.printf("%d %d",e.intData[0],e.intData[1]);
}
void uartRingSerial2TotalInit(void){
    ringBuff_Total_Init(&uartRingSerial2Param);
    cmdBuff_Push(&uartRingSerial2Param, (unsigned char*)"pushRD INT INT\n", pushRaderData);

    cmdBuff_Push(&uartRingSerial2Param, (unsigned char*)"setBmpSize INT INT\n", setBmpSize);
    cmdBuff_Push(&uartRingSerial2Param, (unsigned char*)"setPointCol INT INT INT\n", setPointCol);
    cmdBuff_Push(&uartRingSerial2Param, (unsigned char*)"setBackCol INT INT INT\n", setBackCol);
    cmdBuff_Push(&uartRingSerial2Param, (unsigned char*)"clearBmp\n", clearBmp);
    cmdBuff_Push(&uartRingSerial2Param, (unsigned char*)"clearBmpSize INT INT INT INT\n", clearBmpSize);
    cmdBuff_Push(&uartRingSerial2Param, (unsigned char*)"bmpDrawLine INT INT INT INT\n", bmpDrawLine);
    cmdBuff_Push(&uartRingSerial2Param, (unsigned char*)"bmpDrawRectangle INT INT INT INT\n", bmpDrawRectangle);
    cmdBuff_Push(&uartRingSerial2Param, (unsigned char*)"bmpDrawCircle INT INT INT\n", bmpDrawCircle);
    cmdBuff_Push(&uartRingSerial2Param, (unsigned char*)"bmpShowString INT INT STRING\n", bmpShowString);
    // cmdBuff_Push(&uartRingSerial2Param, (unsigned char*)"\n", );
}