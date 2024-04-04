//
// Created by Monee on 2024/4/2.
//

#ifndef STM32_HX711_H
#define STM32_HX711_H
#include "main.h"




extern uint32_t HX711_Buffer;
extern uint32_t Weight_Maopi;
extern uint32_t Weight_Shiwu;
extern uint8_t Flag_Error;
extern uint32_t Weight ;
extern uint32_t preWeight ;
void ReadWeigh_Init(void);
uint32_t Read_Weigh(void);
uint32_t Read_Average(void);
float MovingAverageFilter(uint32_t input);
void Weight_check(void);

#endif //STM32_HX711_H
