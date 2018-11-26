#ifndef MENU_H
#define MENU_H

/* Includes ------------------------------------------------------------------*/
#include <stdarg.h>
#include "Screen.h"
#include "keyboard.h"
#include "eeprom_user.h"
#include "eeprom.h"
#include "main_state_machine.h"
#include "voltage_relay.h"
#include "help.h"

/* Defines -------------------------------------------------------------------*/
#define BACK_TO_MAIN 15 //in seconds
/* Extern variables ---------------------------------------------------------*/
extern Screen scr;

/* ===============================================================================
                       ##### Menu_Base absract class #####
	==============================================================================		 
										This part stores text and keyboard pointer
 ===============================================================================  */

class Menu_Base
{
	public:
		Menu_Base (const char* _text): text(_text){}
		virtual Menu_Base& action() = 0; 
		virtual void menu_scr() = 0;
		virtual ~Menu_Base() = default;
			
	public:
		const char* text;
	protected:
		static keyboard* t_keys;
};



/* ===============================================================================
                       ##### Tree-like menu item #####
	==============================================================================	
	This menu is used to create a branch with nested leaves. In usual case we get a
	list of menus with root element.
 ===============================================================================  */

class Menu_item:public Menu_Base
{
	public:
		
		//constructor
		Menu_item(const char* text,uint8_t _size,...):Menu_Base(text),size(_size)
		{
			va_list args;
			va_start(args,_size);
			ptr_b = new Menu_Base* [size];// qt of menus
			ptr = ptr_b;
			for (uint8_t i = 0; i < size;i++)
				*(ptr_b+i) = va_arg(args,Menu_Base*); 
			va_end(args);
			ptr_b = ptr;
		}
		
		//virtual fucntions
		public:
			virtual Menu_Base& action();
			virtual void menu_scr();	
		
		
		virtual ~Menu_item()
		{
			delete [] ptr_b;
		}
		
		public:
			void return_to_top();
	/*----realisation-----*/
	private:
		/*pointer to top of array of menus*/
		Menu_Base** ptr_b;
		/*pointer to selected menu*/
		Menu_Base** ptr;
		/*siae of array of menus*/
		uint8_t size;
};


/* ===============================================================================
                       ##### Menu item for reading data #####
 ===============================================================================  */


template <typename T>class Menu_data:public Menu_Base
{
	public:
	
		//constructor
		Menu_data(const char* text,Menu_Base* p,T& d):Menu_Base(text),parent(p),data(&d){}
			
		//virtual fucntions
		virtual Menu_Base& action();
		virtual void menu_scr(){scr << *data;}	
		
		/*realisation*/
	private:
		Menu_Base* parent;
		T* data;
		
};



/* ===============================================================================
                       ##### Menu item for r/w data #####
 ===============================================================================  */

template <typename T> class Menu_ch_data:public Menu_Base
{
	public:

		//constructor
		Menu_ch_data(const char* text,Menu_Base* p,T& d):Menu_Base(text),parent(p), data(&d){}
			
		//virtual fucntions
		virtual Menu_Base& action();
		virtual void menu_scr();
			
		//member functions
	private:
		volatile enum {NOACTION,CH_ENABLE,SAVE,CANCEL} menu_state; // states
		Menu_Base* parent;
		T* data;
		T* data_copy;
		void change_data();
};



/* ===============================================================================
                       ##### Menu item flash read only #####
 ===============================================================================  */
class Menu_flash_read:public Menu_Base
{
	public:
		
		//constructor
		Menu_flash_read(const char* text,keyboard& _t_keys,Flash_Data& _data,Menu_Base* p):Menu_Base(text),Data(&_data), parent(p){}
		
		//virtual functions
		virtual Menu_Base& action();
		virtual void menu_scr();
			
			

	private:
		const Flash_Data* Data;
		 Menu_Base* parent;
	private:
		uint16_t Data_Val; //store the value of data
};


/* ===============================================================================
                ##### Menu item read/write data from eeprom #####
 ===============================================================================  */
class Menu_flash_rw:public Menu_Base
{
public:
	
//constructor
	Menu_flash_rw(const char* text,Flash_Data& _data,Menu_Base* p):Menu_Base(text),Data(&_data), parent(p){}
	
//virtual functions
	virtual Menu_Base& action();
	virtual void menu_scr();
				
protected:
	Flash_Data* Data;
	static Voltage_Relay_Executor* state_machine;
	Menu_Base* parent;
protected:
	enum {NOACTION,CH_ENABLE,SAVE,CANCEL} menu_state; // states
	uint16_t data_val;
	uint16_t* data_val_copy;
protected:
	void change_data();
};

/* ===============================================================================
                       ##### Voltage_relay menu #####
 ===============================================================================  */
class Menu_Voltage_Relay:public Menu_Base
{
	public:
		Menu_Voltage_Relay(voltage_relay& v_rel,Menu_Base* p):Menu_Base(""),volt_relay(&v_rel),parent(p){}
			
	//virtual functions
	public:
		virtual Menu_Base& action();
		virtual void menu_scr();
	
	private:
		voltage_relay* volt_relay;
		Menu_Base* parent;		
};

/******************************************************************************/
/*          		Menu to obtain time saved in flash				        						*/ 
/******************************************************************************/
class Menu_Time:public Menu_flash_rw
{
	/*constructor*/
	public:
		Menu_Time(const char* text,Flash_Data& _data,Menu_Base* p):Menu_flash_rw(text,_data,p){}
		
			/*virtual fuctions*/
	public:
		virtual Menu_Base& action();
		virtual void menu_scr();
	private:
		tm time;
};

/* ===============================================================================
                       ##### Error menu #####
 ===============================================================================  */
class Menu_Error:public Menu_Base
{
	public:
		Menu_Error(const char* text,Menu_Base* p):Menu_Base(text),parent(p){}
		void set_parent(Menu_Base*);
			
	//virtual functions
	public:
		virtual Menu_Base& action();
		virtual void menu_scr();
	
	private:
	  Menu_Base* parent;
};


/* ===============================================================================
                       ##### Exported Functions #####
 ===============================================================================  */

void Call_Error_Menu();

void do_Menu();

#endif 


