#include <esp32_user_sever.h>
#include <WebServer.h>
#include <Arduino.h>
#include <canvasAPI.h>

String htmlCode;
String sever_canvasCmdCode = "ctx.fillStyle=\"#888888\";ctx.fillRect(0,0,480,320);";
String canvasSize;
String webReFlashTime = "100000";

WebServer esp32_server(80);  //声明一个 WebServer 的对象，对象的名称为 esp32_server
                            //设置网络服务器响应HTTP请求的端口号为 80
int webWidth = 480, webHeight = 320;

static void Web_handleRoot(void);
static void Web_handleUpdataImg(void);
static void Web_handleSerialTx(void);
static void Web_handleNotFound(void);

void userSeverInit(void){
    Web_CanvasSetSize();
    esp32_server.begin();  //启动网络服务器
    esp32_server.enableCORS();
    esp32_server.on("/",HTTP_GET,Web_handleRoot);  //函数处理当有HTTP请求 "/HolleWorld" 时执行函数 handleRoot
    esp32_server.on("/updata",HTTP_GET,Web_handleUpdataImg);  //函数处理当有HTTP请求 "/HolleWorld" 时执行函数 handleRoot  
    esp32_server.on("/serialTx",HTTP_GET,Web_handleSerialTx);
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
static int updata_img_index = 0;
void Web_handleUpdataImg(void){
  // const char* tmp = base64Obj.encode((char*)bmpBase.bmp_data,bmpBase.data_length);
  /*
  if((bmpBase.data_length-updata_img_index)/3 > 0){
    if((bmpBase.data_length-updata_img_index)==3){
      esp32_server.send_P(200, "text/plain", base64Obj.encode((char*)&bmpBase.bmp_data[updata_img_index]),3);
      updata_img_index = 0;
    }
    else
    esp32_server.send_P(206, "text/plain", base64Obj.encode((char*)&bmpBase.bmp_data[updata_img_index]),3);
    updata_img_index += 3;
  }
  else{
    esp32_server.send_P(200, "text/plain", base64Obj.encode((char*)&bmpBase.bmp_data[updata_img_index]),(bmpBase.data_length-updata_img_index)%3);
    updata_img_index = 0;
  }
  */
  esp32_server.send_P(200, "text/plain", (char*)bmpBase.bmp_data);
}
void Web_handleSerialTx(void){
    if (esp32_server.hasArg("serialTxInfo")) { // check if the request has a plain text body
      String body = esp32_server.arg("serialTxInfo"); // get the body as a string
      body += "\n";
      // ... do something with the body ...
      Serial.printf("%s",body);
    }
    esp32_server.sendHeader("Location","/");              //跳转回页面的根目录
    esp32_server.send(303);
};
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
        <p>hello world</p>\
        <form id=\"serialTx\" action=\"/serialTx\" method=\"GET\" style=\"display:inline-block\">\
        <input type=\"text\" placeholder=\"输入想往串口发送的数据\" name=\"serialTxInfo\">\
        <input id=\"send-serial-info\" type=\"submit\" style=\"cursor:pointer\" value=\"send\" title=\"send\" style=\"height:10%;\"></form>\
        <img id=\"bmpImg1\"/>"
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
        + sever_canvasCmdCode +
    "}\
    draw();"+
    //XMLHttpRequest code
    "var xhr = new XMLHttpRequest();\
    var img = document.getElementById('bmpImg1'); \
    function updateCounter(){\
        xhr.open('GET', '/updata');\
        xhr.onreadystatechange = function() {\
          if (xhr.readyState == 4 && xhr.status == 200) {\
            img.src = \"data:image/bmp;base64,\"+xhr.responseText;\
          }\
        };\
        xhr.send();\
    };\
    setInterval(updateCounter, 1000);"
    +
    "</script>\
    \
    </html>";
  esp32_server.send(200,"text/html",htmlCode);
}
void Web_handleNotFound(void){                                        // 当浏览器请求的网络资源无法在服务器找到时，
  esp32_server.send(404, "text/plain", "404: Not found");   // NodeMCU将调用此函数。
}
