#include <uart_ring_ex.h>

uartRingParameterTypedef uartRingSerial1Param;

static void setCanvasSize(callBackFun_EventTypedef e){
    char str_tmp[64];
    sprintf(str_tmp,"ctx.fillStyle=\"#FF0000\";ctx.fillRect(0,0,%d,%d);",e.intData[0],e.intData[1]);
    String tmpString = str_tmp;
    Web_CanvasSetSize(e.intData[0],e.intData[1]);
    Web_CanvasPushCmd(tmpString);
    digitalWrite(10,!digitalRead(10));
};
void uartRingSerial1TotalInit(void){
    ringBuff_Total_Init(&uartRingSerial1Param);
    cmdBuff_Push(&uartRingSerial1Param, (unsigned char*)"setCanvasSize INT INT\n", setCanvasSize);
}


