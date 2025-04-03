#include "math.h"
#include "Delay.h"
#include "MPU6050.h"
#ifndef KALMAN_FILTER_H
#define KALMAN_FILTER_H

namespace core{
using namespace module;
#define ZERO 0x000

#define ALPHA 0.8f


	class KalManFilter{
		
		private:
			MPU6050 *mpu6050;
			MPU6050DataUniform *_data;

            
            uint32_t _sampling_time;
            EulerAngles *_gyroEulerAngles;
			EulerAngles	*_accelEulerAngles;
            
            void DataInit(void);

            void GyroAngle();
			void AccelAngle();
		public:
			EulerAngles *_eulerAngles;
			Test test;


        public:
		KalManFilter(){
            _sampling_time =0.5f;
			_gyroEulerAngles =new EulerAngles();
			_accelEulerAngles =new EulerAngles();
			_eulerAngles=new EulerAngles();

			_data =new MPU6050DataUniform();
            mpu6050 =new MPU6050();
            DataInit();
            test.x = 0;
			test.y = 0;
            test.z = 0;
        }
		~KalManFilter(){
            delete _data;
            delete _eulerAngles;
            delete _gyroEulerAngles;
            delete _accelEulerAngles;
            delete mpu6050;
        }
		void CompuEulerAngle();
		void SetSamplingTime(uint32_t sampling_time);

	};
}

#endif // !EULER_H
