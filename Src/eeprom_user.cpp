#include "eeprom_user.h"
#include "main_state_machine.h"
#include "alarms_events.h"
#include "main.h"


/* Extern variables ---------------------------------------------------------*/
extern Voltage_Relay_Executor Main_State_Machine;
extern EVENT Write_Error;
extern Event Some_Event; //pushed to main_state_machine
/* Private variables ---------------------------------------------------------*/
Subject* Flash_Data::subj = new Subject;
/* Global variables ---------------------------------------------------------*/
EEPROM_WRT EEPROM_EVENT_WR = &EEprom_Write_Data; //pointer to the write function. Used to be pushed in to State_Machine from  Menu_flash_rw::change_data()


/*Main data array. Related to FLASH_VAR_NAME*/
Flash_Data EEPROM_DATA[NB_OF_VAR] = 
{
	/*data,upper_limit,lower_limit,adress */
	Flash_Data(235,249,230,0x5550), //upper limit #1
	Flash_Data(245,260,250,0x5551), //upper limit #2
	Flash_Data(250,215,201,0x5552), //lower limit #1 VOLTAGE_TRH_L_1
	Flash_Data(195,200,185,0x5553), //lower limit #2
	Flash_Data(10,900,30,0x5554), //time #1
	Flash_Data(20,600,1,0x5555), //time #2
	Flash_Data(1,0x5556) //default data write flag
};




/* ===============================================================================
                       ##### flash_data member functions #####
 ===============================================================================  */

/**
  * @brief stores value from eeprom in Flash_data object
  * @note  if eeprom read fails the value sets to USHRT_MAX
  * @retval none
  */
void Flash_Data::find_data()
{
	if (!EE_ReadVariable(Adress,&eeprom_value))
		return;
	else
		eeprom_value = USHRT_MAX;
}

/**
  * @brief returns value stored in eeprom
  * @note   
  * @retval eeprom value
  */
uint16_t Flash_Data::get_value() const
{
	return eeprom_value;
};

/**
  * @brief Validation of data
  * @note   
  * @param data to be checked
  * @retval FLASH_COMPLETE on success
  */

FLASH_Status Flash_Data::check_data(uint16_t entry)
{
	if (entry > upper_limit || entry < lower_limit)
		return FLASH_ERROR_WR;
	else return FLASH_COMPLETE;
}

/**
  * @brief writes new data to eeporm
  * @note  writes and notifyes all observers
  * @param data to be written
  * @retval Flash_Comlete in success
  */
FLASH_Status Flash_Data::write_data(uint16_t data)
{
	FLASH_Status ret =(FLASH_Status) HAL_ERROR ;
	if (check_data(data) != HAL_OK)
		return FLASH_ERROR_WR;
	ret = (FLASH_Status)EE_WriteVariable(this->Adress,data);
	if (ret == HAL_OK)
	{
		find_data();
		subj->notify();
	}
	else eeprom_value = USHRT_MAX;
	return ret;
};

void Flash_Data::set_value(uint16_t entry)
{
	 eeprom_value = entry;
}


/* ===============================================================================
                       ##### Exported functions #####
 ===============================================================================  */

/**
  * @brief writes new data to eeprom. 
  * @note gets a pointer to array and writes data to eeprom
  * @note 
  * @retval true on success
  */
bool 	EE_Writes(Flash_Data* Data, uint16_t HowMuchToWrite)
{
	if(HowMuchToWrite >	_EEPROM_Posible_Var)
		return false;
	
	//check address validation
	if(HowMuchToWrite >	_EEPROM_Posible_Var)
		return false;
	/* Get the valid Page start Address */
	uint32_t Address = (uint32_t)(EEPROM_START_ADDRESS + (uint32_t)(PAGE0 * PAGE_SIZE))+4;
	// variables
	FLASH_Status Status;
	
	/* Start of writing data */
	HAL_FLASH_Unlock();
	for(uint8_t i=0 ; i< HowMuchToWrite ; ++i)
	{
		/* Verify if Address and Address+2 contents are 0xFFFFFFFF */
    if ((*(__IO uint32_t*)Address) == 0xFFFFFFFF)
		{
			Status = (FLASH_Status)HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,Address,Data[i].get_default_data());
			if (Status != FLASH_COMPLETE)
      {
				HAL_FLASH_Lock();
        return false;
      }
			
			Data[i].set_value(*( uint16_t*)Address);
			/* Set variable virtual address */
      Status = (FLASH_Status)HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,Address + 2, Data[i].get_addr());
			if (Status != FLASH_COMPLETE)
      {
				HAL_FLASH_Lock();
        return false;
      }
			//go to next addr
      Address = Address + 4;		
		}
	}
	HAL_FLASH_Lock();
	return true;
}


/**
  * @brief updates eeprom_value in Flash_Data
  * @note used after page trancfer process
  * @note  after update process it notifyes all observers
  * @retval None
  */
void Renew_EEpromData(Flash_Data* entry)
{
	for (uint16_t i = 0; i < NB_OF_VAR;++i)
		entry[i].find_data();	
	Flash_Data::subj->notify();
}


/**
  * @brief writes new data to eeprom. 
  * @note one element per iteration
  * @note push this fucntion to main_state_machine
  * @retval None
  */
void EEprom_Write_Data(Flash_Data* entry,uint16_t new_val)
{
	if (entry->write_data(new_val) != HAL_OK)
	{
		Main_State_Machine.pop();
		Event err(Write_Error);
		Some_Event = err;
		Main_State_Machine.push(&Some_Event);
	}
	else 
		Main_State_Machine.pop();
}

/**
  * @brief writes default data to eeprom
  * @note   this function manages first inclusion and writes default_data to eeprom.
	* @note  if default data write_flag is 1 default data was written and we should renew eeprom_data in Flash_Data
	* @note  this function should be used after EE_Init() in main()
  * @param pointer to eeprom_data array
  * @retval none
  */
void Write_Default_Data(Flash_Data* arr)
{
	Renew_EEpromData(arr);
	if (arr[NB_OF_VAR-1].get_value() == 1)
		return;
	else 
	{
		if (!EE_Writes(arr,NB_OF_VAR))
			_Error_Handler((const char*)"Ferr", 182);
		else return;
	}
}



