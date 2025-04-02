#include "WQ25.h"
using namespace module;
/**
 * @brief 初始化GPIO
 */
void WQ25::initGPIO(){
	// 使能GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitTypeDef InitStruct;

	//配置SCK/MOSI引脚为复用推挽输出
	InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &InitStruct);


	//配置MISO引脚为浮空输入
	InitStruct.GPIO_Pin = GPIO_Pin_6;
	InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;// MISO=PA6
	InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
}
/**
 * @brief 初始化SPI
 */
void WQ25::initSPI(){
	SPI_InitTypeDef SPI_InitStruct;
	// 使能SPI时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	// SPI1配置
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;          // 主模式
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;      // 8位数据
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;             // 时钟极性
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;           // 时钟相位
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set; // 软件NSS
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; // 波特率分频
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;     // MSB先行

	SPI_Init(SPI1, &SPI_InitStruct);
	SPI_Cmd(SPI1, ENABLE);  // 使能SPI
}
/** 
 * @brief SPI读写一个字节
 * @param byte 发送数据
 * @return 读取到的数据
*/
uint8_t WQ25::SPI_ReadWriteByte(uint8_t byte){
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET); // 等待发送缓冲区空
	SPI_I2S_SendData(SPI1,byte); // 发送数据
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) == RESET); //等待接收完成
	return SPI_I2S_ReceiveData(SPI1); // 读取接收数据
}
/** 
 * @brief 读取数据
 * @note 读取数据，地址从0开始，长度最大为256字节
 * @param address 读取地址
 * @param pBuffer 读取数据缓冲区
*/
void WQ25::WQ25_ReadData(uint32_t address,uint8_t *pBuffer,uint16_t length){
	WQ25_CS_LOW(); // 片选引脚拉低，开始通信
	SPI_ReadWriteByte(0x03); // 发送读取命令
	SPI_ReadWriteByte((address >> 16) & 0xFF); // 发送地址高8位
	SPI_ReadWriteByte((address >> 8) & 0xFF); // 发送地址中8位
	SPI_ReadWriteByte(address & 0xFF); 
	for(uint16_t i = 0; i < length; i++){
		pBuffer[i] = SPI_ReadWriteByte(0xFF); // 读取数据
	}
	WQ25_CS_HIGH(); // 片选引脚拉高，结束通信
}
/**
 * @brief WQ25写使能
 */
void WQ25::WQ25_WriteEnable(){
	WQ25_CS_LOW(); // 片选引脚拉低，开始通信
	SPI_ReadWriteByte(0x06); // 写使能命令(0x06)
	WQ25_CS_HIGH(); // 片选引脚拉高，结束通信
}
/**
 * @brief 等待写入结束
 */
void WQ25::WQ25_WaitForWriteEnd(){
	do{
		WQ25_CS_LOW();
		SPI_ReadWriteByte(0x06); // 发送读取状态寄存器命令(0x06)
		WQ25_CS_HIGH();
	}while(SPI_ReadWriteByte(0xFF)&0x01); // 等待写入结束，检查状态寄存器的BUSY位
	WQ25_CS_HIGH(); // 片选引脚拉高，结束通信
}

/**
 * @brief 写入数据
 * 
 * @param address 写入地址
 * @note 写入数据，地址从0开始，长度最大为256字节
 * @param pBuffer 写入数据缓冲区
 * @param length 写入数据长度
 */
void WQ25::WQ25_WritePage(uint32_t address,uint8_t *pBuffer,uint16_t length){
	WQ25_WriteEnable(); // 写使能
	SPI_ReadWriteByte(0x02); // 页编程命令(0x02)
	SPI_ReadWriteByte((address >> 16) & 0xFF); // 发送地址高8位
	SPI_ReadWriteByte((address >> 8) & 0xFF); // 发送地址中8位
	SPI_ReadWriteByte(address & 0xFF); // 发送地址低8位

	for(uint16_t i = 0; i < length; i++){
		SPI_ReadWriteByte(pBuffer[i]); // 写入数据
	}
	WQ25_CS_HIGH(); 
	WQ25_WaitForWriteEnd(); // 等待写入结束
}

/**
 * @brief 擦除页数据
 */
void WQ25::erasureData(){

}
/** 
 * @brief 擦除整个芯片
 * @note 擦除整个芯片，擦除时间较长，建议在使用前先擦除
 */
void WQ25::WQ25_EraseChip(void){
	WQ25_WriteEnable();//写使能
	WQ25_CS_LOW(); // 片选引脚拉低，开始通信
	SPI_ReadWriteByte(0xC7); // 发送擦除命令(0xC7)
	WQ25_CS_HIGH(); // 片选引脚拉高，结束通信
	WQ25_WaitForWriteEnd(); // 等待擦除结束
}

void WQ25::Init_HOLD_WP(){
	//INIT HOLD
	// 使能GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef InitStruct;
	InitStruct.GPIO_Pin = WQ25_HOLD_PIN|WQ25_WP_PIN; // HOLD引脚和WP引脚
	InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
	InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &InitStruct);
	GPIO_SetBits(WQ25_HOLD_WP, WQ25_HOLD_PIN);

	//INIT WP
	// GPIO_ResetBits(W25Q_WP_GPIO, W25Q_WP_PIN);
	GPIO_SetBits(WQ25_HOLD_WP, WQ25_WP_PIN); // 使能写保护引脚,拉高允许写入

}