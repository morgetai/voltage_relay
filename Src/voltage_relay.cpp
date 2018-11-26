
/* Includes ------------------------------------------------------------------*/
#include "voltage_relay.h"
#include "alarms_events.h"
#include "main.h"


/* Extern variables ---------------------------------------------------------*/
extern Voltage_Relay_Executor Main_State_Machine;
extern Flash_Data EEPROM_DATA[NB_OF_VAR];
extern Alarm Voltage_Relay_Alarm;
extern struct tm Global_Tick;
extern TIM_HandleTypeDef htim1;
/* Private functions--------------------------------------------------------*/
static void Alarm_Handler();
static void Alarm_Reset();
static void Voltage_Calculation();


/* Global variables ---------------------------------------------------------*/
VPTR vptr = &voltage_relay::execute; //poiner to the execute function voltage_relay::execute. push it in main_state_machine
voltage_relay VOLT_RELAY(EEPROM_DATA);//volage relay object. 
ALARMPTR AL_HANDLER_PTR = &Alarm_Handler;//pointer to the function. Used in voltage_relay::generate_alarm()
ALARMPTR AL_RESET_PTR = &Alarm_Reset; //pointer to the function. Used in voltage_relay::reset_alarm()
EVENT VOLT_CALC = &Voltage_Calculation; //voltage calculation event
/* Private variables ---------------------------------------------------------*/
relay Relay(Relay_GPIO_Port,Relay_Pin);/*RELAY object*/
Voltage_Relay_Executor* voltage_relay::executor_ptr = &Main_State_Machine;
uint16_t voltage_relay::relay_state = TURNED_ON;
/* ===============================================================================
                       ##### ALARM_HANDLER #####
 ===============================================================================  */
void Alarm_Handler()
{
	Relay.off();
	Main_State_Machine.pop();
}


/* ===============================================================================
                       ##### ALARM_RESET #####
 ===============================================================================  */
void Alarm_Reset()
{
	Relay.on();
	Main_State_Machine.pop();
}

/******************************************************************************/
/*        							  		Voltage RMS calculation									        */ 
/******************************************************************************/


void Voltage_Calculation()
{
	VOLT_RELAY.calculate();
	Main_State_Machine.pop();
	HAL_TIM_Base_Start_IT(&htim1);
}



/* ===============================================================================
                       ##### voltage_relay public functions #####
 ===============================================================================  */


/*related to vptr. push vptr to main_state_machine*/
void voltage_relay::execute()
{
	status_detection();
}

//updates measured voltage. use it in ADC interrupt
void voltage_relay::calculate()
{
	volt_acq->calculate();
}

/*returns filtered voltage*/
uint16_t voltage_relay::get_voltage()
{
	return volt_acq->get_voltage();
}

uint16_t voltage_relay::get_voltage_to_screen()
{
	return volt_acq->get_screen_voltage();
}

/*returns mesurments array pointer*/	
uint32_t* voltage_relay::get_arr_ptr()
{
	return volt_acq->get_ptr();
}

/* ===============================================================================
                       ##### voltage_relay member private functions #####
 ===============================================================================  */

/*status detection*/
void voltage_relay::status_detection()
{
	uint16_t voltage = volt_acq->get_voltage();
	
	switch(relay_state)
	{
		case TURNED_ON:
		{
			if (voltage >= data[VOLTAGE_TRH_H_1] || voltage <= data[VOLTAGE_TRH_L_1])
			{			
				relay_state = EVENT_1;				
				last_tick_sec =  (Global_Tick.tm_sec);
				last_tick_min =  (Global_Tick.tm_min);
			}
			else if (voltage >= data[VOLTAGE_TRH_H_2] || voltage <= data[VOLTAGE_TRH_L_2])
			{
				relay_state = EVENT_2;
				last_tick_sec =  (Global_Tick.tm_sec);
				last_tick_min =  (Global_Tick.tm_min);
			}
			break;		
		}
		/*EVENT_1  level_1 tresholds*/
		E1: case EVENT_1:
		{
			/*checking for EVENT_2 treshold*/
			if (voltage >= data[VOLTAGE_TRH_H_2] || voltage <= data[VOLTAGE_TRH_L_2])
			{
				relay_state = EVENT_2;
				goto E2; //go to EVENT_2
			}
			/*cheking for EVENT_1 tresholds*/
			else if (voltage >= data[VOLTAGE_TRH_H_1] || voltage <= data[VOLTAGE_TRH_L_1])
			{
				if (time_elapsed_uint(data[VOLTAGE_TIME_TRH_HL_1],Global_Tick,last_tick_sec,last_tick_min))
				{
					relay_state = ALARM;
					goto A;
				}
				else return;
			}
			
			/*return to OK*/
			else relay_state = TURNED_ON;
			
			break;
		}
			/*EVENT_2 level_2 tresholds*/
		E2:	case EVENT_2:
		{
			/*cheking for EVENT_2 tresholds*/
			if (voltage >= data[VOLTAGE_TRH_H_2] || voltage <= data[VOLTAGE_TRH_L_2])
			{
				if (time_elapsed_uint(data[VOLTAGE_TIME_TRH_HL_2],Global_Tick,last_tick_sec,last_tick_min))
				{
					relay_state = ALARM;
					goto A;
				}
				else return;
			}
			
			/*cheking for EVENT_1 tresholds*/
			else if  (voltage >= data[VOLTAGE_TRH_H_1] || voltage <= data[VOLTAGE_TRH_L_1])
			{
				relay_state = EVENT_1;
				goto E1;//go to EVENT_1
			}
			
			/*return to  OK*/
			else relay_state = TURNED_ON;			
			break;

		}
			/*in ALARM state relay is turned off and state switches to WAITING_FOR_RESET_ALARM*/
		A: case ALARM:
		{
			
			last_tick_sec =  (Global_Tick.tm_sec);
				last_tick_min =  (Global_Tick.tm_min);

			relay_state = WAITING_FOR_RESET_ALARM;
			generate_alarm();
			break;
		}
			/*in this state we are waiting for delay to elapse and for voltage to go under VOLTAGE_TRH_1*/
		case WAITING_FOR_RESET_ALARM:
		{
			if (voltage < data[VOLTAGE_TRH_H_1] && voltage > data[VOLTAGE_TRH_L_1])
			{
				if (time_elapsed_uint(data[VOLTAGE_TIME_TRH_HL_2],Global_Tick,last_tick_sec,last_tick_min))
				{
					relay_state = TURNED_ON;
					reset_alarm();
					return;
				}		
			}
			else 
			{
				last_tick_sec =  (Global_Tick.tm_sec);
				last_tick_min =  (Global_Tick.tm_min);
				return;		
			}	
		}
	}
}

	

/* ===============================================================================
                       ##### utility private functions #####
 ===============================================================================  */



//genetates alarm and push it into main_state_machine
void voltage_relay::generate_alarm()
{
	Voltage_Relay_Alarm.set_task(AL_HANDLER_PTR);
	executor_ptr->push(&Voltage_Relay_Alarm);
}

//genetates alarm reset and push it into main_state_machine
void voltage_relay::reset_alarm() 
{
	Voltage_Relay_Alarm.set_task(AL_RESET_PTR);
	executor_ptr->push(&Voltage_Relay_Alarm);
}

/* ===============================================================================
             ##### Observer pattern part #####
 ===============================================================================  */

//updates values if ee_prom write function was used
void voltage_relay::update()
{
	for (uint8_t i =0; i <= EEprom_data_qty ;++i)
		data[i] = data_eeprom_ptr[i].get_value();
}


