#include "Screen.h"


/* Global variables ---------------------------------------------------------*/
Screen scr;
/* Private typedefs ----------------------------------------------------------*/
typedef void symbols_ (void); 
/* Private functions prototypes------------------------------------------------*/
void Dig_num_out(const char* ,bool);
char* utoa_builtin_div(uint32_t value, char *buffer);
char* time_to_char(tm& time,char* buffer);
void clear_buf(Screen& sc);
/* Private variables ---------------------------------------------------------*/
     /*    ----Segments-----    */
Segment SegA(Seg_A_GPIO_Port,Seg_A_Pin); 
Segment SegB(Seg_B_GPIO_Port,Seg_B_Pin);
Segment SegC(Seg_C_GPIO_Port,Seg_C_Pin);
Segment SegD(Seg_D_GPIO_Port,Seg_D_Pin);
Segment SegE(Seg_E_GPIO_Port,Seg_E_Pin);
Segment SegF(Seg_F_GPIO_Port,Seg_F_Pin);
Segment SegG(Seg_G_GPIO_Port,Seg_G_Pin);
Segment SegPoint(Seg_Dp_GPIO_Port,Seg_Dp_Pin);


  /*    ----Digits-----    */
Digit dig4(Dig_1_GPIO_Port,Dig_1_Pin);
Digit dig3(Dig_2_GPIO_Port,Dig_2_Pin);
Digit dig2(Dig_3_GPIO_Port,Dig_3_Pin);
Digit dig1(Dig_4_GPIO_Port,Dig_4_Pin);
Digit digits [] ={dig1,dig2,dig3,dig4};




/* ===============================================================================
                       ##### Private utility functions #####
 ===============================================================================  */

/**
  * @brief turns off all segments. 
  * @note 
  * @note 
  * @retval none
  */
void clearSegments(){
	SegA.BSRR_Off();
	SegB.BSRR_Off();
	SegC.BSRR_Off();
	SegD.BSRR_Off();
	SegE.BSRR_Off();
	SegF.BSRR_Off();
	SegG.BSRR_Off();	
	SegPoint.BSRR_Off();
};

/**
  * @brief turns on point segment 
  * @note 
  * @note 
  * @retval none
  */
void point (){
	SegPoint.BSRR_On();
};

/* ===============================================================================
                       ##### turns on chosen number or letter #####
 ===============================================================================  */

void num_0(){
SegA.BSRR_On();	
SegB.BSRR_On();
SegC.BSRR_On();
SegD.BSRR_On();
SegE.BSRR_On();
SegF.BSRR_On();
};


void num_1(){
SegB.BSRR_On();
SegC.BSRR_On();	
};



void num_2(){
SegA.BSRR_On();	
SegB.BSRR_On();	
SegG.BSRR_On();
SegE.BSRR_On();
SegD.BSRR_On();		
};


void num_3(){
SegA.BSRR_On();	
SegB.BSRR_On();
SegG.BSRR_On();
SegC.BSRR_On();
SegD.BSRR_On();		
};


void num_4(){
SegF.BSRR_On();	
SegG.BSRR_On();	
SegB.BSRR_On();
SegC.BSRR_On();	
};


void num_5(){
SegA.BSRR_On();	
SegF.BSRR_On();	
SegG.BSRR_On();
SegC.BSRR_On();
SegD.BSRR_On();		
};



void num_6(){
SegA.BSRR_On();
SegF.BSRR_On();
SegE.BSRR_On();
SegD.BSRR_On();
SegC.BSRR_On();
SegG.BSRR_On();	
};


void num_7(){
SegA.BSRR_On();
SegB.BSRR_On();
SegC.BSRR_On();
};


void num_8(){
SegA.BSRR_On();
SegB.BSRR_On();
SegC.BSRR_On();
SegD.BSRR_On();
SegE.BSRR_On();
SegF.BSRR_On();	
SegG.BSRR_On();		
};


void num_9(){
SegA.BSRR_On();	
SegB.BSRR_On();	
SegC.BSRR_On();
SegD.BSRR_On();
SegF.BSRR_On();	
SegG.BSRR_On();		
};


void letter_A(){
SegE.BSRR_On();	
SegF.BSRR_On();	
SegA.BSRR_On();
SegB.BSRR_On();
SegC.BSRR_On();	
SegG.BSRR_On();	
};

void letter_a(){
SegA.BSRR_On();	
SegB.BSRR_On();	
SegC.BSRR_On();
SegD.BSRR_On();
SegE.BSRR_On();
SegG.BSRR_On();
};

void letter_b(){
SegF.BSRR_On();	
SegE.BSRR_On();	
SegD.BSRR_On();
SegC.BSRR_On();
SegG.BSRR_On();
};

void letter_C(){
SegA.BSRR_On();
SegF.BSRR_On();	
SegE.BSRR_On();
SegD.BSRR_On();
};


void letter_c(){
SegG.BSRR_On();	
SegE.BSRR_On();	
SegD.BSRR_On();
};

void letter_t(){
SegF.BSRR_On();	
SegE.BSRR_On();	
SegD.BSRR_On();
SegG.BSRR_On();
};

void letter_h(){
SegF.BSRR_On();	
SegE.BSRR_On();	
SegG.BSRR_On();
SegC.BSRR_On();	
};

void letter_E(){
SegA.BSRR_On();	
SegE.BSRR_On();	
SegF.BSRR_On();
SegD.BSRR_On();
SegG.BSRR_On();		
};

void letter_L(){
SegF.BSRR_On();	
SegE.BSRR_On();
SegD.BSRR_On();		
};

void letter_o(){
SegG.BSRR_On();	
SegE.BSRR_On();
SegD.BSRR_On();	
SegC.BSRR_On();	
};

void letter_r(){
SegG.BSRR_On();	
SegE.BSRR_On();
};

void letter_n(){
SegE.BSRR_On();	
SegG.BSRR_On();
SegC.BSRR_On();	
};

void letter_f(){
	SegA.BSRR_On();
	SegF.BSRR_On();
	SegE.BSRR_On();
	SegG.BSRR_On();	
};

void letter_d(){
	SegB.BSRR_On();
	SegC.BSRR_On();
	SegD.BSRR_On();
	SegE.BSRR_On();
	SegG.BSRR_On();		
};


void letter_O(){
SegA.BSRR_On();	
SegB.BSRR_On();
SegC.BSRR_On();
SegD.BSRR_On();
SegE.BSRR_On();
SegF.BSRR_On();		
};

/* ===============================================================================
        ##### array of number/letter functions.related to ASCII table #####
 ===============================================================================  */
symbols_* symbols [] = {
	point,NULL,
	num_0,num_1,num_2,num_3,num_4,num_5,num_6,num_7,num_8,num_9,
	NULL,NULL,NULL,NULL,NULL,NULL,NULL,
	letter_A,NULL,letter_C,NULL,letter_E,NULL,NULL,NULL,NULL,NULL,NULL,letter_L,
	NULL,NULL,letter_O,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,
	NULL,NULL,NULL,NULL,NULL,NULL,
	letter_a,letter_b,letter_c,letter_d,NULL,letter_f,NULL,letter_h,NULL,NULL,
	NULL,NULL,NULL,letter_n,letter_o,NULL,NULL,letter_r,NULL,letter_t,
	NULL,NULL,NULL,NULL,NULL,NULL
};

/**
  * @brief choose the certain function from array
  * @note turns on needed segments
  * @note 
  * @retval none
  */
void Dig_num_out(const char& ch,bool point = false){
	clearSegments();
	if (ch < 46 || ch > 123)
		return;
	else if (point)
	{
		symbols[ch-46]();
		symbols[0]();		
	}
	else {
	symbols[ch-46]();	
	}
};

/**
  * @brief int to char
  * @note value to be converted
  * @note 	pointer to a buffer
  * @retval return pointer to a given buffer
  */
char * utoa_builtin_div(uint32_t value, char *buffer)
{
	 uint8_t cl = 4;
   buffer += 5; 
		*--buffer = '\0';
   do
   {
		 if (cl == 0)
			 return buffer;
		 		--cl;
      *--buffer = value % 10 + '0';
      value /= 10;
   }
   while (value != 0);
   return buffer;
}

/**
  * @brief turns time to char 
* @note  output char will be this format: "minutes"."seconds"
  * @param time with minutes and srconds
  * @param  pointer to a buffer
  * @retval	return pointer to a given buffer
  */
char* time_to_char(tm& time,char* buffer)
{
	uint8_t cl = 4;
	for (; cl >2 ; --cl)
   {
		 buffer[cl] = time.tm_sec % 10 + '0';
     time.tm_sec /= 10;
   }
	 buffer[2] = '.';
	 buffer[--cl] = time.tm_min % 10 + '0';
	 time.tm_min /= 10;
	 buffer[--cl] = time.tm_min % 10 + '0';
   return buffer;
}


/**
  * @brief clears screen buffer
  * @note sets char to NULL
  * @note 
  * @retval none
  */

void clear_buf(Screen& sc)
{
	std::for_each(sc.buffer,sc.buffer+5,[](char& elem){elem = NULL;});
}

/* ===============================================================================
                       ##### member functions #####
 ===============================================================================  */

void Screen::delete_p(char* pos)
{
	for(char* p = pos; p <= buffer+4; ++p)
		*p = *(p+1);
}

/* ===============================================================================
                       ##### Exported functions #####
 ===============================================================================  */

Screen&  operator<< (Screen& sc,tm time)
{
	char ch[5];
	clear_buf(sc);
	sc << time_to_char(time,ch);
	return sc;
}



Screen&  operator<< (Screen& sc,const uint8_t& ui)
{
	char ch[5];
	clear_buf(sc);
	sc << utoa_builtin_div(static_cast<uint32_t>(ui),ch);
	return sc;
}


Screen& operator<< (Screen& sc,const uint16_t& ui)
{
	char ch[5];
	clear_buf(sc);
	sc << utoa_builtin_div(static_cast<uint32_t>(ui),ch);
	return sc;
}

Screen&  operator<< (Screen& sc,const uint32_t& ui)
{
	char ch[5];
	clear_buf(sc);
	sc << utoa_builtin_div(ui,ch);
	return sc;
}


Screen&  operator<<(Screen& sc,const char* ch)
{
	clear_buf(sc);
	memmove(sc.buffer,ch,strlen(ch));
	return sc;	
}


Screen&  operator<< (Screen& sc,const float& f)
{
	clear_buf(sc);
	sprintf(sc.buffer,"%1.1f",f);
	return sc;
}

/**
  * @brief do main work with screen
  * @note   put this function in timer interrupt hadler.

  * @retval none
  */
void Screen_out(){
	//if (scr.cur_enable)
		//scr.curr_on();
	/*symbol from buffer which is displayed*/
	static uint8_t bf=0; 
	if (scr.digits_period == 0)
		scr.digits_period = SCR_PERIOD;
		/*finding the position of symbol '.'*/
	char* point_pos = std::find (scr.buffer,scr.buffer+4,'.');
		/*turn screen on if pause elapsed*/
	if (scr.digits_period == SCR_PERIOD){
		/*if '.' is in buffer*/
		if (point_pos != scr.buffer+4){
			/*deleting the point from buffer by moving elemets to the left*/
			scr.delete_p(point_pos);
			/*element which is previuos to symbol '.'  . We should add symbol '.' to element and dispay it*/
			if (scr.buffer + bf == point_pos-1) 
				Dig_num_out(scr.buffer[bf],true);
			/*display all other elements*/
			else Dig_num_out(scr.buffer[bf]);
		}
		/*if there is no symbol '.' in buffer */
		else Dig_num_out(scr.buffer[bf]);
		digits[bf].BSRR_On();
	}
	/*turn off display while pause*/
	else if (scr.digits_period == SCR_PAUSE_TIME){
			digits[bf].BSRR_Off();
			++bf;
	}
	--scr.digits_period;
	
	if (bf == 4)
		bf=0;	
};





