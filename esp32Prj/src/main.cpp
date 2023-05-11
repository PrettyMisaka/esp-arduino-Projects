#include <Arduino.h>
#include <WiFi.h>

#include <esp32_user_sever.h>
#include <uart_ring_ex.h>

const char *ssid = "ESP32-S3 WIFI";
const char *password = "123456789";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(10, OUTPUT);
  WiFi.softAP(ssid,password);
  userSeverInit();

  Serial.print("Access Point: ");
  Serial.println(ssid);
  Serial.println(WiFi.softAPIP());
  uartRingSerial1TotalInit();
}

void loop() {
  // put your main code here, to run repeatedly:
  userSeverHandle();
  ringBuffHandleFun(&uartRingSerial1Param);
  // digitalWrite(10,!digitalRead(10));
  // delay(1000);
}

void serialEvent() 
{
  while (Serial.available()) 
  {
    ringBuff_Push( &uartRingSerial1Param, char(Serial.read()));
    delay(2); //这里不能去掉，要给串口处理数据的时间
  }
}