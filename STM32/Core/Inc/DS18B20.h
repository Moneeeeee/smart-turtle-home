
#ifndef __DS18B20_H__
#define __DS18B20_H__

#include "gpio.h"
#include "tim.h"
//
////各部分温度数据
//typedef struct DS18B20_Temperature
//{
//    uint16_t Hun;
//    uint16_t Tens;
//    uint16_t Unit;
//    uint16_t Decimals1;
//    uint16_t Decimals2;
//    uint16_t TempDataAll;
//} DS18B20_TempDataTypeDef;
//extern DS18B20_TempDataTypeDef DS18B20_TempDataStruct;
//
//#define in 1
//#define out 2
//#define DS18B20_IO_Read() HAL_GPIO_ReadPin(DS18B20_GPIO_Port, DS18B20_Pin)
//#define DS18B20_IO_Write_1() HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_SET)
//#define DS18B20_IO_Write_0() HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_RESET)
//
void HAL_Delay_us(uint16_t Delay);
//uint16_t DS18B20_Init(void);
//void DS18B20_GetTemperature(void);

#define  DS18B20_DQ_OUT_HIGH       HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_SET)
#define  DS18B20_DQ_OUT_LOW        HAL_GPIO_WritePin(DS18B20_GPIO_Port, DS18B20_Pin, GPIO_PIN_RESET)
#define  DS18B20_DQ_IN             HAL_GPIO_ReadPin(DS18B20_GPIO_Port, DS18B20_Pin)

uint8_t DS18B20_Init(void);
short DS18B20_Get_Temperature(void);

extern float temperature;

void DS18B20_Init_Check(void);


#endif //__DS18B20_H__
