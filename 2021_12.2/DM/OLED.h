#ifndef __OLED_H__
#define __OLED_H__
#include "stc89c52.h"
#include <intrins.h>

#define	u8 unsigned char
#define	u16 unsigned int
#define	u32 unsigned long
	


//本测试程序使用的是软件模拟SPI接口驱动
//SPI的数据引脚定义和时钟引脚定义都可以任意修改
sbit OLED_MOSI = P1^6;   //OLED屏SPI写数据引脚 P15
sbit OLED_CLK = P1^7;    //OLED屏SPI时钟引脚   P17

#define OLED_MOSI_Clr()  OLED_MOSI = 0
#define OLED_MOSI_Set()  OLED_MOSI = 1

#define OLED_CLK_Clr()   OLED_CLK = 0
#define OLED_CLK_Set()   OLED_CLK = 1

//-------------spi操作函数-----------------/
void SPI_WriteByte(u8 byte);


//--------------OLED参数定义---------------------
#define PAGE_SIZE    8
#define XLevelL		   0x00
#define XLevelH		   0x10
#define YLevel       0xB0
#define	Brightness	 0xFF 
#define WIDTH 	     130
#define HEIGHT 	     64	

//-------------写命令和数据定义-------------------
#define OLED_CMD     0	//写命令
#define OLED_DATA    1	//写数据 
   						  
//-----------------OLED端口定义----------------
sbit OLED_CS = P1^3;    //片选信号           P13
sbit OLED_DC = P1^4;   //数据/命令控制信号  P12
sbit OLED_RST = P1^5;   //复位信号          P33

//-----------------OLED端口操作定义---------------- 
#define OLED_CS_Clr()  OLED_CS = 0
#define OLED_CS_Set()  OLED_CS = 1

#define OLED_DC_Clr()  OLED_DC = 0
#define OLED_DC_Set()  OLED_DC = 1
 					   
#define OLED_RST_Clr()  OLED_RST = 0
#define OLED_RST_Set()  OLED_RST = 1

//OLED控制用函数
void OLED_WR_Byte(unsigned dat,unsigned cmd);
void OLED_Set_Pos(unsigned char x, unsigned char y); 
void OLED_Clear(unsigned dat);  
void OLED_Reset(void);
void OLED_Init(void);
void GUI_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void GUI_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size);
static u32 mypow(u8 m,u8 n);
void GUI_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 Size);
void GUI_ShowCHinese(u8 x,u8 y,u8 *str);
void GUI_DrawBMP(u8 x,u8 y,u8 width, u8 height, u8 BMP[]);
void TEST_BMP(void);
#endif