#ifndef _ESP01S_H_
#define _ESP01S_H_

#define REV_OK		0	//������ɱ�־
#define REV_WAIT	1	//����δ��ɱ�־

extern unsigned char aRxBuffer; // ������ջ�����
extern unsigned char ESP01S_buf[64];
extern unsigned short ESP01S_cnt, ESP01S_cntPre;

void ESP01S_Init(void);
void ESP01S_Clear(void);
void ESP01S_SendData(unsigned char *data, unsigned short len);
unsigned char *ESP01S_GetIPD(unsigned short timeOut);

#endif

