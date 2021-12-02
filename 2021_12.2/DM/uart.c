#include "stc89c52.h"
#include "uart.h"


void Uart_Init(void)//9600bps@11.0592MHZ
{
	SCON = 0x50;		//8位数据,可变波特率	11.0592  57600
	AUXR |= 0x40;		//定时器时钟1T模式
	AUXR &= 0xFE;		//串口1选择定时器为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0xE0;			//设置定时器初始值
	TH1 = 0xFE;			//设置定时器初始值
	ET1 = 0;				//禁止定时器中断
	TR1 = 1;				//定时器1开始计时
}

void Timer0Init(void)		//100微秒@11.0592MHz
{
	AUXR |= 0x80;		//定时器时钟1T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0xAE;		//设置定时初始值
	TH0 = 0xFB;		//设置定时初始值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
}

void Uart_Send_Byte(unsigned char c)//UART Send a byte
{
	SBUF = c;
	while(!TI);		//发送完为1 
	TI = 0;
}



unsigned char Uart_Receive_Byte()//UART Receive a byteg
{	
	unsigned char dat;
	while(!RI);	 //接收完为1 
	RI = 0;
	dat = SBUF;
	return (dat);
}