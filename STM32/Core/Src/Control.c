//
// Created by Monee on 2024/4/4.
//
#include "Control.h"

uint8_t Lumen_Flag = 0;
uint32_t Lumen_Thresold = 0;

uint8_t TDS_Flag = 0;
uint32_t TDS_Thresold = 0;

uint8_t temperature_Flag = 0;
uint32_t temperature_Thresold = 0;

uint8_t Weight_Flag = 0;
uint32_t Weight_Thresold = 0;

uint8_t Eat_Flag = 0;

uint8_t UVA_Flag= 0;
uint8_t UVB_Flag= 0;
uint8_t Pump_Flag = 0;

void Flag_Set(void){
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
        HAL_Delay(5000);
        Motor_Backward();
        HAL_Delay(5000);
    }

}

void Data_Get(void){

    Read_Weigh();
    TDS_Check();
    Value_GY30();
//    DS18B20_Get_Temperature();
}

void OLED_Data_Show(void){
    OLED_ShowString(5, 7,"Tem",12);
    OLED_ShowString(35,7,"Lum",12);
    OLED_ShowString(65,7,"TDS",12);
    OLED_ShowString(95,7,"Wet",12);

    OLED_ShowNum(5, 6, temperature, 2, 12);
    OLED_ShowNum(35, 6, Lumen, 2, 12);
    OLED_ShowNum(65, 6, TDS, 3, 12);
    OLED_ShowNum(95,6,Weight,3,12);


    OLED_ShowString(60, 0,"TemT:", 12);
    OLED_ShowString(60, 2, "LumT:", 12);
    OLED_ShowString(60, 4, "TDST:", 12);

    OLED_ShowNum(100, 0, temperature_Thresold, 2, 12);
    OLED_ShowNum(100, 2, Lumen_Thresold, 2, 12);
    OLED_ShowNum(100, 4, TDS_Thresold, 2, 12);


    OLED_ShowString(0, 0, "UVA:", 12);
    OLED_ShowString(30, 0, UVA_Flag == 1 ? "ON" : "OFF", 12);

    OLED_ShowString(0, 2, "UVB:", 12);
    OLED_ShowString(30, 2, UVB_Flag == 1 ? "ON" : "OFF", 12);

    OLED_ShowString(0, 4, "Pum:", 12);
    OLED_ShowString(30, 4, Pump_Flag == 1 ? "ON" : "OFF", 12);

}