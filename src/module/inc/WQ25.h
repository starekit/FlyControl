#ifndef WQ25_H
#define WQ25_H
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
namespace module{
	#define WQ25_HOLD_WP GPIOB 
	#define WQ25_HOLD_PIN GPIO_Pin_1
	#define WQ25_WP_PIN GPIO_Pin_0

	class WQ25{
		private:
			#define WQ25_CS_LOW() GPIO_ResetBits(GPIOA,GPIO_Pin_4)//CS引脚低电平
			#define WQ25_CS_HIGH() GPIO_SetBits(GPIOA,GPIO_Pin_4)//CS引脚高电平
			
			void initGPIO();
			void initSPI();
			void Init_HOLD_WP();

			uint8_t SPI_ReadWriteByte(uint8_t byte);
			void erasureData();
			void WQ25_WriteEnable();//写使能
			void WQ25_WaitForWriteEnd();//等待写入结束
		
		public:
			WQ25(){
				initGPIO();
				initSPI();
				Init_HOLD_WP();
			}
			void WQ25_EraseChip(void);
			void WQ25_ReadData(uint32_t address,uint8_t *pBuffer,uint16_t length);
			void WQ25_WritePage(uint32_t address,uint8_t *pBuffer,uint16_t length);
	};

}
#endif // WQ25_H

