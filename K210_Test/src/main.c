#include <stdio.h>
#include <unistd.h>
#include "fpioa.h"
#include "gpio.h"

int main() {
  gpio_init();                             //初始化GPIO
  fpioa_set_function(13, FUNC_GPIO3);      //管脚映射
  gpio_set_drive_mode(3, GPIO_DM_OUTPUT);  //设置GPIO驱动模式
  gpio_set_pin(3, GPIO_PV_LOW);            //低电平
  while (1) {
    sleep(1);                              //延时1s
    printf("led on\r\n");                  //串口打印
    gpio_set_pin(3, GPIO_PV_LOW);
    sleep(1);
    printf("led off\r\n");
    gpio_set_pin(3, GPIO_PV_HIGH);
  }
  return 1;
}

