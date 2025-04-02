#ifndef MATH_HPP
#define MATH_HPP

#include "stm32f10x.h"
#define SUQRT_MATH 0x5f3759df

uint32_t Factorial(uint32_t x);
float Square(float x,uint32_t index);
float fastInverseSqrt(float x);

float Tan(float x);
// float Sin(float x);
// float Cos(float x);

float ArcTan(const float x);
// float ArcSin(float x);
// float ArcCos(float x);

#endif // !MATH_HPP