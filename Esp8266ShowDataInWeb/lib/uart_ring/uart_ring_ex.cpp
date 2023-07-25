#include "uart_ring_ex.h"

#include <Arduino.h>
#include <string.h>
#include <stdio.h>

uartRingParameterTypedef serial1Buffer;

static char cmd_tmp[20];
static void callbackFn_hello(callBackFun_EventTypedef e){
  Serial.println("nice to meet you");
}

void UART_RING_Init(void){
  ringBuff_Total_Init(&serial1Buffer);
  sprintf(cmd_tmp,"hello");
  cmdBuff_Push(&serial1Buffer,(unsigned char*)cmd_tmp,callbackFn_hello);
}
