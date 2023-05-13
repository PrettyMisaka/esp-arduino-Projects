#include <Arduino.h>
#include <WiFi.h>

#include <esp32_user_sever.h>
#include <uart_ring_ex.h>
#include <canvasAPI.h>

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
  pinMode(10, OUTPUT);
  WiFi.softAP(ssid,password);
  userSeverInit();

  Serial.print("Access Point: ");
  Serial.println(ssid);
  Serial.println(WiFi.softAPIP());
  uartRingSerial1TotalInit();
  Timer_Init();
}

void loop() {
  // put your main code here, to run repeatedly:
  userSeverHandle();
  ringBuffHandleFun(&uartRingSerial1Param);
  // delay(1000);
  if(FLAG_100ms_timIT == 1){
    digitalWrite(10,!digitalRead(10));
    CoordinateAPI_Coordinate1.showCoordinate();
    sever_canvasCmdCode = CoordinateAPI_Coordinate1.canvasCmdCode;
    FLAG_100ms_timIT = 0;
  }
}

void serialEvent() 
{
  while (Serial.available()) 
  {
    ringBuff_Push( &uartRingSerial1Param, char(Serial.read()));
    delay(2); //这里不能去掉，要给串口处理数据的时间
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