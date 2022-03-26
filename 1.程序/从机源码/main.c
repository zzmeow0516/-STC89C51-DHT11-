#include "main.h"

#define   Data_0_time    4

U16 setTemp = 4000;        //�¶��趨ֵ��4000=40.00��
U16 setRH = 8500;          //ʪ���趨ֵ��8000=80.00%
U8 revData;
U8 RHOverFlag = 0;         //ʪ�ȳ����趨ֵ��־
U8 tempOverFlag = 0;       //�¶ȳ����趨ֵ��־
U8 autoRefresh = 1;        //�Զ�ˢ�±�־
bit revFlag = 0;
bit handSetting = 0;       //�ֶ�״̬��־
unsigned char thr0 = 0, tlr0 = 0;
U8 flag1000ms = 0;        //100ms
U8 flag2000ms = 0;

sbit  fengshan = P2^6;    //���ȿ�������

extern U8  U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
U8  count, count_r=0;
U16 U16temp1,U16temp2;


/*��ʱ��T0����*/
void ConfgiTimer0(unsigned int xms)
{
       unsigned long tmp;
       tmp = 11059200/12;//����Ƶ��
       tmp = (tmp * xms) / 1000;//��ʱxms��Ҫ�ļ���ֵ
       tmp = 65536-tmp;//��ʱװ��ĳ�ֵ
       thr0 = (unsigned char)(tmp >> 8);
       tlr0 = (unsigned char)tmp;
       TMOD &= 0xF0;//����T0����λ
       TMOD |= 0x01;//T0��ʽ1,16λ���趨ʱģʽ
       TH0 = thr0;
       TL0 = tlr0;
       TR0 = 1;
       ET0 = 1;
       EA   = 1;
}

	
//----------------------------------------------
//main()��������:  AT89C51  11.0592MHz 	���ڷ� 
//����ʪ������,������ 9600 
//----------------------------------------------
void main()
{  
	UartInit();
	ConfgiTimer0(10);   //��ʼ����ʱ��������ʱ10ms
	Delay(1);         //��ʱ100US��12M����)
	fengshan = 1;
	while(1)
	{  
		if(revFlag == 1)
		{
			switch(revData)
			{
				case 'A':        //ˮ�ÿ���
					fengshan = 0;  
				break;
				
				case 'B':        //ˮ�ÿ���
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
			ET0 = 0;                 //�ر�ʱ���ж�
			GetTemp2();
			
			ET0 = 1;    //����ʱ���ж�
		}
		if(flag1000ms == 1)        //ÿ1���ӽ���һ��
		{
			flag1000ms = 0;
			ET0 = 0;                 //�ر�ʱ���ж�
			
			
/****************���㴫����1����*****************/	
				GetTemp1();	
		}
		
		
	 

	}
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
		revFlag = 1;
		RI=0;								 

	}
}

/*T0�жϷ���*/
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
