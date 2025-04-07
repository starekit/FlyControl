// Created by 2023/10/12 14:00
#include "core.hpp"
#include "stm32f10x_conf.h"
#ifndef MPU6050_H
#define MPU6050_H



// #define MPU6050_GYRO_CONFIG_2G 
#ifdef MPU6050_GYRO_CONFIG_2G
#define MPU6050_GYRO 0x4000
#endif


// #define MPU6050_GYRO_CONFIG_4G
#ifdef MPU6050_GYRO_CONFIG_4G
#define MPU6050_GYRO 0x2000
#endif     


// #define MPU6050_GYRO_CONFIG_8G
#ifdef MPU6050_GYRO_CONFIG_8G
#define MPU6050_GYRO 0x1000
#endif     


#define MPU6050_GYRO_CONFIG_16G
#ifdef MPU6050_GYRO_CONFIG_16G
#define MPU6050_GYRO 0x800
#endif    



#define MPU6050_ADDRESS	0x69
namespace module{
                                                                                                                                                                                                           
	
	#define	SMPLRT_DIV		0x19
	#define	CONFIG			0x1A
	#define	GYRO_CONFIG		0x1B
	#define	ACCEL_CONFIG	0x1C

	#define	ACCEL_XOUT_H	0x3B
	#define	ACCEL_XOUT_L	0x3C
	#define	ACCEL_YOUT_H	0x3D
	#define	ACCEL_YOUT_L	0x3E
	#define	ACCEL_ZOUT_H	0x3F
	#define	ACCEL_ZOUT_L	0x40
	#define	TEMP_OUT_H		0x41
	#define	TEMP_OUT_L		0x42
	#define	GYRO_XOUT_H		0x43
	#define	GYRO_XOUT_L		0x44
	#define	GYRO_YOUT_H		0x45
	#define	GYRO_YOUT_L		0x46
	#define	GYRO_ZOUT_H		0x47
	#define	GYRO_ZOUT_L		0x48

	#define	PWR_MGMT_1		0x6B
	#define	PWR_MGMT_2		0x6C
	#define	MPU_ID			0x75


	#define MPU6050_AD0_ON()  GPIO_WriteBit(GPIOB, GPIO_Pin_5,Bit_SET)


    class MPU6050{
		private:
           
            void IICInit(void);
			void MPUInit();
            void DataInit();
            void WriteReg(uint8_t RegAddress, uint8_t Data);
			uint8_t ReadReg(uint8_t RegAddress);
			void WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT);
			


		public:
		MPU6050Data *_mpu6050Data;
		void GetData();
			MPU6050(){
                MPU6050_AD0_ON();
                IICInit();
				MPUInit();

				_mpu6050Data = new MPU6050Data;
                DataInit();
            }
			~MPU6050(){
                delete _mpu6050Data;
            }
			uint8_t GetID(void);
			void GetDataUniform(MPU6050DataUniform *data);
    };

	
}// namespace module
#endif // MPU6050_H




