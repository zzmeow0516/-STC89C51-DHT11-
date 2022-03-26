#include "uart.h"

//���ڳ�ʼ��
void UartInit(void)
{
	/*���ڳ�ʼ�� */
	TMOD = 0x20;	  //��ʱ��T1ʹ�ù�����ʽ2
	TH1 = 253;        // ���ó�ֵ
	TL1 = 253;
	TR1 = 1;          // ��ʼ��ʱ
	SCON = 0x50;	  //������ʽ1��������9600bps���������   
	ES = 1;
	EA = 1;           // �������ж�   
	TI = 0;
	RI = 0;
}

void Send_Data(unsigned char DAT)
{
	ES = 0;
	TI=0;
	SBUF = DAT ;
	while(TI==0);
	TI=0;
	ES = 1;
}

void Send_string(unsigned char  *c)
{
	while(*c != '\0')
	{
		Send_Data(*c++);
	}	
}


