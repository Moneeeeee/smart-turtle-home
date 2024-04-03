//
// Created by Monee on 2024/4/3.
//
#include "ULN.h"
// 正转序列
uint16_t F_Rotation[4] = {ULN_1_Pin, ULN_2_Pin, ULN_3_Pin, ULN_4_Pin};
// 反转序列
uint16_t B_Rotation[4] = {ULN_4_Pin, ULN_3_Pin, ULN_2_Pin, ULN_1_Pin};

//// 封装的延迟函数
//void Delay_us(uint32_t us)
//{
//    __HAL_TIM_SET_COUNTER(&htim1, 0); // 假设htim1已经配置好
//    while (__HAL_TIM_GET_COUNTER(&htim1) < us);
//}
// 单步执行函数，用于正转或反转
void StepMotor_TakeStep(uint16_t pin)
{
    // 激活当前步
    HAL_GPIO_WritePin(ULN_1_GPIO_Port, ULN_1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ULN_2_GPIO_Port, ULN_2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ULN_3_GPIO_Port, ULN_3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ULN_4_GPIO_Port, ULN_4_Pin, GPIO_PIN_RESET);

    if(pin == ULN_1_Pin) HAL_GPIO_WritePin(ULN_1_GPIO_Port, ULN_1_Pin, GPIO_PIN_SET);
    else if(pin == ULN_2_Pin) HAL_GPIO_WritePin(ULN_2_GPIO_Port, ULN_2_Pin, GPIO_PIN_SET);
    else if(pin == ULN_3_Pin) HAL_GPIO_WritePin(ULN_3_GPIO_Port, ULN_3_Pin, GPIO_PIN_SET);
    else if(pin == ULN_4_Pin) HAL_GPIO_WritePin(ULN_4_GPIO_Port, ULN_4_Pin, GPIO_PIN_SET);

    // 维持当前状态一段时间
    HAL_Delay(1); // 调整这里以改变步进速度
}

// 步进电机正转函数
void Motor_Forward(void)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        StepMotor_TakeStep(F_Rotation[i]);
    }
}
// 步进电机反转函数
void Motor_Backward(void)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        StepMotor_TakeStep(B_Rotation[i]);
    }
}
// 停止电机的函数现在将motorRunning设置为false
void Motor_Stop(void)
{
    // 停止所有的引脚输出
    HAL_GPIO_WritePin(ULN_1_GPIO_Port, ULN_1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ULN_2_GPIO_Port, ULN_2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ULN_3_GPIO_Port, ULN_3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ULN_4_GPIO_Port, ULN_4_Pin, GPIO_PIN_RESET);
}