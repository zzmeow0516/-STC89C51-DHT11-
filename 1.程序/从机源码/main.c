#include "main.h"

#define   Data_0_time    4

U16 setTemp = 4000;        //温度设定值：4000=40.00℃
U16 setRH = 8500;          //湿度设定值：8000=80.00%
U8 revData;
U8 RHOverFlag = 0;         //湿度超出设定值标志
U8 tempOverFlag = 0;       //温度超出设定值标志
U8 autoRefresh = 1;        //自动刷新标志
bit revFlag = 0;
bit handSetting = 0;       //手动状态标志
unsigned char thr0 = 0, tlr0 = 0;
U8 flag1000ms = 0;        //100ms
U8 flag2000ms = 0;

sbit  fengshan = P2^6;    //风扇控制引脚

extern U8  U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
U8  count, count_r=0;
U16 U16temp1,U16temp2;


/*定时器T0配置*/
void ConfgiTimer0(unsigned int xms)
{
       unsigned long tmp;
       tmp = 11059200/12;//周期频率
       tmp = (tmp * xms) / 1000;//定时xms需要的计数值
       tmp = 65536-tmp;//定时装入的初值
       thr0 = (unsigned char)(tmp >> 8);
       tlr0 = (unsigned char)tmp;
       TMOD &= 0xF0;//清零T0控制位
       TMOD |= 0x01;//T0方式1,16位可设定时模式
       TH0 = thr0;
       TL0 = tlr0;
       TR0 = 1;
       ET0 = 1;
       EA   = 1;
}

	
//----------------------------------------------
//main()功能描述:  AT89C51  11.0592MHz 	串口发 
//送温湿度数据,波特率 9600 
//----------------------------------------------
void main()
{  
	UartInit();
	ConfgiTimer0(10);   //初始化定时器，并定时10ms
	Delay(1);         //延时100US（12M晶振)
	fengshan = 1;
	while(1)
	{  
		if(revFlag == 1)
		{
			switch(revData)
			{
				case 'A':        //水泵控制
					fengshan = 0;  
				break;
				
				case 'B':        //水泵控制
					fengshan = 1;  
				break;
				
				default:
					revFlag = 0;
					break;
			}
			revFlag = 0;
		}
		
		if(flag2000ms == 1)
		{
			flag2000ms = 0;
			ET0 = 0;                 //关闭时钟中断
			GetTemp2();
			
			ET0 = 1;    //开启时钟中断
		}
		if(flag1000ms == 1)        //每1秒钟进入一次
		{
			flag1000ms = 0;
			ET0 = 0;                 //关闭时钟中断
			
			
/****************计算传感器1数据*****************/	
				GetTemp1();	
		}
		
		
	 

	}
}

void RSINTR() interrupt 4 using 2
{
	
	if(TI==1) //发送中断	  
	{
		TI=0;
	}

	if(RI==1)	 //接收中断		  
	{	
		revData = SBUF;
		revFlag = 1;
		RI=0;								 

	}
}

/*T0中断服务*/
void Timer0_ISP() interrupt 1
{
       static unsigned char counter = 0;
       TH0 = thr0;
       TL0 = tlr0; //1ms
       counter++;		
			if(counter == 100)
					flag1000ms =1;
       if (counter >= 200)
       {
       		counter = 0;
            flag2000ms = 1;//1000ms
       }          
}
