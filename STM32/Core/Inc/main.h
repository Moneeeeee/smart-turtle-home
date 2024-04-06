/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "tim.h"
#include "adc.h"
#include "stdio.h"
//#include "core_cm4.h" // 根据你的MCU核心调整，例如STM32F4系列是CM4
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "retarget.h"
#include "esp8266.h"
#include "onenet.h"
#include "MqttKit.h"
#include "OLED.h"
#include "string.h"
#include "math.h"
#include "DS18B20.h"
#include "TDS.h"
#include "HX711.h"
#include "ULN.h"
#include "GY-302.h"
#include "Control.h"
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
#define DS18B20_Pin GPIO_PIN_0
#define DS18B20_GPIO_Port GPIOA
#define TDS_Pin GPIO_PIN_1
#define TDS_GPIO_Port GPIOA
#define ESP_EN_Pin GPIO_PIN_5
#define ESP_EN_GPIO_Port GPIOA
#define BUZZER_Pin GPIO_PIN_0
#define BUZZER_GPIO_Port GPIOB
#define HX117_SCL_Pin GPIO_PIN_1
#define HX117_SCL_GPIO_Port GPIOB
#define HX711_SDA_Pin GPIO_PIN_2
#define HX711_SDA_GPIO_Port GPIOB
#define ULN_2_Pin GPIO_PIN_13
#define ULN_2_GPIO_Port GPIOB
#define ULN_4_Pin GPIO_PIN_14
#define ULN_4_GPIO_Port GPIOB
#define ULN_1_Pin GPIO_PIN_8
#define ULN_1_GPIO_Port GPIOA
#define ULN_3_Pin GPIO_PIN_9
#define ULN_3_GPIO_Port GPIOA
#define UVB_Pin GPIO_PIN_10
#define UVB_GPIO_Port GPIOA
#define UVA_Pin GPIO_PIN_11
#define UVA_GPIO_Port GPIOA
#define LED_1_Pin GPIO_PIN_15
#define LED_1_GPIO_Port GPIOA
#define OLED_SCL_Pin GPIO_PIN_3
#define OLED_SCL_GPIO_Port GPIOB
#define OLED_SDA_Pin GPIO_PIN_4
#define OLED_SDA_GPIO_Port GPIOB
#define BH1750_SCL_Pin GPIO_PIN_6
#define BH1750_SCL_GPIO_Port GPIOB
#define BH1750_SDA_Pin GPIO_PIN_7
#define BH1750_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
