#ifndef BUZZER_HPP
#define BUZZER_HPP

#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_gpio.h"
#include "help.h"

#define Piezo_Pin GPIO_PIN_8
#define Piezo_GPIO_Port GPIOA

class buzzer : public pin_data
{
	public:
		buzzer (GPIO_TypeDef* port, const uint16_t pin):pin_data(port,pin){this->BSRR_Off();}
		
		void BSRR_Off(){HAL_GPIO_WritePin(PORT,PIN,GPIO_PIN_RESET);}
		void BSRR_On(){HAL_GPIO_WritePin(PORT,PIN,GPIO_PIN_SET);}
		void click();
		void l_sound();
		void s_sound();
	private:
		void beep();
};

#endif 
