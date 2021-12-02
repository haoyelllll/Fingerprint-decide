#include"buzz.h"
#include"delay.h"
#include"led.h"
#include"key.h"
#include"OLED.h"
#include"AS608.h"
#include"stdio.h"

unsigned char local_date=2,base_date=2;//ȫ�ֱ�������ǰ��ͷλ��

void DisplayUpdata()
{
	OLED_Clear(0); //�����ʾ��
	GUI_ShowCHinese(17,0,"ָ�ƿ���ϵͳ");
	GUI_ShowString(70,*(char*)&base_date,"*",16);
	GUI_ShowCHinese(0,2,"��ʼ����");
	GUI_ShowCHinese(0,4,"¼��ָ��");
	GUI_ShowCHinese(0,6,"ɾ��");
	GUI_ShowCHinese(33,6,"ָ��");

}
void Delet_FR()
{	
	OLED_Clear(0); //�����ʾ��
	GUI_ShowCHinese(0,0,"ɾ��");
		GUI_ShowCHinese(32,0,"ָ��");
	GUI_ShowString(80,*(char*)&base_date,"*",16);/****************ȡģ*******************/
	GUI_ShowCHinese(0,2,"���");
		GUI_ShowCHinese(32,2,"ָ�ƿ�");

	
}

char keypressup()//�����ƶ����//��//2
{		

	if(local_date==2)local_date=6;
	else if(local_date==4)local_date=2;
	else if(local_date==6)local_date=4;
	return local_date;	
}
char keypressdown()//�����ƶ����//��//3
{
	
	if(local_date==2)local_date=4;
	else if(local_date==4)local_date=6;
	else if(local_date==6)local_date=2;	
	return local_date;	
}
void main()
{	
	
	OLED_Init();   //��Ļ��ʼ��
  Uart_Init();//��ʼ������
	Buzz_Init();//��ʼ��������
	OLED_Clear(0); //�����ʾ��
	Device_Check();//�ж�AS608�Ƿ�����
	OLED_Clear(0); //�����ʾ��
	DisplayUpdata();//��ʾ�˵��� 
	buzz_times(1);//�ɹ���������
	Led_Init();
	Delay_Ms(100);

	while(1)
	{	Led_Init();
		key_num=0;
		KEY_Scan(0);
		Delay_Ms(100);
		//���λ�ñ仯
		if(local_date!=base_date) 
		{
			base_date=local_date;
			OLED_Clear(0); //�����ʾ��
			DisplayUpdata();
		}
		//��
		if(key_num == 3)
		{
	  	 keypressdown();
			}			
		//��
		if(key_num == 2)
		{ 
		  keypressup();
		}
		//ȷ��
		if(key_num==4)
		{	 						
			 switch(local_date)
				{
					case 2://ָ�ƿ���
					OLED_Clear(0); //�����ʾ��				
					key_num=0;				
					press_FR();	
					local_date = 2;
					DisplayUpdata();key_num=0;
					break;
					
					
					case 4:	 //���ָ��
					OLED_Clear(0); //�����ʾ��
					key_num=0;	
					Add_FR();
					local_date = 4;
					DisplayUpdata();key_num=0;	
					break; 
					
					
					case 6:	//���ָ��
					OLED_Clear(0); //�����ʾ��
					base_date=local_date=0;key_num=0;
					Delet_FR();
					while(1)
					{
						key_num=0;
						KEY_Scan(0);
						Delay_Ms(100);
						//���λ�ñ仯
						if(local_date!=base_date) 
						{
								base_date=local_date;
								OLED_Clear(0); //�����ʾ��
								Delet_FR();
							}
						//��
						if(key_num == 3)
						{
								if(local_date==0)local_date=2;
								else if(local_date==2)local_date=0;
							key_num=0;}			
						//��
						if(key_num == 2)
						{ 
								if(local_date==0)local_date=2;
								else if(local_date==2)local_date=0;
						key_num=0;	}
						//ȷ��
						if(key_num==4)
						{	
							switch(local_date)
							{
							case 0://ɾ��ָ��
							key_num=0;								
							Del_FR();						
							local_date = 0;
							Delet_FR();	key_num=0;									
							break;
							case 2:	 //���ָ��
							key_num=0;
							Del_ALL();
							local_date = 2;
							Delet_FR();		key_num=0;				
							break; 
							}	
						}
						//����
						if(key_num==5)
						{	key_num=0;
							break;
						}Delay_Ms(100); //��ʱ�ж�100MS���һ��				
					}
					local_date = 6;	
					key_num=0;					
					break;
				}
			}Delay_Ms(100); //��ʱ�ж�100MS���һ��  		
	}		
}
