#include "keyboard.h"
#include "tsl_globals.h"
#include "tsl_time.h"
/* Extern variables ---------------------------------------------------------*/
extern TSL_TouchKeyData_T MyTKeys_Data[TSLPRM_TOTAL_TKEYS];
extern TSL_Globals_T TSL_Globals;
/* Global variables ---------------------------------------------------------*/
keyboard Keyboard;

/* ===============================================================================
                       ##### Keyboard member functions #####
 ===============================================================================  */
 
 /**
   * @brief This functions is used to check the state of chosen sensor
   * @note   basically used in menu 
   * @param sensor name enum
   * @param  sensor state enum
   * @retval true then chosen sensor is in chosen state
   */
bool keyboard::get_state(SENSOR_NAME name,SENSOR_STATE s_state)
{
	bool ret = false;
	switch(s_state)
	{
		case RELEASE:
				
			if(MyTKeys_Data[name].StateId != TSL_STATEID_DETECT) {
				this->sensors_state[name] = RELEASE;
				ret = true;
			}
			break;
			
			case PRESSED:
			if (MyTKeys_Data[name].StateId == TSL_STATEID_DETECT || MyTKeys_Data[name].StateId==TSL_STATEID_DEB_DETECT){
			PR:	if (TSL_tim_CheckDelay_ms(100,&last_tick_ms) == TSL_STATUS_OK )
					ret = true;
			}
			else if (MyTKeys_Data[name].StateId == TSL_STATEID_RELEASE && this->sensors_state[name] == PRESSED)
				{
					this->sensors_state[name] = RELEASE;
					ret = false;
				}
			break;
			
		case CLICK:
			if (MyTKeys_Data[name].StateId == TSL_STATEID_DETECT || MyTKeys_Data[name].StateId==TSL_STATEID_DEB_DETECT){
			CL:	this->sensors_state[name] = CLICK;
				ret = false;
			}
				else if (MyTKeys_Data[name].StateId == TSL_STATEID_RELEASE && this->sensors_state[name] == CLICK)
				{
					this->sensors_state[name] = RELEASE;
					ret = true;
				}
				break;	
				
		case CLICKORPRESS:
			if (MyTKeys_Data[name].StateId == TSL_STATEID_DETECT || MyTKeys_Data[name].StateId==TSL_STATEID_DEB_DETECT)
			{
				if (this->sensors_state[name] == PRESSED)
					goto PR;
				if (TSL_tim_CheckDelay_ms(600,&last_tick_ms) == TSL_STATUS_OK && this->sensors_state[name] != RELEASE)
					this->sensors_state[name] = PRESSED;		
				else if (this->sensors_state[name] == RELEASE)
					goto CL;
			}
				else if (MyTKeys_Data[name].StateId == TSL_STATEID_RELEASE)
				{
					if (this->sensors_state[name] == PRESSED)
					{
						ret = false;
						this->sensors_state[name] = RELEASE;
					}
					else if (this->sensors_state[name] == CLICK)
					{
						ret = true;	
						this->sensors_state[name] = RELEASE;
					}
				}				
				break;
			}
   return ret;
}

bool keyboard::any_key_pressed()
{
	for (uint16_t i = 0; i < TSLPRM_TOTAL_TKEYS;++i)
	{
		if (sensors_state[i] != RELEASE)
			return true;
	}
		return false;
}

