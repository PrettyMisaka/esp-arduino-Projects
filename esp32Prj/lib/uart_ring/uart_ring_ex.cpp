#include <uart_ring_ex.h>

uartRingParameterTypedef uartRingSerial1Param;

static void setCanvasSize(callBackFun_EventTypedef e){
    webWidth = e.intData[0];
    webHeight = e.intData[1];
    Web_CanvasSetSize();

    char str_tmp[256];
    sprintf(str_tmp,"ctx.fillStyle=\"%s\";ctx.fillRect(0,0,%d,%d);",e.stringData[0],webWidth,webHeight);
    String tmpString = str_tmp;
    Web_CanvasPushCmd(tmpString);
}
static void clearCanvas(callBackFun_EventTypedef e){
    Web_CanvasClear();
}
static void setFillColor(callBackFun_EventTypedef e){
    char str_tmp[256];
    sprintf(str_tmp,"ctx.fillStyle=\"%s\";",e.stringData[0]);
    String tmpString = str_tmp;
    Web_CanvasPushCmd(tmpString);
}
static void setStrokeColor(callBackFun_EventTypedef e){
    char str_tmp[256];
    sprintf(str_tmp,"ctx.strokeStyle=\"%s\";",e.stringData[0]);
    String tmpString = str_tmp;
    Web_CanvasPushCmd(tmpString);
}

static void fillRect(callBackFun_EventTypedef e){
    char str_tmp[256];
    sprintf(str_tmp,"ctx.fillRect(%d,%d,%d,%d);",e.intData[0],e.intData[1],e.intData[2],e.intData[3]);
    String tmpString = str_tmp;
    Web_CanvasPushCmd(tmpString);
}
static void strokeRect(callBackFun_EventTypedef e){
    char str_tmp[256];
    sprintf(str_tmp,"ctx.strokeRect(%d,%d,%d,%d);",e.intData[0],e.intData[1],e.intData[2],e.intData[3]);
    String tmpString = str_tmp;
    Web_CanvasPushCmd(tmpString);
}
static void clearRect(callBackFun_EventTypedef e){
    char str_tmp[256];
    sprintf(str_tmp,"ctx.clearRect(%d,%d,%d,%d);",e.intData[0],e.intData[1],e.intData[2],e.intData[3]);
    String tmpString = str_tmp;
    Web_CanvasPushCmd(tmpString);
}
static void drawLine(callBackFun_EventTypedef e){
    char str_tmp[256];
    sprintf(str_tmp,"ctx.moveTo(%d, %d);ctx.lineTo(%d, %d);ctx.stroke();",e.intData[0],e.intData[1],e.intData[2],e.intData[3]);
    String tmpString = str_tmp;
    Web_CanvasPushCmd(tmpString);
}
static void setFont(callBackFun_EventTypedef e){
    char str_tmp[256];
    sprintf(str_tmp,"ctx.font=\"%s %s\";",e.stringData[0],e.stringData[1]);
    String tmpString = str_tmp;
    Web_CanvasPushCmd(tmpString);
}
static void drawStr(callBackFun_EventTypedef e){
    char str_tmp[256];
    sprintf(str_tmp,"ctx.fillText(\"%s\",%d,%d);",e.stringData[0],e.intData[0],e.intData[1]);
    String tmpString = str_tmp;
    Web_CanvasPushCmd(tmpString);
}
static void showCanvasCmd(callBackFun_EventTypedef e){
    Web_ShowCanvasCmdCode();
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


