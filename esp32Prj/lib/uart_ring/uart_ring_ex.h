#ifndef __UART_RING_EX_H__
#define __UART_RING_EX_H__

#include <uart_ring.h>
#include <esp32_user_sever.h>
#include <Arduino.h>
#include <canvasAPI.h>
#include <bmp_make_ex.h>

extern uartRingParameterTypedef uartRingDebugParam;
extern uartRingParameterTypedef uartRingSerial2Param;

void uartRingDebugTotalInit(void);
void uartRingSerial2TotalInit(void);

#endif
