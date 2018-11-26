#ifndef MENU_H
#define MENU_H


#include "Screen.h"
#include "keyboard.h"
#include <stddef.h>
#include <stdarg.h>
#include <cstdlib>
#include "eeprom_user.h"
#include "eeprom.h"
#define BACK_TO_MAIN 3000


extern Screen scr;


class Menu_Base
{
	public:
		Menu_Base (const char* _text): text(_text){}
		virtual Menu_Base& action() = 0; 
		virtual void menu_scr() = 0;
		virtual ~Menu_Base() = default;
			
	public:
		const char* text;
};


//------------------------------------------
//Tree-like menu item
//------------------------------------------


class Menu_item:public Menu_Base
{
	public:
		
		//constructor
		Menu_item(const char* text,keyboard& _t_keys,uint8_t _size,...):Menu_Base(text),t_keys(&_t_keys),size(_size)
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
		virtual Menu_Base& action();
		virtual void menu_scr();	
		
		
		virtual ~Menu_item()
		{
			delete [] ptr_b;
		}
	private:
		keyboard* t_keys;
		Menu_Base** ptr_b; // pointer to the first menu in srray
		Menu_Base** ptr;// pointer to the running menu
		uint8_t size;
};



//------------------------------------------
//Menu item for reading data
//------------------------------------------


template <typename T>class Menu_data:public Menu_Base
{
	public:
		
		//constructor
		Menu_data(const char* text,keyboard& _t_keys,Menu_Base* p,T& d):Menu_Base(text),t_keys(&_t_keys),parent(p),data(&d){}
			
		//virtual fucntions
		virtual Menu_Base& action();
		virtual void menu_scr(){scr << *data;}	
	private:
		keyboard* t_keys;
		Menu_Base* parent;
		T* data;
		
};


//------------------------------------------
//Menu item for r/w data
//------------------------------------------


template <typename T> class Menu_ch_data:public Menu_Base
{
	public:

		//constructor
		Menu_ch_data(const char* text,keyboard& _t_keys,Menu_Base* p,T& d):Menu_Base(text),t_keys(&_t_keys),parent(p), data(&d){}
			
		//virtual fucntions
		virtual Menu_Base& action();
		virtual void menu_scr();
			
		//member functions
	private:
		keyboard* t_keys;
		enum {NOACTION,CH_ENABLE,SAVE,CANCEL} state; // states
		Menu_Base* parent;
		T* data;
		T* data_copy;
		void change_data();
};

//------------------------------------------
//Menu item flash read only
//------------------------------------------

class Menu_flash_read:public Menu_Base
{
	public:
		
		//constructor
		Menu_flash_read(const char* text,keyboard& _t_keys,Flash_Data& _data,Menu_Base* p):Menu_Base(text),t_keys(&_t_keys),Data(&_data), parent(p){}
		
		//virtual functions
		virtual Menu_Base& action();
		virtual void menu_scr();
			
			

	private:
		keyboard* t_keys;
		Flash_Data* Data;
		Menu_Base* parent;
	private:
		uint16_t* Data_Val; //store the value of data
};

//------------------------------------------
//Menu item flash read/write
//------------------------------------------

class Menu_flash_rw:public Menu_Base
{
public:
	
//constructor
	Menu_flash_rw(const char* text, keyboard& _t_keys,Flash_Data& _data,Menu_Base* p):Menu_Base(text),t_keys(&_t_keys),Data(&_data), parent(p){}
	
//virtual functions
	virtual Menu_Base& action();
	virtual void menu_scr();
				
	

private:
	keyboard* t_keys;
	Flash_Data* Data;
	Menu_Base* parent;
private:
	enum {NOACTION,CH_ENABLE,SAVE,CANCEL,ERROR} state; // states
	uint16_t* data_val;
	uint16_t* data_val_copy;
	bool check_data();
	void change_data();
};


void do_Menu();

#endif 


