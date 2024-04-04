//
// Created by Monee on 2024/4/3.
//
#include "Control.h"

volatile uint8_t Lumen_Thresold =100;
volatile uint8_t TDS_Thresold =100;
volatile uint8_t temperature_Thresold =16;
volatile uint32_t Weight_Thresold =500;

volatile uint8_t Eat_Flag =0;
volatile uint8_t Lumen_Flag = 0;
volatile uint8_t temperature_Flag = 0;
volatile uint8_t Weight_Flag = 0;
volatile uint8_t TDS_Flag = 0;


void Pump(uint8_t state){
    if (state){
        HAL_GPIO_WritePin(Pump_Change_GPIO_Port,Pump_Change_Pin,GPIO_PIN_SET);
    }
    else if(state == 0){
        HAL_GPIO_WritePin(Pump_Change_GPIO_Port,Pump_Change_Pin,GPIO_PIN_RESET);
    }
}
void Heat(uint8_t state){
    if (state){
        HAL_GPIO_WritePin(Pump_Change_GPIO_Port,Pump_Change_Pin,GPIO_PIN_SET);
    }
    else if(state == 0){
        HAL_GPIO_WritePin(Pump_Change_GPIO_Port,Pump_Change_Pin,GPIO_PIN_RESET);
    }
}
void UVA(uint8_t state){
    if (state){
        HAL_GPIO_WritePin(UVA_GPIO_Port,UVA_Pin,GPIO_PIN_SET);
    }
    else if(state == 0){
        HAL_GPIO_WritePin(UVA_GPIO_Port,UVA_Pin,GPIO_PIN_RESET);
    }
}
void UVB(uint8_t state){
    if (state){
        HAL_GPIO_WritePin(UVB_GPIO_Port,UVB_Pin,GPIO_PIN_SET);
    }
    else if(state == 0){
        HAL_GPIO_WritePin(UVB_GPIO_Port,UVB_Pin,GPIO_PIN_RESET);
    }
}

void Data_Get(){

    Value_GY30();

    temperature = DS18B20_Get_Temperature();

    TDS_Check();

//    Read_Weigh();

}
void OLED_Show(void){


//    OLED_ShowNum(40,0,(uint32_t)Lumen,4,12);
//    OLED_ShowNum(40,1,(uint32_t)temperature,4,12);
//    OLED_ShowNum(40,2,(uint32_t)TDS,4,12);
//    OLED_ShowNum(40,3,(uint32_t)Weight,4,12);

    OLED_ShowString(5,7,"Lum",12);
    OLED_ShowString(35,7,"Tem",12);
    OLED_ShowString(65,7,"TDS",12);
    OLED_ShowString(95,7,"Wet",12);
    OLED_ShowNum(5, 6,Lumen , 2, 12);
    OLED_ShowNum(35, 6, temperature, 2, 12);
    OLED_ShowNum(65, 6, TDS, 3, 12);
    OLED_ShowNum(95,6,Weight,3,12);


    OLED_ShowString(60, 0,"LumT:", 12);
    OLED_ShowString(60, 2, "TemT:", 12);
    OLED_ShowString(60, 4, "TDST:", 12);
    OLED_ShowNum(100, 0, temperature_Thresold, 3, 12);
    OLED_ShowNum(100, 2, TDS_Thresold, 3, 12);
    OLED_ShowNum(100, 4, Weight_Thresold, 3, 12);

    OLED_ShowString(0, 0, "UVA:", 12);
    OLED_ShowString(0, 2, "UVB:", 12);
    OLED_ShowString(0, 4, "Pum:", 12);

    OLED_ShowString(30, 0, "ON", 12);
    OLED_ShowString(30, 2, "ON", 12);
    OLED_ShowString(30, 4, "ON", 12);
}

// 用于检查各种传感器数据是否超过阈值的函数
void Flag_Check(void) {
    Data_Get(); // 调用Data_Get()函数来获取最新的传感器数据

    // 光照阈值检查
    if (Lumen > Lumen_Thresold) {
        Lumen_Flag = 1; // 如果光照超过阈值，设置光照标志位为1
    } else {
        Lumen_Flag = 0; // 如果光照没有超过阈值，设置光照标志位为0
    };

    // TDS（总溶解固体）阈值检查
    if (TDS > TDS_Thresold) {
        TDS_Flag = 1; // 如果TDS超过阈值，设置TDS标志位为1
    } else {
        TDS_Flag = 0; // 如果TDS没有超过阈值，设置TDS标志位为0
    };

    // 温度阈值检查
    if (temperature > temperature_Thresold) {
        temperature_Flag = 1; // 如果温度超过阈值，设置温度标志位为1
    } else {
        temperature_Flag = 0; // 如果温度没有超过阈值，设置温度标志位为0
    };

    // 投喂标志位检查（暂时为空，可能在未来实现）
    if (Eat_Flag) {
        // 如果需要投喂，相关代码应该在这里实现
    }
}


// 控制主要功能的函数
void Control_Main(void){
    // 如果TDS水质检测过高
    if (TDS_Flag == 1){
        Pump(1); // 开启水泵进行过滤或换水
    }else {
        Pump(0); // 关闭水泵
    }
    // 如果温度过高
    if (temperature_Flag == 1){
        Heat(1); // 打开加热器
        UVA(1); // 打开UVA灯
    }else{
        Heat(0); // 关闭加热器
        UVA(0); // 关闭UVA灯
    }
    // 如果光照过强
    if (Lumen_Flag == 1){
        UVB(1); // 打开UVB灯
    }else{
        UVB(0); // 关闭UVB灯
    }
    // 如果设置了投喂标志位
    if(Eat_Flag){
        Motor_Forward(); // 步进电机正转，进行投喂
        HAL_Delay(5000); // 等待5秒
        Motor_Backward(); // 步进电机反转，结束投喂
        HAL_Delay(5000); // 再次等待5秒
    }
}


// 定时器中断回调函数
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    // 检查是不是TIM2触发了中断
    if (htim->Instance == TIM2) {
        //记录5s前的重量和现在的重量
        preWeight = Weight; // 记录上一个重量值
        Weight = Read_Weigh(); // 读取当前重量
        // 检查重量是否有突然的大幅度变化，超过设定的阈值
        if(abs(Weight - preWeight) > Weight_Thresold){
            Weight_Flag = 1; // 如果变化超过阈值，设置重量标志位为逃跑
        }
    }
}