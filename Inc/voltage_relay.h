#ifndef VOLTAGE_RELAY_H
#define VOLTAGE_RELAY_H





/* Includes ------------------------------------------------------------------*/
#include "voltage_acq.h"
#include "eeprom_user.h"
#include "tsl_globals.h"
#include "help.h"
#include "stm32f0xx_hal_gpio.h"
#include "observer.h"
#include "main_state_machine.h"

/* Defines -------------------------------------------------------------------*/
#define Relay_Pin GPIO_PIN_8
#define Relay_GPIO_Port GPIOB
#define EEprom_data_qty 6 //used in update() member function


/* ===============================================================================
                       ##### voltage_relay #####
 ===============================================================================  */
 

typedef enum {TURNED_ON = 10,EVENT_1 = 11,EVENT_2 = 12,ALARM  = 13,WAITING_FOR_RESET_ALARM = 14}RELAY_STATE;
 
 
 
class voltage_relay:public Observer
{
	public:
	/* constructor*/
	voltage_relay(Flash_Data* ptr):data_eeprom_ptr(ptr)
	{
		volt_acq = new voltage_acq(MESURES_QTY);
		for (uint8_t i =0; i <= EEprom_data_qty ;++i)
			data[i] = data_eeprom_ptr[i].get_value();	//copying data from eeprom
		Flash_Data::subj->attach(this);
		/*last_tick_sec = new uint16_t;
		last_tick_min = new uint16_t;*/
	}
	//destructor
	~voltage_relay(){delete volt_acq;}
	
	
	public:
		void calculate();
		void execute();
		uint16_t get_voltage();
		uint16_t get_voltage_to_screen();
		uint32_t* get_arr_ptr();
	//update data taken from subject i.e. flash_data
	protected:
		virtual void update();

		
	//realisation
	private:
		voltage_acq* volt_acq; 
		Flash_Data* data_eeprom_ptr; //ptr to EEPROM_DATA
		static Voltage_Relay_Executor* executor_ptr; //ptr to main_state_machine
		uint16_t data[EEprom_data_qty]; //related to FLAH_VAR_NAME enum
		static uint16_t relay_state; // states
		//static struct tm last_tick_relay;//used to hold the time of first entry
	uint16_t never_use;
	uint16_t last_tick_sec;
	uint16_t last_tick_min;
		
	//utulity functions
	private:
		void status_detection();
		static void generate_alarm() ;
		static void reset_alarm() ;
};


/* ===============================================================================
                       ##### Relay #####
 ===============================================================================  */

class relay:public pin_data
{
	public:
		relay(GPIO_TypeDef* port,const uint16_t pin):pin_data(port,pin){}
		void off(){HAL_GPIO_WritePin(PORT,PIN,GPIO_PIN_RESET);};
		void on(){HAL_GPIO_WritePin(PORT,PIN,GPIO_PIN_SET);};
};

/* ===============================================================================
                       ##### typedefs #####
 ===============================================================================  */

typedef void(voltage_relay::*VPTR) ();




#endif


