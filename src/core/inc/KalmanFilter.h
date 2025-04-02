#include "math.h"
#include "Delay.h"
#include "MPU6050.h"
#ifndef KALMAN_FILTER_H
#define KALMAN_FILTER_H

namespace core{
using namespace module;

#define ALPHA 0.4f


	class KalManFilter{
		private:
			MPU6050 mpu6050;
			MPU6050DataUniform _data;
            MPU6050DataUniform _now_data;
            uint32_t _sampling_time;
			EulerAngles _eulerAngles;

            void DataInit(void);

        public:
		KalManFilter(){
            _sampling_time =1;
            mpu6050 =MPU6050();
            DataInit();
        }
		~KalManFilter(){
			
		}
		void CompuEulerAngle(EulerAngles *eulerAngles);
		void SetSamplingTime(uint32_t sampling_time);

	};
}

#endif // !EULER_H
