/*
 * ************************************************
 *        FLY CONTORL
 * ************************************************
*/

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "KalmanFilter.h"
#include "PWM.h"
#include "MPU6050.h"
#include "WQ25.h"
#include "OLED.h"
#include "Delay.h"
#include "math.h"
using namespace module;
using namespace core;


uint8_t MID;							//定义用于存放MID号的变量
uint16_t DID;	
int main(){
    // PWM pwm;
    WQ25 wq25;

    OLED_Init();		//OLED初始化
    // MPU6050 mpu;
    OLED_ShowString(1, 1, "Hello World!");
    OLED_ShowString(1, 1, "MID:   DID:");

    wq25.ReadID(&MID, &DID);			//获取W25Q64的ID号
	OLED_ShowHexNum(1, 5, MID, 2);		//显示MID
	OLED_ShowHexNum(1, 12, DID, 4);		//显示DID

    // MPU6050_Data data;
    // KalManFilter euler;
   

    /*显示ID号*/
	// OLED_ShowString(1, 1, "ID:");		//显示静态字符串
    // uint8_t ID = mpu.GetID();           // 获取MPU6050的ID号
    // OLED_ShowHexNum(1, 4, ID, 2);		//OLED显示ID号
    // int x = 0;
    // // MPU6050DataUniform data;

    // while (1){
    //     pwm.setCompares(100, 200, 400, 750);
    //     Delay_s(1);

        // euler.CompuEulerAngle();
        // mpu.GetDataUniform(&data);
        // mpu.GetData();

        // OLED_ShowFloatNum(1, 8, data.temp,2,2);
        // OLED_ShowString(2, 1, "X:");
        // OLED_ShowString(3, 1, "Y:");	
        // OLED_ShowString(4, 1, "Z:");
        // OLED_ShowFloatNum(2, 3, x/3.0f,3,2);	
      
		// OLED_ShowFloatNum(3, 3,y/3.0f, 3,2);
       
		// OLED_ShowFloatNum(4, 3, z/3.0f,3,2);

        // OLED_ShowFloatNum(2, 3, data.gyro_X, 2, 2);
        // OLED_ShowSignedNum(2, 1, mpu._mpu6050Data->gyro_x, 5);
        // OLED_ShowSignedNum(3, 1, mpu._mpu6050Data->gyro_y, 5);
        // OLED_ShowSignedNum(4, 1, mpu._mpu6050Data->gyro_z, 5);
        // OLED_ShowNum(2, 1, x, 3);
        // x++;

        // OLED_ShowSignedNum(2, 3, euler._eulerAngles->_Euler_X, 5);
        // OLED_ShowSignedNum(3, 3, euler._eulerAngles->_Euler_Y, 5);
        // OLED_ShowSignedNum(4, 3, euler._eulerAngles->_Euler_Z, 5);
        // OLED_ShowFloatNum(2, 1, 1.233f, 2, 5);
    // }


}
