#ifndef MATH_HPP
#define MATH_HPP


#include "stm32f10x.h"
#define SUQRT_MATH 0x5f3759df
#define PI 3.1415926f
#define RAD 57.295f

uint32_t Factorial(uint32_t x);
int32_t Square(int16_t x);
float Power(float x,int16_t index);

float fastInverseSqrt(int32_t x);

float Tan(float x);
// float Sin(float x);
// float Cos(float x);
int32_t round_x(float x, uint8_t multiple);
float ArcTan(const float x);
// float ArcSin(float x);
// float ArcCos(float x);

#endif // !MATH_HPP