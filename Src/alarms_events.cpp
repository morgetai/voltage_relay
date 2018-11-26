#include "alarms_events.h"

/* Extern variables ---------------------------------------------------------*/
extern VPTR vptr;
extern voltage_relay VOLT_RELAY;
extern VPTR volt_calculation;
extern EVENT VOLT_CALC; //voltage calculation event
/* Global variables ---------------------------------------------------------*/
Event_Check<voltage_relay,VPTR> Volt_relay_Event_Check(VOLT_RELAY,vptr); //push this function to main state machine in main()
Event Volt_Calculation(VOLT_CALC); // voltage calculation process function. pushed to state_machine in DMA interrupt
Alarm Voltage_Relay_Alarm;
Event_EEprom_Write event_write; //push it in case of eeprom write demand
Event Some_Event; //pushed to main_state_machine
/* ===============================================================================
                       ##### Common Event #####
 ===============================================================================  */

void Event::execute_task()
{
	event_handler();
}

Event& Event::operator= (const Event& entry)
{
	event_handler = entry.event_handler;
	return *this;
}

/* ===============================================================================
                       ##### EEprom write Event #####
 ===============================================================================  */
void Event_EEprom_Write::execute_task()
{
	event_handler(eeprom_data,new_data);
}


Event_EEprom_Write& Event_EEprom_Write::operator=(const Event_EEprom_Write& entry)
{
	event_handler = entry.event_handler;
	eeprom_data = entry.eeprom_data;
	new_data = entry.new_data;
	return *this;
}

void Event_EEprom_Write::set_task(EEPROM_WRT& event,Flash_Data* fdata,uint16_t new_val)
{
	event_handler = event;
	eeprom_data = fdata;
	new_data = new_val;
}



/* ===============================================================================
                       ##### Alarm event #####
 ===============================================================================  */
void Alarm::execute_task()
{
	alarm_handler();
}


void Alarm::set_task(ALARMPTR& ptr)
{
	alarm_handler = ptr;
}

