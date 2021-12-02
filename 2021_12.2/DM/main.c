#include"buzz.h"
#include"delay.h"
#include"led.h"
#include"key.h"
#include"OLED.h"
#include"AS608.h"
#include"stdio.h"

unsigned char local_date=2,base_date=2;//全局变量，当前箭头位置

void DisplayUpdata()
{
	OLED_Clear(0); //清空显示屏
	GUI_ShowCHinese(17,0,"指纹考勤系统");
	GUI_ShowString(70,*(char*)&base_date,"*",16);
	GUI_ShowCHinese(0,2,"开始考勤");
	GUI_ShowCHinese(0,4,"录入指纹");
	GUI_ShowCHinese(0,6,"删除");
	GUI_ShowCHinese(33,6,"指纹");

}
void Delet_FR()
{	
	OLED_Clear(0); //清空显示屏
	GUI_ShowCHinese(0,0,"删除");
		GUI_ShowCHinese(32,0,"指纹");
	GUI_ShowString(80,*(char*)&base_date,"*",16);/****************取模*******************/
	GUI_ShowCHinese(0,2,"清空");
		GUI_ShowCHinese(32,2,"指纹库");

	
}

char keypressup()//按键移动光标//上//2
{		

	if(local_date==2)local_date=6;
	else if(local_date==4)local_date=2;
	else if(local_date==6)local_date=4;
	return local_date;	
}
char keypressdown()//按键移动光标//下//3
{
	
	if(local_date==2)local_date=4;
	else if(local_date==4)local_date=6;
	else if(local_date==6)local_date=2;	
	return local_date;	
}
void main()
{	
	
	OLED_Init();   //屏幕初始化
  Uart_Init();//初始化串口
	Buzz_Init();//初始化蜂鸣器
	OLED_Clear(0); //清空显示屏
	Device_Check();//判断AS608是否连接
	OLED_Clear(0); //清空显示屏
	DisplayUpdata();//显示菜单栏 
	buzz_times(1);//成功可以用了
	Led_Init();
	Delay_Ms(100);

	while(1)
	{	Led_Init();
		key_num=0;
		KEY_Scan(0);
		Delay_Ms(100);
		//光标位置变化
		if(local_date!=base_date) 
		{
			base_date=local_date;
			OLED_Clear(0); //清空显示屏
			DisplayUpdata();
		}
		//下
		if(key_num == 3)
		{
	  	 keypressdown();
			}			
		//上
		if(key_num == 2)
		{ 
		  keypressup();
		}
		//确定
		if(key_num==4)
		{	 						
			 switch(local_date)
				{
					case 2://指纹考勤
					OLED_Clear(0); //清空显示屏				
					key_num=0;				
					press_FR();	
					local_date = 2;
					DisplayUpdata();key_num=0;
					break;
					
					
					case 4:	 //添加指纹
					OLED_Clear(0); //清空显示屏
					key_num=0;	
					Add_FR();
					local_date = 4;
					DisplayUpdata();key_num=0;	
					break; 
					
					
					case 6:	//清空指纹
					OLED_Clear(0); //清空显示屏
					base_date=local_date=0;key_num=0;
					Delet_FR();
					while(1)
					{
						key_num=0;
						KEY_Scan(0);
						Delay_Ms(100);
						//光标位置变化
						if(local_date!=base_date) 
						{
								base_date=local_date;
								OLED_Clear(0); //清空显示屏
								Delet_FR();
							}
						//下
						if(key_num == 3)
						{
								if(local_date==0)local_date=2;
								else if(local_date==2)local_date=0;
							key_num=0;}			
						//上
						if(key_num == 2)
						{ 
								if(local_date==0)local_date=2;
								else if(local_date==2)local_date=0;
						key_num=0;	}
						//确定
						if(key_num==4)
						{	
							switch(local_date)
							{
							case 0://删除指纹
							key_num=0;								
							Del_FR();						
							local_date = 0;
							Delet_FR();	key_num=0;									
							break;
							case 2:	 //清空指纹
							key_num=0;
							Del_ALL();
							local_date = 2;
							Delet_FR();		key_num=0;				
							break; 
							}	
						}
						//返回
						if(key_num==5)
						{	key_num=0;
							break;
						}Delay_Ms(100); //延时判断100MS检测一次				
					}
					local_date = 6;	
					key_num=0;					
					break;
				}
			}Delay_Ms(100); //延时判断100MS检测一次  		
	}		
}
