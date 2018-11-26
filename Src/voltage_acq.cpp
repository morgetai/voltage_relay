#include "voltage_acq.h"


/* Private variables ---------------------------------------------------------*/
 float32_t  kCoef[NUM_COEFS] = 
{
   -0.06393767,  0.0351015,   0.13615213,   0.24341564,  0.28938796,
     0.24341564,   0.13615213,  0.0351015, -0.06393767    
};
 /*float32_t  kCoef[NUM_COEFS] = 
{
   -0.06949739158,  0.03815380484,   0.1479914486,   0.2645822167,   0.3145521283,
     0.2645822167,   0.1479914486,  0.03815380484, -0.06949739158    
};*/
 float32_t  pState[20];
/* Extern variables ---------------------------------------------------------*/
extern  TSL_Globals_T TSL_Globals;;

/* ===============================================================================
                       ##### voltage acqusition #####
 ===============================================================================  */

/*-----------Private member functions---------*/


/**
  * @brief calculates voltage RMS 
  * @note   
  * @retval none
  */

void voltage_acq::calculate()
{
	float var;
	voltage = 0;
	for (uint16_t i = 0; i < measures;++i)
	{
		var = static_cast<float>(voltage_measures[i])-voltage_const_part;
		voltage += (var*var)/measures;
	}
	voltage = std::sqrt(voltage)*GAIN_FACTOR;
	filter_voltage();		
};

/**
  * @brief filtering the voltage using move_avg filter		
  * @note   
  * @retval none
  */
void voltage_acq::filter_voltage()
{
	filtered_voltage = low_pass.execute_filter(voltage);
};


/*----------------Public member functions-------------*/
/**
  * @brief returns filtered voltage
  * @note   
  * @retval none
  */
uint16_t voltage_acq::get_voltage() const
{
	return filtered_voltage;
}

/**
  * @brief return voltage after second avg_filtering
  * @note  pass it to screen
  * @retval filtered voltage
  */
uint16_t voltage_acq::get_screen_voltage() const
{
	static uint8_t tick = 0;
	if (time_elapsed(3,static_cast<uint8_t>(TSL_Globals.Tick_sec),tick))
	{
		screen_filtered_voltage = filtered_voltage;
		tick = TSL_Globals.Tick_sec;
		return screen_filtered_voltage;
	}
	else 	return screen_filtered_voltage;
}
/**
  * @brief returns pointer to array of measurments
  * @note    
  * @retval pointer
  */
uint32_t* voltage_acq::get_ptr()
{
	return voltage_measures;
}
/* ===============================================================================
                       ##### moving average filter #####
 ===============================================================================  */


/**
  * @brief executes the filter
  * @note   
  * @param value to be filtered 
  * @retval filtered value
  */
uint16_t move_avg::execute_filter(uint16_t val)
{
	filtered_value = filtered_value + (val - input_values[pos]);
	input_values[pos] = val;
	pos = (pos+1)%FILTER_CAP;
	return filtered_value >> FILTER_DEV;
}


/******************************************************************************/
/*          								low pass filter										 				        */ 
/******************************************************************************/
/**
  * @brief FIR filter execution
  * @param value of voltage RMS
  * @retval filtered voltage RMS
  */
uint16_t low_pass_filter::execute_filter (float new_val)
{
	uint16_t ret=0;
	input_values[pos] = static_cast<float32_t>(new_val);
	pos = (pos+1)%SAMPLES;
	arm_fir_f32 (S,input_values,filtered_values,SAMPLES);
	for (uint16_t i = 0; i < SAMPLES; ++i)
		ret +=  static_cast<uint16_t>(filtered_values[i]);
	return ret/SAMPLES;
}



