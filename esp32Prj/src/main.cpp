#include <Arduino.h>
#include <WiFi.h>
#include "Wire.h"
#include <SPI.h>

#include <esp32_user_sever.h>
#include <uart_ring_ex.h>

#include "bmp_font.h"
#include <bmp_23SevenSchool.h>
#include "bmp_radar.h"
#include "canvasAPI.h"
#include "MPU6050_ex.h"

const char *ssid = "ESP32-S3 WIFI";
const char *password = "123456789";

/* 创建硬件定时器 */
void Timer_Init();
hw_timer_t * timer = NULL;
 
/* 创建定时器中断触发标志 */
int FLAG_100ms_timIT = 0;
 
// 中断服务函数，为使编译器将代码分配到IRAM内，中断处理程序应该具有 IRAM_ATTR 属性
void IRAM_ATTR Callback_TimerIT()
{
  FLAG_100ms_timIT = 1;    //定时器中断触发标志置1，中断函数中不能阻塞
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 2, 1);
  pinMode(10, OUTPUT);
  // pinMode(11, OUTPUT);
  WiFi.softAP(ssid,password);
  userSeverInit();
  MPU6050_Init();

  Serial2.printf("444");
  Serial.print("Access Point: ");
  Serial.println(ssid);
  Serial.println(WiFi.softAPIP());
  uartRingDebugTotalInit();
  uartRingSerial2TotalInit();
  Timer_Init();
  Radar_Init();


  // const char* tmp;
  bmpBase.pushHeader2data();
  bmpBase.clear(0xfe19);
  bmpBase.drawRectangle(10,10,67,89,BLUE);
  bmpBase.drawCircle(100,100,10,RED);
  bmpBase.printf_bmpString( 0, 120, BLACK, WHITE, 16, "hello world");
  SS23_Init();
  SS23_InitMap();
  // bmpBase.showImage(0,0,240,135,gImage_misaka);
  if(0){
    Serial.printf("%d %d \n",strlen((char*)bmpBase.bmp_data),bmpBase.bmp_data[4098]);
    Serial.printf("%s",((char*)bmpBase.bmp_data));
  }
}

uint16_t color = 0xffff;
void loop() {
  // put your main code here, to run repeatedly:
  userSeverHandle();
  ringBuffHandleFun(&uartRingDebugParam);
  ringBuffHandleFun(&uartRingSerial2Param);
  // delay(1000);
  if(FLAG_100ms_timIT == 1){
    // Radar_flash();
    digitalWrite(10,!digitalRead(10));
    // digitalWrite(11,!digitalRead(11));
    CoordinateAPI_Coordinate1.showCoordinate();
    sever_canvasCmdCode = CoordinateAPI_Coordinate1.canvasCmdCode;
    FLAG_100ms_timIT = 0;
    if(1&ESP32_MPU6050_ENABLE){
      // 角度读取
      MPU6050_Updata();
      // double mpu_yaw = MPU6050_getAngleZ();   // tockn的getangle，通过一阶置信计算
      // double mpu_pitch = MPU6050_getAngleY(); // tockn的getangle，通过一阶置信计算
      // double mpu_roll =MPU6050_getAngleX();  // tockn的getangle，通过一阶置信计算;
      double mpu_yaw = MPU6050_getGyroAngleZ();   // tockn的getangle，通过一阶置信计算
      double mpu_pitch = MPU6050_getGyroAngleY(); // tockn的getangle，通过一阶置信计算
      double mpu_roll = MPU6050_getGyroAngleX();  // tockn的getangle，通过一阶置信计算;
      Serial.printf("%lf,%lf,%lf\n",mpu_roll,mpu_pitch,mpu_yaw);
        }
  }
}

void serialEvent() 
{
  while (Serial.available()) 
  {
    ringBuff_Push( &uartRingDebugParam, char(Serial.read()));
    // delay(2); //这里不能去掉，要给串口处理数据的时间
  }
}

void serialEvent2() 
{
  char tmp;
  while (Serial2.available()) 
  {
    tmp = char(Serial2.read());
    ringBuff_Push( &uartRingSerial2Param, tmp);
    Serial.printf("%c",tmp);
    // delay(2); //这里不能去掉，要给串口处理数据的时间
  }
}
 
void Timer_Init()
{
  /*  1/(80MHZ/80) = 1us  */
  timer = timerBegin(1, 80, true);  //定时器0，80分频，向上计数
 
  /* 将Callback_TimerIT函数附加到我们的计时器 */
  timerAttachInterrupt(timer, &Callback_TimerIT, true); //第三个参数（edge）：如果为true，则报警将产生边缘类型中断
 
  /* *设置闹钟每5秒调用Callback_TimerIT函数1 tick为1us   => 1秒为1000000us * / 
  / *重复闹钟（第三个参数）*/
  timerAlarmWrite(timer, 500000, true);
 
  timerAlarmEnable(timer);  //中断使能
  Serial.println("start timer");  
}