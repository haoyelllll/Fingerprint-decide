
#include <reg52.h>

#define uchar unsigned char
#define uint  unsigned int

uchar num;//�����ݴ����

void delay(uint z)
{
	uint x,y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0 ; y--);
}	
void UART_init()
{
	TMOD = 0x20;  	//T1����ģʽ2  8λ�Զ���װ
	TH1 = 0xfd;
	TL1 = 0xfd; 	//������9600 
	TR1 = 1;		//����T1��ʱ��
	SM0 = 0;
	SM1 = 1; 		//���ڹ�����ʽ1 10λ�첽
	REN = 1;		//�����������
	EA  = 1;		//�����ж�
	ES  = 1;		//�����жϴ�
	
/*	
		SCON = 0x50;		//8λ����,�ɱ䲨����
	//AUXR |= 0x40;		//��ʱ��ʱ��1Tģʽ
	//AUXR &= 0xFE;		//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TL1 = 0xE0;		//���ö�ʱ��ʼֵ
	TH1 = 0xFE;		//���ö�ʱ��ʼֵ
	ET1 = 0;		//��ֹ��ʱ��%d�ж�
	TR1 = 1;		//��ʱ��1��ʼ��ʱ
	*/
	
	
	
	
}
void main()
{
	UART_init(); //���ڳ�ʼ��
	while(1);	
}


void UART() interrupt 4  //�����ж�
{
	if(RI)	//����Ƿ������� ��Ӳ������һ������ʱ RI ��λ
	{
		num = SBUF;	  //num ȡ�����ջ�������ֵ
		delay(100);
		num++;//�ѽ��յ����ݼ�1������
		RI = 0;
		SBUF = num;	  //�����ݷ��ػ�����
		while(!TI);
		TI = 0;
	}
}
