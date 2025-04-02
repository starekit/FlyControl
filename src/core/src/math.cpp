

/*
 * ************************************************
 * 
 * ________________MATH_FUNCTION___________ _____    
 * 
 * ************************************************
*/

/**
 * @brief x^n 
 * @param x 底数
 * @param index 指数
 * @return float 
 */
#include "math.h"
float Square(float x,uint32_t index){
    if(index==0x00){
        return 1;
    }else if(index<0x00){
        float sum=1;
        for (int i = index; index >0;i--){
            sum *= x;
        }
        return 1/sum;
    }
    else{
        float sum=1;
        for (int i = index; index >0;i--){
            sum *= x;
        }
        return sum;
    }
}
/**
 * @brief X!
 * 
 * @param x 阶乘数
 * @return int 
 */
uint32_t Factorial(uint32_t x){
    uint32_t sum = 1;
    while(x>1){
        sum *= x;
        x--;
    }
    return sum;
}

/**
 * @brief sin(x)
 * @param x 
 * @return float 
 */
// float Sin(float x){
//     return x - Square(x, 3) / Factorial(3) + Square(x, 5) / Factorial(5) - Square(x, 7) / Factorial(7);
// }
/**
 * @brief cos(x)
 * 
 * @param x 
 * @return float 
 */
// float Cos(float x){
//     return 1.0f;
// }
/**
 * @brief tan(x)
 * 
 * @param x 
 * @return float 
 */
float Tan(float x){
    return 1.0f;
}
// float ArcSin(float x){
//     return Sin(x);
// }
// float ArcCos(float x){
//     return Cos(x);
// }
float ArcTan(const float x){
    float sum = x;
    for (int i = 0; i < 4; i++) {
        sum += Square(-1.0f, i) * Square(x, 2 * i + 1) / (2 * i + 1);
    }
    return sum;
}

/**
 * @brief 快速平方根倒数
 * 
 * @param x 开平方的数
 * @return float 
 */
float fastInverseSqrt(float x){
    union {
        float f;
        uint32_t i;
    }u;

    u.f = x;
    u.i = SUQRT_MATH - (u.i >> 1);
    float y = u.f;
    y       = y * (1.5 - 0.5 * x * y * y);
    return y;
}
