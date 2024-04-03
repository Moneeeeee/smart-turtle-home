/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "retarget.h"
#include "esp8266.h"
#include "onenet.h"
#include "MqttKit.h"

#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern unsigned char aRxBuffer;
extern unsigned char ESP01S_buf[64];
extern unsigned short ESP01S_cnt, ESP01S_cntPre;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
    const char *devSubTopic[] = {"/mysmartturtle/sub"};
    const char devPubTopic[] = "/mysmartturtle/pub";
    unsigned short timeCount = 0;	//
    char PUB_BUF[128];//上传数据的PUB
    unsigned char *dataPtr = NULL;
    uint16_t count = 0;


  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */



  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
//  RetargetInit(&huart1);

    OLED_Init();
//    DS18B20_Init_Check();
    OLED_Clear();
    BH1750_Init();
//    OLED_ShowNum(0,0,1,16,12);

//    DWT_Delay_Init();

//    DS18B20_Init();

//  HAL_UART_Receive_IT(&huart2, &aRxBuffer, 1); // 启动中断接收
//  ESP01S_Init();  //8266初始
//  while(OneNet_DevLink())  //接入onenet
//  ESP01S_Clear();    //*/
//  OneNet_Subscribe(devSubTopic, 1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
//      OLED_ShowString(24,0,"Init OK",12);
      HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_10);



//      Motor_Backward();
//      Motor_Forward();

//光照阈值
      if (Lumen > Lumen_Thresold){
          Lumen_Flag = 1;
      }else{
          Lumen_Flag = 0;
      };
//TDS阈值
      if (TDS > TDS_Thresold){
          TDS_Flag = 1;
      }else{
          TDS_Flag = 0;
      };
//温度阈值
      if (temperature > temperature_Thresold){
          temperature_Flag = 1;
      }else{
          temperature_Flag = 0;
      };
//体重
      if (Weight > Weight_Thresold){
          Weight_Flag = 1;
      }else{
          Weight_Flag = 0;
      };

//投喂
      if (Eat_Flag){
          Motor_Forward();
      } else{
          Weight_Flag = 0;
      };

      Value_GY30();
      OLED_ShowNum(0,0,(uint32_t)Lumen,4,12);

      temperature = DS18B20_Get_Temperature();
      OLED_ShowNum(0,1,(uint32_t)temperature,4,12);

      TDS_Check();
      OLED_ShowNum(0,2,(uint32_t)TDS,4,12);

      Read_Weigh();
      OLED_ShowNum(0,3,(uint32_t)Weight,4,12);

      HAL_Delay(10);

//      if(++timeCount >= 100){
//          sprintf(PUB_BUF,"{\"Temp\":%d,\"TDS\":%d,\"Lumen\":%d}",
//                  temperature,TDS,Lumen);
//          OneNet_Publish(devPubTopic, PUB_BUF);
//
//          timeCount = 0;
//          ESP01S_Clear();
//      }
//      dataPtr = ESP01S_GetIPD(3);
//      if(dataPtr != NULL)
//          OneNet_RevPro(dataPtr);


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

    if(ESP01S_cnt >= sizeof(ESP01S_buf))  //溢出判断
    {
        ESP01S_cnt = 0;
        memset(ESP01S_buf,0x00,sizeof(ESP01S_buf));
//        HAL_UART_Transmit(&huart1, (uint8_t *)"接收缓存溢出", 10,0xFFFF);
    }
    else
    {
        ESP01S_buf[ESP01S_cnt++] = aRxBuffer;   //接收数据转存
//		  if(aRxBuffer=='1')  HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
//        if(aRxBuffer=='0')  HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
    }

    HAL_UART_Receive_IT(&huart2, &aRxBuffer, 1);   //再开启接收中�?????????????????????????????????????????????????????
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
