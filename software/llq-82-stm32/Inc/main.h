/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY_POS12_Pin GPIO_PIN_13
#define KEY_POS12_GPIO_Port GPIOC
#define KEY_POS13_Pin GPIO_PIN_14
#define KEY_POS13_GPIO_Port GPIOC
#define KEY_POS14_Pin GPIO_PIN_15
#define KEY_POS14_GPIO_Port GPIOC
#define KEY_POS1_Pin GPIO_PIN_0
#define KEY_POS1_GPIO_Port GPIOC
#define KEY_POS2_Pin GPIO_PIN_1
#define KEY_POS2_GPIO_Port GPIOC
#define KEY_POS3_Pin GPIO_PIN_2
#define KEY_POS3_GPIO_Port GPIOC
#define KEY_POS4_Pin GPIO_PIN_3
#define KEY_POS4_GPIO_Port GPIOC
#define SW_KEY_A_Pin GPIO_PIN_0
#define SW_KEY_A_GPIO_Port GPIOA
#define SW_KEY_B_Pin GPIO_PIN_1
#define SW_KEY_B_GPIO_Port GPIOA
#define SW_KEY_NET2_Pin GPIO_PIN_2
#define SW_KEY_NET2_GPIO_Port GPIOA
#define LCD_BLK_Pin GPIO_PIN_3
#define LCD_BLK_GPIO_Port GPIOA
#define LCD_RS_Pin GPIO_PIN_4
#define LCD_RS_GPIO_Port GPIOA
#define LCD_SCL_Pin GPIO_PIN_5
#define LCD_SCL_GPIO_Port GPIOA
#define LDC_RES_Pin GPIO_PIN_6
#define LDC_RES_GPIO_Port GPIOA
#define LCD_SDA_Pin GPIO_PIN_7
#define LCD_SDA_GPIO_Port GPIOA
#define KEY_POS5_Pin GPIO_PIN_4
#define KEY_POS5_GPIO_Port GPIOC
#define KEY_POS6_Pin GPIO_PIN_5
#define KEY_POS6_GPIO_Port GPIOC
#define LEFT_KEY_Pin GPIO_PIN_0
#define LEFT_KEY_GPIO_Port GPIOB
#define KEY_SEG1_Pin GPIO_PIN_1
#define KEY_SEG1_GPIO_Port GPIOB
#define LED_EN_Pin GPIO_PIN_2
#define LED_EN_GPIO_Port GPIOB
#define KEY_SEG2_Pin GPIO_PIN_10
#define KEY_SEG2_GPIO_Port GPIOB
#define KEY_SEG3_Pin GPIO_PIN_11
#define KEY_SEG3_GPIO_Port GPIOB
#define KEY_SEG4_Pin GPIO_PIN_12
#define KEY_SEG4_GPIO_Port GPIOB
#define KEY_SEG5_Pin GPIO_PIN_13
#define KEY_SEG5_GPIO_Port GPIOB
#define KEY_SEG6_Pin GPIO_PIN_14
#define KEY_SEG6_GPIO_Port GPIOB
#define ESP32_EN_Pin GPIO_PIN_15
#define ESP32_EN_GPIO_Port GPIOB
#define KEY_POS7_Pin GPIO_PIN_6
#define KEY_POS7_GPIO_Port GPIOC
#define KEY_POS8_Pin GPIO_PIN_7
#define KEY_POS8_GPIO_Port GPIOC
#define KEY_POS9_Pin GPIO_PIN_8
#define KEY_POS9_GPIO_Port GPIOC
#define SW_SDA_Pin GPIO_PIN_9
#define SW_SDA_GPIO_Port GPIOC
#define SW_SCL_Pin GPIO_PIN_8
#define SW_SCL_GPIO_Port GPIOA
#define LCD_EN_Pin GPIO_PIN_15
#define LCD_EN_GPIO_Port GPIOA
#define KEY_POS10_Pin GPIO_PIN_12
#define KEY_POS10_GPIO_Port GPIOC
#define KEY_POS11_Pin GPIO_PIN_2
#define KEY_POS11_GPIO_Port GPIOD
#define TOUCH_IRQ_Pin GPIO_PIN_3
#define TOUCH_IRQ_GPIO_Port GPIOB
#define TOUCH_IRQ_EXTI_IRQn EXTI3_IRQn
#define LEFT_A_Pin GPIO_PIN_4
#define LEFT_A_GPIO_Port GPIOB
#define LEFT_B_Pin GPIO_PIN_5
#define LEFT_B_GPIO_Port GPIOB
#define WS2812_CTR1_Pin GPIO_PIN_6
#define WS2812_CTR1_GPIO_Port GPIOB
#define WS2812_CTR2_Pin GPIO_PIN_7
#define WS2812_CTR2_GPIO_Port GPIOB
#define WS2812_CTR3_Pin GPIO_PIN_8
#define WS2812_CTR3_GPIO_Port GPIOB
#define SW_IRQ_Pin GPIO_PIN_9
#define SW_IRQ_GPIO_Port GPIOB
#define SW_IRQ_EXTI_IRQn EXTI9_5_IRQn
/* USER CODE BEGIN Private defines */
typedef signed char int8_t;
typedef signed short int int16_t;
typedef signed int int32_t;
typedef signed long long int64_t;

/* exact-width unsigned integer types */
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef unsigned char bool_t;
typedef float fp32;
typedef double fp64;


typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;
typedef unsigned long long u64;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
