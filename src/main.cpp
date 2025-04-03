/*
 * ************************************************
 * 
 *        
 * 
 * ************************************************
*/

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "KalmanFilter.h"
#include "PWM.h"
// #include "WQ25.h"
#include "OLED.h"
#include "Delay.h"
#include "math.h"
using namespace module;
using namespace core;

int main(){

	OLED_Init();		//OLED初始化
    // MPU6050 mpu;
 
    // MPU6050_Data data;
    KalManFilter euler;
   

    /*显示ID号*/
	// OLED_ShowString(1, 1, "ID:");		//显示静态字符串
    // uint8_t ID = mpu.GetID();           // 获取MPU6050的ID号
    // OLED_ShowHexNum(1, 4, ID, 2);		//OLED显示ID号
	
	while (1){

        float x = 0;
        float y = 0;
        float z = 0;
        for(int i=0;i<3;i++){
            euler.CompuEulerAngle();
            x+=euler._eulerAngles->_Euler_X;
            y+=euler._eulerAngles->_Euler_Y;
            z+=euler._eulerAngles->_Euler_Z;
        }


        // OLED_ShowFloatNum(1, 8, data.temp,2,2);
        OLED_ShowString(2, 1, "X:");
        OLED_ShowString(3, 1, "Y:");	
        OLED_ShowString(4, 1, "Z:");
        OLED_ShowFloatNum(2, 3, x/3.0f,3,2);	
      
		OLED_ShowFloatNum(3, 3,y/3.0f, 3,2);
       
		OLED_ShowFloatNum(4, 3, z/3.0f,3,2);
    
        // OLED_ShowSignedNum(2, 1, euler.test.x, 5);
        // OLED_ShowSignedNum(3, 1, euler.test.y, 5);
        // OLED_ShowSignedNum(4, 1, euler.test.z, 5);
        // OLED_ShowFloatNum(2, 1, 1.233f, 2, 5);
    }


}
