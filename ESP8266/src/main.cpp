#include <Arduino.h>
#include <ESP8266WiFi.h>                      
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>
#include <string.h>
#include <iostream>
#include <regex>

const char* wifi_ssid     = "西电极品高质量宿舍";                // 需要连接到的WiFi名
const char* wifi_password = "haitang328r";             // 连接的WiFi密码 
const char* URL_Time = "http://quan.suning.com/getSysTime.do";//获取当前时间指令
const char* URL_Weather = "http://api.seniverse.com/v3/weather/now.json?key=Szer-_lf_7LP5karX&location=xian&language=en&unit=c";
//const char* URL_Weather = "http://www.weather.com.cn/data/sk/101051301.html";
//const char* URL_Weather = "https://restapi.amap.com/v3/weather/weatherInfo?key=08a9828d22a07d8efc0374b216a633b3&city=110000&extensions=all";
//获取天气
const char* URL_Weather_Host = "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/104.0.5112.102 Safari/537.36 Edg/104.0.1293.70";

String rx_data;//串口中断数据存储
String myserialrx_data;//虚拟串口数据存储
size_t ModofEsp8266 = 0;

String CmdStorage[] ={
  "showcmd",
  "gettime",
  "getdate",
  "weather",
  "testcmd",
  "endsign"
};

HTTPClient http_time;
HTTPClient http_weather;
WiFiClient client;
//SoftwareSerial mySerial(0, 1); // RX, TX

void esp8266Http_GetTimeorDate();
size_t HandleReceiveData(String ReceiveData);
void WIFI_init();
void ledFlash(int ms);
void esp8266Http_GetWeather();
std::string String2stdstring(String String_data);
String stdstring2String(std::string stdstring_data);
String HandleStringbyRegex(std::string pattern,String PendingHandleData);
                                            
void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // Initialize the LED_BUILTIN pin as an output
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.begin(9600);                         // 初始化串口通讯波特率为115200
  //while (!Serial) {}
  //mySerial.begin(9600);
  WIFI_init();                                                                         
 
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);  
}
 
void loop() {

  if (ModofEsp8266==0)
  {
     ledFlash(500);
  }
     
  if (ModofEsp8266==1||ModofEsp8266==2)
  {
    esp8266Http_GetTimeorDate(); 
     ledFlash(50);
     ledFlash(50);
     ledFlash(50);
    ModofEsp8266 = 0;
  }

  if (ModofEsp8266==3)
  {
    esp8266Http_GetWeather(); 
     ledFlash(50);
     ledFlash(50);
     ledFlash(50);
    ModofEsp8266 = 0;
  }
     
        
}

void esp8266Http_GetTimeorDate(){
  http_time.begin(client, URL_Time);
  String timedata;
  while (1)
  {
    int httpResponseCode_time = http_time.GET();
    if (httpResponseCode_time == HTTP_CODE_OK)
    {
      timedata = http_time.getString();
      //Serial.printf("HTTP Response code: %d",httpResponseCode_time);
      //Serial.println(timedata);
      break;
    }
  }
  http_time.end();

  if (ModofEsp8266 == 1){
    //Serial.println("set_time");
    std::string pattern_time("([0-9]{2}):([0-9]{2}):([0-9]{2})");
    Serial.println(HandleStringbyRegex(pattern_time,timedata));
    //mySerial.println(HandleStringbyRegex(pattern_time,timedata));
  }
  if (ModofEsp8266 == 2)
  {
    //Serial.println("set_date");
    std::string pattern_time("([0-9]{2})-([0-9]{2})-([0-9]{2})");
    Serial.println(HandleStringbyRegex(pattern_time,timedata));
    //mySerial.println(HandleStringbyRegex(pattern_time,timedata));
  }
  

}

void esp8266Http_GetWeather(){
  http_weather.begin(client, URL_Weather);
  http_weather.setUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/104.0.5112.102 Safari/537.36 Edg/104.0.1293.70");
  //http_weather.begin(client, URL_Weather_Host, 80, URL_Weather, bool https)
  String weatherdata;
  while (1)
  {
    int httpResponseCode_weather = http_weather.GET();
    if (httpResponseCode_weather == HTTP_CODE_OK)
    {
      weatherdata = http_weather.getString();
      //Serial.printf("HTTP Response code: %d",httpResponseCode_weather);
      //Serial.println(weatherdata);
      break;
    }
  }
  http_weather.end();
  std::string pattern_weather("(code\":\")[0-9]{1,2}");
  //String weatherdata1 = "\"code\":\"23\"jijiji";
  String Weathercode = HandleStringbyRegex(pattern_weather,weatherdata);
  String tmp = Weathercode.substring(7,Weathercode.length());
  // 结束
  if (tmp.length() == 1){
    Serial.println("wwcode0"+tmp);
    //mySerial.println("wwcode0"+tmp);
  }
  else{
    Serial.println("wwcode"+tmp);
    //mySerial.println("wwcode"+tmp);
  }
}

void serialEvent() //串口中断
{
  rx_data="";
  while (Serial.available()) 
  {
    rx_data += char(Serial.read()); 
    delay(2); //这里不能去掉，要给串口处理数据的时间
  }
  //Serial.println(rx_data);
  ModofEsp8266 = HandleReceiveData(rx_data);
}

size_t HandleReceiveData(String ReceiveData)
{
  for (size_t i = 0;i<100 ; i++)
  {
    if (ReceiveData.equals(CmdStorage[i]))
    {
      //Serial.printf("%d",i);
      return i;
    }
    if (CmdStorage[i].equals("endsign"))
    {
      //Serial.printf("%d",-1);
      return -1;
    }
  }
  return -1;
}

void WIFI_init()
{
  
  WiFi.mode(WIFI_STA);                          // 设置Wifi工作模式为STA,默认为AP+STA模式
  WiFi.begin(wifi_ssid, wifi_password);                   // 通过wifi名和密码连接到Wifi
  //Serial.print("\r\nConnecting to ");           // 串口监视器输出网络连接信息
  //Serial.print(wifi_ssid); Serial.println(" ...");   // 显示NodeMCU正在尝试WiFi连接
  
  int i = 0;                                    // 检查WiFi是否连接成功
  while (1)         // WiFi.status()函数的返回值是由NodeMCU的WiFi连接状态所决定的。 
  {                                             // 如果WiFi连接成功则返回值为WL_CONNECTED
    if(WiFi.status() != WL_CONNECTED)
    {
        delay(1000);                                // 此处通过While循环让NodeMCU每隔一秒钟检查一次WiFi.status()函数返回值
        //Serial.print("waiting for ");                          
        //Serial.print(i++); Serial.println("s...");
    }
    if (WiFi.status() == WL_WRONG_PASSWORD)
    {
        //Serial.print("connect failed");
        //Serial.print("wrong password");
        break;
    }
    if (WiFi.status() == WL_CONNECTED)
    {
        ledFlash(50);
        ledFlash(50);
        ledFlash(50);
        //Serial.print("connect success");
        //Serial.println("");                           // WiFi连接成功后
        //Serial.println("WiFi connected!");            // NodeMCU将通过串口监视器输出"连接成功"信息。
        //Serial.print("IP address: ");                 // 同时还将输出NodeMCU的IP地址。这一功能是通过调用
        //Serial.println(WiFi.localIP());               // WiFi.localIP()函数来实现的。该函数的返回值即NodeMCU的IP地址。

        break;
    }
           
  }      
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);   
}

void ledFlash(int ms)
{
    digitalWrite(LED_BUILTIN, LOW);
    delay(ms);
    digitalWrite(LED_BUILTIN, HIGH);                 
    delay(ms);
}

std::string String2stdstring(String String_data)
{
  std::string stdstring_data_tmp = ""; //std::string类型
  stdstring_data_tmp = String_data.c_str();
  return stdstring_data_tmp;
}

String stdstring2String(std::string stdstring_data)
{
  char char_data_tmp[10];
  strcpy(char_data_tmp,stdstring_data.c_str());
  String String_data_tmp = char_data_tmp; 
  return String_data_tmp;
}

String HandleStringbyRegex(std::string pattern,String PendingHandleData)
{
	pattern = "[[:alpha:]]*" + pattern + "[[:alpha:]]*";
	std::regex r(pattern);
	std::smatch results;
	//String test_str("receipt freind theif receive");
  std::string PendingHandleData_stdstring = String2stdstring(PendingHandleData);
	if ( std::regex_search(PendingHandleData_stdstring, results, r ))
  {
    return stdstring2String(results.str());
  }
  return "error";//这里要加一个返回值 防止产生错误
  
}

