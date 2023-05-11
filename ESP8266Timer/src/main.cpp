#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <ESP8266WiFi.h>                      
#include <ESP8266HTTPClient.h>
#include <string.h>
#include <iostream>
#include <regex>
#include <ThreeWire.h>  
#include <RtcDS1302.h>
#include "DFRobot_SHT20.h"

//const char* wifi_ssid     = "西电极品高质量宿舍";                // 需要连接到的WiFi名
//const char* wifi_password = "haitang328r";             // 连接的WiFi密码 
const char* wifi_ssid     = "iPhone";                // 需要连接到的WiFi名
const char* wifi_password = "987654321";             // 连接的WiFi密码 
const char* URL_Time = "http://quan.suning.com/getSysTime.do";//获取当前时间指令
const char* URL_Weather = "http://api.seniverse.com/v3/weather/now.json?key=Szer-_lf_7LP5karX&location=xian&language=en&unit=c";

String rx_data;//串口中断数据存储
String URL_weather_front = "http://api.seniverse.com/v3/weather/now.json?key=Szer-_lf_7LP5karX&location=";
String URL_weather_behind = "&language=en&unit=c";
size_t SerialHandleDataMod = 0,WIFIisCONNECT = 0;
char time_data[9],date_data[11],alarmclock_data[9] = "00:00:00",CityName_save[10] = "xian";
char alarmclock_cmd_data[20];
char TemandHemSTRsave[24] = "error";
char DS1302_time2write[9], DS1302_date2write[12];
char int2string_bettershow_data[3][3];
int weather_code = -1;
int oledShowMod = 1;
bool alarmclockisOPEN = 0;

DFRobot_SHT20 sht20(&Wire, SHT20_I2C_ADDR);
ThreeWire myWire(D6,D5,D7); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);
RtcDateTime DS1302data2write;
RtcDateTime DS1302_data;
HTTPClient http;
WiFiClient client;
U8G2_SSD1306_128X64_NONAME_2_SW_I2C u8g2(U8G2_R0, /* scl clock=*/ D3, /*  sda data=*/ D4, /* reset=*/ U8X8_PIN_NONE);
//Buffer Size 1,2orF(full frame buffer) F更耗内存

void esp8266Http_GetTimeorDate(size_t mod);
void esp8266Http_GetWeather(char * CityName);
size_t HandleReceiveData(String ReceiveData);
void WIFI_init();
std::string String2stdstring(String String_data);
String stdstring2String(std::string stdstring_data);
String HandleStringbyRegex(std::string pattern,String PendingHandleData);
void ledshow();
void date2ds1302format(char* datedata,char* formatdata);
void int2string_battershow(uint8_t num ,char* tmp_num);
  
String CmdStorage[] ={
  "showcmd",//                  0
  "setalarm-<hh:mm:ss>",//        1
  "closetheal",//               2
  "opentheal",//                3
  "endsign"//                   4
};

char weathercode_name[][25]{
  "Sunny","Clear","Fair","Fair","Cloudy","Partly Cloudy","Partly Cloudy","Mostly Cloudy","Mostly Cloudy","Overcast",
  "Shower","Thundershower","Thundershower with Hail","Light Rain","Moderate Rain","Heavy Rain","Storm","Heavy Storm","Severe Storm","Ice Rain",
  "Sleet","Snow Flurry","Light Snow","Moderate Snow","Heavy Snow","Snowstorm","Dust","Sand","Duststorm","Sandstorm",
  "Foggy","Haze","Windy","Blustery","Hurricane","Tropical Storm","Tornado","Cold","Hot"
};

void setup() {
  Wire.begin();
  u8g2.begin();//初始化
  u8g2.enableUTF8Print(); // enable UTF8 support for the Arduino print() function
  Serial.begin(9600); 

  sht20.initSHT20();
  Rtc.Begin();
  
  WIFI_init(); 
  if(WIFIisCONNECT){
    esp8266Http_GetTimeorDate(1);
    esp8266Http_GetWeather(CityName_save);
    date2ds1302format(date_data,DS1302_date2write);

    
    esp8266Http_GetTimeorDate(0);
    DS1302data2write = RtcDateTime(DS1302_date2write, time_data);
    Rtc.SetDateTime(DS1302data2write);
  }
  else{
    DS1302_data = Rtc.GetDateTime();
    int2string_battershow((int)DS1302_data.Month(),int2string_bettershow_data[0]);
    int2string_battershow((int)DS1302_data.Day(),int2string_bettershow_data[1]);
    sprintf(date_data,"%d-%s-%s",(int)DS1302_data.Year(),int2string_bettershow_data[0],int2string_bettershow_data[1]);

  }
}

void loop() {
  sprintf(TemandHemSTRsave,"Tem:%4.1f       Hum:%4.1f",sht20.readTemperature(),sht20.readHumidity());
  DS1302_data = Rtc.GetDateTime();
  int2string_battershow((int)DS1302_data.Hour(),int2string_bettershow_data[0]);
  int2string_battershow((int)DS1302_data.Minute(),int2string_bettershow_data[1]);
  int2string_battershow((int)DS1302_data.Second(),int2string_bettershow_data[2]);
  sprintf(time_data,"%s:%s:%s",int2string_bettershow_data[0],int2string_bettershow_data[1],int2string_bettershow_data[2]);
  ledshow();

  if(alarmclockisOPEN){
    if(strcmp(alarmclock_data,time_data) == 0){
      for(size_t i = 0;i<10;i++){
        u8g2.firstPage();
        do{
          if (i%2 == 0){
          u8g2.setFont(u8g2_font_timR24_tn);//设定字体
          u8g2.drawStr(5, 45, alarmclock_data);//在指定位置显示字符串
          }
        }while(u8g2.nextPage());
        delay(200);
      }
    }
  }

  oledShowMod++;
  if(oledShowMod == 45){
    oledShowMod = 0;
  }
}

//0时间  1日期
void esp8266Http_GetTimeorDate(size_t mod){
  http.begin(client, URL_Time);
  String timedata;
  while (1)
  {
    int httpResponseCode_time = http.GET();
    if (httpResponseCode_time == HTTP_CODE_OK)
    {
      timedata = http.getString();
      //Serial.printf("HTTP Response code: %d",httpResponseCode_time);
      //Serial.println(timedata);
      break;
    }
  }
  http.end();

  if (mod == 0){
    std::string pattern_time("([0-9]{2}):([0-9]{2}):([0-9]{2})");
    strcpy(time_data,HandleStringbyRegex(pattern_time,timedata).c_str());
  }
  if (mod == 1)
  {
    std::string pattern_time("([0-9]{4})-([0-9]{2})-([0-9]{2})");
    strcpy(date_data,HandleStringbyRegex(pattern_time,timedata).c_str());
  }
  

}

void esp8266Http_GetWeather(char * CityName){
  String tmp_URL = URL_weather_front + CityName + URL_weather_behind;
  http.begin(client, (char*)tmp_URL.c_str());
  //http.begin(client, URL_Weather);
  http.setUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/104.0.5112.102 Safari/537.36 Edg/104.0.1293.70");
  //http_weather.begin(client, URL_Weather_Host, 80, URL_Weather, bool https)
  String weatherdata;
  while (1)
  {
    int httpResponseCode_weather = http.GET();
    if (httpResponseCode_weather == HTTP_CODE_OK){
      weatherdata = http.getString();
      break;
    }
  }
  http.end();
  std::string pattern_weather("(code\":\")[0-9]{1,2}");
  String Weathercode = HandleStringbyRegex(pattern_weather,weatherdata);
  String tmp = Weathercode.substring(7,Weathercode.length());
  // 结束
  char tmp_code[11];
  sprintf(tmp_code,"%s",(char*)tmp.c_str());
  if (tmp.length() == 1){
    weather_code = tmp_code[0] - '0';
  }
  else{
    weather_code = (tmp_code[0] - '0')*10 + tmp_code[1] - '0';
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
  SerialHandleDataMod = HandleReceiveData(rx_data);
}

void ledshow(){
  //if(strcmp(time_data_save,time_data) != 0){
    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_timR24_tn);//设定字体
      u8g2.drawStr(5, 45, time_data);//在指定位置显示字符串

      u8g2.setFont(u8g2_font_ncenB08_tr);//设定字体
      if (WIFIisCONNECT){
        u8g2.drawStr(80, 10, "WIFI on");//在指定位置显示字符串
      }
      else{
        u8g2.drawStr(80, 10, "WIFI off");//在指定位置显示字符串
      }

      u8g2.setFont(u8g2_font_ncenB08_tr);//设定字体
      u8g2.drawStr(5, 10, date_data);
      //oledShowMod 1-10 weather 11-20 T&H 21-30 alarm clock
      if (oledShowMod/15 == 0){
        u8g2.drawStr(0, 64, "Weather:");
        if (weather_code != -1){
          u8g2.drawStr(60, 64, weathercode_name[weather_code]);
        }
        else{
          u8g2.drawStr(60, 64, "error");
        }
      }
      else if(oledShowMod/15 == 1){
        u8g2.drawStr(0, 64, TemandHemSTRsave);
      }
      else if(oledShowMod/15 == 2){
        if(alarmclockisOPEN){
          char  tmp[13];
          sprintf(tmp,"AL>>%s",alarmclock_data);
          u8g2.drawStr(0, 64, tmp);
        }
        else{
          u8g2.drawStr(0, 64, "Alarm Clock is close");
        }
      }
      
    } while ( u8g2.nextPage() );
  //  strcpy(time_data_save,time_data);
  //}

}


size_t HandleReceiveData(String ReceiveData)
{
  size_t tmp_handlemod = -1;
  for (size_t i = 0;i<10 ; i++){
    if (i == 1){
      strcpy(alarmclock_cmd_data ,(char*)ReceiveData.c_str()) ;
        //setalarm-<hh:mm:ss>
      bool issetalarm = 1;
      for (size_t j = 0; j < 10; j++){
        if (alarmclock_cmd_data[j] != ((char*)CmdStorage[1].c_str())[j]){
          issetalarm = 0;
          break;
        }
      }
      if (issetalarm){
        if(0 == (alarmclock_cmd_data[18] == '>'&&(alarmclock_cmd_data[12] == ':' && alarmclock_cmd_data[15] == ':' ))){
          issetalarm = 0;
        }
      }
      if (issetalarm){
        for(size_t j = 0;j<8;j++){
          alarmclock_data[j] = alarmclock_cmd_data[10+j];
        }
        Serial.println("finish");
        strcpy(alarmclock_cmd_data,"12345678");
        tmp_handlemod = i;
        break;
      }
    }
    
    if (ReceiveData.equals(CmdStorage[i])){
      tmp_handlemod = i;
      break;
    }
    if (CmdStorage[i].equals("endsign")){
      tmp_handlemod = -1;
      break;
    }
  }
  switch (tmp_handlemod)
  {
  case 0:
    Serial.printf("cmd is\n");
    for (size_t i = 0; i < 10; i++){
      Serial.println(CmdStorage[i]);
      if (CmdStorage[i].equals("endsign")){
      break;
    }}
    break;
  case 1:
    break;
  case 2:
  case 3:
    alarmclockisOPEN = tmp_handlemod - 2;
    Serial.println("finish");
    break;

  default:
    Serial.println("send <showcmd> to get cmd list");
    break;
  }
  return tmp_handlemod;
}

void WIFI_init()
{
  
  WiFi.mode(WIFI_STA);                          // 设置Wifi工作模式为STA,默认为AP+STA模式
  WiFi.begin(wifi_ssid, wifi_password);                   // 通过wifi名和密码连接到Wifi
  int i = 0;

  while (1)         // WiFi.status()函数的返回值是由NodeMCU的WiFi连接状态所决定的。 
  {                                             // 如果WiFi连接成功则返回值为WL_CONNECTED
    if(WiFi.status() != WL_CONNECTED)
    {
        delay(1000);                                // 此处通过While循环让NodeMCU每隔一秒钟检查一次WiFi.status()函数返回值
        i++;
        u8g2.firstPage();
        do{
          u8g2.setFont(u8g2_font_unifont_t_symbols);//设定字体
          u8g2.drawStr(0, 20, "connecting");//在指定位置显示字符串
          char tmp[30];
          sprintf(tmp,"waiting %d s...",i);
          u8g2.drawStr(0, 40, tmp);//在指定位置显示字符串
        } while (u8g2.nextPage());
    }
    if (WiFi.status() == WL_WRONG_PASSWORD || WiFi.status() == WL_CONNECT_FAILED || WiFi.status() == WL_NO_SSID_AVAIL){
      u8g2.firstPage();
       do{
      u8g2.setFont(u8g2_font_unifont_t_symbols);//设定字体
      u8g2.drawStr(0, 20, "wifi connect");//在指定位置显示字符串
      u8g2.drawStr(0, 40, "failed");//在指定位置显示字符串
      u8g2.drawStr(0, 60,"Initializing...");
      } while (u8g2.nextPage());
      delay(1000);
      WIFIisCONNECT = 0;
      break;
    }
    if (WiFi.status() == WL_CONNECTED)
    {
      u8g2.firstPage();
      do{
      u8g2.setFont(u8g2_font_unifont_t_symbols);//设定字体
      u8g2.drawStr(0, 20, "wifi connect");//在指定位置显示字符串
      u8g2.drawStr(0, 40, "success");//在指定位置显示字符串
      u8g2.drawStr(0, 60,"Initializing...");
      } while (u8g2.nextPage());
      WIFIisCONNECT = 1;
      break;
    }
           
  }      
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);   
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

void date2ds1302format(char* datedata,char* formatdata){
  char monthdataEn[12][4] = {
    "Jan","Feb","Mar","Apr","May","Jun",
    "Jul","Aug","Sep","Oct","Nov","Dec"
  };
  int tmp_n = (datedata[5] - '0')*10 + datedata[6] - '0';
  sprintf(formatdata,"%s %c%c %c%c%c%c",monthdataEn[tmp_n - 1],datedata[8],datedata[9],datedata[0],datedata[1],datedata[2],datedata[3]);
  //月 日 年
}

void int2string_battershow(uint8_t num ,char* tmp_num){
  if ((int)num < 10){
    tmp_num[0] = '0';
    tmp_num[1] = '0'+num;
  }
  else{
    tmp_num[0] = '0' + num/10;
    tmp_num[1] = '0'+ num%10;
  }
  
}