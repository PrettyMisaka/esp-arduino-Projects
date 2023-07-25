#ifndef __MPU6050_EX_H__
#define __MPU6050_EX_H__

#include "MPU6050_tockn.h"

#define ESP32_MPU6050_ENABLE    1

#if ESP32_MPU6050_ENABLE
    #define MPU6050_I2C_NUM 0
    #define MPU6050_I2C_SDA 8
    #define MPU6050_I2C_SCL 9
    #define MPU6050_I2C_FRE 400000UL
#endif

void MPU6050_Init();
void MPU6050_Updata();
double MPU6050_getAngleZ();
double MPU6050_getAngleY();
double MPU6050_getAngleX();

double MPU6050_getGyroAngleX();
double MPU6050_getGyroAngleY();
double MPU6050_getGyroAngleZ();


#endif
