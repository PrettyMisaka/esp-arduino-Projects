#include <Arduino.h>

void ledFlash(int ms);
int num = 1;

void setup() {
  Serial.begin(9600);   
  // put your setup code here, to run once:
}

void loop() {
  Serial.printf("testnum%d",num);
  delay(900);
  num = (num+1)%10;
  ledFlash(100);
  // put your main code here, to run repeatedly:
}

void ledFlash(int ms){
    digitalWrite(LED_BUILTIN, LOW);
    delay(ms);
    digitalWrite(LED_BUILTIN, HIGH);                 
    delay(ms);
}