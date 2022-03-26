#include "dht11.h"

U8  U8FLAG,k;
U8  U8count,U8temp;
U8  U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
U8  U8T_data_H_temp,U8T_data_L_temp,U8RH_data_H_temp,U8RH_data_L_temp,U8checkdata_temp;
U8  U8comdata;
U8  str[11];
	//--------------------------------
	//-----湿度读取子程序 ------------
	//--------------------------------
	//----以下变量均为全局变量--------
	//----温度高8位== U8T_data_H------
	//----温度低8位== U8T_data_L------
	//----湿度高8位== U8RH_data_H-----
	//----湿度低8位== U8RH_data_L-----
	//----校验 8位 == U8checkdata-----
	//----调用相关子程序如下----------
	//---- Delay();, Delay_10us();,COM(); 
	//--------------------------------

void RH(void)
{
	//主机拉低18ms 
		 P2_0=0;
	 Delay(180);
	 P2_0=1;
 //总线由上拉电阻拉高 主机延时20us
	 Delay_10us();
	 Delay_10us();
	 Delay_10us();
	 Delay_10us();
 //主机设为输入 判断从机响应信号 
	 P2_0=1;
 //判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行	  
	 if(!P2_0)		 //T !	  
	 {
	 U8FLAG=2;
 //判断从机是否发出 80us 的低电平响应信号是否结束	 
	 while((!P2_0)&&U8FLAG++);
	 U8FLAG=2;
 //判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
	 while((P2_0)&&U8FLAG++);
 //数据接收状态		 
	 COM();
	 U8RH_data_H_temp=U8comdata;
	 COM();
	 U8RH_data_L_temp=U8comdata;
	 COM();
	 U8T_data_H_temp=U8comdata;
	 COM();
	 U8T_data_L_temp=U8comdata;
	 COM();
	 U8checkdata_temp=U8comdata;
	 P2_0=1;
 //数据校验 
 
	 U8temp=(U8T_data_H_temp+U8T_data_L_temp+U8RH_data_H_temp+U8RH_data_L_temp);
	 if(U8temp==U8checkdata_temp)
	 {
			U8RH_data_H=U8RH_data_H_temp;
			U8RH_data_L=U8RH_data_L_temp;
		U8T_data_H=U8T_data_H_temp;
			U8T_data_L=U8T_data_L_temp;
			U8checkdata=U8checkdata_temp;
	 }//fi
	 }//fi

}

void RH2(void)
{
	//主机拉低18ms 
		 P0_6=0;
	 Delay(180);
	 P0_6=1;
 //总线由上拉电阻拉高 主机延时20us
	 Delay_10us();
	 Delay_10us();
	 Delay_10us();
	 Delay_10us();
 //主机设为输入 判断从机响应信号 
	 P0_6=1;
 //判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行	  
	 if(!P0_6)		 //T !	  
	 {
	 U8FLAG=2;
 //判断从机是否发出 80us 的低电平响应信号是否结束	 
	 while((!P0_6)&&U8FLAG++);
	 U8FLAG=2;
 //判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
	 while((P0_6)&&U8FLAG++);
 //数据接收状态		 
	 COM2();
	 U8RH_data_H_temp=U8comdata;
	 COM2();
	 U8RH_data_L_temp=U8comdata;
	 COM2();
	 U8T_data_H_temp=U8comdata;
	 COM2();
	 U8T_data_L_temp=U8comdata;
	 COM2();
	 U8checkdata_temp=U8comdata;
	 P0_6=1;
 //数据校验 
 
	 U8temp=(U8T_data_H_temp+U8T_data_L_temp+U8RH_data_H_temp+U8RH_data_L_temp);
	 if(U8temp==U8checkdata_temp)
	 {
			U8RH_data_H=U8RH_data_H_temp;
			U8RH_data_L=U8RH_data_L_temp;
		U8T_data_H=U8T_data_H_temp;
			U8T_data_L=U8T_data_L_temp;
			U8checkdata=U8checkdata_temp;
	 }//fi
	 }//fi

}


void  COM(void)
{
	U8 i;		
	for(i=0;i<8;i++)	   
	{

			U8FLAG=2;	
	while((!P2_0)&&U8FLAG++);
	Delay_10us();
		Delay_10us();
	Delay_10us();
		U8temp=0;
	 if(P2_0)U8temp=1;
		U8FLAG=2;
 while((P2_0)&&U8FLAG++);
	//超时则跳出for循环		  
	 if(U8FLAG==1)break;
	//判断数据位是0还是1	 
		 
// 如果高电平高过预定0高电平值则数据位为 1 
	 
	 U8comdata<<=1;
		 U8comdata|=U8temp;        //0
	 }//rof
 
}

void  COM2(void)
{
	U8 i;		
	for(i=0;i<8;i++)	   
	{

			U8FLAG=2;	
	while((!P0_6)&&U8FLAG++);
	Delay_10us();
		Delay_10us();
	Delay_10us();
		U8temp=0;
	 if(P0_6)U8temp=1;
		U8FLAG=2;
 while((P0_6)&&U8FLAG++);
	//超时则跳出for循环		  
	 if(U8FLAG==1)break;
	//判断数据位是0还是1	 
		 
// 如果高电平高过预定0高电平值则数据位为 1 
	 
	 U8comdata<<=1;
		 U8comdata|=U8temp;        //0
	 }//rof
 
}

void GetTemp1(void)
{
	
	//------------------------
	 //调用温湿度读取子程序 
	ET0 = 0;
		RH();
	ET0 = 1;
	 //串口显示程序 
	 //--------------------------
		str[0]='M';
		str[1]='1';
		str[2]=U8RH_data_H/10+0x30;
		str[3]=U8RH_data_H%10+0x30;
		str[4]=U8RH_data_L/10+0x30;
		str[5]=U8RH_data_L%10+0x30;
	
		str[6]=U8T_data_H/10+0x30;
		str[7]=U8T_data_H%10+0x30;
		str[8]=U8T_data_L/10+0x30;
		str[9]=U8T_data_L%10+0x30;
		str[10]='N';
		str[11]='\0';

		Send_string(str);
		Send_string("\r\n");
		str[0]='\0';
		Delay_10us();
}


void GetTemp2(void)
{
	//------------------------
	 //调用温湿度读取子程序
		ET0 = 0;
		RH2();
	ET0 = 1;
	 //串口显示程序 
	 //--------------------------
	str[0]='M';
	str[1]='2';
	str[2]=U8RH_data_H/10+0x30;
	str[3]=U8RH_data_H%10+0x30;
	str[4]=U8RH_data_L/10+0x30;
	str[5]=U8RH_data_L%10+0x30;

	str[6]=U8T_data_H/10+0x30;
	str[7]=U8T_data_H%10+0x30;
	str[8]=U8T_data_L/10+0x30;
	str[9]=U8T_data_L%10+0x30;
	str[10]='N';
	str[11]='\0';

	Send_string(str);
	Send_string("\r\n");
	str[0]='\0';
	Delay_10us();
}


