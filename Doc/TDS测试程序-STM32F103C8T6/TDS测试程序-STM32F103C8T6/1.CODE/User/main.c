/********************(C) COPRIGHT 2019 Crownto electronic **************************
 * 文件名  ：main.c
**********************************************************************************/
#include "stm32f10x.h"
#include <string.h>
#include "delay.h"
#include "stdio.h"
#include "bsp_SysTick.h"
#include "math.h"
#include "bsp_adc.h"
#include "ds18b20.h"

#include "OLED_I2C.h"
#include "timer.h"
#include "bsp_usart1.h"
#include "bsp_usart2.h"


volatile uint32_t time = 0; // ms 计时变量 

////定义变量


 
GPIO_InitTypeDef  GPIO_InitStructure; 
unsigned char AD_CHANNEL=0;


float TDS=0.0,TDS_voltage;
float TDS_value=0.0,voltage_value;
float compensationCoefficient=1.0;//温度校准系数
float compensationVolatge;
float kValue=1.67;
float TEMP_Value=0.0;
float averageVoltage=0;

char  TEMP_Buff[5];   //温度存放数组
char  TDS_Buff[6];   //TDS存放数组


extern  u8 SET_Flag,SET_Count; //设置标志位
extern u8 CLC_Flag; //清屏标志位
extern u8 Warning_flag;
u8 Warning_count=0;




// ADC1转换的电压值通过MDA方式传到SRAM
extern __IO uint16_t ADC_ConvertedValue[4];

// 用于保存转换计算后的电压值 	 
float ADC_ConvertedValueLocal[4]; 



 
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable the GPIO  Clock */					 		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC| RCC_APB2Periph_AFIO,ENABLE);
	
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);		//屏蔽所有作为JTAG口的GPIO口
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		//屏蔽PB口上IO口JTAG功能


}

/**************温度采集函数***************/
void TEMP_Value_Conversion()
{
	
	  TEMP_Value=DS18B20_Get_Temp();
	
	  TEMP_Buff[0]=(int)(TEMP_Value)%1000/100+'0';	
	  TEMP_Buff[1]=(int)(TEMP_Value)%100/10+'0';
	  TEMP_Buff[2]='.';
	  TEMP_Buff[3]=(int)(TEMP_Value)%10+'0';
}
/*
int getMedianNum(int bArray[], int iFilterLen)
{
	int  i, j, bTemp;
  int bTab[iFilterLen];
  for (i = 0; i < iFilterLen; i++)
    bTab[i] = bArray[i];
 
  for (j = 0; j < iFilterLen - 1; j++)
  {
    for (i = 0; i < iFilterLen - j - 1; i++)
    {
      if (bTab[i] > bTab[i + 1])
      {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  if ((iFilterLen & 1) > 0)
    bTemp = bTab[(iFilterLen - 1) / 2];
  else
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  return bTemp;
}
*/

/**************TDS值采集函数***************/
void TDS_Value_Conversion()
{
	  u32 ad=0;
	  u8 i;

	  float compensationCoefficient;
	  float compensationVolatge;
	  float temperature=25.0;//假设温度是25度，进行温度补偿，如果有18B20，则采集温度后，更新这个温度值
	  
		
	  ad=0;
	  for(i=0;i<50;i++)//读取50次的AD数值取其平均数较为准确	
		{
			ADC_SoftwareStartConvCmd(ADC1, ENABLE);
			while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//转换结束标志位
			ad=ad+ADC_GetConversionValue(ADC1);//返回最近一次ADCx规则组的转换结果		
		}
		ad=ad/50;
	//ADC_ConvertedValueLocal[0]=(float)ad/4096*3.3; //AD转换
	 averageVoltage	=(float)ad/4096*3.3; //AD转换
		
	// averageVoltage = getMedianNum(analogBufferTemp, SCOUNT) * (float)VREF / 1024.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
	 compensationCoefficient = 1.0 + 0.02 * (temperature - 25.0); //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
   compensationVolatge = averageVoltage / compensationCoefficient; //temperature compensation
   TDS_value = (133.42 * compensationVolatge * compensationVolatge * compensationVolatge - 255.86 * compensationVolatge * compensationVolatge + 857.39 * compensationVolatge) * 0.5; //convert voltage value to tds value	
	 	
		
		
		
		
	/*	
	compensationCoefficient=1.0+0.02*((TEMP_Value/10)-25.0); 
	compensationVolatge=ADC_ConvertedValueLocal[1]/compensationCoefficient;
	if((ADC_ConvertedValueLocal[2]>=0)&&(ADC_ConvertedValueLocal[1]<0.1))
	{compensationVolatge=0;}
	TDS_value=(133.42*compensationVolatge*compensationVolatge*compensationVolatge - 
	255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5*kValue;
	
	  if((TDS_value<=0)){TDS_value=111;}
		if((TDS_value>1400)){TDS_value=1400;}	
*/
	
	/*显示EC*/
	TDS_Buff[0]=(int)(TDS_value)/1000+'0';
	TDS_Buff[1]=(int)(TDS_value)%1000/100+'0';
	TDS_Buff[2]=(int)(TDS_value*100)%100/10+'0';	
	TDS_Buff[3]=(int)(TDS_value*100)%10+'0';

}
///*************数据显示函数***************/
void Display_Data()
{
	OLED_ShowStr(24,2,TEMP_Buff,2);//测试6*8字符
	OLED_ShowStr(36,4,TDS_Buff,2);//测试6*8字符
}

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{	 
	
		GPIO_Configuration(); //IO口配置
    /* 配置USART1 */
    USART1_Config();
	
    /* 配置USART2 */
    USART2_Config();
    
    /* 初始化系统定时器 */
    SysTick_Init();
    
	
	  TIM3_Init();                 //初始化配置TIM
	
		I2C_Configuration(); //I2C初始化
	  //OLED_Init();  //OLED初始化
	  //ADCx_Init();		// ADC 初始化
	  adc_init();

	
		//DS18B20_Init();//DS18B20初始化 	
		
   // OLED_CLS();//清屏
		

		//OLED_ShowStr(0,2,"T:",2);
		//OLED_ShowStr(0,4,"TDS:",2);
		//OLED_ShowStr(56,2,"C",2);

//TDS 接A1
//接上串口模块 A9-RX   A10-TX
//波特率115200
//接好后，直接打开串口软件看输出TDS值。
//注意,如果启用温度补偿，需要解开”温度数据转换“调用，然后把读到的值更新到TDS温度补偿值中即可。
//其他OLED显示是屏蔽了，要是OLED显示，解开初始和调用函数屏蔽即可。

  while(1)
	{	

			//TEMP_Value_Conversion();//温度数据转换
			TDS_Value_Conversion();//TDS
			//Display_Data();	//显示数据	
		  //printf("averageVoltage=%.3f\r\n",averageVoltage);
		  printf("TDS_value=%.3f\r\n",TDS_value);
		  delay_ms(100);
		
	}	
}


/*********************************************END OF FILE**********************/
