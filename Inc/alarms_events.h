#ifndef ALARMS_EVENTS_H
#define ALARMS_EVENTS_H


/* Includes ------------------------------------------------------------------*/
#include "main_state_machine.h"
#include "voltage_relay.h"
#include "eeprom_user.h"



/* Private typedefs ----------------------------------------------------------*/
typedef void (*ALARMPTR)(void);
typedef void (*EVENT)(void);



/* ===============================================================================
                       ##### Common Event #####
 ===============================================================================  */
class Event:public OPERATION_BASE
{
	public:
		Event():event_handler(NULL){}
		Event(EVENT ptr):event_handler(ptr){}
	public:
		Event& operator=(const Event&);
	public:
		virtual void execute_task();	
	private:
		EVENT event_handler;
};

/* ===============================================================================
                       ##### EEprom write Event #####
 ===============================================================================  */

class Event_EEprom_Write:public OPERATION_BASE
{
	public:
		Event_EEprom_Write():event_handler(NULL),eeprom_data(NULL),new_data(NULL){}
		Event_EEprom_Write(EEPROM_WRT ptr,Flash_Data* f_ptr,uint16_t val):event_handler(ptr),eeprom_data(f_ptr),new_data(val){}
	public:
		Event_EEprom_Write& operator=(const Event_EEprom_Write&);
	public:
		void set_task(EEPROM_WRT&,Flash_Data*,uint16_t);
	public:
		virtual void execute_task();		
		virtual ~Event_EEprom_Write() = default;
	private:
		EEPROM_WRT event_handler;
		Flash_Data* eeprom_data;
		uint16_t new_data;
};


/* ===============================================================================
                       ##### Event for member function #####
 ===============================================================================  */

template <typename Obj_type,typename FPTR>class Event_Check:public OPERATION_BASE
{
	public:
		Event_Check (Obj_type& obj,FPTR ptr):Obj(&obj),fptr(ptr){};
		virtual void execute_task(){(Obj->*fptr)();};
		virtual ~Event_Check() = default;
	private:
		Obj_type* Obj;
		FPTR fptr;
};

/* ===============================================================================
                       ##### Alarm event #####
 ===============================================================================  */

class Alarm:public OPERATION_BASE
{
	public:
		Alarm():alarm_handler(NULL){}
		Alarm (ALARMPTR ptr):alarm_handler(ptr){}
		Alarm& operator=(const Alarm& entry){alarm_handler = entry.alarm_handler;return *this;}
		
	public:
		void set_task(ALARMPTR&);
	public:
		virtual void execute_task();
		virtual ~Alarm() = default;
	private:
		ALARMPTR alarm_handler;

};




#endif



