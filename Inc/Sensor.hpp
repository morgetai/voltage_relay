#ifndef SENSOR_HPP
#define SENSOR_HPP

#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_tsc.h"

#include <algorithm>


#define ON_TRH 8
#define OFF_TRH 4

typedef enum
{
	OK = 0,
	ESC = 1,
	UP,
	DOWN,
	LEFT
}SENSOR_NAME;

typedef enum
{
	RELEASE = 0,
	PRESSED = 1,
	CLICK 
}SENSOR_STATE;



class keyboard;


class sensor
{
	public:
	friend class keyboard;
	friend bool touch_counter(sensor& sens);	
	friend void arr_off_fill(sensor&, uint16_t&);	
	friend void arr_on_fill(sensor& sens,uint16_t& value);
	//friend bool sens_check(sensor& sens);
	friend bool sens_check(sensor& sens);
	
	sensor(uint32_t index,uint32_t channel,uint32_t sampling,SENSOR_NAME _name):gx_index(index),config (new TSC_IOConfigTypeDef),on_trh(ON_TRH),off_trh(OFF_TRH),cnt(0),name(_name)
			{
				config->ChannelIOs = channel;
				config->SamplingIOs = sampling;
			}
		
	public:	
	void set_ontrh(uint16_t& value){on_trh = value;}
	void set_offtrh(uint8_t& value){off_trh = value;}
	void arr_off_fill(uint16_t&);
	
	
private:
	uint32_t gx_index;
	static TSC_HandleTypeDef* _htsc;
	TSC_IOConfigTypeDef* config;
	uint16_t sens_off_time_arr[5];
	uint16_t sens_on_time_arr[5];
	uint16_t sens_off_time ;
  uint16_t sens_on_time;                           // Среднее время заряда для сенсора без касания
	uint8_t on_trh;
	uint8_t	off_trh;
	uint16_t	cnt;
	bool sens_status;
	SENSOR_NAME name;
};



class keyboard
{
	public:
		keyboard(sensor* ptr):sensors(ptr){}

	public:
		void check();
		bool get_state(SENSOR_NAME,SENSOR_STATE);
		uint8_t ret_click();
		void res_click();
		

	private:
		sensor* sensors;
		bool IDR[5];
		SENSOR_STATE state[5];
		uint8_t click;
};


bool touch_counter(sensor& sens);
bool sens_check(sensor& sens);

#endif

