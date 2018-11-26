/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */
typedef void (*MAINPTR) (void);
void	_MAIN_FUNC ();
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define Voltage_IO_Pin GPIO_PIN_0
#define Voltage_IO_GPIO_Port GPIOA
#define Seg_E_Pin GPIO_PIN_1
#define Seg_E_GPIO_Port GPIOA
#define Aux_led_Pin GPIO_PIN_2
#define Aux_led_GPIO_Port GPIOA
#define Seg_D_Pin GPIO_PIN_3
#define Seg_D_GPIO_Port GPIOA
#define Seg_A_Pin GPIO_PIN_4
#define Seg_A_GPIO_Port GPIOA
#define Seg_Dp_Pin GPIO_PIN_5
#define Seg_Dp_GPIO_Port GPIOA
#define Seg_F_Pin GPIO_PIN_6
#define Seg_F_GPIO_Port GPIOA
#define Seg_C_Pin GPIO_PIN_7
#define Seg_C_GPIO_Port GPIOA
#define Dig_3_Pin GPIO_PIN_0
#define Dig_3_GPIO_Port GPIOB
#define Dig_1_Pin GPIO_PIN_1
#define Dig_1_GPIO_Port GPIOB
#define Seg_G_Pin GPIO_PIN_2
#define Seg_G_GPIO_Port GPIOB
#define Seg_B_Pin GPIO_PIN_10
#define Seg_B_GPIO_Port GPIOB
#define Dig_2_Pin GPIO_PIN_11
#define Dig_2_GPIO_Port GPIOB
#define SampCap_G6_Pin GPIO_PIN_12
#define SampCap_G6_GPIO_Port GPIOB
#define But_Ok_Pin GPIO_PIN_13
#define But_Ok_GPIO_Port GPIOB
#define But_Up_Pin GPIO_PIN_14
#define But_Up_GPIO_Port GPIOB
#define But_Left_Pin GPIO_PIN_9
#define But_Left_GPIO_Port GPIOA
#define But_Down_Pin GPIO_PIN_10
#define But_Down_GPIO_Port GPIOA
#define But_Esc_Pin GPIO_PIN_11
#define But_Esc_GPIO_Port GPIOA
#define SampCap_G4_Pin GPIO_PIN_12
#define SampCap_G4_GPIO_Port GPIOA
#define Piezo_1_Pin GPIO_PIN_6
#define Piezo_1_GPIO_Port GPIOB
#define Piezo_2_Pin GPIO_PIN_7
#define Piezo_2_GPIO_Port GPIOB
#define Relay_Pin GPIO_PIN_8
#define Relay_GPIO_Port GPIOB
#define Dig_4_Pin GPIO_PIN_9
#define Dig_4_GPIO_Port GPIOB

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(const char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
