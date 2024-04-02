#include "HX711.h"



#define SDA_PIN GPIO_PIN_2
#define SCL_PIN GPIO_PIN_1
// 假设HX711的DOUT连接到GPIOB的Pin 2，SCK连接到PinS 3
#define HX711_DOUT_GPIO_PORT  GPIOB
#define HX711_DOUT_GPIO_PIN   GPIO_PIN_2
#define HX711_SCK_GPIO_PORT   GPIOB
#define HX711_SCK_GPIO_PIN    GPIO_PIN_1

// 读取DOUT引脚的宏定义
#define HX711_DOUT_READ()  HAL_GPIO_ReadPin(HX711_DOUT_GPIO_PORT, HX711_DOUT_GPIO_PIN)

// 设置SCK引脚为高或低的宏定义
#define HX711_SCK_HIGH()   HAL_GPIO_WritePin(HX711_SCK_GPIO_PORT, HX711_SCK_GPIO_PIN,GPIO_PIN_SET)
#define HX711_SCK_LOW()    HAL_GPIO_WritePin(HX711_SCK_GPIO_PORT, HX711_SCK_GPIO_PIN,GPIO_PIN_RESET)

#define HX711_SDA_HIGH()   HAL_GPIO_WritePin(HX711_DOUT_GPIO_PORT, HX711_DOUT_GPIO_PIN,GPIO_PIN_SET)
#define HX711_SDA_LOW()    HAL_GPIO_WritePin(HX711_DOUT_GPIO_PORT, HX711_DOUT_GPIO_PIN,GPIO_PIN_RESET)

uint32_t Read_Weigh(void)
{
    uint8_t i;
    uint32_t value = 0;
    /**
    数据手册写到，当数据输出管脚 DOUT 为高电平时，表明A/D 转换器还未准备好输出数据，此时串口时
    钟输入信号 PD_SCK 应为低电平，所以下面设置引脚状态。
    **/
//    GPIO_SetBits(GPIOB, SDA_PIN); //初始状态DT引脚为高电平
//    GPIO_ResetBits(GPIOB, SCL_PIN); //初始状态SCK引脚为低电平
    HAL_GPIO_WritePin(HX711_DOUT_GPIO_PORT, HX711_DOUT_GPIO_PIN,GPIO_PIN_SET);
    HAL_GPIO_WritePin(HX711_SCK_GPIO_PORT, HX711_SCK_GPIO_PIN,GPIO_PIN_RESET);
    /**
    等待DT引脚变为高电平
    **/
    while(HAL_GPIO_ReadPin(HX711_DOUT_GPIO_PORT, HX711_DOUT_GPIO_PIN));
    HAL_Delay_us(1);

    /**
    当 DOUT 从高电平变低电平后，PD_SCK 应输入 25 至 27 个不等的时钟脉冲
    25个时钟脉冲 ---> 通道A 增益128
    26个时钟脉冲 ---> 通道B 增益32
    27个时钟脉冲 ---> 通道A 增益64
    **/
    for(i=0; i<24; i++) //24位输出数据从最高位至最低位逐位输出完成
    {
//		//方法一：
//		GPIO_SetBits(GPIOB, GPIO_Pin_6); //时钟高电平
//		value = value << 1; //如果DT位为低左移一位
//		Delay_us(1);
//		GPIO_ResetBits(GPIOB, GPIO_Pin_6); //时钟低电平
//		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7))
//			value++; //如果DT位为高，值+1
//		Delay_us(1);


        //方法二：
//        GPIO_SetBits(GPIOB, SCL_PIN);
        HAL_GPIO_WritePin(HX711_SCK_GPIO_PORT, HX711_SCK_GPIO_PIN,GPIO_PIN_SET);
        HAL_Delay_us(1);
//        GPIO_ResetBits(GPIOB, SCL_PIN);
        HAL_GPIO_WritePin(HX711_SCK_GPIO_PORT, HX711_SCK_GPIO_PIN,GPIO_PIN_RESET);
        if(HAL_GPIO_ReadPin(HX711_DOUT_GPIO_PORT, HX711_DOUT_GPIO_PIN) == 0)
        {
            value = value << 1;
            value |= 0x00;
        }
        if(HAL_GPIO_ReadPin(HX711_DOUT_GPIO_PORT, HX711_DOUT_GPIO_PIN) == 1)
        {
            value = value << 1;
            value |= 0x01;
        }
        HAL_Delay_us(1);
    }

    //第 25至 27 个时钟脉冲用来选择下一次 A/D 转换的输入通道和增益
//    GPIO_SetBits(GPIOB, SCL_PIN);
    HAL_GPIO_WritePin(HX711_SCK_GPIO_PORT, HX711_SCK_GPIO_PIN,GPIO_PIN_SET);
    value = value^0x800000;
    HAL_Delay_us(1);
//    GPIO_ResetBits(GPIOB, SCL_PIN);
    HAL_GPIO_WritePin(HX711_SCK_GPIO_PORT, HX711_SCK_GPIO_PIN,GPIO_PIN_RESET);
    HAL_Delay_us(1);
    return abs((900000-(value-8000000))/1000-30);
}

#define FILTER_SIZE 10  // 定义滤波器大小

float MovingAverageFilter(uint32_t input) {
    static uint32_t buffer[FILTER_SIZE] = {0};  // 环形缓冲区
    static uint32_t sum = 0;  // 累计和
    static int index = 0;  // 当前写入缓冲区的索引
    static int count = 0;  // 已接收的数据点计数

    // 从累计和中减去即将被覆盖的值
    sum -= buffer[index];
    // 将新值加到累计和中
    sum += input;
    // 将新值存储到缓冲区中
    buffer[index] = input;

    // 管理索引和计数，实现环形缓冲区的行为
    index = (index + 1) % FILTER_SIZE;
    if (count < FILTER_SIZE) {
        count++;
    }

    // 计算并返回当前的移动平均值
    return (float)sum / count;
}

//比例系数确定数据：
//	42500 	-->  160g --> 265.65
//	80000 	-->  285g --> 280.7
//	115000 	-->  405g --> 283.95
//	405000 	--> 1400g --> 289.29
//拟合直线：
// y = 91.92x-3580.2 (忽略截距)