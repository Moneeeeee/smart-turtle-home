//
// Created by Monee on 2024/4/1.
//
#include "TDS.h"
uint8_t TDS = 0;

uint16_t ADC_IN_1(void) //ADC采集程序
{
    HAL_ADC_Start(&hadc1);//开始ADC采集
    HAL_ADC_PollForConversion(&hadc1,100);//等待采集结束
    if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))//读取ADC完成标志位
    {
        return HAL_ADC_GetValue(&hadc1);//读出ADC数值
    }
    return 0;
}


void TDS_Check()
{
    HAL_ADCEx_Calibration_Start(&hadc1);

    TDS = ADC_IN_1();

}