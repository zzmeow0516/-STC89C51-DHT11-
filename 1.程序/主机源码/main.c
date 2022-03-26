//����ͷ����
#include <reg52.h>
#include <intrins.h>	//����ͷ�ļ�
//��ʾ����
#include <display.h>
#include "uart.h"

//�궨��
#define uint unsigned int 
#define uchar unsigned char
	
#define LESS   10
#define NORMAL 30
	
sbit Key1=P3^2;
sbit Key2=P3^3;
sbit Key3=P3^4;
sbit beep=P1^1;  

uchar T_50ms=0;       //��ʱ50ms
uchar revData=0;                  //ZIGBEE��������
bit revFlag = 0;             //ZIGBEE���ձ�־λ

uchar revBuf[11]={0,0,0,0,0,0,0,0,0,0};
uchar revBufLen=0;
uchar set;					  //���ñ���
uchar revHandleFlag = 0;


//��ʪ������ֵ��ʵʱֵ
uchar setTemp1 = 40;      //�¶�1����ֵ
uchar setTemp2 = 39;      //�¶�2����ֵ
uchar setRH1 = 70;        //ʪ��1����ֵ
uchar setRH2 = 69;        //ʪ��2����ֵ
uint curTemp1 = 0;         //�¶�1ʵʱֵ
uint curTemp2 = 0;         //�¶�2ʵʱֵ
uint curRH1 = 0;           //ʪ��1����ֵ
uint curRH2 = 0;           //ʪ��2����ֵ

bit curUI = 0;             //��ǰ����



void init()				//��ʱ����ʼ������
{
	TMOD &= 0xF0;//����T0����λ
  TMOD |= 0x01;//T0��ʽ1,16λ���趨ʱģʽ
 	TL0=0xb0;
 	TH0=0x3c;			//����ֵ50ms
 	EA=1;				//���ж��ܿ���
	ET0=1;				//�򿪶�ʱ��0�ж�������
 	TR0=1;				//�򿪶�ʱ��0��ʱ����
}

void Key()					  //��������
{
	if(Key1==0)				  //���ü�����
	{
		while(Key1==0);		  //�����ɿ�
		set++;				  //���ñ�����
		TR0=0;				  //�رն�ʱ��
	}
	if(set==1)				  //���ñ���ֵʱ
	{
		write_com(0x38);//��Ļ��ʼ��
		write_com(0x80+15);//λ��
   	write_com(0x0f);//����ʾ �޹�� �����˸
   	write_com(0x06);//������дһ���ַ���ָ���һһλ
	}
	else if(set==2)
	{
		write_com(0x38);//��Ļ��ʼ��
		write_com(0x80+0x40+15);//λ��
   	write_com(0x0f);//����ʾ �޹�� �����˸
   	write_com(0x06);//������дһ���ַ���ָ���һһλ
	}
	else if(set>=3)		//�������ʱ
	{
		set=0;			//��������
		write_com(0x38);//��Ļ��ʼ��
		write_com(0x0c);//����ʾ �޹�� �޹����˸
		TR0=1;			//�򿪶�ʱ��
	}
	if(Key2==0&&set==1)			//�����¶�ֵʱ�Ӽ�����
	{
		while(Key2==0);			//�����ɿ�
		if(curUI == 0)
		{
			setTemp1++;				//����ֵ��
			if(setTemp1>=99)		//���ӵ�255
				setTemp1=0;				//����
			write_com(0x80+14);	   //ѡ��Һ�����ϵĵڶ��е�ʮһ��
			write_data('0'+setTemp1/10);   //��ʼ��ʾ����ֵ�����籨��ֵ��123,123����100������1�����ϡ�0����Ϊ�˵õ�Һ���ж�Ӧ1����ʾ��
			write_data('0'+setTemp1%10);//23����10��������3
			write_com(0x80+15);//λ��
		}
		else
		{
			setTemp2++;				//����ֵ��
			if(setTemp2>=99)		//���ӵ�255
				setTemp2=0;				//����
			write_com(0x80+14);	   //ѡ��Һ�����ϵĵڶ��е�ʮһ��
			write_data('0'+setTemp2/10);   //��ʼ��ʾ����ֵ�����籨��ֵ��123,123����100������1�����ϡ�0����Ϊ�˵õ�Һ���ж�Ӧ1����ʾ��
			write_data('0'+setTemp2%10);//23����10��������3
			write_com(0x80+15);//λ��
		}	
	}
	else if(Key2==0&&set==2)			//�����¶�ֵʱ�Ӽ�����
	{
		while(Key2==0);			//�����ɿ�
		if(curUI == 0)
		{
			setRH1++;				//����ֵ��
			if(setRH1>=99)		//���ӵ�255
				setRH1=0;				//����
			write_com(0x80+0x40+14);	   //ѡ��Һ�����ϵĵڶ��е�ʮһ��
			write_data('0'+setRH1/10);   //��ʼ��ʾ����ֵ�����籨��ֵ��123,123����100������1�����ϡ�0����Ϊ�˵õ�Һ���ж�Ӧ1����ʾ��
			write_data('0'+setRH1%10);//23����10��������3
			write_com(0x80+0x40+15);//λ��
		}
		else
		{
			setRH2++;				//����ֵ��
			if(setRH2>=99)		//���ӵ�255
				setRH2=0;				//����
			write_com(0x80+0x40+14);	   //ѡ��Һ�����ϵĵڶ��е�ʮһ��
			write_data('0'+setRH2/10);   //��ʼ��ʾ����ֵ�����籨��ֵ��123,123����100������1�����ϡ�0����Ϊ�˵õ�Һ���ж�Ӧ1����ʾ��
			write_data('0'+setRH2%10);//23����10��������3
			write_com(0x80+0x40+15);//λ��
		}	
	}
	else if(Key2==0&&set==0)
	{
		while(Key2==0);			//�����ɿ�
		curUI ^= 1;         //�л���ǰ����
		write_com(0x80+0x40+1);
		write_data('1'+ ((uchar)curUI));
		write_com(0x80+0x40+1);//λ��
		Display_Refresh();
		
	}
	if(Key3==0&&set==1)				//ע��ͬ�Ӱ���
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
			write_com(0x80+15);//λ��
		}
		else
		{
			setTemp2--;				
			if(setTemp2<=0)
				setTemp2=99;
			write_com(0x80+14);
			write_data('0'+setTemp2/10);
			write_data('0'+setTemp2%10);
			write_com(0x80+15);//λ��
		}
	}
	else if(Key3==0&&set==2)				//ע��ͬ�Ӱ���
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
			write_com(0x80+0x40+15);//λ��
		}
		else
		{
			setRH2--;				
			if(setRH2<=0)
				setRH2=99;
			write_com(0x80+0x40+14);
			write_data('0'+setRH2/10);
			write_data('0'+setRH2%10);
			write_com(0x80+0x40+15);//λ��
		}
	}

}


void main()				//������
{
	Init1602();						 //��ʼ��Һ������
	UartInit();
	init();							 //��ʼ����ʱ��
	
	while(1)						 //����ѭ��
	{
		if(revFlag == 1)
		{
			switch(revBuf[10])
			{
				
				case 'N':         //�Զ�ˢ������
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
		
		Key();						 //���ð�������
	}
}



void  time1_int(void) interrupt 1		  //��ʱ����������
{
	uchar count;						  //�����ʱ����
	TL0=0xb0;
 	TH0=0x3c;							  //���¸���ֵ50ms
  count++;							  //������һ�ξ���50ms
	T_50ms++;
}


void RSINTR() interrupt 4 using 2
{
	if(TI==1) //�����ж�	  
	{
		TI=0;
	}

	if(RI==1)	 //�����ж�		  
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
