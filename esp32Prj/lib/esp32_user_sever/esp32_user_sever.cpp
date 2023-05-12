#include <esp32_user_sever.h>
#include <WebServer.h>
#include <Arduino.h>
#include <canvasAPI.h>

String htmlCode;
String canvasCmdCode = "ctx.fillStyle=\"#888888\";ctx.fillRect(0,0,480,320);";
String canvasSize;
String webReFlashTime = "1000";

WebServer esp32_server(80);  //声明一个 WebServer 的对象，对象的名称为 esp32_server
                            //设置网络服务器响应HTTP请求的端口号为 80
int webWidth = 480, webHeight = 320;

static void Web_handleRoot(void);
static void Web_handleNotFound(void);

void userSeverInit(void){
    Web_CanvasSetSize();
    esp32_server.begin();  //启动网络服务器
    esp32_server.on("/",HTTP_GET,Web_handleRoot);  //函数处理当有HTTP请求 "/HolleWorld" 时执行函数 handleRoot 
    esp32_server.onNotFound(Web_handleNotFound);  //当请求的网络资源不在服务器的时候，执行函数 handleFound 
}

void Web_CanvasSetSize(void){
  char str_tmp[64];
  sprintf(str_tmp,"<canvas id=\"tutorial\" width=\"%d\" height=\"%d\"></canvas>", webWidth, webHeight);
  canvasSize = str_tmp;
}
void Web_SetReFlashTime(int time_ms){
  char str_tmp[10];
  sprintf(str_tmp,"%d", time_ms);
  webReFlashTime = str_tmp;
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
    function reFlash(){\
        location.reload();\
    }\
    setInterval(reFlash, "+ webReFlashTime +");\
    function draw(){\
        var canvas = document.getElementById('tutorial');\
        if(!canvas.getContext) return;\
        var ctx = canvas.getContext(\"2d\");"
        + canvasAPI_draw.canvasCmdCode +
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
