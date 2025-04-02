#include "MPU6050.h"
using namespace module;

/**
  * @brief：MPU6050初始化
  * @param：none
  * @return:none
  */
void MPU6050::IICInit(void){
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);		//开启I2C2的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//开启GPIOB的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//将PB10和PB11引脚初始化为复用开漏输出
	
	/*I2C初始化*/
	I2C_InitTypeDef I2C_InitStructure;						//定义结构体变量
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;				//模式，选择为I2C模式
	I2C_InitStructure.I2C_ClockSpeed = 50000;				//时钟速度，选择为50KHz
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;		//时钟占空比，选择Tlow/Thigh = 2
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;				//应答，选择使能
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	//应答地址，选择7位，从机模式下才有效
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;				//自身地址，从机模式下才有效
	I2C_Init(I2C2, &I2C_InitStructure);						//将结构体变量交给I2C_Init，配置I2C2
	
	/*I2C使能*/
	I2C_Cmd(I2C2, ENABLE);									//使能I2C2，开始运行
	 
}
void MPU6050::MPUInit(void){
	WriteReg(PWR_MGMT_1, 0x01);				//电源管理寄存器1，取消睡眠模式，选择时钟源为X轴陀螺仪
	WriteReg(PWR_MGMT_2, 0x00);				//电源管理寄存器2，保持默认值0，所有轴均不待机
	WriteReg(SMPLRT_DIV, 0x09);				//采样率分频寄存器，配置采样率
	WriteReg(CONFIG, 0x06);					//配置寄存器，配置DLPF
	WriteReg(GYRO_CONFIG, 0x18);			//陀螺仪配置寄存器，选择满量程为±2000°/s
	WriteReg(ACCEL_CONFIG, 0x18);			//加速度计配置寄存器，选择满量程为±16g	
}

void MPU6050::DataInit(){
	_mpu6050Data->accel_x = 0;
	_mpu6050Data->accel_y=0;
	_mpu6050Data->accel_z=0;

	_mpu6050Data->temp=0;
	_mpu6050Data->gyro_x=0;
	_mpu6050Data->gyro_y=0;
	_mpu6050Data->gyro_z=0;
}
/**
  * @brief：MPU6050等待事件
  * @param：同I2C_CheckEvent
  * @return:none
  */
void MPU6050::WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT){
	 uint32_t Timeout;
	 Timeout = 10000;									//给定超时计数时间
	 while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)	//循环等待指定事件
	 {
		 Timeout --;										//等待时，计数值自减
		 if (Timeout == 0)								//自减到0后，等待超时
		 {
			 /*超时的错误处理代码，可以添加到此处*/
			 break;										//跳出等待，不等了
		 }
	 }
 }
 
 /**
   * @brief  MPU6050写入寄存器
   * @param RegAddress寄存器地址，范围：参考MPU6050手册的寄存器描述
   * @param Data 要写入寄存器的数据，范围：0x00~0xFF
   */
void MPU6050::WriteReg(uint8_t RegAddress, uint8_t Data){
	I2C_GenerateSTART(I2C2, ENABLE);										//硬件I2C生成起始条件
	WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);					//等待EV5
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);	//硬件I2C发送从机地址，方向为发送
	WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);	//等待EV6
	
	I2C_SendData(I2C2, RegAddress);											//硬件I2C发送寄存器地址
	WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);			//等待EV8
	
	I2C_SendData(I2C2, Data);												//硬件I2C发送数据
	WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);				//等待EV8_2
	
	I2C_GenerateSTOP(I2C2, ENABLE);											//硬件I2C生成终止条件
}
 
 /**
   * @brief：MPU6050读寄存器
   * @param：RegAddress 寄存器地址，范围：参考MPU6050手册的寄存器描述
   * @return：读取寄存器的数据，范围：0x00~0xFF
   */
 uint8_t MPU6050::ReadReg(uint8_t RegAddress){
	uint8_t Data;
	
	I2C_GenerateSTART(I2C2, ENABLE);										//硬件I2C生成起始条件
	WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);					//等待EV5
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);	//硬件I2C发送从机地址，方向为发送
	WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);	//等待EV6
	
	I2C_SendData(I2C2, RegAddress);											//硬件I2C发送寄存器地址
	WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);				//等待EV8_2
	
	I2C_GenerateSTART(I2C2, ENABLE);										//硬件I2C生成重复起始条件
	WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT);					//等待EV5
	
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Receiver);		//硬件I2C发送从机地址，方向为接收
	WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);		//等待EV6
	
	I2C_AcknowledgeConfig(I2C2, DISABLE);									//在接收最后一个字节之前提前将应答失能
	I2C_GenerateSTOP(I2C2, ENABLE);											//在接收最后一个字节之前提前申请停止条件
	
	WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);				//等待EV7
	Data = I2C_ReceiveData(I2C2);											//接收数据寄存器
	
	I2C_AcknowledgeConfig(I2C2, ENABLE);									//将应答恢复为使能，为了不影响后续可能产生的读取多字节操作
	
	return Data;
 }
 /**
   * @brief 获取MPU6050的ID号
   * @return：MPU6050的ID号
   */
uint8_t MPU6050::GetID(void){
	return ReadReg(MPU_ID);		//返回WHO_AM_I寄存器的值
}
 
 /**
   * @brief：MPU6050获取数据
   * @param：AccX AccY AccZ 加速度计X、Y、Z轴的数据，使用输出参数的形式返回，范围：-32768~32767
   * @param：GyroX GyroY GyroZ 陀螺仪X、Y、Z轴的数据，使用输出参数的形式返回，范围：-32768~32767
   * @return：none
   */
 void MPU6050::GetData(){
	uint8_t DataH, DataL;						//定义数据高8位和低8位的变量
	
	DataH = ReadReg(ACCEL_XOUT_H);				//读取加速度计X轴的高8位数据
	DataL = ReadReg(ACCEL_XOUT_L);				//读取加速度计X轴的低8位数据
	_mpu6050Data->accel_x = (DataH << 8) | DataL;		//数据拼接，通过输出参数返回
	
	DataH = ReadReg(ACCEL_YOUT_H);				//读取加速度计Y轴的高8位数据
	DataL = ReadReg(ACCEL_YOUT_L);				//读取加速度计Y轴的低8位数据
	_mpu6050Data->accel_y = (DataH << 8) | DataL;		//数据拼接，通过输出参数返回
	
	DataH = ReadReg(ACCEL_ZOUT_H);				//读取加速度计Z轴的高8位数据
	DataL = ReadReg(ACCEL_ZOUT_L);				//读取加速度计Z轴的低8位数据
	_mpu6050Data->accel_z = (DataH << 8) | DataL;		//数据拼接，通过输出参数返回
	
	DataH = ReadReg(GYRO_XOUT_H);				//读取陀螺仪X轴的高8位数据
	DataL = ReadReg(GYRO_XOUT_L);				//读取陀螺仪X轴的低8位数据
	_mpu6050Data->gyro_x = (DataH << 8) | DataL;		//数据拼接，通过输出参数返回
	
	DataH = ReadReg(GYRO_YOUT_H);				//读取陀螺仪Y轴的高8位数据
	DataL = ReadReg(GYRO_YOUT_L);				//读取陀螺仪Y轴的低8位数据
	_mpu6050Data->gyro_y = (DataH << 8) | DataL;		//数据拼接，通过输出参数返回
	
	DataH = ReadReg(GYRO_ZOUT_H);				//读取陀螺仪Z轴的高8位数据
	DataL = ReadReg(GYRO_ZOUT_L);				//读取陀螺仪Z轴的低8位数据
	_mpu6050Data->gyro_z= (DataH << 8) | DataL;			//数据拼接，通过输出参数返回

	DataH = ReadReg(TEMP_OUT_H);
	DataL = ReadReg(TEMP_OUT_L);
	_mpu6050Data->temp = (DataH << 8) | DataL;
}

void MPU6050::GetDataUniform(MPU6050DataUniform *data){
    GetData();
	data->accel_X =_mpu6050Data->gyro_x/MPU6050_GYRO;
	data->accel_Y= _mpu6050Data->gyro_y/MPU6050_GYRO;
	data->accel_Z= _mpu6050Data->gyro_z/MPU6050_GYRO;

	data->gyro_X =_mpu6050Data->gyro_x/10;
	data->gyro_Y = _mpu6050Data->gyro_y/10;
	data->gyro_Z = _mpu6050Data->gyro_z/10;
}