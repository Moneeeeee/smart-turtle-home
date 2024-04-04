//
// Created by Monee on 2024/4/3.
//

#ifndef STM32_CONTROL_C_H
#define STM32_CONTROL_C_H
#include "main.h"
extern volatile uint8_t Lumen_Thresold;
extern volatile uint8_t TDS_Thresold;
extern volatile uint8_t temperature_Thresold;
extern volatile uint32_t Weight_Thresold;

extern volatile uint8_t Eat_Flag;
extern volatile uint8_t Lumen_Flag ;
extern volatile uint8_t temperature_Flag ;
extern volatile uint8_t Weight_Flag ;
extern volatile uint8_t TDS_Flag ;

void OLED_Show(void);

void Flag_Check(void);
void Pump(uint8_t state);
void Data_Get();
void UVA(uint8_t state);
void UVB(uint8_t state);
void Buzzer_Beep(uint32_t onTimeMs, uint32_t offTimeMs, uint8_t repetitions) ;
#endif //STM32_CONTROL_C_H
