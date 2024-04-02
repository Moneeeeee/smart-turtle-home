
#include "DS18B20.h"
//
//static void DS18B20_IO_Mode(uint8_t in_or_out);
//static void DS18B20_Write_Byte(uint8_t Wdata);
//static uint8_t DS18B20_Read_Byte(void);
//
//uint8_t Sign = ' '; //温度的正负标志
//DS18B20_TempDataTypeDef DS18B20_TempDataStruct = {0};
//基于TIM6的微秒(us)延时函数

float temperature = 0;



void DS18B20_Init_Check(){
    while(DS18B20_Init()){
        OLED_ShowString(24,0,"DS18B20 Fail!",12);
        HAL_Delay(500);
    }
    OLED_ShowString(12,2,"DS18B20 OK",12);
    HAL_Delay(1000);
}

void HAL_Delay_us(uint16_t Delay)
{
    uint16_t tickstart = __HAL_TIM_GET_COUNTER(&htim1);
    uint16_t wait = Delay;
    HAL_TIM_Base_Start(&htim1);
    while ((__HAL_TIM_GET_COUNTER(&htim1) - tickstart) < wait)
    {
    }
    HAL_TIM_Base_Stop(&htim1);
    __HAL_TIM_SET_COUNTER(&htim1, 0);
}
//
//
////DS18B20_IO模式配置
//static void DS18B20_IO_Mode(uint8_t in_or_out)
//{
//    GPIO_InitTypeDef GPIO_InitStruct = {0};
//    GPIO_InitStruct.Pin = DS18B20_Pin;
//    if (in_or_out == 1)
//    {
//        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//    }
//    else if (in_or_out == 2)
//    {
//        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    }
//    GPIO_InitStruct.Pull = GPIO_PULLUP;
//    HAL_GPIO_Init(DS18B20_GPIO_Port, &GPIO_InitStruct);
//}
//
//
////DS18B20初始化
//uint16_t DS18B20_Init(void)
//{
//    uint16_t DS_flag;
//    DS18B20_IO_Mode(out);
//    DS18B20_IO_Write_0();
//    HAL_HAL_Delay_us(600);
//    DS18B20_IO_Write_1();
//    HAL_HAL_Delay_us(30);
//    DS18B20_IO_Mode(in);
//    DS_flag = DS18B20_IO_Read();
//    HAL_HAL_Delay_us(500);
//    return DS_flag;
//}
//
////向DS18B20写入一个字节数据
//static void DS18B20_Write_Byte(uint8_t Wdata)
//{
//    uint8_t i;
//    for (i = 8; i > 0; i--)
//    {
//        DS18B20_IO_Mode(out);
//        DS18B20_IO_Write_0();
//        HAL_HAL_Delay_us(2);
//        if ((Wdata & 0x01) == 1) //从最低位开始写
//            DS18B20_IO_Write_1();
//        else
//            DS18B20_IO_Write_0();
//        HAL_HAL_Delay_us(70);
//        DS18B20_IO_Write_1();
//        Wdata >>= 1;
//    }
//}
//
////向DS18B20读出一个字节数据
//static uint8_t DS18B20_Read_Byte(void)
//{
//    uint8_t i;
//    uint8_t Rdata = 0;
//    for (i = 8; i > 0; i--)
//    {
//        DS18B20_IO_Mode(out);
//        DS18B20_IO_Write_0();
//        HAL_HAL_Delay_us(2);
//        DS18B20_IO_Write_1();
//        HAL_HAL_Delay_us(15);
//        Rdata >>= 1;
//        DS18B20_IO_Mode(in);
//        if (DS18B20_IO_Read() == 1)
//            Rdata |= 0x80;
//        HAL_HAL_Delay_us(50);
//    }
//    return Rdata;
//}
//
////DS18B20获取温度
//void DS18B20_GetTemperature(void)
//{
//    /*
//    读到的温度数据 Byte0,Byte1 (12位精度)
//    TempL  2^3  2^2  2^1  2^0  2^-1  2^-2  2^-3  2^-4
//    TempH   S    S    S    S    S    2^6   2^5   2^4
//    S=0(温度为正)  S=1(温度为负)
//    */
//    uint8_t TempL;
//    uint8_t TempH;
//
//    DS18B20_Init();
//    DS18B20_Write_Byte(0xcc); //跳过读取序列号(ROM)
//    DS18B20_Write_Byte(0x44); //启动温度检测
//
//    HAL_Delay(800); //延时800ms(温度检测时间  12位精度检测时间>750ms(上电默认12位分辨率))
//
//    DS18B20_Init();
//    DS18B20_Write_Byte(0xcc);
//    DS18B20_Write_Byte(0xbe); //读取温度寄存器内容(读取头两个值为温度)
//
//    TempL = DS18B20_Read_Byte(); //读取Byte0温度数据
//    TempH = DS18B20_Read_Byte(); //读取Byte1温度数据
//    DS18B20_TempDataStruct.TempDataAll = (TempH << 8) | TempL;
//    if (TempH > 7)
//    {
//        DS18B20_TempDataStruct.TempDataAll = ~DS18B20_TempDataStruct.TempDataAll + 1;
//        Sign = '-';
//    }
//    DS18B20_TempDataStruct.TempDataAll = DS18B20_TempDataStruct.TempDataAll * 0.0625 * 100 + 0.5;
//    /*留两位小数就*100，+0.5是四舍五入，因为C语言浮点数转换为整型的时候把小数点
//    后面的数自动去掉，不管是否大于0.5，而+0.5之后大于0.5的就是进1了，小于0.5的就
//    算加上0.5，还是在小数点后面*/
//
//    //拆解各个部分的数据
//    DS18B20_TempDataStruct.Hun = DS18B20_TempDataStruct.TempDataAll / 10000;
//    DS18B20_TempDataStruct.Tens = (DS18B20_TempDataStruct.TempDataAll / 1000) % 10;
//    DS18B20_TempDataStruct.Unit = (DS18B20_TempDataStruct.TempDataAll / 100) % 10;
//    DS18B20_TempDataStruct.Decimals1 = (DS18B20_TempDataStruct.TempDataAll / 10) % 10;
//    DS18B20_TempDataStruct.Decimals2 = DS18B20_TempDataStruct.TempDataAll % 10;
//}
/****************************************************************************
函数名：DS18B20_IO_IN
功能：使DS18B20_DQ引脚变为输入模式
输入：无
输出：无
返回值：无
备注：DQ引脚为PA5
****************************************************************************/
void DS18B20_IO_IN(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_0;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOA,&GPIO_InitStructure);
}


/****************************************************************************
函数名：DS18B20_IO_OUT
功能：使DS18B20_DQ引脚变为推挽输出模式
输入：无
输出：无
返回值：无
备注：DQ引脚为PA5
****************************************************************************/
void DS18B20_IO_OUT(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_0;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA,&GPIO_InitStructure);
}


/***************************************************************************
函数名：DS18B20_Rst
功  能：发送复位信号
输  入: 无
输  出：无
返回值：无
备  注：
***************************************************************************/
void DS18B20_Rst(void){
    DS18B20_IO_OUT();//引脚输出模式

    //拉低总线并延时750us
    DS18B20_DQ_OUT_LOW;
    HAL_Delay_us(750);

    //释放总线为高电平并延时等待15~60us
    DS18B20_DQ_OUT_HIGH;
    HAL_Delay_us(15);
}


/***************************************************************************
函数名：DS18B20_Check
功  能：检测DS18B20返回的存在脉冲
输  入: 无
输  出：无
返回值：0:成功  1：失败   2:释放总线失败
备  注：
***************************************************************************/
uint8_t DS18B20_Check(void){
    //定义一个脉冲持续时间
    uint8_t retry = 0;
    //引脚设为输入模式
    DS18B20_IO_IN();
    while(DS18B20_DQ_IN && retry < 200){
        retry++;
        HAL_Delay_us(1);
    }

    if(retry >= 200)
        return 1;
    else
        retry = 0;

    //判断DS18B20是否释放总线
    while(!DS18B20_DQ_IN && retry < 240){
        retry++;
        HAL_Delay_us(1);
    }

    if(retry >= 240)
        return 2;

    return 0;
}
/***************************************************************************
函数名：DS18B20_Write_Byte
功  能：向DS18B20写一个字节
输  入: 要写入的字节
输  出：无
返回值：无
备  注：
***************************************************************************/
void DS18B20_Write_Byte(uint8_t data){
    uint8_t j;
    uint8_t databit;
    DS18B20_IO_OUT();
    for(j=1;j<=8;j++){
        databit=data&0x01;//取数据最低位
        data=data>>1;     //右移一位
        if(databit){      //当前位写1
            DS18B20_DQ_OUT_LOW;
            HAL_Delay_us(2);
            DS18B20_DQ_OUT_HIGH;
            HAL_Delay_us(60);
        }else{          //当前位写0
            DS18B20_DQ_OUT_LOW;
            HAL_Delay_us(60);
            DS18B20_DQ_OUT_HIGH;
            HAL_Delay_us(2);
        }
    }
}

/***************************************************************************
函数名：DS18B20_Read_Bit
功  能：向DS18B20读一个位
输  入: 无
输  出：无
返回值：读入数据
备  注：
***************************************************************************/
uint8_t DS18B20_Read_Bit(void){
    uint8_t data;
    DS18B20_IO_OUT();
    DS18B20_DQ_OUT_LOW;
    HAL_Delay_us(2);
    DS18B20_DQ_OUT_HIGH;
    DS18B20_IO_IN();
    HAL_Delay_us(12);

    if(DS18B20_DQ_IN)
        data = 1;
    else
        data = 0;

    HAL_Delay_us(50);
    return data;
}


/***************************************************************************
函数名：DS18B20_Read_Byte
功  能：向DS18B20读一个字节
输  入: 无
输  出：无
返回值：读入数据
备  注：
***************************************************************************/
uint8_t DS18B20_Read_Byte(void){
    uint8_t i,j,data;
    data = 0;
    for(i=1;i<=8;i++){
        j = DS18B20_Read_Bit();
        data = (j<<7)|(data>>1);
        /*j=0或1，j<<7=0x00或0x80，和data右移一位相或，即把1/0写入最高位，下次再往右移位*/

    }
    return data;
}
/***************************************************************************
函数名：DS18B20_Start
功  能：DS18B20开启
输  入: 无
输  出：无
返回值：无
备  注：
***************************************************************************/
void DS18B20_Start(void){
    DS18B20_Rst();
    DS18B20_Check();
    DS18B20_Write_Byte(0xcc);//跳过ROM
    DS18B20_Write_Byte(0x44);//温度变换命令
}


/***************************************************************************
函数名：DS18B20_Init
功  能：DS18B20初始化
输  入: 无
输  出：无
返回值：无
备  注：
***************************************************************************/
uint8_t DS18B20_Init(void){
    //引脚初始化
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_5;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA,&GPIO_InitStructure);

    DS18B20_Rst();
    return DS18B20_Check();
}

/***************************************************************************
函数名：DS18B20_Read_Temperature
功  能：读取一次温度
输  入: 无
输  出：无
返回值：读取到的温度数据
备  注：适用于总线上只有一个DS18B20的情况
***************************************************************************/
short DS18B20_Get_Temperature(){
    uint8_t temp;
    uint8_t TL,TH;
    short temperature;

    DS18B20_Start();
    DS18B20_Rst();
    DS18B20_Check();
    DS18B20_Write_Byte(0xcc);//跳过ROM
    DS18B20_Write_Byte(0xbe);//读暂存器
    TL = DS18B20_Read_Byte();//低八位
    TH = DS18B20_Read_Byte();//高八位

    //判断温度值是否为负数
    if(TH>0x70){
        TH = ~TH;
        TL = ~TL;
        temp = 0;
    }else
        temp = 1;

    temperature = TH;
    temperature <<= 8;
    temperature += TL;
    temperature = (float)temperature*0.625;
    if(temp)
        return temperature;
    else
        return -temperature;
}

