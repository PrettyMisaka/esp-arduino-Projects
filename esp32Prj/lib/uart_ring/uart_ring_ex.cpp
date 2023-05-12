#include <uart_ring_ex.h>

uartRingParameterTypedef uartRingSerial1Param;

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

void uartRingSerial1TotalInit(void){
    ringBuff_Total_Init(&uartRingSerial1Param);
    cmdBuff_Push(&uartRingSerial1Param, (unsigned char*)"setCanvasSize INT INT STRING\n", setCanvasSize);
    cmdBuff_Push(&uartRingSerial1Param, (unsigned char*)"clearCanvas\n", clearCanvas);
    cmdBuff_Push(&uartRingSerial1Param, (unsigned char*)"setFillColor STRING\n", setFillColor);
    cmdBuff_Push(&uartRingSerial1Param, (unsigned char*)"setStrokeColor STRING\n", setStrokeColor);

    cmdBuff_Push(&uartRingSerial1Param, (unsigned char*)"fillRect INT INT INT INT\n", fillRect);
    cmdBuff_Push(&uartRingSerial1Param, (unsigned char*)"strokeRect INT INT INT INT\n", strokeRect);
    cmdBuff_Push(&uartRingSerial1Param, (unsigned char*)"clearRect INT INT INT INT\n", clearRect);

    cmdBuff_Push(&uartRingSerial1Param, (unsigned char*)"drawLine INT INT INT INT\n", drawLine);
    
    cmdBuff_Push(&uartRingSerial1Param, (unsigned char*)"setFont STRING STRING\n", setFont);
    cmdBuff_Push(&uartRingSerial1Param, (unsigned char*)"drawStr INT INT STRING\n", drawStr);
    cmdBuff_Push(&uartRingSerial1Param, (unsigned char*)"showCanvasCmd\n", showCanvasCmd);
    
    cmdBuff_Push(&uartRingSerial1Param, (unsigned char*)"setWebReFlashTime INT\n", setWebReFlashTime);
}


