#ifndef KEYBOARD_H
#define KEYBOARD_H

/* Includes ------------------------------------------------------------------*/
#include "tsl_user.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_gpio.h"
#include "help.h"
	
/* Enums ------------------------------------------------------------------*/
typedef enum
{
	OK = 0,
	ESC = 1,
	UP,
	DOWN,
	LEFT
}SENSOR_NAME;

typedef enum
{
	RELEASE = 0,
	PRESSED = 1,
	CLICK ,
	CLICKORPRESS
}SENSOR_STATE;

/* ===============================================================================
                       ##### Keyboard #####
 ===============================================================================  */
class keyboard
{

	public:
		bool any_key_pressed();
		bool get_state(SENSOR_NAME,SENSOR_STATE);
	private:
		volatile SENSOR_STATE sensors_state[TSLPRM_TOTAL_TKEYS];
		uint8_t last_tick_sec;
		TSL_tTick_ms_T last_tick_ms;
};

#endif //keyboard

