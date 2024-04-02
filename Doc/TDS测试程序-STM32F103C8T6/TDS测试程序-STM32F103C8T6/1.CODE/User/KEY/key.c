#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
#include "OLED_I2C.h"

u8 SET_Flag=0; //���ñ�־λ
u8 CLC_Flag=0; //������־λ
u8 SET_Count=0;
u8 Warning_flag=0;

float set_h_t=30; //�¶�����ֵ
float set_l_t=20; //�¶�����ֵ
float set_h_ph=7.0; //PH����ֵ
float set_l_ph=6.0; //PH����ֵ
float set_h_tds=600; //EC����ֵ
float set_l_tds=300; //EC����ֵ

char  SET_H_PH_Buff[5];   //PH���ݴ洢����
char  SET_L_PH_Buff[5];   //PH���ݴ洢����
char  SET_H_TEMP_Buff[3];   //�Ƕ����ݴ洢����
char  SET_L_TEMP_Buff[3];   //�Ƕ����ݴ洢����
char  SET_H_TDS_Buff[6];   //�Ƕ����ݴ洢����
char  SET_L_TDS_Buff[6];   //�Ƕ����ݴ洢����
								    
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTA,PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//KEY1-KEY4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��

//	//��ʼ�� WK_UP   ��������
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó����룬Ĭ������	  
//	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��

}
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY3���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY_UP!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY1==0||WK_UP==1))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY1==0)return KEY1_PRES;
		else if(WK_UP==1)return WKUP_PRES;
	}else if(KEY1==1&&WK_UP==0)key_up=1; 	    
 	return -1;// �ް�������
}

/**************************************************************************************
 * ��  �� : KEY�����⣬����ָʾ������
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
void Key_SET_Scan(void)
{
	 if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 0)
	  {
	    delay_ms(200);
	    if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 0)	     //ģʽ��������
	    {
				SET_Flag=1;
				CLC_Flag=1;
				Warning_flag=0;
				GPIO_SetBits(GPIOB , GPIO_Pin_1);       //����ʱ�رշ�����
				SET_Count+=1;
				if(SET_Count>7) SET_Count=1;		
	    }
	  }
}

void Key_CONFIRM_Scan()
{
   if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == 0)					//ȷ����    
		 {
		   	delay_ms(200);     //��ʱ����
			  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == 0)
		   	   {
						 	SET_Flag=0;
							CLC_Flag=1;
						  Warning_flag=1;
						  SET_Count=0;
			     }
		 }
}

/**************�¶������Ӻ���******************/
void Temp_H_set(void)     //�¶������趨�Ӻ���
{		 
	 
	  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) ==0)					//��+����  
		 {
		   	delay_ms(200);
			  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)==0)
		   	 {
           set_h_t+=1;
           if(set_h_t>=99) set_h_t=99;
			   }
		 }
	   if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)==0)					//��-���� 
		 {
		   	delay_ms(200);
			  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)==0)
		   	 {
           set_h_t-=1;
           if(set_h_t<=0) set_h_t=0;
			   }
		 }
		 
		SET_H_TEMP_Buff[0]=(int)(set_h_t)/10+'0';	
	  SET_H_TEMP_Buff[1]=(int)(set_h_t)%10+'0';
		 
		 
	if(CLC_Flag==1)//ֻ��һ������������ֹ����
		{
			OLED_CLS();//����
			CLC_Flag=0;
		}
		
		OLED_ShowStr(0,0,"SET H TEMP",2);
		OLED_ShowStr(32,4,SET_H_TEMP_Buff,2);//����6*8�ַ�	 
}
void Temp_L_set(void)   //�¶������趨�Ӻ���
{		 
	 
	  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) ==0)					//��+����  
		 {
		   	delay_ms(200);
			  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)==0)
		   	 {
           set_l_t+=1;
           if(set_l_t>=100) set_l_t=100;
			   }
		 }
	   if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)==0)					//��-���� 
		 {
		   	delay_ms(200);
			  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)==0)
		   	 {
           set_l_t-=1;
           if(set_l_t<=0) set_l_t=0;
			   }
		 }
		 
		SET_L_TEMP_Buff[0]=(int)(set_l_t)/10+'0';	
	  SET_L_TEMP_Buff[1]=(int)(set_l_t)%10+'0';
		 
		 
	if(CLC_Flag==1)//ֻ��һ������������ֹ����
		{
			OLED_CLS();//����
			CLC_Flag=0;
		}
		
		OLED_ShowStr(0,0,"SET L TEMP",2);
		OLED_ShowStr(32,4,SET_L_TEMP_Buff,2);//����6*8�ַ�	 
}
/**************PH�����Ӻ���******************/
void PH_H_set(void)   //PH�����趨����
{		 
	 
	  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) ==0)					//��+����  
		 {
		   	delay_ms(200);
			  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)==0)
		   	 {
           set_h_ph+=0.1;
           if(set_h_ph>=14.0) set_h_ph=14.0;
			   }
		 }
	   if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)==0)					//��-���� 
		 {
		   	delay_ms(200);
			  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)==0)
		   	 {
           set_h_ph-=0.1;
           if(set_h_ph<=0.0) set_h_ph=0.0;
			   }
		 }
		 
		SET_H_PH_Buff[0]=(int)(set_h_ph*10)/100+'0';	
		SET_H_PH_Buff[1]=(int)(set_h_ph*10)%100/10+'0';
		SET_H_PH_Buff[2]='.';
		SET_H_PH_Buff[3]=(int)(set_h_ph*10)%10+'0';	
		 
		 
	if(CLC_Flag==1)//ֻ��һ������������ֹ����
		{
			OLED_CLS();//����
			CLC_Flag=0;
		}
		
		OLED_ShowStr(0,0,"SET H PH",2);
		OLED_ShowStr(32,4,SET_H_PH_Buff,2);//����6*8�ַ�
		 
		 
}

void PH_L_set(void)  //PH �����趨����
{		 
	 
	  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) ==0)					//��+����  
		 {
		   	delay_ms(200);
			  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)==0)
		   	 {
           set_l_ph+=0.1;
           if(set_l_ph>=14.0) set_l_ph=14.0;
			   }
		 }
	   if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)==0)					//��-���� 
		 {
		   	delay_ms(200);
			  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)==0)
		   	 {
           set_l_ph-=0.1;
           if(set_l_ph<=0.0) set_l_ph=0.0;
			   }
		 }
		 
		SET_L_PH_Buff[0]=(int)(set_l_ph*10)/100+'0';	
		SET_L_PH_Buff[1]=(int)(set_l_ph*10)%100/10+'0';
		SET_L_PH_Buff[2]='.';
		SET_L_PH_Buff[3]=(int)(set_l_ph*10)%10+'0';	
		 
		 
	if(CLC_Flag==1)//ֻ��һ������������ֹ����
		{
			OLED_CLS();//����
			CLC_Flag=0;
		}
		
		OLED_ShowStr(0,0,"SET L PH",2);
		OLED_ShowStr(32,4,SET_L_PH_Buff,2);//����6*8�ַ�
		 
		 
}

void TDS_H_set(void)   //EC�����趨�Ӻ���
{		 
	 
	  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) ==0)					//��+����  
		 {
		   	delay_ms(200);
			  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)==0)
		   	 {
           set_h_tds+=10;
           if(set_h_tds>=1400) set_h_tds=1400;
			   }
		 }
	   if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)==0)					//��-���� 
		 {
		   	delay_ms(200);
			  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)==0)
		   	 {
           set_h_tds-=10;
           if(set_h_tds<=0) set_h_tds=0;
			   }
		 }
		
		SET_H_TDS_Buff[0]=(int)(set_h_tds)/1000+'0';	
		SET_H_TDS_Buff[1]=(int)(set_h_tds)%1000/100+'0';
		SET_H_TDS_Buff[2]=(int)(set_h_tds)%100/10+'0';
		SET_H_TDS_Buff[3]=(int)(set_h_tds)%10+'0';	
		 
		 
	if(CLC_Flag==1)//ֻ��һ������������ֹ����
		{
			OLED_CLS();//����
			CLC_Flag=0;
		}
		
		OLED_ShowStr(0,0,"SET H TDS",2);
		OLED_ShowStr(32,4,SET_H_TDS_Buff,2);//����6*8�ַ�
		 
}
void TDS_L_set(void)   //EC�����趨�Ӻ���
{		 
	 
	  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) ==0)					//��+����  
		 {
		   	delay_ms(200);
			  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)==0)
		   	 {
           set_l_tds+=10;
           if(set_l_tds>=1400) set_l_tds=1400;
			   }
		 }
	   if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)==0)					//��-���� 
		 {
		   	delay_ms(200);
			  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)==0)
		   	 {
           set_l_tds-=10;
           if(set_l_tds<=0) set_l_tds=0;
			   }
		 }
		
		SET_L_TDS_Buff[0]=(int)(set_l_tds)/1000+'0';	
		SET_L_TDS_Buff[1]=(int)(set_l_tds)%1000/100+'0';
		SET_L_TDS_Buff[2]=(int)(set_l_tds)%100/10+'0';
		SET_L_TDS_Buff[3]=(int)(set_l_tds)%10+'0';	
		 
		 
	if(CLC_Flag==1)//ֻ��һ������������ֹ����
		{
			OLED_CLS();//����
			CLC_Flag=0;
		}
		
		OLED_ShowStr(0,0,"SET L TDS",2);
		OLED_ShowStr(32,4,SET_L_TDS_Buff,2);//����6*8�ַ�
		 
}