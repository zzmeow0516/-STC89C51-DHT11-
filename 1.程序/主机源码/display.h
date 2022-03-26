//宏定义
#define uint unsigned int 
#define uchar unsigned char

//LCD管脚声明
sbit LCDRS = P2^5;
sbit LCDRW = P2^6;
sbit LCDEN= P2^7;

//初始画时显示的内容
uchar code Init1[]="NO T:----- ST:00";
uchar code Init2[]=" 1 R:----- SR:00";

extern uchar setTemp1;      //温度1设置值
extern uchar setTemp2;      //温度2设置值
extern uchar setRH1;        //湿度1设置值
extern uchar setRH2;        //湿度2设置值
extern uint curTemp1;         //温度1实时值
extern uint curTemp2;         //温度2实时值
extern uint curRH1;           //湿度1设置值
extern uint curRH2;           //湿度2设置值
extern bit curUI;             //当前界面

//LCD延时
void LCDdelay(uint z)
{
  uint x,y;
  for(x=z;x>0;x--)
    for(y=10;y>0;y--);
}
//写命令
void write_com(uchar com)
{
  LCDRS=0;
  P0=com;
  LCDdelay(5);
  LCDEN=1;
  LCDdelay(5);
  LCDEN=0;
}
//写数据
void write_data(uchar date)
{
  LCDRS=1;
  P0=date;
  LCDdelay(5);
  LCDEN=1;
  LCDdelay(5);
  LCDEN=0;
}
//1602初始化
void Init1602()
{
  uchar i=0;
	LCDRW = 0;
  write_com(0x38);//屏幕初始化
  write_com(0x0c);//打开显示 无光标 无光标闪烁
  write_com(0x06);//当读或写一个字符是指针后一一位
  write_com(0x01);//清屏
  write_com(0x80);//设置位置

  for(i=0;i<16;i++)
  {
		write_data(Init1[i]);
  }
  write_com(0x80+0x40);//设置位置
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
//    addr = 0x00 + x; //第一行的x位置显示
//  }
//  else
//  {
//    addr = 0x40 + x; //第二行x的位置显示
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


