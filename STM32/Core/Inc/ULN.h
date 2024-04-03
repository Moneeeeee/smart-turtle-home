//
// Created by Monee on 2024/4/3.
//

#ifndef STM32_ULN_H
#define STM32_ULN_H
#include "main.h"
void StepMotor_TakeStep(uint16_t pin);
void Motor_Forward(void);
void Motor_Backward(void);
void Motor_Stop(void);
#endif //STM32_ULN_H
