//
// Created by Monee on 2024/4/4.
//

#ifndef STM32_CONTROL_H
#define STM32_CONTROL_H
#include "main.h"

extern uint8_t Lumen_Flag ;
extern uint32_t Lumen_Thresold ;

extern uint8_t TDS_Flag ;
extern uint32_t TDS_Thresold ;

extern uint8_t temperature_Flag ;
extern uint32_t temperature_Thresold ;

extern uint8_t Weight_Flag ;
extern uint32_t Weight_Thresold ;

extern uint8_t Eat_Flag ;

extern uint8_t UVA_Flag;
extern uint8_t UVB_Flag;
extern uint8_t Pump_Flag;
void Data_Get(void);
void Flag_Set(void);
void OLED_Data_Show(void);
#endif //STM32_CONTROL_H
