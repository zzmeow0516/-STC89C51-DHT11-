//程序头函数
#include <reg52.h>
#include <intrins.h>	//包含头文件
//显示函数
#include <display.h>
#include "uart.h"

//宏定义
#define uint unsigned int 
#define uchar unsigned char
	
#define LESS   10
#define NORMAL 30
	
sbit Key1=P3^2;
sbit Key2=P3^3;
sbit Key3=P3^4;
sbit beep=P1^1;  

uchar T_50ms=0;       //计时50ms
uchar revData=0;                  //ZIGBEE接收数据
bit revFlag = 0;             //ZIGBEE接收标志位

uchar revBuf[11]={0,0,0,0,0,0,0,0,0,0};
uchar revBufLen=0;
uchar set;					  //设置变量
uchar revHandleFlag = 0;


//温湿度设置值、实时值
uchar setTemp1 = 40;      //温度1设置值
uchar setTemp2 = 39;      //温度2设置值
uchar setRH1 = 70;        //湿度1设置值
uchar setRH2 = 69;        //湿度2设置值
uint curTemp1 = 0;         //温度1实时值
uint curTemp2 = 0;         //温度2实时值
uint curRH1 = 0;           //湿度1设置值
uint curRH2 = 0;           //湿度2设置值

bit curUI = 0;             //当前界面



void init()				//定时器初始化函数
{
	TMOD &= 0xF0;//清零T0控制位
  TMOD |= 0x01;//T0方式1,16位可设定时模式
 	TL0=0xb0;
 	TH0=0x3c;			//赋初值50ms
 	EA=1;				//打开中断总开关
	ET0=1;				//打开定时器0中断允许开关
 	TR0=1;				//打开定时器0定时开关
}

void Key()					  //按键函数
{
	if(Key1==0)				  //设置键按下
	{
		while(Key1==0);		  //按键松开
		set++;				  //设置变量加
		TR0=0;				  //关闭定时器
	}
	if(set==1)				  //设置报警值时
	{
		write_com(0x38);//屏幕初始化
		write_com(0x80+15);//位置
   	write_com(0x0f);//打开显示 无光标 光标闪烁
   	write_com(0x06);//当读或写一个字符是指针后一一位
	}
	else if(set==2)
	{
		write_com(0x38);//屏幕初始化
		write_com(0x80+0x40+15);//位置
   	write_com(0x0f);//打开显示 无光标 光标闪烁
   	write_com(0x06);//当读或写一个字符是指针后一一位
	}
	else if(set>=3)		//设置完成时
	{
		set=0;			//变量清零
		write_com(0x38);//屏幕初始化
		write_com(0x0c);//打开显示 无光标 无光标闪烁
		TR0=1;			//打开定时器
	}
	if(Key2==0&&set==1)			//设置温度值时加键按下
	{
		while(Key2==0);			//按键松开
		if(curUI == 0)
		{
			setTemp1++;				//报警值加
			if(setTemp1>=99)		//最大加到255
				setTemp1=0;				//清零
			write_com(0x80+14);	   //选中液晶屏上的第二行第十一列
			write_data('0'+setTemp1/10);   //开始显示报警值。例如报警值是123,123除以100的商是1，加上‘0’是为了得到液晶中对应1的显示码
			write_data('0'+setTemp1%10);//23除以10的余数是3
			write_com(0x80+15);//位置
		}
		else
		{
			setTemp2++;				//报警值加
			if(setTemp2>=99)		//最大加到255
				setTemp2=0;				//清零
			write_com(0x80+14);	   //选中液晶屏上的第二行第十一列
			write_data('0'+setTemp2/10);   //开始显示报警值。例如报警值是123,123除以100的商是1，加上‘0’是为了得到液晶中对应1的显示码
			write_data('0'+setTemp2%10);//23除以10的余数是3
			write_com(0x80+15);//位置
		}	
	}
	else if(Key2==0&&set==2)			//设置温度值时加键按下
	{
		while(Key2==0);			//按键松开
		if(curUI == 0)
		{
			setRH1++;				//报警值加
			if(setRH1>=99)		//最大加到255
				setRH1=0;				//清零
			write_com(0x80+0x40+14);	   //选中液晶屏上的第二行第十一列
			write_data('0'+setRH1/10);   //开始显示报警值。例如报警值是123,123除以100的商是1，加上‘0’是为了得到液晶中对应1的显示码
			write_data('0'+setRH1%10);//23除以10的余数是3
			write_com(0x80+0x40+15);//位置
		}
		else
		{
			setRH2++;				//报警值加
			if(setRH2>=99)		//最大加到255
				setRH2=0;				//清零
			write_com(0x80+0x40+14);	   //选中液晶屏上的第二行第十一列
			write_data('0'+setRH2/10);   //开始显示报警值。例如报警值是123,123除以100的商是1，加上‘0’是为了得到液晶中对应1的显示码
			write_data('0'+setRH2%10);//23除以10的余数是3
			write_com(0x80+0x40+15);//位置
		}	
	}
	else if(Key2==0&&set==0)
	{
		while(Key2==0);			//按键松开
		curUI ^= 1;         //切换当前界面
		write_com(0x80+0x40+1);
		write_data('1'+ ((uchar)curUI));
		write_com(0x80+0x40+1);//位置
		Display_Refresh();
		
	}
	if(Key3==0&&set==1)				//注释同加按键
	{
		while(Key3==0);
		if(curUI == 0)
		{
			setTemp1--;				
			if(setTemp1<=0)
				setTemp1=99;
			write_com(0x80+14);
			write_data('0'+setTemp1/10);
			write_data('0'+setTemp1%10);
			write_com(0x80+15);//位置
		}
		else
		{
			setTemp2--;				
			if(setTemp2<=0)
				setTemp2=99;
			write_com(0x80+14);
			write_data('0'+setTemp2/10);
			write_data('0'+setTemp2%10);
			write_com(0x80+15);//位置
		}
	}
	else if(Key3==0&&set==2)				//注释同加按键
	{
		while(Key3==0);
		if(curUI == 0)
		{
			setRH1--;				
			if(setRH1<=0)
				setRH1=99;
			write_com(0x80+0x40+14);
			write_data('0'+setRH1/10);
			write_data('0'+setRH1%10);
			write_com(0x80+0x40+15);//位置
		}
		else
		{
			setRH2--;				
			if(setRH2<=0)
				setRH2=99;
			write_com(0x80+0x40+14);
			write_data('0'+setRH2/10);
			write_data('0'+setRH2%10);
			write_com(0x80+0x40+15);//位置
		}
	}

}


void main()				//主函数
{
	Init1602();						 //初始化液晶函数
	UartInit();
	init();							 //初始化定时器
	
	while(1)						 //进入循环
	{
		if(revFlag == 1)
		{
			switch(revBuf[10])
			{
				
				case 'N':         //自动刷新数据
				{
					if(revBufLen > 0)
					{
						revHandleFlag = 1;
						if((revBuf[1] - 0x30) == 1)
						{
							curRH1 = (revBuf[2]-0x30)*1000 + (revBuf[3]-0x30)*100 + (revBuf[4]-0x30)*10 + (revBuf[5]-0x30);
							curTemp1   = (revBuf[6]-0x30)*1000 + (revBuf[7]-0x30)*100 + (revBuf[8]-0x30)*10 + (revBuf[9]-0x30);
							Display_Refresh();
						}
						else if((revBuf[1] - 0x30) == 2)
						{
							curRH2 = (revBuf[2]-0x30)*1000 + (revBuf[3]-0x30)*100 + (revBuf[4]-0x30)*10 + (revBuf[5]-0x30);
							curTemp2   = (revBuf[6]-0x30)*1000 + (revBuf[7]-0x30)*100 + (revBuf[8]-0x30)*10 + (revBuf[9]-0x30);
							Display_Refresh();
						}
						revBufLen = 0;
						revBuf[0] = 0;
						revBuf[1] = 0;
						revBuf[2] = 0;
						revBuf[3] = 0;
						revBuf[4] = 0;
						revBuf[5] = 0;
						revBuf[6] = 0;
						revBuf[7] = 0;
						revBuf[8] = 0;
						revBuf[9] = 0;
						revBuf[10] = 0;
						revHandleFlag = 0;
					}
				}
				break;
				
				default:				
					break;
			}
			revFlag = 0;
		}
		if(set == 0)
		{
			if(T_50ms>=20)
			{
				T_50ms=0;
				
				if(((setTemp1*100)<curTemp1) || ((setTemp2*100)<curTemp2))
					beep = 0;
				else
					beep = 1;
				
				if(((setRH1*100)<curRH1) || ((setRH2*100)<curRH2))
					Send_Data('A');
				else
					Send_Data('B');
				
				
			}
		}
		
		Key();						 //调用按键函数
	}
}



void  time1_int(void) interrupt 1		  //定时器工作函数
{
	uchar count;						  //定义计时变量
	TL0=0xb0;
 	TH0=0x3c;							  //重新赋初值50ms
  count++;							  //变量加一次就是50ms
	T_50ms++;
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
		if(revHandleFlag == 0)
		{
			if(revData == 'M')
			{
				revBufLen=1;
				revBuf[0] = revData;
			}
			else if(revBufLen>0)
			{
				revBuf[revBufLen] = revData;
				revBufLen++;
			}
		}
		revFlag = 1;
		RI=0;								 

	}
}
