#ifndef __UART_H__
#define __UART_H__

void Timer0Init(void);//��ʱ����ʼ��
void Uart_Init(void);//���ڳ�ʼ��
void Uart_Send_Byte(unsigned char c);//��������
char Uart_Receive_Byte();//UART Receive a byteg
#endif