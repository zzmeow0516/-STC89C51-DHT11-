#include "dht11.h"

U8  U8FLAG,k;
U8  U8count,U8temp;
U8  U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
U8  U8T_data_H_temp,U8T_data_L_temp,U8RH_data_H_temp,U8RH_data_L_temp,U8checkdata_temp;
U8  U8comdata;
U8  str[11];
	//--------------------------------
	//-----ʪ�ȶ�ȡ�ӳ��� ------------
	//--------------------------------
	//----���±�����Ϊȫ�ֱ���--------
	//----�¶ȸ�8λ== U8T_data_H------
	//----�¶ȵ�8λ== U8T_data_L------
	//----ʪ�ȸ�8λ== U8RH_data_H-----
	//----ʪ�ȵ�8λ== U8RH_data_L-----
	//----У�� 8λ == U8checkdata-----
	//----��������ӳ�������----------
	//---- Delay();, Delay_10us();,COM(); 
	//--------------------------------

void RH(void)
{
	//��������18ms 
		 P2_0=0;
	 Delay(180);
	 P2_0=1;
 //������������������ ������ʱ20us
	 Delay_10us();
	 Delay_10us();
	 Delay_10us();
	 Delay_10us();
 //������Ϊ���� �жϴӻ���Ӧ�ź� 
	 P2_0=1;
 //�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������	  
	 if(!P2_0)		 //T !	  
	 {
	 U8FLAG=2;
 //�жϴӻ��Ƿ񷢳� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ����	 
	 while((!P2_0)&&U8FLAG++);
	 U8FLAG=2;
 //�жϴӻ��Ƿ񷢳� 80us �ĸߵ�ƽ���緢����������ݽ���״̬
	 while((P2_0)&&U8FLAG++);
 //���ݽ���״̬		 
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
 //����У�� 
 
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
	//��������18ms 
		 P0_6=0;
	 Delay(180);
	 P0_6=1;
 //������������������ ������ʱ20us
	 Delay_10us();
	 Delay_10us();
	 Delay_10us();
	 Delay_10us();
 //������Ϊ���� �жϴӻ���Ӧ�ź� 
	 P0_6=1;
 //�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������	  
	 if(!P0_6)		 //T !	  
	 {
	 U8FLAG=2;
 //�жϴӻ��Ƿ񷢳� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ����	 
	 while((!P0_6)&&U8FLAG++);
	 U8FLAG=2;
 //�жϴӻ��Ƿ񷢳� 80us �ĸߵ�ƽ���緢����������ݽ���״̬
	 while((P0_6)&&U8FLAG++);
 //���ݽ���״̬		 
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
 //����У�� 
 
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
	//��ʱ������forѭ��		  
	 if(U8FLAG==1)break;
	//�ж�����λ��0����1	 
		 
// ����ߵ�ƽ�߹�Ԥ��0�ߵ�ƽֵ������λΪ 1 
	 
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
	//��ʱ������forѭ��		  
	 if(U8FLAG==1)break;
	//�ж�����λ��0����1	 
		 
// ����ߵ�ƽ�߹�Ԥ��0�ߵ�ƽֵ������λΪ 1 
	 
	 U8comdata<<=1;
		 U8comdata|=U8temp;        //0
	 }//rof
 
}

void GetTemp1(void)
{
	
	//------------------------
	 //������ʪ�ȶ�ȡ�ӳ��� 
	ET0 = 0;
		RH();
	ET0 = 1;
	 //������ʾ���� 
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
	 //������ʪ�ȶ�ȡ�ӳ���
		ET0 = 0;
		RH2();
	ET0 = 1;
	 //������ʾ���� 
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


