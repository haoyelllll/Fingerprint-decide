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
	uint pageID;//指纹ID
	uint mathscore;//匹配得分
}SearchResult;

//SysPara;
typedef struct
{
	uint PS_max;//指纹最大容量
	uchar  PS_level;//安全等级
	ulong PS_addr;
	uchar  PS_size;//通讯数据包大小
	uchar  PS_N;//波特率基数N
}SysPara;









//录入指纹
void Add_FR(void);
//刷指纹考勤
void press_FR(void);
//删除单个指纹
void Del_FR(void);
//清空指纹库
void Del_ALL(void);


#endif