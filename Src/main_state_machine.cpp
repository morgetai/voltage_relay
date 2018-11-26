#include "main_state_machine.h"
#include "main.h"
/* Global variables ---------------------------------------------------------*/
Voltage_Relay_Executor Main_State_Machine;

/* ===============================================================================
                       ##### Stack #####
 ===============================================================================  */

template <typename Data> bool Stack<Data>::push(Data& entry)
{
	if (top > capacity)
		return false;
	else 
	{
	  storage	[++top] = entry;	
		return true;
	}
}

template <typename Data> void Stack<Data>::pop()
{
	if (top == 0)
		return;
	else 
	{
		storage[top] = NULL;
		--top;	
	}
}

template <typename Data> Data& Stack<Data>::get()
{
	return storage[top];
}

/* ===============================================================================
                       ##### Voltage_Relay_Executor #####
 ===============================================================================  */

void Voltage_Relay_Executor::push(OPERATION_BASE* entry)
{
	if (Stack_ptr->push(entry))
		;
	else _Error_Handler("push_error", 45);
}

void Voltage_Relay_Executor::pop()
{
	Stack_ptr->pop();
}


void Voltage_Relay_Executor::run_task()
{
	if (Stack_ptr->get() != NULL)
		Stack_ptr->get()->execute_task();
	else return;
}
