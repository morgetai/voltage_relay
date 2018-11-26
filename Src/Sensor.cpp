#include "Sensor.hpp"


extern TSC_HandleTypeDef htsc;

TSC_HandleTypeDef* sensor::_htsc = &htsc;

sensor sensors[] = 
{
sensor(TSC_GROUP6_IDX,TSC_GROUP6_IO3,TSC_GROUP6_IO2,OK),
sensor(TSC_GROUP4_IDX,TSC_GROUP4_IO3,TSC_GROUP4_IO4,ESC),
sensor(TSC_GROUP6_IDX,TSC_GROUP6_IO4,TSC_GROUP6_IO2,UP),
sensor(TSC_GROUP4_IDX,TSC_GROUP4_IO2,TSC_GROUP4_IO4,DOWN),
sensor(TSC_GROUP4_IDX,TSC_GROUP4_IO1,TSC_GROUP4_IO4,LEFT)
};

keyboard Keyboard(sensors);







bool touch_counter(sensor& sens)
{
	HAL_TSC_StateTypeDef state  = HAL_TSC_GetState(sens._htsc);
	static uint8_t counter = 0;
	if (state==HAL_TSC_STATE_READY && counter == 0)
	{
		HAL_TSC_IODischarge(sens._htsc,ENABLE);
		while (counter != 10)
			++counter;
		HAL_TSC_IOConfig(sens._htsc,sens.config);
		HAL_TSC_Start(sens._htsc);
		return false;
	}
	switch (state)
		{
			case HAL_TSC_STATE_READY:
				counter = 0;
				return true;
			case HAL_TSC_STATE_BUSY:
				return false;
			case HAL_TSC_STATE_ERROR:
				counter = 0;
				return true;
		}
}


void sensor::arr_off_fill(uint16_t& value)
{
	switch (cnt)
	{
		case 0:
			std::for_each(sens_off_time_arr,sens_off_time_arr+5,[&](uint16_t& elem){elem = value;});
			std::for_each(sens_on_time_arr,sens_on_time_arr+5,[&](uint16_t& elem){elem = value;});
			this->sens_off_time=(*sens_off_time_arr+*(sens_off_time_arr+1)+*(sens_off_time_arr+2)+*(sens_off_time_arr+3)+*(sens_off_time_arr+4))/5;
			++cnt;
			break;
		case 1000:
			cnt=11;
			if (sens_status)
				break;
			else if (value < 1.2*sens_off_time && value >= 0.9*sens_off_time){
				for(uint8_t i = 4; i >= 1; i--)
					sens_off_time_arr[i] = sens_off_time_arr[i-1];
				*sens_off_time_arr = value;
				this->sens_off_time=(*sens_off_time_arr+*(sens_off_time_arr+1)+*(sens_off_time_arr+2)+*(sens_off_time_arr+3)+*(sens_off_time_arr+4))/5;	
			}
			break;
			
		default:
			++cnt;
			break;
		}
}



void arr_on_fill(sensor& sens,uint16_t& value)                                         // ‘ункци€ дл€ последовательного заполнени€ значений массива arr2
{
	
	for(uint8_t i = 4; i >= 1; i--)                            // ÷икл дл€ заполнени€ массива
		sens.sens_on_time_arr[i] = sens.sens_on_time_arr[i-1];                                     // —двигаем более старые значени€ к концу массива, удал€€ последнее
	
	if(value <= sens.sens_off_time)
		*sens.sens_on_time_arr = value;  
	
	sens.sens_on_time = (*sens.sens_on_time_arr+*(sens.sens_on_time_arr+1)+*(sens.sens_on_time_arr+2)+*(sens.sens_on_time_arr+3)+*(sens.sens_on_time_arr+4))/5;
	
}


bool sens_check(sensor& sens)
{
	bool ret = touch_counter(sens);
	if (ret){
	uint16_t count = HAL_TSC_GroupGetValue(sens._htsc,sens.gx_index);
	sens.arr_off_fill(count);
	if(count < 2000 )
	{
		//sens.arr_off_fill(count);
		arr_on_fill(sens,count);
	}
	if (sens.sens_on_time+sens.on_trh < sens.sens_off_time){
		sens.sens_status = true;
		//std::for_each(sens.sens_on_time_arr,sens.sens_on_time_arr+5,[&sens](uint16_t& elem){elem = sens.sens_off_time;});
	}
	else if (sens.sens_on_time >= sens.sens_off_time-sens.off_trh)
		sens.sens_status = false;
	return true;
	}
	else return false;
}

void keyboard::check()
{
	switch(sensors->name)
	{
		case OK:
			if (sens_check(*sensors)){
			IDR[OK] = sensors->sens_status;
			++sensors;
			}
			break;
		case ESC:
			if (sens_check(*sensors)){
			IDR[ESC] = sensors->sens_status;
			++sensors;
			}
			break;
		case UP:
			if (sens_check(*sensors)){
			IDR[UP] = sensors->sens_status;
			++sensors;
			}
			break;
		case DOWN:
			if (sens_check(*sensors)){
			IDR[DOWN] = sensors->sens_status;
			++sensors;
			}
			break;
		case LEFT:
			if (sens_check(*sensors)){
			IDR[LEFT] = sensors->sens_status;
			sensors -=4;
			}
			break;		
	}
}


bool keyboard::get_state(SENSOR_NAME name,SENSOR_STATE state)
{
	bool ret = false;
	switch(state)
	{
		case RELEASE:
			if(IDR[name]) {
				this->state[name] = RELEASE;
				ret = false;
			}
			else if (this->state[name] != RELEASE)
				ret = false;
			else{
				this->state[name] = RELEASE;
				ret =true;
			}
			break;
			
		case PRESSED:
			if (!IDR[name]){
				this->state[name] = RELEASE;
				ret = false;
			}
			else {
				this->state[name] = PRESSED;
				ret = true;
			}
			break;
			
		case CLICK:
			if (IDR[name]){
				this->state[name] = PRESSED;
				ret = false;
				click = 0;
			}
				else if (!IDR[name] && this->state[name] == PRESSED)
				{
					this->state[name] = RELEASE;
					ret = true;
					click = 1;
				}
				break;	
	}
   return ret;
}

uint8_t keyboard::ret_click()
{
	return click;
}

void keyboard::res_click()
{
	click = 0;
}
