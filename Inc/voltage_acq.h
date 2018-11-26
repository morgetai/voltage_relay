#ifndef VOLTAGE_ACQ
#define VOLTAGE_ACQ

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "arm_math.h"
#include <math.h>
#include <algorithm>
#include "help.h"

/* Defines -------------------------------------------------------------------*/
/*voltage measure*/
#define MESURES_QTY 20
#define GAIN_FACTOR 0.228
#define CONST_PART 1885
/*move avg filter*/
#define FILTER_CAP 8 //fiter capacity
#define FILTER_DEV 3  // devider which respresents the power of 2
/*FIR filter*/
#define SAMPLES 5 
#define NUM_COEFS 9
/* extern variables -------------------------------------------------------------------*/
extern  	float32_t  kCoef[NUM_COEFS];
extern  	float32_t  pState[20];


/* ===============================================================================
                       ##### moving average filter #####
 ===============================================================================  */
class move_avg
{
	public:
		
		move_avg(){input_values = new uint16_t [FILTER_CAP];}
		move_avg(uint16_t cap){input_values = new uint16_t [cap]; std::fill_n(input_values, FILTER_CAP, 220);}
		~move_avg(){delete [] input_values;}
		//member functions
		uint16_t execute_filter(uint16_t);
	private:
		uint16_t filtered_value; 
		uint16_t* input_values; // array of measurements
		uint8_t pos; //position in input_values array
};


/******************************************************************************/
/*     									     		low pass filter												        */ 
/******************************************************************************/
class low_pass_filter
{
	public:
		//default constructor
		low_pass_filter()
			{
				pkCoef = ::kCoef;				
				pSate = ::pState;
				S = new arm_fir_instance_f32;
				filtered_values = new  float32_t  [SAMPLES];
				input_values = new float32_t [SAMPLES];
				arm_fir_init_f32(S,NUM_COEFS,pkCoef,pSate,SAMPLES);
				pos = 0;
			}
			//destructor
		~low_pass_filter(){delete [] input_values;delete [] filtered_values;delete S;}
			
	public:
		uint16_t execute_filter (float);	
	/*realisation*/
	private:
		arm_fir_instance_f32* S;
		float32_t * filtered_values;
		float32_t * input_values;
		float32_t * pkCoef;
		float32_t* pSate;
		uint16_t pos;
};



/* ===============================================================================
                       ##### voltage acqusition #####
 ===============================================================================  */

class voltage_acq
{
	/*constructors and destructors*/
	public:
	  voltage_acq(uint8_t _measures):measures(_measures){voltage_measures = new uint32_t [measures] ; std::fill_n(voltage_measures, MESURES_QTY, 220);}
		~voltage_acq(){delete [] voltage_measures;}
	/*public functions*/
	public:
		void calculate();
		uint16_t get_voltage() const;
		uint16_t get_screen_voltage() const;
		uint32_t* get_ptr();
	
/*private utility functions*/
	private:
		void filter_voltage();
	
	/*realisation*/		
	private:
		uint32_t* voltage_measures; //ptr to top of array
		float voltage_const_part = CONST_PART; //constant component of the voltage
		float voltage; //voltage rms
		uint16_t filtered_voltage = 220;	//Filtered voltage RMS
		mutable uint16_t screen_filtered_voltage = 220; //Filtered voltage prepared fo screen
		const uint8_t measures;
	
	private:
		low_pass_filter low_pass;
}	;



#endif

