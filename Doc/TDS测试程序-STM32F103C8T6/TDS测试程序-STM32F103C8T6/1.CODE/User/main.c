/********************(C) COPRIGHT 2019 Crownto electronic **************************
 * �ļ���  ��main.c
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


volatile uint32_t time = 0; // ms ��ʱ���� 

////�������


 
GPIO_InitTypeDef  GPIO_InitStructure; 
unsigned char AD_CHANNEL=0;


float TDS=0.0,TDS_voltage;
float TDS_value=0.0,voltage_value;
float compensationCoefficient=1.0;//�¶�У׼ϵ��
float compensationVolatge;
float kValue=1.67;
float TEMP_Value=0.0;
float averageVoltage=0;

char  TEMP_Buff[5];   //�¶ȴ������
char  TDS_Buff[6];   //TDS�������


extern  u8 SET_Flag,SET_Count; //���ñ�־λ
extern u8 CLC_Flag; //������־λ
extern u8 Warning_flag;
u8 Warning_count=0;




// ADC1ת���ĵ�ѹֵͨ��MDA��ʽ����SRAM
extern __IO uint16_t ADC_ConvertedValue[4];

// ���ڱ���ת�������ĵ�ѹֵ 	 
float ADC_ConvertedValueLocal[4]; 



 
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable the GPIO  Clock */					 		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC| RCC_APB2Periph_AFIO,ENABLE);
	
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);		//����������ΪJTAG�ڵ�GPIO��
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);		//����PB����IO��JTAG����


}

/**************�¶Ȳɼ�����***************/
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

/**************TDSֵ�ɼ�����***************/
void TDS_Value_Conversion()
{
	  u32 ad=0;
	  u8 i;

	  float compensationCoefficient;
	  float compensationVolatge;
	  float temperature=25.0;//�����¶���25�ȣ������¶Ȳ����������18B20����ɼ��¶Ⱥ󣬸�������¶�ֵ
	  
		
	  ad=0;
	  for(i=0;i<50;i++)//��ȡ50�ε�AD��ֵȡ��ƽ������Ϊ׼ȷ	
		{
			ADC_SoftwareStartConvCmd(ADC1, ENABLE);
			while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//ת��������־λ
			ad=ad+ADC_GetConversionValue(ADC1);//�������һ��ADCx�������ת�����		
		}
		ad=ad/50;
	//ADC_ConvertedValueLocal[0]=(float)ad/4096*3.3; //ADת��
	 averageVoltage	=(float)ad/4096*3.3; //ADת��
		
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
	
	/*��ʾEC*/
	TDS_Buff[0]=(int)(TDS_value)/1000+'0';
	TDS_Buff[1]=(int)(TDS_value)%1000/100+'0';
	TDS_Buff[2]=(int)(TDS_value*100)%100/10+'0';	
	TDS_Buff[3]=(int)(TDS_value*100)%10+'0';

}
///*************������ʾ����***************/
void Display_Data()
{
	OLED_ShowStr(24,2,TEMP_Buff,2);//����6*8�ַ�
	OLED_ShowStr(36,4,TDS_Buff,2);//����6*8�ַ�
}

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{	 
	
		GPIO_Configuration(); //IO������
    /* ����USART1 */
    USART1_Config();
	
    /* ����USART2 */
    USART2_Config();
    
    /* ��ʼ��ϵͳ��ʱ�� */
    SysTick_Init();
    
	
	  TIM3_Init();                 //��ʼ������TIM
	
		I2C_Configuration(); //I2C��ʼ��
	  //OLED_Init();  //OLED��ʼ��
	  //ADCx_Init();		// ADC ��ʼ��
	  adc_init();

	
		//DS18B20_Init();//DS18B20��ʼ�� 	
		
   // OLED_CLS();//����
		

		//OLED_ShowStr(0,2,"T:",2);
		//OLED_ShowStr(0,4,"TDS:",2);
		//OLED_ShowStr(56,2,"C",2);

//TDS ��A1
//���ϴ���ģ�� A9-RX   A10-TX
//������115200
//�Ӻú�ֱ�Ӵ򿪴�����������TDSֵ��
//ע��,��������¶Ȳ�������Ҫ�⿪���¶�����ת�������ã�Ȼ��Ѷ�����ֵ���µ�TDS�¶Ȳ���ֵ�м��ɡ�
//����OLED��ʾ�������ˣ�Ҫ��OLED��ʾ���⿪��ʼ�͵��ú������μ��ɡ�

  while(1)
	{	

			//TEMP_Value_Conversion();//�¶�����ת��
			TDS_Value_Conversion();//TDS
			//Display_Data();	//��ʾ����	
		  //printf("averageVoltage=%.3f\r\n",averageVoltage);
		  printf("TDS_value=%.3f\r\n",TDS_value);
		  delay_ms(100);
		
	}	
}


/*********************************************END OF FILE**********************/
