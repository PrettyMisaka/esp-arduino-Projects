#include <esp32_user_sever.h>
#include <WebServer.h>

char htmlCode[4096] = "";
WebServer esp32_server(80);  //声明一个 WebServer 的对象，对象的名称为 esp32_server
                            //设置网络服务器响应HTTP请求的端口号为 80
void userSeverInit(void){
    esp32_server.begin();  //启动网络服务器
    esp32_server.on("/",HTTP_GET,Web_handleRoot);  //函数处理当有HTTP请求 "/HolleWorld" 时执行函数 handleRoot 
    esp32_server.onNotFound(Web_handleNotFound);  //当请求的网络资源不在服务器的时候，执行函数 handleFound 
}
void userSeverHandle(void){
    esp32_server.handleClient();
}
void Web_handleRoot(void) {   //处理网站根目录“/”的访问请求 
  htmlCodeReFlash();
  esp32_server.send(200,"text/html",htmlCode);
}
void Web_handleNotFound(void){                                        // 当浏览器请求的网络资源无法在服务器找到时，
  esp32_server.send(404, "text/plain", "404: Not found");   // NodeMCU将调用此函数。
}
void htmlCodeReFlash(void){
  sprintf(htmlCode,"<!DOCTYPE html>\
<html lang=\"zh-CN\">\
<head>\
    <meta charset=\"UTF-8\">\
    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
    <title>Document</title>\
</head>\
<body>\
    <p>hello world</p>\
    <canvas id=\"tutorial\" width=\"300\" height=\"300\"></canvas>\
</body>\
<script>\
function draw(){\
    var canvas = document.getElementById('tutorial');\
    if(!canvas.getContext) return;\
    var ctx = canvas.getContext(\"2d\");\
    ctx.fillStyle = \"rgb(200,0,0)\";\
    ctx.fillRect (10, 10, 55, 50);\
    ctx.fillStyle = \"rgba(0, 0, 200, 0.5)\";\
    ctx.fillRect (30, 30, 55, 50);\
}\
draw();\
</script>\
\
</html>");
}