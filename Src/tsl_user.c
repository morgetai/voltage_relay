/**
  ******************************************************************************
  * @file    STM32F0518_Ex01_3TKeys_EVAL\src\tsl_user.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    04-April-2014
  * @brief   Touch-Sensing user configuration and api file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 
/* Includes ------------------------------------------------------------------*/
#include "tsl_user.h"
#include "main.h"
/* Extern variables ---------------------------------------------------------*/
extern MAINPTR PTR_TO_MAIN ;
//==============================================================================
// Channels
//==============================================================================

// Source and Configuration (ROM)
CONST TSL_ChannelSrc_T MyChannels_Src[TSLPRM_TOTAL_CHANNELS] =
{
  { CHANNEL_0_SRC, CHANNEL_0_IO_MSK, CHANNEL_0_GRP_MSK },
  { CHANNEL_1_SRC, CHANNEL_1_IO_MSK, CHANNEL_1_GRP_MSK },
  { CHANNEL_2_SRC, CHANNEL_2_IO_MSK, CHANNEL_2_GRP_MSK },
	{ CHANNEL_3_SRC, CHANNEL_3_IO_MSK, CHANNEL_3_GRP_MSK },
	{ CHANNEL_4_SRC, CHANNEL_4_IO_MSK, CHANNEL_4_GRP_MSK }
};

// Destination (ROM)
CONST TSL_ChannelDest_T MyChannels_Dest[TSLPRM_TOTAL_CHANNELS] =
{
  { CHANNEL_0_DEST },
  { CHANNEL_1_DEST },
  { CHANNEL_2_DEST },
	{ CHANNEL_3_DEST },
	{ CHANNEL_4_DEST }
};

// Data (RAM)
TSL_ChannelData_T MyChannels_Data[TSLPRM_TOTAL_CHANNELS];

//==============================================================================
// Banks
//==============================================================================

// List (ROM)
CONST TSL_Bank_T MyBanks[TSLPRM_TOTAL_BANKS] = {
  {&MyChannels_Src[0], &MyChannels_Dest[0], MyChannels_Data, BANK_0_NBCHANNELS, BANK_0_MSK_CHANNELS, BANK_0_MSK_GROUPS},
  {&MyChannels_Src[1], &MyChannels_Dest[1], MyChannels_Data, BANK_1_NBCHANNELS, BANK_1_MSK_CHANNELS, BANK_1_MSK_GROUPS},
  {&MyChannels_Src[2], &MyChannels_Dest[2], MyChannels_Data, BANK_2_NBCHANNELS, BANK_2_MSK_CHANNELS, BANK_2_MSK_GROUPS},
	{&MyChannels_Src[3], &MyChannels_Dest[3], MyChannels_Data, BANK_3_NBCHANNELS, BANK_3_MSK_CHANNELS, BANK_3_MSK_GROUPS},
	{&MyChannels_Src[4], &MyChannels_Dest[4], MyChannels_Data, BANK_4_NBCHANNELS, BANK_4_MSK_CHANNELS, BANK_4_MSK_GROUPS}
};

//==============================================================================
// TouchKeys
//==============================================================================

// Data (RAM)
TSL_TouchKeyData_T MyTKeys_Data[TSLPRM_TOTAL_TKEYS];

// Parameters (RAM)
TSL_TouchKeyParam_T MyTKeys_Param[TSLPRM_TOTAL_TKEYS];

// State Machine (ROM)

void MyTKeys_ErrorStateProcess(void);
void MyTKeys_OffStateProcess(void);

CONST TSL_State_T MyTKeys_StateMachine[] =
{
  // Calibration states
  /*  0 */ { TSL_STATEMASK_CALIB,              TSL_tkey_CalibrationStateProcess },
  /*  1 */ { TSL_STATEMASK_DEB_CALIB,          TSL_tkey_DebCalibrationStateProcess },
  // Release states 
  /*  2 */ { TSL_STATEMASK_RELEASE,            TSL_tkey_ReleaseStateProcess },
#if TSLPRM_USE_PROX > 0
  /*  3 */ { TSL_STATEMASK_DEB_RELEASE_PROX,   TSL_tkey_DebReleaseProxStateProcess },
#else
  /*  3 */ { TSL_STATEMASK_DEB_RELEASE_PROX,   0 },
#endif
  /*  4 */ { TSL_STATEMASK_DEB_RELEASE_DETECT, TSL_tkey_DebReleaseDetectStateProcess },
  /*  5 */ { TSL_STATEMASK_DEB_RELEASE_TOUCH,  TSL_tkey_DebReleaseTouchStateProcess },
#if TSLPRM_USE_PROX > 0
  // Proximity states
  /*  6 */ { TSL_STATEMASK_PROX,               TSL_tkey_ProxStateProcess },
  /*  7 */ { TSL_STATEMASK_DEB_PROX,           TSL_tkey_DebProxStateProcess },
  /*  8 */ { TSL_STATEMASK_DEB_PROX_DETECT,    TSL_tkey_DebProxDetectStateProcess },
  /*  9 */ { TSL_STATEMASK_DEB_PROX_TOUCH,     TSL_tkey_DebProxTouchStateProcess },  
#else
  /*  6 */ { TSL_STATEMASK_PROX,               0 },
  /*  7 */ { TSL_STATEMASK_DEB_PROX,           0 },
  /*  8 */ { TSL_STATEMASK_DEB_PROX_DETECT,    0 },
  /*  9 */ { TSL_STATEMASK_DEB_PROX_TOUCH,     0 },
#endif
  // Detect states
  /* 10 */ { TSL_STATEMASK_DETECT,             TSL_tkey_DetectStateProcess },
  /* 11 */ { TSL_STATEMASK_DEB_DETECT,         TSL_tkey_DebDetectStateProcess },
  // Touch state
  /* 12 */ { TSL_STATEMASK_TOUCH,              TSL_tkey_TouchStateProcess },
  // Error states
  /* 13 */ { TSL_STATEMASK_ERROR,              MyTKeys_ErrorStateProcess },
  /* 14 */ { TSL_STATEMASK_DEB_ERROR_CALIB,    TSL_tkey_DebErrorStateProcess },
  /* 15 */ { TSL_STATEMASK_DEB_ERROR_RELEASE,  TSL_tkey_DebErrorStateProcess },
  /* 16 */ { TSL_STATEMASK_DEB_ERROR_PROX,     TSL_tkey_DebErrorStateProcess },
  /* 17 */ { TSL_STATEMASK_DEB_ERROR_DETECT,   TSL_tkey_DebErrorStateProcess },
  /* 18 */ { TSL_STATEMASK_DEB_ERROR_TOUCH,    TSL_tkey_DebErrorStateProcess },
  // Other states
  /* 19 */ { TSL_STATEMASK_OFF,                MyTKeys_OffStateProcess }
};

// Methods for "extended" type (ROM)
CONST TSL_TouchKeyMethods_T MyTKeys_Methods =
{
  TSL_tkey_Init,
  TSL_tkey_Process
};

// TouchKeys list (ROM)
CONST TSL_TouchKey_T MyTKeys[TSLPRM_TOTAL_TKEYS] =
{
  { &MyTKeys_Data[0], &MyTKeys_Param[0], &MyChannels_Data[CHANNEL_0_DEST], MyTKeys_StateMachine, &MyTKeys_Methods },
  { &MyTKeys_Data[1], &MyTKeys_Param[1], &MyChannels_Data[CHANNEL_1_DEST], MyTKeys_StateMachine, &MyTKeys_Methods },
  { &MyTKeys_Data[2], &MyTKeys_Param[2], &MyChannels_Data[CHANNEL_2_DEST], MyTKeys_StateMachine, &MyTKeys_Methods },
	{ &MyTKeys_Data[3], &MyTKeys_Param[3], &MyChannels_Data[CHANNEL_3_DEST], MyTKeys_StateMachine, &MyTKeys_Methods },
	{ &MyTKeys_Data[4], &MyTKeys_Param[4], &MyChannels_Data[CHANNEL_4_DEST], MyTKeys_StateMachine, &MyTKeys_Methods },
};

//==============================================================================
// Generic Objects
//==============================================================================

// List (ROM)
CONST TSL_Object_T MyObjects[TSLPRM_TOTAL_OBJECTS] =
{
  { TSL_OBJ_TOUCHKEY, (TSL_TouchKey_T *)&MyTKeys[0] },
  { TSL_OBJ_TOUCHKEY, (TSL_TouchKey_T *)&MyTKeys[1] },
  { TSL_OBJ_TOUCHKEY, (TSL_TouchKey_T *)&MyTKeys[2] },
	{ TSL_OBJ_TOUCHKEY, (TSL_TouchKey_T *)&MyTKeys[3] },
	{ TSL_OBJ_TOUCHKEY, (TSL_TouchKey_T *)&MyTKeys[4] }
};

// Group (RAM)
TSL_ObjectGroup_T MyObjGroup =
{
  &MyObjects[0],        // First object
  TSLPRM_TOTAL_OBJECTS, // Number of objects
  0x00,                 // State mask reset value
  TSL_STATE_NOT_CHANGED // Current state
};

//==============================================================================
// TSL Common Parameters placed in RAM or ROM
// --> external declaration in tsl_conf.h
//==============================================================================

TSL_Params_T TSL_Params =
{
  TSLPRM_ACQ_MIN,
  TSLPRM_ACQ_MAX,
  TSLPRM_CALIB_SAMPLES,
  TSLPRM_DTO,
#if TSLPRM_TOTAL_TKEYS > 0  
  MyTKeys_StateMachine,   // Default state machine for TKeys
  &MyTKeys_Methods,       // Default methods for TKeys
#endif
#if TSLPRM_TOTAL_LNRTS > 0
  MyLinRots_StateMachine, // Default state machine for LinRots
  &MyLinRots_Methods      // Default methods for LinRots
#endif
};

/* Private functions prototype -----------------------------------------------*/

/* Global variables ----------------------------------------------------------*/
__IO uint32_t Gv_SystickCounter;
__IO TSL_tTick_ms_T Gv_ECS_last_tick; // Hold the last time value for ECS
__IO uint32_t Gv_EOA; // Set by TS interrupt routine to indicate the End Of Acquisition


/**
  * @brief  Initialize the STMTouch Driver
  * @param  None
  * @retval None
  */
void TSL_user_Init(void)
{
#if TSLPRM_TSC_GPIO_CONFIG == 0    
  // Automatic GPIO configuration not selected:
  // This function must be created by the user to initialize the Touch Sensing GPIOs.
  TSL_user_InitGPIOs(); 
#endif
  
  TSL_obj_GroupInit(&MyObjGroup); // Init Objects
  
  TSL_Init(MyBanks); // Init timing and acquisition modules
  
  TSL_user_SetThresholds(); // Init thresholds for each object individually
}


/**
  * @brief  Execute STMTouch Driver main State machine
  * @param  None
  * @retval status Return TSL_STATUS_OK if the acquisition is done
  */
TSL_Status_enum_T TSL_user_Action(void)
{
  static uint32_t idx_bank = 0;
  static uint32_t config_done = 0;
  TSL_Status_enum_T status;

  // Configure bank
  if (!config_done)
  {
    TSL_acq_BankConfig(idx_bank); // Configure Bank
    TSL_acq_BankStartAcq(); // Start Bank acquisition
    config_done = 1;
#if TSLPRM_USE_ACQ_INTERRUPT > 0
    Gv_EOA = 0; // Will be set by the TS interrupt routine
#endif    
  }

  // Check end of acquisition
#if TSLPRM_USE_ACQ_INTERRUPT > 0
  if (Gv_EOA) // Set by the TS interrupt routine
#else
  if (TSL_acq_BankWaitEOC() == TSL_STATUS_OK)
#endif
  {
    STMSTUDIO_LOCK;
    TSL_acq_BankGetResult(idx_bank, 0, 0); // Get Bank Result
    STMSTUDIO_UNLOCK;
    idx_bank++; // Next bank
    config_done = 0;
  }

  // Process objects, DxS and ECS
  // Check if all banks have been acquired
  if (idx_bank > TSLPRM_TOTAL_BANKS-1)
  {
    // Reset flags for next banks acquisition
    idx_bank = 0;
    config_done = 0;
    
    // Process Objects
    TSL_obj_GroupProcess(&MyObjGroup);
    
    // DxS processing (if TSLPRM_USE_DXS option is set)
    TSL_dxs_FirstObj(&MyObjGroup);
    
    // ECS every 100ms
    if (TSL_tim_CheckDelay_ms(100, &Gv_ECS_last_tick) == TSL_STATUS_OK)
    {
      if (TSL_ecs_Process(&MyObjGroup) == TSL_STATUS_OK)
      {
        ;
      }
      else
      {
        ;
      }
    }
    
    status = TSL_STATUS_OK; // All banks have been acquired and sensors processed
    
  }
  else
  {
    status = TSL_STATUS_BUSY;
  }
  
  return status;
}


/**
  * @brief  Set thresholds for each object (optional).
  * @param  None
  * @retval None
  */
void TSL_user_SetThresholds(void)
{
  // Example: Decrease the Detect thresholds for the TKEY 0
  MyTKeys_Param[1].DetectInTh = 30;
  MyTKeys_Param[1].DetectOutTh = 20;
	MyTKeys_Param[3].DetectInTh = 30;
  MyTKeys_Param[3].DetectOutTh = 20;
}

void MyLinRots_ErrorStateProcess(void)
{
  // Add here your own processing when a sensor is in Error state

  for (uint16_t i; i != 1000; ++i)
  {
		;
  }
	
	PTR_TO_MAIN ();
}

void MyLinRots_OffStateProcess(void)
{
	while(1)
	{
		;
	};
  // Add here your own processing when a sensor is in Off state
	//TSL_linrot_CalibrationStateProcess();
}

void MyTKeys_OffStateProcess(void)
{
	while (1)
	{
		;
	};
}
void MyTKeys_ErrorStateProcess (void)
{
	TSL_tkey_CalibrationStateProcess();
	PTR_TO_MAIN ();
}






/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
