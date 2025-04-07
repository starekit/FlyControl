#include "WQ25.h"
using namespace module;
/**
 * @brief 初始化GPIO
 */
void WQ25::initGPIO(){
	// 使能GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	GPIO_InitTypeDef InitStruct;

	//初始化PA4
	InitStruct.GPIO_Pin  = GPIO_Pin_4;
    InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
   	InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &InitStruct);

    //配置SCK/MOSI引脚为复用推挽输出
	InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &InitStruct);


	//配置MISO引脚为浮空输入
	InitStruct.GPIO_Pin = GPIO_Pin_6;
	InitStruct.GPIO_Mode = GPIO_Mode_IPU;// MISO=PA6
	InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &InitStruct);
}
/**
 * @brief 初始化SPI
 */
void WQ25::initSPI(){
	SPI_InitTypeDef SPI_InitStruct;
	// 使能SPI时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	// SPI1配置
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;          // 主模式
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;      // 8位数据
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;     // MSB先行
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128; // 波特率分频
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;             // 时钟极性
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;           // 时钟相位
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft ; // 软件NSS
	SPI_InitStruct.SPI_CRCPolynomial = 7;	
	SPI_Init(SPI1, &SPI_InitStruct);
	
	SPI_Cmd(SPI1, ENABLE);  // 使能SPI

    SPI_Stop();
}

/** 
 * @brief SPI读写一个字节
 * @param byte 发送数据
 * @return 读取到的数据
*/
uint8_t WQ25::SwapByte(uint8_t byte){
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) !=SET); // 等待发送缓冲区空
	SPI_I2S_SendData(SPI1,byte); // 发送数据
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) !=SET); //等待接收完成
	return SPI_I2S_ReceiveData(SPI1); // 读取接收数据
}

void WQ25::ReadID(uint8_t *MID, uint16_t *DID){
    SPI_Start();
    SwapByte(JEDEC_ID);//发送读取ID命令
    *MID = SwapByte(DUMMY_BYTE);	//交换接收MID,通过输出参数返回
	*DID = SwapByte(DUMMY_BYTE);	//交换接收DID高8位
	*DID<<=8;
    *DID = SwapByte(DUMMY_BYTE);
    SPI_Stop();
}

void WQ25::WriteEnable(){
	SPI_Start();
    SwapByte(WRITE_ENABLE);//发送写使能命令
	SPI_Stop();
}

void WQ25::WaitBusy(){
    uint32_t Timeout;
	SPI_Start();
    SwapByte(READ_STATUS_REGISTER_1);
    Timeout = 100000;
	while((SwapByte(DUMMY_BYTE)& 0x01)==0x01){
        Timeout--;
		if(Timeout==0){
			break;
		}
    }
	SPI_Stop();
}
void WQ25::PageProgram(uint32_t Address,uint8_t *DataArray,uint16_t Count){
    uint16_t i;

    WriteEnable();

	SPI_Start();
    SwapByte(PAGE_PROGRAM);			//交换发送页编程的指令
    SwapByte(Address >> 16);		//交换发送地址23~16位
    SwapByte(Address >> 8);			//交换发送地址15~8位
    SwapByte(Address);				//交换发送地址低7~0位

    for (i = 0;i<Count;i++){
        SwapByte(DataArray[i]);
    }
	SPI_Stop();

    WaitBusy();
}
/** 
 * @brief 读取数据
 * @note 读取数据，地址从0开始，长度最大为256字节
 * @param address 读取地址
 * @param pBuffer 读取数据缓冲区
*/
void WQ25::ReadData(uint32_t Address,uint8_t *pBuffer,uint16_t Count){
    uint32_t i;

	SPI_Start();	// 开始通信
	SwapByte(READ_DATA); // 发送读取命令
	SwapByte(Address >> 16); // 发送地址高8位
	SwapByte(Address >> 8); // 发送地址中8位
	SwapByte(Address); 
	for(i = 0; i < Count; i++){
		pBuffer[i] = SwapByte(DUMMY_BYTE); // 读取数据
	}
	SPI_Stop();
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
void WQ25::EraseChip(void){
	WriteEnable();//写使能
	SPI_Start();
	SwapByte(0xC7); // 发送擦除命令(0xC7)
	SPI_Stop();
	WaitBusy(); // 等待擦除结束
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