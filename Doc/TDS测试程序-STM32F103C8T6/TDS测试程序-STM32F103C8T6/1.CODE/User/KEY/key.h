#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 


#define KEY1  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)//��ȡ����1
//#define KEY2  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)//��ȡ����2
//#define KEY3  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)//��ȡ����3
#define WK_UP   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)//��ȡ����0(WK_UP) 

 

#define KEY1_PRES 	1	//KEY1����
//#define KEY2_PRES	  2	//KEY2����
//#define KEY3_PRES	  3//KEY2����
#define WKUP_PRES   0//KEY_UP����(��WK_UP/KEY_UP)


void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(u8);  	//����ɨ�躯��	
void Key_SET_Scan(void);
void Key_CONFIRM_Scan(void);
void Temp_set(void);
void PH_set(void);
void TU_set(void);
void TDS_set(void);

void Temp_H_set(void);     //�¶������趨�Ӻ���
void Temp_L_set(void);   //�¶������趨�Ӻ���
void PH_H_set(void);   //PH�����趨����
void PH_L_set(void);  //PH �����趨����
void TDS_H_set(void);   //EC�����趨�Ӻ���
void TDS_L_set(void);   //EC�����趨�Ӻ���

#endif
