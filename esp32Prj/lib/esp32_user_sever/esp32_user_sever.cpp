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
static void Web_handleMoveControl(void);
static void Web_handleNotFound(void);

void userSeverInit(void){
    Web_CanvasSetSize();
    esp32_server.begin();  //启动网络服务器
    esp32_server.enableCORS();
    esp32_server.on("/",HTTP_GET,Web_handleRoot);  //函数处理当有HTTP请求 "/" 时执行函数 handleRoot
    esp32_server.on("/updata",HTTP_GET,Web_handleUpdataImg);  //函数处理当有HTTP请求 "/HolleWorld" 时执行函数 handleRoot  
    esp32_server.on("/serialTx",HTTP_POST,Web_handleSerialTx);  
    esp32_server.on("/moveControl",HTTP_GET,Web_handleMoveControl);
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
  bmpBase.printf_bmpString( 0, 80, BLACK, WHITE, 16, "count:%d",updata_img_index);
  updata_img_index++;
  esp32_server.send_P(200, "text/plain", (char*)bmpBase.bmp_data);
}
void Web_handleSerialTx(void){
    if (esp32_server.hasArg("serialTxInfo")) { // check if the request has a plain text body
      String body = esp32_server.arg("serialTxInfo"); // get the body as a string
      body += "\n";
      // ... do something with the body ...
      Serial.printf("%s",body);
      Serial2.printf("%s",body);
    }
    esp32_server.sendHeader("Location","/");              //跳转回页面的根目录
    esp32_server.send(303);
};
static int speed;
void Web_handleMoveControl(){
  if (esp32_server.hasArg("moveState")) {
    int movestate = -1;
    String body = esp32_server.arg("moveState"); // get the body as a string
    if(body.equals("movelu")){ 
      movestate = 0;
      Serial2.printf("5340%d",speed);}
    else if (body.equals("forward")){ 
      movestate = 1;
      Serial2.printf("5440%d",speed);}
    else if (body.equals("moveru")){ 
      movestate = 2;
      Serial2.printf("5540%d",speed);}
    else if (body.equals("left")){ 
      movestate = 3;
      Serial2.printf("4340%d",speed);}
    else if (body.equals("stop")){ 
      movestate = 3;
      Serial2.printf("4440%d",speed);}
    else if (body.equals("right")){ 
      movestate = 3;
      Serial2.printf("4540%d",speed);}
    else if (body.equals("moveld")){ 
      movestate = 3;
      Serial2.printf("3340%d",speed);}
    else if (body.equals("back")){ 
      movestate = 3;
      Serial2.printf("3440%d",speed);}
    else if (body.equals("moverd")){ 
      movestate = 3;
      Serial2.printf("3540%d",speed);}
    else if (body.equals("rotatel")){ 
      movestate = 3;
      Serial2.printf("4430%d",speed);}
    else if (body.equals("setspeed")){ 
      movestate = 3;
      Serial2.printf("4440%d",speed);
        if (esp32_server.hasArg("speed")){
          String speedString = esp32_server.arg("speed");
          // Serial2.printf("%d\n",speedString.toInt());
          speed = speedString.toInt()/50;
          if(speed >= 10) speed = 9;
          else if(speed < 0) speed = 0;
          // Serial2.printf("speed:%s\n",esp32_server.arg("speed"));
        }
      }
    else if (body.equals("rotater")){ 
      movestate = 3;
      Serial2.printf("4450%d",speed);}
    // Serial2.printf("movestate|%s|\n",body);
    // Serial2.printf("movestate%d\n",movestate);
  }
  esp32_server.sendHeader("Location","/");              //跳转回页面的根目录
  esp32_server.send(303);
}
String html_control = "<div><form action=\"/moveControl\" method=\"GET\" id=\"move-control\">\
        <input type=\"range\" max=\"500\" min=\"0\" name=\"speed\" style=\"width: 300px;\">\
        <div><button name=\"moveState\" value=\"movelu\">movelu </button>\
            <button name=\"moveState\" value=\"forward\">forward</button>\
            <button name=\"moveState\" value=\"moveru\">moveru </button></div>\
        <div><button name=\"moveState\" value=\"left\">left   </button>\
            <button name=\"moveState\" value=\"stop\">stop   </button>\
            <button name=\"moveState\" value=\"right\">right  </button></div>\
        <div><button name=\"moveState\" value=\"moveld\">moveld </button>\
            <button name=\"moveState\" value=\"back\">back   </button>\
            <button name=\"moveState\" value=\"moverd\">moverd </button></div>\
        <div><button name=\"moveState\" value=\"rotatel\">rotatel </button>\
            <button name=\"moveState\" value=\"setspeed\">setspeed   </button>\
            <button name=\"moveState\" value=\"rotater\">rotater </button></div>\
    </form></div>\
    <style>\
        form#move-control div{\
            height: 100px;}\
        form#move-control button{\
            margin: 0;\
            float: left;\
            height: 100px;\
            width: 100px;\
        }\
    </style>";
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
        <form id=\"serialTx\" action=\"/serialTx\" method=\"POST\" style=\"display:inline-block\">\
        <input type=\"text\" placeholder=\"输入想往串口发送的数据\" name=\"serialTxInfo\">\
        <input id=\"send-serial-info\" type=\"submit\" style=\"cursor:pointer\" value=\"send\" title=\"send\" style=\"height:10%;\"></form>"+
        html_control
        +
        "<div><img id=\"bmpImg1\"/></div>"
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
    "}"+
    // "draw();"+
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
