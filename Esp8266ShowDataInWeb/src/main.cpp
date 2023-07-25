#include <Arduino.h>
#include <string.h>
#include <iostream>
#include <SoftwareSerial.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>   //  ESP8266WiFiMulti库
#include <ESP8266WebServer.h>   //  ESP8266WebServer库
#include <uart_ring_ex.h>

static const char *ssid = "esp8266-wifi";    // wifi名称
static const char * pass = "12345678";      // wifi密码
static int station_count = 0;               // 已经连接的结点数量

ESP8266WebServer esp8266_server(80);// 建立ESP8266WebServer对象，对象名称为esp8266_server
                                      // 括号中的数字是网路服务器响应http请求的端口号
                                      // 网络服务器标准http端口号为80，因此这里使用80为端口号

char htmlCode[4096] = "";

void Web_init();
void htmlCodeReFlash();
void Web_handleRoot();
// void handleLED();
// void handleStm32LED();
// void handleStm32LEDDelayMs();
// void handleUpdata();
void Web_handleNotFound();
// void Serial2_callback();
std::string String2stdstring(String String_data);

void setup() {
  pinMode(LED_BUILTIN,OUTPUT);
  // 串口工作在9600模式
  Serial.begin(115200);
  Serial.println("create ap....");

  // 进入soft-ap工作模式，如果失败则提示
  bool ok = WiFi.softAP(ssid, pass);
  if (!ok) {
    Serial.println("Create ap failed.");
    return;
  } 
  Web_init();

  UART_RING_Init();
  // 如果成功则在串口中设置其ip地址
  Serial.println("AP created.\n");
  Serial.print("ipadd:");
  Serial.println(WiFi.softAPIP());
  pinMode(LED_BUILTIN,INPUT);
}

ring_state ring_state_num;

void loop() {
  esp8266_server.handleClient();     // 处理http服务器访问
  
  // 获取当前的连接设备数量，如果有没变化，不做处理
  uint8_t cnt = WiFi.softAPgetStationNum();
  if (cnt == station_count) {
    // return;
  }
  else{
    // 数量变多，意味着新设备接收，提示；当设备断开连接时也提示
    if (cnt > station_count) {
      Serial.println("New station connected");
    } else if (cnt < station_count) {
      Serial.println("Station disconnected");
    }
    // 最后打印当前已经连接的设备数量
    Serial.printf("Station count: %d\n", cnt);
    // 数量发生变化时，记录最新的值
    station_count = cnt;
  }
  ring_state_num = ringBuffHandleFun(&serial1Buffer);
  // Serial.printf("%d\n",ring_state_num);
}

void serialEvent() 
{
  while (Serial.available()) 
  {
    ringBuff_Push( &serial1Buffer,char(Serial.read()));
    delay(2); //这里不能去掉，要给串口处理数据的时间
  }
}

void Web_init()
{
  esp8266_server.begin();                   //  详细讲解请参见太极创客网站《零基础入门学用物联网》
  esp8266_server.on("/",HTTP_GET,Web_handleRoot);       //  第3章-第2节 ESP8266-NodeMCU网络服务器-1
  // esp8266_server.on("/LED",HTTP_POST,handleLED);  //跳转到这个页面控制电灯
  // esp8266_server.on("/stm32LED",HTTP_POST,handleStm32LED);
  // esp8266_server.on("/stm32LEDms",HTTP_POST,handleStm32LEDDelayMs);
  // esp8266_server.on("/updata",HTTP_POST,handleUpdata);
  esp8266_server.onNotFound(Web_handleNotFound);        
  Serial.println("WEB init finish"); 
}

void Web_handleRoot() {   //处理网站根目录“/”的访问请求 
  htmlCodeReFlash();
  esp8266_server.send(200,"text/html",htmlCode);
}
// 设置处理404情况的函数'handleNotFound'
void Web_handleNotFound(){                                        // 当浏览器请求的网络资源无法在服务器找到时，
  esp8266_server.send(404, "text/plain", "404: Not found");   // NodeMCU将调用此函数。
}
void htmlCodeReFlash(){
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
std::string String2stdstring(String String_data)
{
  std::string stdstring_data_tmp = ""; //std::string类型
  stdstring_data_tmp = String_data.c_str();
  return stdstring_data_tmp;
}
