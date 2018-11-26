#ifndef HELP_H
#define HELP_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_gpio.h"
#include "tsl_globals.h"
#include <time.h>
/* ===============================================================================
                       ##### MIN_MAX #####
 ===============================================================================  */

/*template <typename T> class MIN_MAX
{
	public:
		MIN_MAX(T min, T max):min_val(min),max_val(max){}
		T operator()(const T& num)
		{
			if (num>= min_val && num<=max_val)
				return num;
			else if (num > max_val)
				return max_val;
			else 
				return min_val;		
		}
	private:
		T min_val;
		T max_val;
};*/

/* ===============================================================================
                       ##### Pin Data #####
 ===============================================================================  */
class pin_data
{
	public:
		pin_data (GPIO_TypeDef* port, uint16_t pin):PORT(port),PIN(pin){}
	protected:
		GPIO_TypeDef* PORT;
		uint16_t PIN;
};


/* ===============================================================================
                       ##### Exported Fucntions #####
 ===============================================================================  */

bool time_elapsed_uint(const uint16_t delay,const tm& tick,const uint16_t _last_tick_sec,const uint16_t _last_tick_min);
bool time_elapsed(const uint16_t delay,const uint8_t tick,const uint8_t last_tick);
bool time_elapsed_tm(const uint16_t delay,const tm& tick,const tm& last_tick);
#endif

