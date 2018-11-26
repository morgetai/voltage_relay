#ifndef EEPROM_USER_H
#define EEPROM_USER_H



/* Includes ------------------------------------------------------------------*/
#include "eeprom.h" 
#include "observer.h"
#include <limits.h>
/* Defines -------------------------------------------------------------------*/
#define	_EEPROM_Posible_Var	(PAGE_SIZE-2/4)
#define ERROR_VAL NULL

 
enum FLASH_VAR_NAME
{
	VOLTAGE_TRH_H_1 = 0,
	VOLTAGE_TRH_H_2 = 1,
	VOLTAGE_TRH_L_1 = 2,
	VOLTAGE_TRH_L_2,
	VOLTAGE_TIME_TRH_HL_1,
	VOLTAGE_TIME_TRH_HL_2
};




/* ===============================================================================
                       ##### wrapper for data saved in eeprom #####
 ===============================================================================  */
class voltage_relay;


class Flash_Data
{	
	public:
		friend class voltage_relay;
		friend void Renew_EEpromData(Flash_Data*);
		friend void EEprom_Write_Data(Flash_Data*,uint16_t);
		friend bool EE_Writes(Flash_Data* Data, uint16_t HowMuchToWrite);
	
	/*constructor*/
	public:
		Flash_Data(uint16_t d_data,uint16_t addr):Deault_Data(d_data),upper_limit(USHRT_MAX),lower_limit(NULL),Adress(addr){}
		Flash_Data(uint16_t d_data,uint16_t up_lim,uint16_t low_lim,uint16_t addr):Deault_Data(d_data),upper_limit(up_lim),lower_limit(low_lim),Adress(addr){}
			
	/*meber functions*/
	public:
		uint16_t get_default_data() const {return Deault_Data;};
		uint16_t get_value() const;
		
	/*realisation*/
	private:
		const uint16_t Deault_Data;
		const uint16_t upper_limit;
		const uint16_t lower_limit;
		const uint16_t Adress;
		uint16_t eeprom_value;
		
	/*utility functions*/
	private:
		FLASH_Status check_data(uint16_t);
		FLASH_Status write_data(uint16_t data);
		void find_data();
		void set_value(uint16_t); 
		uint16_t get_addr () const {return Adress;};
		
	/*subject for observer pattern implementation*/
	private:
		static Subject* subj;
};





/* ===============================================================================
                       ##### Exported functions #####
 ===============================================================================  */

/* Private typedefs ----------------------------------------------------------*/
typedef void (*EEPROM_WRT)(Flash_Data*,uint16_t);

/* Exported functions ------------------------------------------------------- */

void EEprom_Write_Data(Flash_Data*,uint16_t);

bool EE_Writes(Flash_Data* Data, uint16_t HowMuchToWrite);

void Write_Default_Data(Flash_Data*);

void Renew_EEpromData(Flash_Data*);

#endif // EEPROM_USER_H

