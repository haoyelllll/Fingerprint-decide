#include "stc89c52.h"
#include "uart.h"


void Uart_Init(void)//9600bps@11.0592MHZ
{
	SCON = 0x50;		//8λ����,�ɱ䲨����	11.0592  57600
	AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
	AUXR &= 0xFE;		//����1ѡ��ʱ��Ϊ�����ʷ�����
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0xE0;			//���ö�ʱ����ʼֵ
	TH1 = 0xFE;			//���ö�ʱ����ʼֵ
	ET1 = 0;				//��ֹ��ʱ���ж�
	TR1 = 1;				//��ʱ��1��ʼ��ʱ
}

void Timer0Init(void)		//100΢��@11.0592MHz
{
	AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0xAE;		//���ö�ʱ��ʼֵ
	TH0 = 0xFB;		//���ö�ʱ��ʼֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
}

void Uart_Send_Byte(unsigned char c)//UART Send a byte
{
	SBUF = c;
	while(!TI);		//������Ϊ1 
	TI = 0;
}



unsigned char Uart_Receive_Byte()//UART Receive a byteg
{	
	unsigned char dat;
	while(!RI);	 //������Ϊ1 
	RI = 0;
	dat = SBUF;
	return (dat);
}