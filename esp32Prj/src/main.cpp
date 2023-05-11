#include <Arduino.h>
#include <WiFi.h>

#include <esp32_user_sever.h>

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
}

void loop() {
  userSeverHandle();
  // put your main code here, to run repeatedly:
  digitalWrite(10,!digitalRead(10));
  // delay(1000);
}