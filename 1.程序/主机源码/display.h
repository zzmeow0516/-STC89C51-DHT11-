//�궨��
#define uint unsigned int 
#define uchar unsigned char

//LCD�ܽ�����
sbit LCDRS = P2^5;
sbit LCDRW = P2^6;
sbit LCDEN= P2^7;

//��ʼ��ʱ��ʾ������
uchar code Init1[]="NO T:----- ST:00";
uchar code Init2[]=" 1 R:----- SR:00";

extern uchar setTemp1;      //�¶�1����ֵ
extern uchar setTemp2;      //�¶�2����ֵ
extern uchar setRH1;        //ʪ��1����ֵ
extern uchar setRH2;        //ʪ��2����ֵ
extern uint curTemp1;         //�¶�1ʵʱֵ
extern uint curTemp2;         //�¶�2ʵʱֵ
extern uint curRH1;           //ʪ��1����ֵ
extern uint curRH2;           //ʪ��2����ֵ
extern bit curUI;             //��ǰ����

//LCD��ʱ
void LCDdelay(uint z)
{
  uint x,y;
  for(x=z;x>0;x--)
    for(y=10;y>0;y--);
}
//д����
void write_com(uchar com)
{
  LCDRS=0;
  P0=com;
  LCDdelay(5);
  LCDEN=1;
  LCDdelay(5);
  LCDEN=0;
}
//д����
void write_data(uchar date)
{
  LCDRS=1;
  P0=date;
  LCDdelay(5);
  LCDEN=1;
  LCDdelay(5);
  LCDEN=0;
}
//1602��ʼ��
void Init1602()
{
  uchar i=0;
	LCDRW = 0;
  write_com(0x38);//��Ļ��ʼ��
  write_com(0x0c);//����ʾ �޹�� �޹����˸
  write_com(0x06);//������дһ���ַ���ָ���һһλ
  write_com(0x01);//����
  write_com(0x80);//����λ��

  for(i=0;i<16;i++)
  {
		write_data(Init1[i]);
  }
  write_com(0x80+0x40);//����λ��
  for(i=0;i<16;i++)
  {
		write_data(Init2[i]);
  }
}

//void Display_1602(uchar x,uchar y,uchar *str)
//{
//	 unsigned char addr;
//  
//  if (y == 0)
//  {
//    addr = 0x00 + x; //��һ�е�xλ����ʾ
//  }
//  else
//  {
//    addr = 0x40 + x; //�ڶ���x��λ����ʾ
//  }
//  write_com(addr + 0x80);
//  while (*str != '\0')
//  {
//    write_data(*str++);
//  }

//}

void Display_Refresh(void)
{
	if(curUI == 0)
	{
		write_com(0x80+5);							
		write_data('0'+curTemp1/1000);
		write_data('0'+curTemp1%1000/100);
		write_data('.');
		write_data('0'+curTemp1%100/10);	
		write_data('0'+curTemp1%10);	
		
		write_com(0x80+14);
		write_data('0'+setTemp1/10);	
		write_data('0'+setTemp1%10);	
		
		write_com(0x80+0x40+5);
		write_data('0'+curRH1/1000);
		write_data('0'+curRH1%1000/100);
		write_data('.');
		write_data('0'+curRH1%100/10);	
		write_data('0'+curRH1%10);

		write_com(0x80+0x40+14);
		write_data('0'+setRH1/10);	
		write_data('0'+setRH1%10);
		write_com(0x80+0x40+15);
	}
	else
	{
		write_com(0x80+5);							
		write_data('0'+curTemp2/1000);
		write_data('0'+curTemp2%1000/100);
		write_data('.');
		write_data('0'+curTemp2%100/10);	
		write_data('0'+curTemp2%10);	
		
		write_com(0x80+14);
		write_data('0'+setTemp2/10);	
		write_data('0'+setTemp2%10);	
		
		write_com(0x80+0x40+5);
		write_data('0'+curRH2/1000);
		write_data('0'+curRH2%1000/100);
		write_data('.');
		write_data('0'+curRH2%100/10);	
		write_data('0'+curRH2%10);

		write_com(0x80+0x40+14);
		write_data('0'+setRH2/10);	
		write_data('0'+setRH2%10);
		write_com(0x80+0x40+15);
	}
	

}


