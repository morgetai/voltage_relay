#include "menu.h"
#include "alarms_events.h" 
#include <string.h>
#include <time.h>
#include "help.h"
/* Extern variables ---------------------------------------------------------*/
extern EEPROM_WRT EEPROM_EVENT_WR ; //poinetr to the EE_Write function
extern keyboard Keyboard;
extern Screen scr;
extern Voltage_Relay_Executor Main_State_Machine;
extern Flash_Data EEPROM_DATA[NB_OF_VAR];
extern voltage_relay VOLT_RELAY;	
extern Event_EEprom_Write event_write; //pushed to main_state_machine
/* Global variables ---------------------------------------------------------*/
EVENT Write_Error = &Call_Error_Menu;//pointer to the function.

/* -----------///////////----Private variables----///////////-------------------------*/
/*static members*/
Voltage_Relay_Executor* Menu_flash_rw::state_machine = &Main_State_Machine;
keyboard* Menu_Base::t_keys = &Keyboard;
/*-----------Menus utility objects----------------- */
extern Menu_item Parent; // Begins from this menu
Menu_Base* nullmenu(NULL);//nullmenu
Menu_Base* selected = &Parent;// pointer to executed menu
/*--------------------Menus----------------- */
//                  text   keyboard   menu_base
CONST Menu_flash_rw v_h_1("c1",EEPROM_DATA[VOLTAGE_TRH_H_1],&Parent);
CONST Menu_flash_rw v_h_2("c2",EEPROM_DATA[VOLTAGE_TRH_H_2],&Parent);
CONST Menu_flash_rw v_l_1("c3",EEPROM_DATA[VOLTAGE_TRH_L_1],&Parent);
CONST Menu_flash_rw v_l_2("c4",EEPROM_DATA[VOLTAGE_TRH_L_2],&Parent);
CONST Menu_Time t_hl_1("c5",EEPROM_DATA[VOLTAGE_TIME_TRH_HL_1],&Parent);
CONST Menu_Time t_hl_2("c6",EEPROM_DATA[VOLTAGE_TIME_TRH_HL_2],&Parent);
CONST Menu_Voltage_Relay Voltage(VOLT_RELAY,&Parent);

/*-------------------Main menu initialization---------------*/
Menu_item Parent("",//no name for this menu
7, // Quantity of menus
&Voltage,
&v_h_1,&v_h_2,&v_l_1,
&v_l_2, &t_hl_1,&t_hl_2);


/*----Error Menu----*/
Menu_Error Err_Menu("Err",&Parent);

/* Private functions prototype-----------------------------------------------------*/
static void back_to_main(Menu_item& menu);




/* ===============================================================================
                       ##### Tree-like menu item #####
 ===============================================================================  */

Menu_Base& Menu_item::action()
{
	if (t_keys->get_state(UP,CLICK))
	{
		if (ptr == ptr_b)
			ptr = ptr_b+size-1;
		else 
			--ptr;	
	}	
	else if (t_keys->get_state(DOWN,CLICK))
	{
		if (ptr == ptr_b+size-1)
			ptr = ptr_b;
		else
			++ptr;	
	}
	else if (t_keys->get_state(OK,CLICK))
		return **ptr;	
	
	else if (t_keys->get_state(ESC,CLICK))
		ptr = ptr_b;
	
	return *this;
}


void Menu_item::menu_scr()
{
	if (strncmp((*ptr)->text,"",1)== 0)
		(*ptr)->menu_scr();
	else 
		scr << (*ptr)->text;
}


void Menu_item::return_to_top()
{
	ptr = ptr_b;
}

/* ===============================================================================
                       ##### Menu item for reading data #####
 ===============================================================================  */



template <typename T> Menu_Base& Menu_data<T>::action()
{
	if (t_keys->get_state(ESC,CLICK))
		return *parent;
	else 
		return *this;
}


/* ===============================================================================
                       ##### Menu item for r/w data #####
 ===============================================================================  */

template <typename T> Menu_Base& Menu_ch_data<T>::action()
{
	if (t_keys->get_state(OK,CLICK))
	{
		switch (menu_state)
		{
			case NOACTION:
				menu_state = CH_ENABLE;
				change_data();
				break;
			case CH_ENABLE:
				menu_state = SAVE;
				change_data();
				break;
			default:
				;
		}
	}
	else if (t_keys->get_state(ESC,CLICK))
	{
		switch (menu_state)
		{
			case NOACTION:
				return *parent;
			case CH_ENABLE:
				menu_state = CANCEL;
				change_data();
				break;		
			default:
				;
		}
	}
	
	if (t_keys->get_state(UP,CLICKORPRESS) && menu_state == CH_ENABLE)
		++*data_copy;	
	else if (t_keys->get_state(DOWN,CLICKORPRESS) && menu_state == CH_ENABLE)
		--*data_copy;	

		return *this;
}


template <typename T> void  Menu_ch_data<T>::change_data()
{
	switch(menu_state)
	{
		case CH_ENABLE:
			data_copy = new T(*data);
			break;
		case SAVE:
				*data = *data_copy;
				delete data_copy;
				menu_state = NOACTION;
				break;
		case CANCEL:
			delete data_copy;
			menu_state = NOACTION;
			break;
		default:
			;
	}
}

template <typename T> void  Menu_ch_data<T>::menu_scr()
{
	switch (menu_state)
	{
		case CH_ENABLE:
			scr << *data_copy;
			break;
		default :
			scr << *data;
	}
}


/* ===============================================================================
                       ##### Menu item flash read only #####
 ===============================================================================  */
void Menu_flash_read::menu_scr()
{
	if (Data_Val != NULL)
		scr << Data_Val;
	else 
		scr << "Err";
}


Menu_Base& Menu_flash_read::action()
{
	static bool enter = false;
	if (!enter)
	{
		Data_Val = Data->get_value();
			enter = true;
		}
			
	if (t_keys->get_state(ESC,CLICK))
	{
		enter = false;
		return *parent;
	}
	else return *this;
}


/* ===============================================================================
                       ##### Menu item for r/w flash data #####
 ===============================================================================  */
/**
  * @brief checking sensors state 
  * @note   performs the main menu work
  * @retval pointer to the parent menu
  */
Menu_Base& Menu_flash_rw::action()
{
	data_val = Data->get_value();
	if (t_keys->get_state(OK,CLICK))
	{
		switch (menu_state)
		{
			case NOACTION:
				menu_state = CH_ENABLE;
				change_data();
				break;
			case CH_ENABLE:
				menu_state = SAVE;
				change_data();
				break;
			default:
				;
		}
	}
	else if (t_keys->get_state(ESC,CLICK))
	{
		switch (menu_state)
		{
			case NOACTION:
				return *parent;
			case CH_ENABLE:
				menu_state = CANCEL;
				change_data();
				break;	
			default:
				;
		}
	}
	
	if (t_keys->get_state(UP,CLICKORPRESS) && menu_state == CH_ENABLE)
		++*data_val_copy;	
	else if (t_keys->get_state(DOWN,CLICKORPRESS) && menu_state == CH_ENABLE)
		--*data_val_copy;	
		return *this;
}

/**
  * @brief utility function for change data process
  * @note  in SAVE state puches event_write to main state machine
  * @retval none
  */
void Menu_flash_rw::change_data()
{
	switch(menu_state)
	{
		case CH_ENABLE:
			data_val_copy = new uint16_t(data_val);
			break;
		case SAVE:{
			menu_state = NOACTION;
			event_write.set_task(EEPROM_EVENT_WR,Data,*data_val_copy);
			delete data_val_copy;
			state_machine->push(&event_write);}
			break;
		case CANCEL:
			delete data_val_copy;
			menu_state = NOACTION;
			break;
		default:
			;
	}
}


/**
  * @brief displays eeprom data on screen 
  * @note   if state is CH_ENABLE dispays copy of eeprom data
  * @retval none
  */
void Menu_flash_rw::menu_scr()
{
	
	switch (menu_state)
	{
		case CH_ENABLE:
			scr << *data_val_copy;
			break;
		default :
			scr << data_val;
	}
}

/* ===============================================================================
                       ##### Voltage_relay menu #####
 ===============================================================================  */

/**
  * @brief checking sensors state 
  * @note   performs the main menu work
  * @retval pointer to the parent menu
  */
Menu_Base& Menu_Voltage_Relay::action()
{
	return *parent;
}

/**
  * @brief displays voltage RMS on screen 
  * @note   
  * @retval none
  */
void Menu_Voltage_Relay::menu_scr()
{
	scr <<volt_relay->get_voltage_to_screen();
}

/******************************************************************************/
/*          		Menu to obtain time saved in flash				        						*/ 
/******************************************************************************/

/**
  * @brief 
  * @note   
  * @param
  * @param  
  * @retval
  */
Menu_Base& Menu_Time::action()
{
	data_val = Data->get_value();
	if (t_keys->get_state(OK,CLICK))
	{
		switch (menu_state)
		{
			case NOACTION:
				menu_state = CH_ENABLE;
				change_data();
				break;
			case CH_ENABLE:
				menu_state = SAVE;
				change_data();
				break;
			default:
				;
		}
	}
	else if (t_keys->get_state(ESC,CLICK))
	{
		switch (menu_state)
		{
			case NOACTION:
				return *parent;
			case CH_ENABLE:
				menu_state = CANCEL;
				change_data();
				break;	
			default:
				;
		}
	}
	if (menu_state == CH_ENABLE)
	{
		if (t_keys->get_state(UP,CLICKORPRESS))
			++*data_val_copy;
		else if (t_keys->get_state(DOWN,CLICKORPRESS))
			--*data_val_copy;
	}
		return *this;
}

/**
  * @brief 
  * @note   
  * @param
  * @param  
  * @retval
  */
void Menu_Time::menu_scr()
{
	switch (menu_state)
	{
		case CH_ENABLE:
			time.tm_min = *data_val_copy/60;
			time.tm_sec = *data_val_copy%60;
			break;
		default :
			data_val = Data->get_value();
			time.tm_min = data_val/60;
			time.tm_sec = data_val%60;
	}
	scr << time;
}


/* ===============================================================================
                       ##### Error menu #####
 ===============================================================================  */
Menu_Base& Menu_Error::action()
{
	if (t_keys->get_state(OK,CLICK))
		return *parent;
	else if (t_keys->get_state(ESC,CLICK))
		return *parent;
	return *this;
}

void Menu_Error::menu_scr()
{
	scr<<text;
}

void Menu_Error::set_parent(Menu_Base* entry)
{
	parent = entry;
}

/* ===============================================================================
                       ##### Exported Functions #####
 ===============================================================================  */


/**
  * @brief calls error menu
  * @note  changes current menu to error menu if there is a problem in eeprom write process.
	* @note  this functin is pushed to main_state_machine. related to Write_Error ptr
  * @param none
  * @retval None
  */
void Call_Error_Menu()
{
	Err_Menu.set_parent(selected);
	selected = &Err_Menu;
	Main_State_Machine.pop();
}

/**
  * @brief executes menu task
  * @note  put in timer interrupt
  * @param none
  * @retval None
  */

void do_Menu()
{
	selected = &selected->action();
	selected->menu_scr();
	back_to_main(Parent);
};

/* ===============================================================================
                       ##### private functions #####
 ===============================================================================  */

/**
  * @brief turns selected menu to parent after elapsed time 
  * @note  return if no sensors were toched
  * @param menu to turn back to
  * @retval None
  */

static void back_to_main(Menu_item& menu)
{
	static uint16_t time_s;
	if (Keyboard.any_key_pressed())
	{
		time_s = TSL_Globals.Tick_sec;
		return;
	}
	else 
	{
		if (time_elapsed(BACK_TO_MAIN,TSL_Globals.Tick_sec,time_s))
		{
			selected = &menu;
			menu.return_to_top();
		}
	}
}


