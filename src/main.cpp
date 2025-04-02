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

	// OLED_Init();		//OLED初始化
    // MPU6050 mpu;
    // MPU6050_Data data;
    KalManFilter euler;
    EulerAngles euler_angles;

    /*显示ID号*/
	// OLED_ShowString(1, 1, "ID:");		//显示静态字符串
    // uint8_t ID = mpu.GetID();           // 获取MPU6050的ID号
    // OLED_ShowHexNum(1, 4, ID, 2);		//OLED显示ID号
	
	while (1){

        euler.CompuEulerAngle(&euler_angles);
        // Delay_ms(500);
        // mpu.GetData(&data);		//获取MPU6050的数据
		// // float gyro_x=(float)(data.ayro_x/2048.0f);
        // OLED_ShowFloatNum(1, 8, data.temp,2,2);
        OLED_ShowString(2, 1, "X:");
        OLED_ShowFloatNum(2, 3, euler_angles._Euler_X,2,2);	
        OLED_ShowString(3, 1, "Y:");				//OLED显示数据
		OLED_ShowFloatNum(3, 3, euler_angles._Euler_Y, 2,2);
        OLED_ShowString(4, 1, "Z:");
		OLED_ShowFloatNum(4, 3, euler_angles._Euler_Z,2,2);
		// OLED_ShowFloatNum(2, 8, (float)(data.gyro_x/100.0f), 2,2);
		// OLED_ShowFloatNum(3, 8, (float)(data.gyro_y/100.0f), 2,2);
		// OLED_ShowFloatNum(4, 8, (float)(data.gyro_z/100.0f), 2,2);
        // OLED_ShowFloatNum(2, 1, 1.233f, 2, 5);
    }


}
