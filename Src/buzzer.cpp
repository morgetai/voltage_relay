#include "buzzer.h"
#include "Sensor.hpp"
#include "boxing_timer.h"



buzzer buz(Piezo_GPIO_Port,Piezo_Pin);

void buzzer::click()
{	
	static uint16_t click_cnt = 0;
	BSRR_On(); // устанавливаем вывод 8 порта A в логическую единицу.
	click_cnt++;	
	if(click_cnt == 5)
		BSRR_Off(); // сбрасываем вывод 8 порта A в логический ноль
	else if(click_cnt >= 100)
		click_cnt = 0;	
};

void buzzer::l_sound()
{
	static uint16_t beep_cnt1 = 0;
	if(beep_cnt1 < 10000)
		{
			beep();
			beep_cnt1++;
		}
			else if(beep_cnt1 >= 10000)
				{
					beep_cnt1 = 0;
					BSRR_Off();             // сбрасываем вывод 8 порта A в логический ноль
				}
};


void buzzer::s_sound()
{
	static uint16_t beep_cnt2 = 0;
					beep_cnt2++;		
				if(beep_cnt2 < 300)
					beep();
				else if(beep_cnt2 >= 300 && beep_cnt2 < 1000)
					BSRR_Off();                              // сбрасываем вывод 8 порта A в логический ноль
				
				else if(beep_cnt2 >= 1000&& beep_cnt2 < 1300)
					beep();
				else if(beep_cnt2 >= 1300 && beep_cnt2 < 2000)
					BSRR_Off();                              // сбрасываем вывод 8 порта A в логический ноль
				
				else if(beep_cnt2 >= 2000&& beep_cnt2 < 2300)
					beep();
				else if(beep_cnt2 >= 2300 && beep_cnt2 < 3000)
					BSRR_Off();                              // сбрасываем вывод 8 порта A в логический ноль				
				
				else if(beep_cnt2 >= 4000)
				{
					beep_cnt2 = 0;
				}
};


void buzzer::beep()
{
	static uint16_t beep_cnt = 0;
	beep_cnt++;
	if(beep_cnt == 1)
			BSRR_On(); 															// устанавливаем вывод 8 порта A в логическую единицу.
	else if(beep_cnt >= 9 && beep_cnt < 11)
			BSRR_Off(); 														// сбрасываем вывод 8 порта A в логический ноль
	else if(beep_cnt >= 11)
		beep_cnt = 0;		
}


