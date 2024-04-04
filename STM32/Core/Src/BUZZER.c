//
// Created by Monee on 2024/4/3.
//
#include "BUZZER.h"
void BEEP_Init(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
}


void BEEP_ON(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
}
void BEEP_OFF(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
}


void Buzzer_Beep(uint32_t onTimeMs, uint32_t offTimeMs, uint8_t repetitions) {
    for(uint8_t i = 0; i < repetitions; i++) {
        // Turn on the buzzer
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
        // Wait for 'onTimeMs' milliseconds
        HAL_Delay(onTimeMs);
        // Turn off the buzzer
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
        // Wait for 'offTimeMs' milliseconds
        HAL_Delay(offTimeMs);
    }
}
