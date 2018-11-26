#ifndef SCREEN_H
#define SCREEN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_gpio.h"
#include <algorithm>
#include <string.h>
#include "help.h"

/* Defines -------------------------------------------------------------------*/
#define Seg_E_Pin GPIO_PIN_1
#define Seg_E_GPIO_Port GPIOA
#define Seg_D_Pin GPIO_PIN_3
#define Seg_D_GPIO_Port GPIOA
#define Seg_A_Pin GPIO_PIN_4
#define Seg_A_GPIO_Port GPIOA
#define Seg_Dp_Pin GPIO_PIN_5
#define Seg_Dp_GPIO_Port GPIOA
#define Seg_F_Pin GPIO_PIN_6
#define Seg_F_GPIO_Port GPIOA
#define Seg_C_Pin GPIO_PIN_7
#define Seg_C_GPIO_Port GPIOA
#define Dig_3_Pin GPIO_PIN_0
#define Dig_3_GPIO_Port GPIOB
#define Dig_1_Pin GPIO_PIN_1
#define Dig_1_GPIO_Port GPIOB
#define Seg_G_Pin GPIO_PIN_2
#define Seg_G_GPIO_Port GPIOB
#define Seg_B_Pin GPIO_PIN_10
#define Seg_B_GPIO_Port GPIOB
#define Dig_2_Pin GPIO_PIN_11
#define Dig_2_GPIO_Port GPIOB
#define Dig_4_Pin GPIO_PIN_9
#define Dig_4_GPIO_Port GPIOB

#define SCR_PERIOD 10
#define SCR_PAUSE_TIME 5


/* ===============================================================================
                       ##### Segment #####
 ===============================================================================  */
class Segment : public pin_data{

	public:
		Segment(GPIO_TypeDef* port, const uint16_t pin):pin_data(port,pin){

							this->BSRR_Off();
		}
		void BSRR_On(){HAL_GPIO_WritePin(PORT,PIN,GPIO_PIN_RESET);}
		void BSRR_Off(){HAL_GPIO_WritePin(PORT,PIN,GPIO_PIN_SET);}



};

/* ===============================================================================
                       ##### Digit #####
 ===============================================================================  */

class Digit : public pin_data{
	public:
		Digit(GPIO_TypeDef* port, const uint16_t pin):pin_data(port,pin){
						
							this->BSRR_Off();}
		void BSRR_On(){HAL_GPIO_WritePin(PORT,PIN,GPIO_PIN_RESET);};
		void BSRR_Off(){HAL_GPIO_WritePin(PORT,PIN,GPIO_PIN_SET);};
		

};





/* ===============================================================================
                       ##### Screen #####
 ===============================================================================  */

class Screen{
		
 public:
	friend void Screen_out();
  friend  Screen& operator<<(Screen&,const uint8_t&);
	friend  Screen& operator<<(Screen&,const uint16_t&);
  friend  Screen& operator<<(Screen&,const uint32_t&);
	friend  Screen& operator<<(Screen&,const char*);
	friend  Screen& operator<<(Screen&,const float&);
	friend  void clear_buf(Screen& sc);
 
	public:	
		explicit Screen ():digits_period(SCR_PERIOD),br(SCR_PAUSE_TIME){};
		Screen(const Screen&) = delete;
	private:	
		void delete_p(char *pos);
		uint8_t& get_br() {return br;}
		void clear_screen(){clear_buf(*this);}
	

	public:
		char buffer[5];//array of char which will be displayed
	private:
		uint8_t digits_period;//period for digit
		uint8_t br;
		uint8_t symbols;
};



/* ===============================================================================
                       ##### Exported functions #####
 ===============================================================================  */

void Screen_out();
void bright_change(const uint8_t&);
/*operator overloading*/
Screen& operator<<(Screen&,const uint8_t&);
Screen& operator<<(Screen&,const uint16_t&);
Screen& operator<<(Screen&,const uint32_t&);
Screen& operator<<(Screen&,const char*);
Screen& operator<<(Screen&,const float&);
Screen& operator<< (Screen& sc,tm time);



	 
	

#endif
