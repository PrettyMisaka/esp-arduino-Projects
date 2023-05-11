#ifndef __ESP32_USER_SEVER_H__
#define __ESP32_USER_SEVER_H__

#include <WebServer.h>

// extern String canvasCmdCode;
// extern  WebServer esp32_server;  //声明一个 WebServer 的对象，对象的名称为 esp32_server
//                             //设置网络服务器响应HTTP请求的端口号为 80

void userSeverInit(void);
void userSeverHandle(void);
void Web_CanvasPushCmd(String cmdString);
void Web_CanvasClear(void);
void Web_CanvasSetSize(int width, int height);

#endif
