#include <Arduino.h>
#include <string.h>
#include <iostream>
#include <SoftwareSerial.h>

#include <ESP8266WiFi.h>        // 本程序使用 ESP8266WiFi库
#include <ESP8266WiFiMulti.h>   //  ESP8266WiFiMulti库
#include <ESP8266WebServer.h>   //  ESP8266WebServer库
#include <ArduinoJson.h>        
#include "DFRobot_SHT20.h"
#include <OneWire.h>
#include <DS18B20.h>
#include <Ticker.h>

const char* wifi_home_ssid     = "501lab";                
const char* wifi_home_password = "501501501";             
const char* wifi_myphone_ssid     = "iPhone";                
const char* wifi_myphone_password = "987654321";
const char* wifi_328r_ssid     = "西电极品高质量宿舍";                
const char* wifi_328r_password = "haitang328r";            

ESP8266WiFiMulti wifiMulti;     // 建立ESP8266WiFiMulti对象,对象名称是'wifiMulti'
ESP8266WebServer esp8266_server(80);// 建立ESP8266WebServer对象，对象名称为esp8266_server
                                      // 括号中的数字是网路服务器响应http请求的端口号
                                      // 网络服务器标准http端口号为80，因此这里使用80为端口号
DFRobot_SHT20 sht20(&Wire, SHT20_I2C_ADDR);
OneWire oneWire(D5);
DS18B20 myDS18B20(&oneWire);
Ticker myTicker;
SoftwareSerial Serial2(D3, D6);//rx tx

char htmlCode[4096] = "";
bool ledMod = INPUT,pinModeD0,stm32ledMod = 1;
float tem = 0;
String serialRxData,serial2RxData;
char htmlShowString[20];

void WIFI_init();
void Web_init();
void htmlCodeReFlash();
void Web_handleRoot();
void handleLED();
void handleStm32LED();
void handleStm32LEDDelayMs();
void handleUpdata();
void Web_handleNotFound();
void Serial2_callback();
std::string String2stdstring(String String_data);

void setup() {
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(115200);
  Serial2.begin(56000);
  // Serial2.onReceive(Serial2_callback);
  WIFI_init();
  Web_init();
  sht20.initSHT20();
  myDS18B20.begin();
  pinMode(LED_BUILTIN,INPUT);
  Serial2.print("MS002000");
}

void loop() {
  esp8266_server.handleClient();     // 处理http服务器访问
  pinModeD0 = digitalRead(D0);
  myDS18B20.requestTemperatures();
  tem = myDS18B20.getTempC();
  if(Serial2.available()){
    Serial2_callback();
  }
}

void htmlCodeReFlash(){

  sprintf(htmlCode,/*style*/"%s"/*led*/"%s%d%s" /*stm32led*/"%s%d%s"/*stm32ledms*/"%s"/*D0mode*/"%s%d%s" 
                  /*ds18b20*/"%s%4.1f%s" /*sht20*/"%s%4.1f%s%4.1f%s" /*ShowString*/"%s" /*script*/"%s",
                  /*style*/
                  "<style>*{font-size:40px}</style>",
                  /*led*/
                  "<form id=\"led\" action=\"/LED\" method=\"POST\" style=\"display:inline-block\"><input id=\"ledmod\" type=\"submit\" style=\"cursor:pointer\" value=\"LEDMODE",
                  ledMod,
                  "\" style=\"height:10%;\"></form>",                  
                  /*stm32led*/
                  "<form id=\"stm32led\" action=\"/stm32LED\" method=\"POST\" style=\"display:inline-block\"><input id=\"stm32ledmod\" type=\"submit\" style=\"cursor:pointer\" value=\"stm32LEDMODE",
                  stm32ledMod,
                  "\" style=\"height:10%;\"></form>",
                  /*stm32ledms*/
                  "<form id=\"stm32ledms\" action=\"/stm32LEDms\" method=\"POST\" style=\"display:inline-block\">\
                  <input type=\"number\" placeholder=\"max 999999ms\" name=\"time_ms\">\
                  <input id=\"setstm32delayms\" type=\"submit\" style=\"cursor:pointer\" value=\"setStm32DelayMs\" title=\"stm32led interval of each switching state(ms)\" style=\"height:10%;\"></form>",
                  /*D0mode*/
                  "<p id=\"pinModeD0\">D0:",
                  pinModeD0,
                  "</P>",
                  /*ds18b20*/
                  "<p id=\"DS18B20Tem\">DS18B20Tem:",
                  tem,
                  "</p>",
                  /*sht20*/
                  "<p id=\"sht20data\">SHT20:Tem:",
                  sht20.readTemperature(),
                  "Hum:",
                  sht20.readHumidity(),
                  "</p>",
                  /*ShowString*/
                  "<p id=\"showString\" title=\"showStringForStm32\"></p>",
                  /*script*/
                  "<button id=\"updata\" type=\"button\">openAutoUpdata</button>\
                  <script>\
                  var arr = [];\
                  var xhr = new XMLHttpRequest();\
                  function updateCounter() {\
                  xhr.open('POST', '/updata');\
                  xhr.onreadystatechange = function() {\
                  if (xhr.readyState == 4 && xhr.status == 200) {\
                  var responseText = xhr.responseText;\
                  arr = responseText.split(\"|\");\
                  document.getElementById('ledmod').value = \"LEDMODE\" + arr[0];\
                  document.getElementById('stm32led').value = \"stm32LEDMODE\" + arr[2];\
                  document.getElementById('pinModeD0').innerHTML = \"D0:\" + arr[1];\
                  document.getElementById('DS18B20Tem').innerHTML = \"DS18B20Tem:\" + arr[3];\
                  document.getElementById('sht20data').innerHTML = \"SHT20:Tem:\"+arr[4]+\"Hum:\"+arr[5];\
                  document.getElementById('showString').innerHTML = arr[6];}\
                  };\
                  xhr.send();}\
                  var autoUpdata;"
                  //var autoUpdata = setInterval(updateCounter, 1000);
                  "document.getElementById('ledmod').onclick=()=>{\
                    document.getElementById('ledmod').disable = true;\
                  };\
                  document.getElementById('stm32led').onclick=()=>{\
                    document.getElementById('stm32led').disable = true;\
                  };\
                  document.getElementById('setstm32delayms').onclick=()=>{\
                    document.getElementById('setstm32delayms').disable = true;\
                  };\
                  var autoUpdataIsOpen = 0;\
                  document.getElementById('updata').onclick=()=>{\
                    if(autoUpdataIsOpen){\
                      document.getElementById('updata').innerHTML = \"closeAutoUpdata\";\
                      autoUpdataIsOpen = 0;\
                      clearInterval(autoUpdata);\
                    }\
                    else{\
                      document.getElementById('updata').innerHTML = \"openAutoUpdata\";\
                      autoUpdataIsOpen = 1;\
                      autoUpdata = setInterval(updateCounter, 1000);\
                    }\
                  };\
                    </script>");
}

void Serial2_callback() {
  serial2RxData = "";
  while (Serial2.available()) 
  {
    serial2RxData += char(Serial2.read()); 
    delay(2); //这里不能去掉，要给串口处理数据的时间
  }
  Serial.println(serial2RxData);
  if(serial2RxData.substring(0,4).equals("show")&&serial2RxData[4]=='"'&&serial2RxData[serial2RxData.length()-1]=='"'){
    sprintf(htmlShowString,serial2RxData.substring(5,serial2RxData.length()-1).c_str());
  }
  Serial.println(htmlShowString);
}

void serialEvent() //串口中断
{
  if(Serial.available()){
    serialRxData = "";
    while (Serial.available()) 
    {
      serialRxData += char(Serial.read()); 
      delay(2); //这里不能去掉，要给串口处理数据的时间
    }
    Serial.println(serialRxData);
  }
  //Serial.println(rx_data);
}

void WIFI_init(){
  //通过addAp函数存储  WiFi名称       WiFi密码
  wifiMulti.addAP(wifi_328r_ssid, wifi_328r_password);  // 这三条语句通过调用函数addAP来记录3个不同的WiFi网络信息.
  wifiMulti.addAP(wifi_home_ssid, wifi_home_password); 
  wifiMulti.addAP(wifi_myphone_ssid, wifi_myphone_password); 
  Serial.println("connect to wifi...");
  int i = 0;                                 
  while (wifiMulti.run() != WL_CONNECTED) {  // 此处的wifiMulti.run()是重点。通过wifiMulti.run()，NodeMCU将会在当前
    delay(1000);                             // 环境中搜索addAP函数所存储的WiFi。如果搜到多个存储的WiFi那么NodeMCU
    Serial.print(i++);                       // 将会连接信号最强的那一个WiFi信号。
    Serial.print(' ');    
  }
  Serial.println('\n');                     // WiFi连接成功后
  Serial.println("Connected to "+ WiFi.SSID());            
  Serial.println("IP address:"+ WiFi.localIP().toString());            
}

void Web_init()
{
  esp8266_server.begin();                   //  详细讲解请参见太极创客网站《零基础入门学用物联网》
  esp8266_server.on("/",HTTP_GET,Web_handleRoot);       //  第3章-第2节 ESP8266-NodeMCU网络服务器-1
  esp8266_server.on("/LED",HTTP_POST,handleLED);  //跳转到这个页面控制电灯
  esp8266_server.on("/stm32LED",HTTP_POST,handleStm32LED);
  esp8266_server.on("/stm32LEDms",HTTP_POST,handleStm32LEDDelayMs);
  esp8266_server.on("/updata",HTTP_POST,handleUpdata);
  esp8266_server.onNotFound(Web_handleNotFound);        
  Serial.println("WEB init finish"); 
}

bool isHandleRoot = 1;
void Web_handleRoot() {   //处理网站根目录“/”的访问请求 
if(isHandleRoot){
  isHandleRoot = 0;
  htmlCodeReFlash();
  esp8266_server.send(200,"text/html",htmlCode);
  isHandleRoot = 1;
}
}

bool isHandleLED = 1;
void handleLED(){
  if(isHandleLED){
    isHandleLED = 0;
    ledMod = !ledMod;
    pinMode(LED_BUILTIN,ledMod);
    esp8266_server.sendHeader("Location","/");              //跳转回页面的根目录
    esp8266_server.send(303);                               // 发送Http相应代码303 跳转 
    isHandleLED = 1;
  }
}

bool isHandleStm32LED = 1;
void handleStm32LED(){
  if(isHandleStm32LED){
    isHandleStm32LED = 0;
    stm32ledMod = !stm32ledMod;
    if (stm32ledMod) Serial2.print("closeled");
    else Serial2.print("open_led");
    esp8266_server.sendHeader("Location","/");              //跳转回页面的根目录
    esp8266_server.send(303);  
    isHandleStm32LED = 1;
  }   
}

bool isHandleStm32LEDDelayMs = 1;
void handleStm32LEDDelayMs() {
  if(isHandleStm32LEDDelayMs){
  isHandleStm32LEDDelayMs = 0;
  if (esp8266_server.hasArg("time_ms")) { // check if the request has a plain text body
    String body = esp8266_server.arg("time_ms"); // get the body as a string
    // ... do something with the body ...
    Serial.println(body);
    body = body.substring(0, 6);
    for(;;){
      if(body.length() == 6) break;
      body = "0" + body;
    }
    Serial.printf("%d|%s",body.length(),String2stdstring(body).c_str());
    char tmpstr[8] = "";
    sprintf(tmpstr,"MS%s",String2stdstring(body).c_str());
    Serial.println(tmpstr);
    Serial2.print(tmpstr);
    esp8266_server.sendHeader("Location","/");              //跳转回页面的根目录
    esp8266_server.send(303);     }
  // } else {
  //   esp8266_server.send(400, "text/plain", "Bad Request"); // send an error
  // }
    isHandleStm32LEDDelayMs = 1;
  }
}

bool isHandleUpdata = 1;
void handleUpdata(){
  if(isHandleUpdata){
    isHandleUpdata = 0;
  char data[100];
  sprintf(data,"%d|%d|%d|%4.1f|%4.1f|%4.1f|%s",
              ledMod,pinModeD0,stm32ledMod,
              tem,sht20.readTemperature(),sht20.readHumidity(),
              htmlShowString);
  esp8266_server.send(200, "text/plain", data);
    isHandleUpdata = 1;
  }
}

// 设置处理404情况的函数'handleNotFound'
void Web_handleNotFound(){                                        // 当浏览器请求的网络资源无法在服务器找到时，
  esp8266_server.send(404, "text/plain", "404: Not found");   // NodeMCU将调用此函数。
}


std::string String2stdstring(String String_data)
{
  std::string stdstring_data_tmp = ""; //std::string类型
  stdstring_data_tmp = String_data.c_str();
  return stdstring_data_tmp;
}