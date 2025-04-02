#include "KalmanFilter.h"
using namespace core;



void KalManFilter::DataInit(void){
    _eulerAngles._Euler_X = 0.0f;
    _eulerAngles._Euler_Y = 0.0f;
    _eulerAngles._Euler_Z = 0.0f;

	_data.gyro_X = 0.0f;
	_data.gyro_Y = 0.0f;
	_data.gyro_Z = 0.0f;

	_now_data.gyro_X = 0.0f;
	_now_data.gyro_Y = 0.0f;
	_now_data.gyro_Z = 0.0f;
}
/**
 * @brief 设置采样间隔时间
 * @note 默认是1s
 * @param sampling_time 采样间隔时间
 */
void KalManFilter::SetSamplingTime(uint32_t sampling_time){
    this->_sampling_time = sampling_time;
}

/**
 * @brief 姿态解算
 * 
 * @return EulerAngles 欧拉角
 */
void KalManFilter::CompuEulerAngle(EulerAngles *eulerAngles){
	mpu6050.GetDataUniform(&_data);
	Delay_ms(250);

    //处理数据
    // float gyro_x=(data_gyro_x+ data_gyro_x)*_sampling_time/2;
	// float gyro_y=(data_gyro_y+ data_gyro_y)*_sampling_time/2;
	// float gyro_z=(data_gyro_z+ data_gyro_z)*_sampling_time/2;


	float gyro_x=(_data.gyro_X+ _now_data.gyro_X)*0.125f;
	float gyro_y=(_data.gyro_X+ _now_data.gyro_Y)*0.125f;
	float gyro_z=(_data.gyro_X+ _now_data.gyro_Z)*0.125f;
	


	//--基于陀螺仪rool_gyro             //每s采样一次
	//Time 采样                       #基于离散积分计算
	//计算翻滚角变换值 Wx*dealt(t)
	//计算翻滚角(相对与轴角度)

	// float data_acc_x = mpu_data.accel_x/MPU6050_GYRO;
	// float data_acc_y = mpu_data.accel_y/MPU6050_GYRO;
	// float data_acc_z = mpu_data.accel_z/MPU6050_GYRO;

	// float acc_x = ArcTan(data_acc_x / fastInverseSqrt(Square(data_acc_y, 2) + Square(data_acc_z, 2)));
	// float acc_y = ArcTan(data_acc_y / fastInverseSqrt(Square(data_acc_x, 2) + Square(data_acc_x, 2)));
	// float acc_z = ArcTan(data_acc_z / fastInverseSqrt(Square(data_acc_x, 2) + Square(data_acc_y, 2)));
	//--基于加速度计rool_acc
	//计算翻转角 rool=arctan2(ay,az)

	//--互补滤波融合
	//roll(翻转角)=A*rool_gyro+(1-A)*roll_acc
	// eulerAngles->_Euler_X=ALPHA* gyro_x + (1 - ALPHA) * acc_x;
	// eulerAngles->_Euler_Y=ALPHA* gyro_y + (1 - ALPHA) * acc_y;
	// eulerAngles->_Euler_Z=ALPHA* gyro_z + (1 - ALPHA) * acc_z;

	eulerAngles->_Euler_X=gyro_x;
	eulerAngles->_Euler_Y=gyro_y;
	eulerAngles->_Euler_Z=gyro_z;
	
	_now_data=_data;
	//化为弧度
}