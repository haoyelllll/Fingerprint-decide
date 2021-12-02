#ifndef __UART_H__
#define __UART_H__

void Timer0Init(void);//定时器初始化
void Uart_Init(void);//串口初始化
void Uart_Send_Byte(unsigned char c);//发送数据
char Uart_Receive_Byte();//UART Receive a byteg
#endif