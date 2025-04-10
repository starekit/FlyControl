#ifndef CORE_HPP
#define CORE_HPP
#include "stm32f10x.h"
typedef struct{
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;
    int16_t temp;
    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;
}MPUData;

typedef struct{
    int16_t accel_X;
    int16_t accel_Y;
    int16_t accel_Z;
    float temp;
    float gyro_X;
    float gyro_Y;
    float gyro_Z;
} MPUDataUniform;



typedef struct {
    float _Euler_X;
    float _Euler_Y;
    float _Euler_Z;
}EulerAngles;
typedef struct{
    int16_t x;
    int16_t y;
    int16_t z;
} Test;

#endif