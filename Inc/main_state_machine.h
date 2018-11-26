#ifndef MAIN_STATE_MACHINE_H
#define MAIN_STATE_MACHINE_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include <stack>
/* Defines -------------------------------------------------------------------*/
#define STACK_SIZE 10


/* ===============================================================================
                       ##### Stack #####
 ===============================================================================  */
template <typename Data> class Stack
{
	public:
		Stack(uint8_t cap):capacity(cap){storage = new Data [capacity];}
		~Stack(){delete [] storage;}
		
	public:
		bool push(Data& entry);
		void pop();
		Data& get();
	private:
		Data* storage;
		uint8_t top;
		uint8_t capacity;
};

/* ===============================================================================
                       ##### OPERATION_BASE #####
 ===============================================================================  */

class OPERATION_BASE
{
	public:
		virtual void execute_task()  = 0;
	public:
		virtual ~OPERATION_BASE() = default;
};


/* ===============================================================================
                       ##### Main_State_machine #####
 ===============================================================================  */
class Voltage_Relay_Executor
{
	public:
		Voltage_Relay_Executor(){Stack_ptr = new Stack<OPERATION_BASE*>(STACK_SIZE); }
		~Voltage_Relay_Executor(){delete Stack_ptr;}
	public:
		void push(OPERATION_BASE*);
		void pop();
		void run_task();
	private:
		Stack<OPERATION_BASE*>* Stack_ptr;
};

#endif

