#ifndef __AS608_H__
#define __AS608_H__

#define uchar unsigned char 
#define uint unsigned int
#define ulong unsigned long

void Uart_Init();
void Device_Check(void);




//SearchResult;
typedef struct  
{
	uint pageID;//ָ��ID
	uint mathscore;//ƥ��÷�
}SearchResult;

//SysPara;
typedef struct
{
	uint PS_max;//ָ���������
	uchar  PS_level;//��ȫ�ȼ�
	ulong PS_addr;
	uchar  PS_size;//ͨѶ���ݰ���С
	uchar  PS_N;//�����ʻ���N
}SysPara;









//¼��ָ��
void Add_FR(void);
//ˢָ�ƿ���
void press_FR(void);
//ɾ������ָ��
void Del_FR(void);
//���ָ�ƿ�
void Del_ALL(void);


#endif