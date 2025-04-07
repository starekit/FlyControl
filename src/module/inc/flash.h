#ifndef FLASH_H
#define FLASH_H
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
namespace module{
	#define WQ25_HOLD_WP GPIOB 
	#define WQ25_HOLD_PIN GPIO_Pin_1
	#define WQ25_WP_PIN GPIO_Pin_0
	#define JEDEC_ID							0x9F
	#define DUMMY_BYTE							0xFF
	#define WRITE_ENABLE 						0x06
	#define PAGE_PROGRAM						0x02
	#define READ_STATUS_REGISTER_1				0x05
	#define READ_DATA 							0x03
	#define SECTOR_ERASE_4KB 					0x20

	#define	SPI_Start() GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET)
	#define SPI_Stop()  GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET)
	#define WQ25_CS_LOW() GPIO_ResetBits(GPIOA,GPIO_Pin_4)//CS引脚低电平
	#define WQ25_CS_HIGH() GPIO_SetBits(GPIOA,GPIO_Pin_4)//CS引脚高电平

	class Flash{
		private:
			void initGPIO();
			void initSPI();
			void Init_HOLD_WP();

            uint8_t SwapByte(uint8_t byte);

			void WriteEnable();//写使能
            void WaitBusy();
		
		public:
			Flash(){
				initGPIO();
				initSPI();
				Init_HOLD_WP();
			}
			~Flash(){

			}
            void ReadID(uint8_t *MID, uint16_t *DID);
			void ReadData(uint32_t address,uint8_t *pBuffer,uint16_t length);
			void PageProgram(uint32_t Address,uint8_t *DataArray,uint16_t Count);
            void EraseChip(void);
            void SectorErase(uint32_t Address);
    };

}
#endif // FLASH_H

