#include "MPU6050_ex.h"

#if ESP32_MPU6050_ENABLE
    TwoWire MPU6050_I2C = TwoWire(MPU6050_I2C_NUM);
    MPU6050 mpu6050(MPU6050_I2C);
#endif

void MPU6050_Init(){
#if ESP32_MPU6050_ENABLE
  MPU6050_I2C.begin(MPU6050_I2C_SDA, MPU6050_I2C_SCL, MPU6050_I2C_FRE);
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
#endif
}
void MPU6050_Updata(){
#if ESP32_MPU6050_ENABLE 
    mpu6050.update();
#endif
}
double MPU6050_getAngleZ(){
#if ESP32_MPU6050_ENABLE 
    return mpu6050.getAngleZ();
#else 
    return 0.0;
#endif
}
double MPU6050_getAngleY(){
#if ESP32_MPU6050_ENABLE
    return mpu6050.getAngleY();
#else
    return 0.0;
#endif
}
double MPU6050_getAngleX(){
#if ESP32_MPU6050_ENABLE 
    return mpu6050.getAngleX();
#else 
    return 0.0;
#endif
}
double MPU6050_getGyroAngleX(){
#if ESP32_MPU6050_ENABLE 
    return mpu6050.getGyroAngleX();
#else 
    return 0.0;
#endif
};
double MPU6050_getGyroAngleY(){
#if ESP32_MPU6050_ENABLE 
    return mpu6050.getGyroAngleY();
#else 
    return 0.0;
#endif
};
double MPU6050_getGyroAngleZ(){
#if ESP32_MPU6050_ENABLE 
    return mpu6050.getGyroAngleZ();
#else 
    return 0.0;
#endif
};