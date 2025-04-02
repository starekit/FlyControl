
#include "stm32f10x.h"
#ifndef PWM_H
#define PWM_H

namespace module{
#define ARR 999 // 自动重装载值
#define PSC 71 // 预分频值
	class PWM{
		private:
			uint16_t compare1, compare2, compare3, compare4;
			void initPWM();

		public:
		PWM():compare1(0), compare2(0), compare3(0), compare4(0){
			initPWM();
		}
		~PWM(){
			// 析构函数
		}
		void setCompares(uint16_t compare1, uint16_t compare2, uint16_t compare3, uint16_t compare4);

	};
}//namespace module


#endif // !PWM_H