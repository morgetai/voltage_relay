/* Includes ------------------------------------------------------------------*/
#include "help.h"


/* Global variables ---------------------------------------------------------*/
tm Global_Tick;

/* ===============================================================================
                       ##### Exported Fucntions #####
 ===============================================================================  */
/**
  * @brief checking if time has elapsed
  * @note   this function works with seconds. associated with TSL_Globals.Tick_sec
	* @note  which is counting up to 64 seconds.
  * @param delay is a time which determines elapced time period
  * @param 	current second
	* @param  second from which count begins
  * @retval returns true if time has elapsed
  */
bool time_elapsed(const uint16_t delay,const uint8_t tick,const uint8_t last_tick)
{
	//disableInterrupts();
	uint16_t diff;
	if (tick >= last_tick)
		diff = tick - last_tick;
	else 
		diff = (0x003C-last_tick)+tick+1;
	if (diff >= delay)
		{
			//enableInterrupts();
			return true;
		}
	//enableInterrupts();
	return false;	
}

/**
    * @brief checking if time has elapsed
    * @note  this function works with struct tm 
    * @param delay time in seconds
    * @param  current time
    * @param time from which count begins
    * @retval returns true if time has elapsed
    */	
bool time_elapsed_tm(const uint16_t delay,const tm& tick,const tm& last_tick)
{
	uint16_t diff;
	if (tick.tm_sec >= last_tick.tm_sec && tick.tm_min >= last_tick.tm_min)
	{
		diff = tick.tm_sec - last_tick.tm_sec;
		diff += (tick.tm_min - last_tick.tm_min)*60;	
	}
	else 
	{
		/*adding seconds*/
		diff = (0x003C - last_tick.tm_sec) + tick.tm_sec+1;
		/*addinf minutes turned to seconds*/
		diff +=  ((0x003C - last_tick.tm_min)+tick.tm_min)*60;
	}
	
	if (diff >= delay)
		return true;
	else return false;
}

bool time_elapsed_uint(const uint16_t delay,const tm& tick,const uint16_t _last_tick_sec,const uint16_t _last_tick_min)
{
	uint16_t diff;
	if (tick.tm_sec >= _last_tick_sec && tick.tm_min >= _last_tick_min)
	{
		diff = tick.tm_sec - _last_tick_sec;
		diff += (tick.tm_min - _last_tick_min)*60;	
	}
	else 
	{
		/*adding seconds*/
		diff = (0x003C - _last_tick_sec) + tick.tm_sec+1;
		/*addinf minutes turned to seconds*/
		diff +=  ((0x003C - _last_tick_min)+tick.tm_min)*60;
	}
	
	if (diff >= delay)
		return true;
	else return false;
}


