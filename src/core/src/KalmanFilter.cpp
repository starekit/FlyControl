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
	Delay_s(_sampling_time);
    float integral_scope = _sampling_time / 2;

    //处理数据


	float gyro_x=(_data.gyro_X+ _now_data.gyro_X)*integral_scope;
	float gyro_y=(_data.gyro_X+ _now_data.gyro_Y)*integral_scope;
	float gyro_z=(_data.gyro_X+ _now_data.gyro_Z)*integral_scope;
	


	//--基于陀螺仪rool_gyro             //每s采样一次
	//Time 采样                       #基于离散积分计算
	//计算翻滚角变换值 Wx*dealt(t)
	//计算翻滚角(相对与轴角度)

	float acc_x = ArcTan(_data.accel_X / fastInverseSqrt(Square(_data.accel_Y, 2) + Square(_data.accel_Z, 2)));
	float acc_y = ArcTan(_data.accel_Y / fastInverseSqrt(Square(_data.accel_X, 2) + Square(_data.accel_Z, 2)));
	float acc_z = ArcTan(_data.accel_Z / fastInverseSqrt(Square(_data.accel_X, 2) + Square(_data.accel_Y, 2)));
	//--基于加速度计rool_acc
	//计算翻转角 rool=arctan2(ay,az)

	//--互补滤波融合
	//roll(翻转角)=A*rool_gyro+(1-A)*roll_acc
	eulerAngles->_Euler_X=ALPHA* gyro_x + (1 - ALPHA) * acc_x;
	eulerAngles->_Euler_Y=ALPHA* gyro_y + (1 - ALPHA) * acc_y;
	eulerAngles->_Euler_Z=ALPHA* gyro_z + (1 - ALPHA) * acc_z;
	
	_now_data=_data;
	//化为弧度
}