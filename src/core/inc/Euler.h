#include "math.h"
#include "PWM.h"
#include "Delay.h"
#include "MPU6050.h"
#include "OLED.h"
#ifndef EULER_H
#define EULER_H



namespace module{

#define ALPHA 0.4f


	typedef struct {
		float _Euler_X;
		float _Euler_Y;
		float _Euler_Z;
	} EulerAngles;

	class EulerAngle{
		private:
			MPU6050 mpu6050;
			MPU6050DataUniform _data;
            MPU6050DataUniform _now_data;
            uint32_t _sampling_time;
			EulerAngles _eulerAngles;

            void DataInit(void);

        public:
		EulerAngle(){
            OLED_Init();
            _sampling_time =1;
            mpu6050 =MPU6050();
            DataInit();

            OLED_ShowNum(1, 1, mpu6050.GetID(), 2);
        }
		~EulerAngle(){
			
		}
		void CompuEulerAngle(EulerAngles *eulerAngles);
		void SetSamplingTime(uint32_t sampling_time);

	};
}

#endif // !EULER_H
