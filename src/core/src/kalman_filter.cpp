#include "kalman_filter.h"
#include "oled.h"
using namespace core;



void KalManFilter::DataInit(void){

    _data->gyro_X = ZERO;
    _data->gyro_Y = ZERO;
    _data->gyro_Z = ZERO;

    _eulerAngles->_Euler_X=ZERO;
    _eulerAngles->_Euler_Y = ZERO;
    _eulerAngles->_Euler_Z = ZERO;

    _gyroEulerAngles->_Euler_X = ZERO;
    _gyroEulerAngles->_Euler_Y = ZERO;
    _gyroEulerAngles->_Euler_Z = ZERO;

    _accelEulerAngles->_Euler_X = ZERO;
    _accelEulerAngles->_Euler_Y = ZERO;
    _accelEulerAngles->_Euler_Z = ZERO;
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
void KalManFilter::CompuEulerAngle(){
	mpu->GetDataUniform(_data);
    GyroAngle();
	AccelAngle();

    //--互补滤波融合
	//roll(翻转角)=A*rool_gyro+(1-A)*roll_acc
	// eulerAngles->_Euler_X=ALPHA* gyro_x + (1 - ALPHA) * acc_x;
	// eulerAngles->_Euler_Y=ALPHA* gyro_y + (1 - ALPHA) * acc_y;
	// eulerAngles->_Euler_Z=ALPHA* gyro_z + (1 - ALPHA) * acc_z;

    // _eulerAngles->_Euler_X=_accelEulerAngles->_Euler_X;
    // _eulerAngles->_Euler_Y = _accelEulerAngles->_Euler_Y;
    // _eulerAngles->_Euler_Z = _accelEulerAngles->_Euler_Z;
    // eulerAngles->_Euler_Y += gyro_y;
    // eulerAngles->_Euler_Z += gyro_z;

    // eulerAngles->_temp = _data.temp;

    Delay_ms(200);

}
	
//--基于陀螺仪rool_gyro             //每s采样一次
//Time 采样                       #基于离散积分计算
//计算翻滚角变换值 Wx*dealt(t)
//计算翻滚角(相对与轴角度)
	
void KalManFilter::GyroAngle(){
	float integral_scope =0.2f;
    _gyroEulerAngles->_Euler_X = _data->gyro_X * integral_scope;
	_gyroEulerAngles->_Euler_Y=_data->gyro_Y * integral_scope;
	_gyroEulerAngles->_Euler_Z=_data->gyro_Z * integral_scope;

    _eulerAngles->_Euler_X = _gyroEulerAngles->_Euler_X;
    _eulerAngles->_Euler_Y = _gyroEulerAngles->_Euler_Y;
    _eulerAngles->_Euler_Z = _gyroEulerAngles->_Euler_Z;
}
//--基于加速度计rool_acc
//计算翻转角 rool=arctan2(ay,az)
void KalManFilter::AccelAngle(){
    // float angles_x = round(1 / (fastInverseSqrt(Square(_data->accel_Y) + Square(_data->accel_Z))), 100) / 100.0f;
	// float angles_y = round(1 / (fastInverseSqrt(Square(_data->accel_X) + Square(_data->accel_Z))), 100) / 100.0f;
	// float angles_z = round(1 / (fastInverseSqrt(Square(_data->accel_X) + Square(_data->accel_Y))), 100) / 100.0f;

    float angles_x= 1 / (fastInverseSqrt(Square(_data->accel_Y) + Square(_data->accel_Z)));
    float angles_y= 1 / (fastInverseSqrt(Square(_data->accel_X) + Square(_data->accel_Z)));
    float angles_z=1 / (fastInverseSqrt(Square(_data->accel_X) + Square(_data->accel_Y)));

    _accelEulerAngles->_Euler_X=ArcTan((float)_data->accel_X /angles_x)*RAD;
	_accelEulerAngles->_Euler_Y=ArcTan((float)_data->accel_Y /angles_y)*RAD;
	_accelEulerAngles->_Euler_Z=ArcTan((float)_data->accel_Z /angles_z)*RAD;

	// _accelEulerAngles->_Euler_X=angles_x;
	// _accelEulerAngles->_Euler_Y=angles_y;
	// _accelEulerAngles->_Euler_Z=angles_z;

	// test.x=_data->accel_X;
	// test.y=_data->accel_Y;
	// test.z=_data->accel_Z;
}