#ifndef __ESP32_USER_SEVER_H__
#define __ESP32_USER_SEVER_H__

#include <WebServer.h>
#include <bmp_make_ex.h>
#include <user_base64.h>

extern int webWidth, webHeight;
extern String sever_canvasCmdCode;
// extern String canvasCmdCode;
// extern  WebServer esp32_server;  //声明一个 WebServer 的对象，对象的名称为 esp32_server
//                                  //设置网络服务器响应HTTP请求的端口号为 80

void userSeverInit(void);
void userSeverHandle(void);
void Web_CanvasPushCmd(String cmdString);
void Web_CanvasClear(void);
void Web_CanvasSetSize(void);
void Web_ShowCanvasCmdCode(void);
void Web_SetReFlashTime(int time_ms);

#endif
