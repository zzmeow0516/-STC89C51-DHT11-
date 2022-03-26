#include "uart.h"

//串口初始化
void UartInit(void)
{
	/*串口初始化 */
	TMOD = 0x20;	  //定时器T1使用工作方式2
	TH1 = 253;        // 设置初值
	TL1 = 253;
	TR1 = 1;          // 开始计时
	SCON = 0x50;	  //工作方式1，波特率9600bps，允许接收   
	ES = 1;
	EA = 1;           // 打开所以中断   
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


