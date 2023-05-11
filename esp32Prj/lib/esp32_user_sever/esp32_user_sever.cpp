#include <esp32_user_sever.h>
#include <WebServer.h>
#include <Arduino.h>

String htmlCode;
String canvasCmdCode = "";
String canvasSize;
WebServer esp32_server(80);  //声明一个 WebServer 的对象，对象的名称为 esp32_server
                            //设置网络服务器响应HTTP请求的端口号为 80

static void Web_handleRoot(void);
static void Web_handleNotFound(void);

void userSeverInit(void){
    Web_CanvasSetSize(480,320);
    esp32_server.begin();  //启动网络服务器
    esp32_server.on("/",HTTP_GET,Web_handleRoot);  //函数处理当有HTTP请求 "/HolleWorld" 时执行函数 handleRoot 
    esp32_server.onNotFound(Web_handleNotFound);  //当请求的网络资源不在服务器的时候，执行函数 handleFound 
}
void Web_CanvasPushCmd(String cmdString){
  canvasCmdCode += cmdString;
}
void Web_CanvasClear(void){
  canvasCmdCode == "";
}
void Web_CanvasSetSize(int width, int height){
  char str_tmp[64];
  sprintf(str_tmp,"<canvas id=\"tutorial\" width=\"%d\" height=\"%d\"></canvas>",width,height);
  canvasSize = str_tmp;
}

void userSeverHandle(void){
    esp32_server.handleClient();
}
void Web_handleRoot(void) {   //处理网站根目录“/”的访问请求 
  htmlCode = "<!DOCTYPE html>\
    <html lang=\"zh-CN\">\
    <head>\
        <meta charset=\"UTF-8\">\
        <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\
        <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
        <title>Document</title>\
    </head>\
    <body>\
        <p>hello world</p>"
        + canvasSize +
    "</body>\
    <script>\
    function draw(){\
        var canvas = document.getElementById('tutorial');\
        if(!canvas.getContext) return;\
        var ctx = canvas.getContext(\"2d\");"
        + canvasCmdCode +
    "}\
    draw();\
    </script>\
    \
    </html>";
  esp32_server.send(200,"text/html",htmlCode);
}
void Web_handleNotFound(void){                                        // 当浏览器请求的网络资源无法在服务器找到时，
  esp32_server.send(404, "text/plain", "404: Not found");   // NodeMCU将调用此函数。
}
