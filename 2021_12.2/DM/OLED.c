#include <intrins.h>
#include "stc89c52.h"
#include <stdlib.h>
#include <string.h>
#include "oled_font.h"
#include "delay.h"
#include "oled.h"




//spi通信协议

void SPI_WriteByte(u8 byte)
{	
	u8 i;
  for(i=0;i<8;i++)
	{			  
		if(byte&0x80)
		{
		  OLED_MOSI_Set();
		}
		else
		{
		  OLED_MOSI_Clr();
		}
		OLED_CLK_Clr();
		OLED_CLK_Set();
		byte<<=1;   
	}				 		  
}
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
	{
		OLED_DC_Set();
	}
	else
	{
		OLED_DC_Clr();
	}
	OLED_CS_Clr();
	SPI_WriteByte(dat);
	OLED_CS_Set();
}

void OLED_Set_Pos(unsigned char x, unsigned char y) 
{
 	OLED_WR_Byte(YLevel+y,OLED_CMD);
	OLED_WR_Byte(((x+2&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x+2&0x0f),OLED_CMD); 
}  
   			
void OLED_Clear(unsigned dat)  
{ 
	u8 i,n,color;	
	if(dat)
	{
		color = 0xff;
	}
	else
	{
		color = 0;
	}
	for(i=0;i<PAGE_SIZE;i++)  
	{  
		OLED_WR_Byte(YLevel+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte(XLevelL,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte(XLevelH,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<WIDTH;n++)
		{
			OLED_WR_Byte(color,OLED_DATA); 
		}
	}   //更新显示
}

void OLED_Reset(void)
{
	OLED_RST_Set();
	Delay_Ms(100);
	OLED_RST_Clr();
	Delay_Ms(100);
	OLED_RST_Set(); 
}	
	
void OLED_Init(void)
{
	OLED_Reset();     //复位OLED

/**************初始化SSD1306*****************/	
	OLED_WR_Byte(0xAE,OLED_CMD); /*display off*/
	OLED_WR_Byte(0x00,OLED_CMD); /*set lower column address*/
	OLED_WR_Byte(0x10,OLED_CMD); /*set higher column address*/
	OLED_WR_Byte(0x40,OLED_CMD); /*set display start line*/ 
	OLED_WR_Byte(0xB0,OLED_CMD); /*set page address*/
	OLED_WR_Byte(0x81,OLED_CMD); /*contract control*/ 
	OLED_WR_Byte(0xFF,OLED_CMD); /*128*/
	OLED_WR_Byte(0xA1,OLED_CMD); /*set segment remap*/ 
	OLED_WR_Byte(0xA6,OLED_CMD); /*normal / reverse*/
	OLED_WR_Byte(0xA8,OLED_CMD); /*multiplex ratio*/ 
	OLED_WR_Byte(0x3F,OLED_CMD); /*duty = 1/64*/
	OLED_WR_Byte(0xC8,OLED_CMD); /*Com scan direction*/
	OLED_WR_Byte(0xD3,OLED_CMD); /*set display offset*/ 
	OLED_WR_Byte(0x00,OLED_CMD);
	OLED_WR_Byte(0xD5,OLED_CMD); /*set osc division*/ 
	OLED_WR_Byte(0x80,OLED_CMD);
	OLED_WR_Byte(0xD9,OLED_CMD); /*set pre-charge period*/ 
	OLED_WR_Byte(0XF1,OLED_CMD);
	OLED_WR_Byte(0xDA,OLED_CMD); /*set COM pins*/ 
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD); /*set vcomh*/ 
	OLED_WR_Byte(0x30,OLED_CMD);
	OLED_WR_Byte(0x8D,OLED_CMD); /*set charge pump disable*/ 
	OLED_WR_Byte(0x14,OLED_CMD);
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/
}  



void GUI_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	  unsigned char c=0,i=0,j=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>WIDTH-1)
		{
			x=0;
			y=y+2;
		}
		if(Char_Size ==16)
		{
		  for(j=0;j<2;j++)
			{
				OLED_Set_Pos(x,y+j);
				for(i=0;i<8;i++)
				{
						OLED_WR_Byte(F8X16[c*16+i+j*8],OLED_DATA);
				}
			}
		 }
		 else 
		 {	
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
				{
						OLED_WR_Byte(F6x8[c][i],OLED_DATA);
				}
		}
}

void GUI_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
	unsigned char j=0,csize;
	if(Char_Size == 16)
  {
	  csize = Char_Size/2;
	}
  else if(Char_Size == 8)
  {
	  csize = Char_Size/2+2;
	}
	else
	{
		return;
	}
	while (chr[j]!='\0')
	{		
		GUI_ShowChar(x,y,chr[j],Char_Size);
		x+=csize;
		if(x>120)
		{
			x=0;
			y+=Char_Size/8;
		}
		j++;
	}
}

static u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}

void GUI_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 Size)
{         	
	u8 t,temp;
	u8 enshow=0,csize;
  if(Size == 16)
  {
	  csize = Size/2;
	}
  else if(Size == 8)
  {
	  csize = Size/2+2;
	} 	
	else
	{
		return;
	}
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				GUI_ShowChar(x+csize*t,y,' ',Size);
				continue;
			}
			else 
			{
				enshow=1; 
		 	}
		}
	 	GUI_ShowChar(x+csize*t,y,temp+'0',Size); 
	}
}
void GUI_Showfont16(u8 x,u8 y,u8 *s)
{
	u8 i,j,k,tmp;
	u16 num;
	num = sizeof(cfont16)/sizeof(typFNT_GB16);
  for(i=0;i<num;i++)
	{
		if((cfont16[i].Index[0]==*s)&&(cfont16[i].Index[1]==*(s+1)))
		{
			for(k=0;k<2;k++)
			{
			  OLED_Set_Pos(x,y+k);
				for(j=0;j<16;j++)
				{
					tmp = cfont16[i].Msk[j+k*16];
					OLED_WR_Byte(tmp,OLED_DATA);
				}
			}	
			break;
		}	
	}
}
void GUI_ShowCHinese(u8 x,u8 y,u8 *str)
{ 
	u8 hsize =16;
	while(*str!='\0')
	{
			GUI_Showfont16(x,y,str);
		x+=hsize;
		if(x>WIDTH-hsize)
		{
			x=0;
			y+=hsize/8;
		}
		str+=2;
	}			
}

/*void GUI_DrawBMP(u8 x,u8 y,u8 width, u8 height, u8 BMP[])
{ 	
 u8 i,j;
 u8 tmp;
 for(i=0;i<(height+7)/8;i++)
 {
		OLED_Set_Pos(x,y+i);
		for(j=0;j<width;j++)
		{		
					tmp = BMP[i*width+j];
			Delay_Ms(5);
				OLED_WR_Byte(tmp,OLED_DATA);
		}
 } 
}
void kaiss(u8 x,u8 y)
{
	u8 i,j,k;
	u8 a=0;
	u8 tmp;
 for(i=0;i<11;i++)
 {
	 for(k=0;k<2;k++)
	 {
		 OLED_Set_Pos(x,y+k);
			for(j=0;j<12;j++)
			{
					tmp = kais[a+j+12*k];
				OLED_WR_Byte(tmp,OLED_DATA);
			}
			
	 } a=24*i;x =i*12;
 }
}
void TEST_BMP(void)
{
	GUI_DrawBMP(40,0,48,48,BMP);
	Delay_Ms(50);
	kaiss(4,6);
	
}*/
