#ifndef OBSERVER_H
#define OBSERVER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Defines -------------------------------------------------------------------*/
#define OBSERVERS_QTY 10

/* ===============================================================================
                       ##### Observer pattern part #####
 ===============================================================================  */
class Observer
{
	public:
		virtual void update() = 0;
};

/* ===============================================================================
                       ##### Subject pattern part #####
 ===============================================================================  */
class Subject
{
	public:
		Subject(){viewers = new Observer* [OBSERVERS_QTY]; index = 0;}
		~Subject(){delete[] viewers;}
	public:
		void attach(Observer*);
		void detach(Observer*);
		void notify();
	private:
		Observer** viewers;
		uint8_t index;
};


#endif





