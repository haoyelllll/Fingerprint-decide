
#include <reg52.h>

#define uchar unsigned char
#define uint  unsigned int

uchar num;//发送暂存变量

void delay(uint z)
{
	uint x,y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0 ; y--);
}	
void UART_init()
{
	TMOD = 0x20;  	//T1工作模式2  8位自动重装
	TH1 = 0xfd;
	TL1 = 0xfd; 	//比特率9600 
	TR1 = 1;		//启动T1定时器
	SM0 = 0;
	SM1 = 1; 		//串口工作方式1 10位异步
	REN = 1;		//串口允许接收
	EA  = 1;		//开总中断
	ES  = 1;		//串口中断打开
	
/*	
		SCON = 0x50;		//8位数据,可变波特率
	//AUXR |= 0x40;		//定时器时钟1T模式
	//AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0xE0;		//设置定时初始值
	TH1 = 0xFE;		//设置定时初始值
	ET1 = 0;		//禁止定时器%d中断
	TR1 = 1;		//定时器1开始计时
	*/
	
	
	
	
}
void main()
{
	UART_init(); //串口初始化
	while(1);	
}


void UART() interrupt 4  //串口中断
{
	if(RI)	//检测是否接收完成 当硬件接收一个数据时 RI 高位
	{
		num = SBUF;	  //num 取出接收缓存器的值
		delay(100);
		num++;//把接收的数据加1并发送
		RI = 0;
		SBUF = num;	  //将数据返回缓存区
		while(!TI);
		TI = 0;
	}
}
