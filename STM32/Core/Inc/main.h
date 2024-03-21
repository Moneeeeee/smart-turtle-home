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
#define ESP_RST_Pin GPIO_PIN_1
#define ESP_RST_GPIO_Port GPIOA
#define ESP_TX_Pin GPIO_PIN_2
#define ESP_TX_GPIO_Port GPIOA
#define ESP_RX_Pin GPIO_PIN_3
#define ESP_RX_GPIO_Port GPIOA
#define DS18_Pin GPIO_PIN_4
#define DS18_GPIO_Port GPIOA
#define ULN1_Pin GPIO_PIN_5
#define ULN1_GPIO_Port GPIOA
#define ULN2_Pin GPIO_PIN_6
#define ULN2_GPIO_Port GPIOA
#define ULN3_Pin GPIO_PIN_7
#define ULN3_GPIO_Port GPIOA
#define ULN4_Pin GPIO_PIN_0
#define ULN4_GPIO_Port GPIOB
#define KEY_2_Pin GPIO_PIN_12
#define KEY_2_GPIO_Port GPIOB
#define KEY_3_Pin GPIO_PIN_13
#define KEY_3_GPIO_Port GPIOB
#define KEY_4_Pin GPIO_PIN_14
#define KEY_4_GPIO_Port GPIOB
#define KEY_5_Pin GPIO_PIN_15
#define KEY_5_GPIO_Port GPIOB
#define KEY_1_Pin GPIO_PIN_8
#define KEY_1_GPIO_Port GPIOA
#define BEEP_Pin GPIO_PIN_11
#define BEEP_GPIO_Port GPIOA
#define UVB_Pin GPIO_PIN_4
#define UVB_GPIO_Port GPIOB
#define UVA_Pin GPIO_PIN_5
#define UVA_GPIO_Port GPIOB
#define OLED_SCL_Pin GPIO_PIN_6
#define OLED_SCL_GPIO_Port GPIOB
#define OLED_SDA_Pin GPIO_PIN_7
#define OLED_SDA_GPIO_Port GPIOB
#define HX711_SDA_Pin GPIO_PIN_8
#define HX711_SDA_GPIO_Port GPIOB
#define HX711_SCL_Pin GPIO_PIN_9
#define HX711_SCL_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
