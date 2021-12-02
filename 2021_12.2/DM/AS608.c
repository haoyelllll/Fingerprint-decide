#include "stc89c52.h"
#include "delay.h"
#include "buzz.h"
#include "key.h"
#include "OLED.h"
#include <stdio.h>
#include <string.h>
#include "AS608.h"
#include "led.h"


 
//as608ͨѶЭ�鶨��
volatile unsigned char FPM10A_RECEICE_BUFFER[32];
code unsigned char FPM10A_Get_Device[10] ={0x01,0x00,0x07,0x13,0x00,0x00,0x00,0x00,0x00,0x1b};//������֤
code unsigned char FPM10A_Pack_Head[6] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF};  //Э���ͷ
code unsigned char FPM10A_Get_Img[6] = {0x01,0x00,0x03,0x01,0x00,0x05};    //���ָ��ͼ��
code unsigned char FPM10A_Get_Templete_Count[6] ={0x01,0x00,0x03,0x1D,0x00,0x21 }; //���ģ������
code unsigned char FPM10A_Search[11]={0x01,0x00,0x08,0x04,0x01,0x00,0x00,0x03,0xE7,0x00,0xF8}; //����ָ��������Χ0 - 999,ʹ��BUFFER1�е�����������
code unsigned char FPM10A_Search_0_9[11]={0x01,0x00,0x08,0x04,0x01,0x00,0x00,0x00,0x13,0x00,0x21}; //����0-9��ָ��
code unsigned char FPM10A_Img_To_Buffer1[7]={0x01,0x00,0x04,0x02,0x01,0x00,0x08}; //��ͼ����뵽BUFFER1
code unsigned char FPM10A_Img_To_Buffer2[7]={0x01,0x00,0x04,0x02,0x02,0x00,0x09}; //��ͼ����뵽BUFFER2
code unsigned char FPM10A_Reg_Model[6]={0x01,0x00,0x03,0x05,0x00,0x09}; //��BUFFER1��BUFFER2�ϳ�����ģ��
code unsigned char FPM10A_Delete_All_Model[6]={0x01,0x00,0x03,0x0D,0x00,0x11};//ɾ��ָ��ģ�������е�ģ��

//volatile:ϵͳ�������´������ڵ��ڴ��ȡ���ݣ���ʹ��ǰ���ָ��ոմӸô���ȡ������
volatile unsigned char  FPM10A_Save_Finger[9]={0x01,0x00,0x06,0x06,0x01,0x00,0x0B,0x00,0x19};//��BUFFER1�е��������ŵ�ָ����λ��

//as608Ĭ��ģ���ַ
ulong AS608Addr = 0XFFFFFFFF;

uint ID_NUM=0;

char *rdata;



/****���ڳ�ʼ��****/
void Uart_Init(void){//9600bps@11.0592MHz
    SCON=0x50;   //UART��ʽ1:8λUART;   REN=1:������� 
    PCON=0x00;   //SMOD=0:�����ʲ��ӱ� 
    TMOD=0x20;   //T1��ʽ2,����UART������ 
    TH1=0xFD; 
    TL1=0xFD;   //UART����������:FDFD��9600;FFFF,57600
    TR1=1;	 //����T1���� 
    EA=1;
}
void Uart_Send_Byte(unsigned char c){//��������
	SBUF = c;
	while(!TI);		//������Ϊ1 
	TI = 0;
}
unsigned char Uart_Receive_Byte(){//��������
	unsigned char dat;
	while(!RI);	 //������Ϊ1 
	RI = 0;
	dat = SBUF;
	return (dat);
}







/********************** FINGERPRINT������ *********************/
//���Ͱ�ͷ
void FPM10A_Cmd_Send_Pack_Head(void){
	int i;	
	for(i=0;i<6;i++){ //��ͷ 
     Uart_Send_Byte(FPM10A_Pack_Head[i]);   
    }		
}
//����ָ��
void FPM10A_Cmd_Check(void){
	int i=0;
	FPM10A_Cmd_Send_Pack_Head(); //����ͨ��Э���ͷ
	for(i=0;i<10;i++){		
		Uart_Send_Byte(FPM10A_Get_Device[i]);
	  }
}
//���շ������ݻ���
void FPM10A_Receive_Data(unsigned char ucLength){
  unsigned char i;
  for (i=0;i<ucLength;i++)
     FPM10A_RECEICE_BUFFER[i] = Uart_Receive_Byte();
}
//FINGERPRINT_���ָ��ͼ������
char FPM10A_Cmd_Get_Img(void){
    unsigned char i;
    FPM10A_Cmd_Send_Pack_Head(); //����ͨ��Э���ͷ
    for(i=0;i<6;i++){ //�������� 0x1d
		Uart_Send_Byte(FPM10A_Get_Img[i]);
	}
		return 1;
}
//��ͼ��ת��������������Buffer1��
void FINGERPRINT_Cmd_Img_To_Buffer1(void){
 	unsigned char i;
	FPM10A_Cmd_Send_Pack_Head(); //����ͨ��Э���ͷ      
   	for(i=0;i<7;i++){   //�������� ��ͼ��ת���� ������ ����� CHAR_buffer1     
		Uart_Send_Byte(FPM10A_Img_To_Buffer1[i]);
	}
}
//��ͼ��ת��������������Buffer2��
void FINGERPRINT_Cmd_Img_To_Buffer2(void){
     unsigned char i;
     for(i=0;i<6;i++){    //���Ͱ�ͷ	
		Uart_Send_Byte(FPM10A_Pack_Head[i]);   
   	 }
     for(i=0;i<7;i++){   //�������� ��ͼ��ת���� ������ ����� CHAR_buffer1     
		Uart_Send_Byte(FPM10A_Img_To_Buffer2[i]);
   	  }
}
//����ȫ���û�999ö
void FPM10A_Cmd_Search_Finger(void){
	unsigned char i;	   	    
	FPM10A_Cmd_Send_Pack_Head(); //����ͨ��Э���ͷ
	for(i=0;i<11;i++){         
		Uart_Send_Byte(FPM10A_Search[i]);   
	}
}
void FPM10A_Cmd_Reg_Model(void){
	unsigned char i;	   	    
	FPM10A_Cmd_Send_Pack_Head(); //����ͨ��Э���ͷ
	for(i=0;i<6;i++){          
		Uart_Send_Byte(FPM10A_Reg_Model[i]);   
	}
}
//ɾ��ָ��ģ���������ָ��ģ��
void FINGERPRINT_Cmd_Delete_All_Model(void){
	unsigned char i;    
    for(i=0;i<6;i++){		//��ͷ
		Uart_Send_Byte(FPM10A_Pack_Head[i]); 
	} 		  
    for(i=0;i<6;i++){ //����ϲ�ָ��ģ��	
		Uart_Send_Byte(FPM10A_Delete_All_Model[i]);   
	}	
}
//ɾ��ָ��ģ�����ĳһ��ָ��ģ��
char FINGERPRINT_Cmd_Delete_Model(uint PageID,uint N){
	uint temp;
	uchar str[8];
	str[0]=0xef;str[1]=0x01;str[2]=AS608Addr>>24;
	str[3]=AS608Addr>>16;str[4]=AS608Addr>>8;
	str[5]=AS608Addr;str[6]=0x07;str[7]='\0'; 
	Uart_Send_Byte(0xEF);//���Ͱ�ͷ
	Uart_Send_Byte(0x01);//���Ͱ�ͷ
	Uart_Send_Byte(AS608Addr>>24);//���͵�ַ
	Uart_Send_Byte(AS608Addr>>16);//���͵�ַ
	Uart_Send_Byte(AS608Addr>>8);//���͵�ַ
	Uart_Send_Byte(AS608Addr);//���͵�ַ
	Uart_Send_Byte(0x01);//�������ʶ
	Uart_Send_Byte(0x07>>8);//���Ͱ�����
	Uart_Send_Byte(0x07);//���Ͱ�����
	Uart_Send_Byte(0x0C);//����ָ����
	Uart_Send_Byte(PageID>>8);
	Uart_Send_Byte(PageID);
	Uart_Send_Byte(N>>8);
	Uart_Send_Byte(N);
	temp = 0x01+0x07+0x0C
	+(PageID>>8)+(uchar)PageID
	+(N>>8)+(uchar)N;
	Uart_Send_Byte(temp>>8);//����У���
	Uart_Send_Byte(temp);//����У���
	rdata=str;
	return 0x00;
}
//����ָ��
void FPM10A_Cmd_Save_Finger( unsigned int storeID ){
	unsigned long temp = 0;
	unsigned char i;
	FPM10A_Save_Finger[5] =(storeID&0xFF00)>>8;
	FPM10A_Save_Finger[6] = (storeID&0x00FF);
	for(i=0;i<7;i++){
		temp = temp + FPM10A_Save_Finger[i];
	}   //����У���			 
	FPM10A_Save_Finger[7]=(temp & 0x00FF00) >> 8; //���У������
	FPM10A_Save_Finger[8]= temp & 0x0000FF;		   
	FPM10A_Cmd_Send_Pack_Head(); //����ͨ��Э���ͷ	
	for(i=0;i<9;i++){
		Uart_Send_Byte(FPM10A_Save_Finger[i]);      //�������� ��ͼ��ת���� ������ ����� CHAR_buffer1
	}  		
}











//¼��ָ��
void Add_FR(void)
{

	Led_Init();
					GUI_ShowCHinese(0,3,"������ָ��");
					GUI_ShowString(81,3,"ID",16);
					ID_NUM=0;
					key_num=0;
					KEY_Scan(0);
					Delay_Ms(100);
					while(key_num!=4)
					{
						key_num=KEY_Scan(0);
						Delay_Ms(100);
						if(key_num==3){

						if(ID_NUM>0)ID_NUM--;
							key_num=0;
						}
						if(key_num==2){
						
						
						if(ID_NUM<99)ID_NUM++;
							key_num=0;
						}
						/*if(key_num==5){
						key_num=0;
						break;
						}*/
						GUI_ShowString(0,6,"ID:",16);
						GUI_ShowNum(25,6,ID_NUM,3,16);
					}
					OLED_Clear(0); //�����ʾ��
					GUI_ShowCHinese(19,0,"������ָ��");
					
					
					
					FPM10A_Cmd_Get_Img(); //���ָ��ͼ��
					
					GUI_ShowCHinese(19,2,"��ȡ");
					FPM10A_Receive_Data(12);
			
					GUI_ShowCHinese(19,4,"ָ��");
				//�жϽ��յ���ȷ����,����0ָ�ƻ�ȡ�ɹ�
				if(FPM10A_RECEICE_BUFFER[9]==0){ 
					
						FINGERPRINT_Cmd_Img_To_Buffer1();
				
						FPM10A_Receive_Data(12);//��������
						OLED_Clear(0); //����
						GUI_ShowCHinese(48,2,"�ɹ�");//1111111111111
						
						GUI_ShowCHinese(0,2,"���ٴη���ָ��");
						/************************/		
						FPM10A_Cmd_Get_Img(); //���ָ��ͼ��
						FPM10A_Receive_Data(12);Delay_Ms(2000);
					Delay_Ms(3000);
				}
				//�жϽ��յ���ȷ����,����0ָ�ƻ�ȡ�ɹ�
				if(FPM10A_RECEICE_BUFFER[9]==0){					
					Delay_Ms(200);
					OLED_Clear(0); //����
					GUI_ShowCHinese(0,0,"ȷ�ϳɹ�");
					GUI_ShowString(0,6,"ID:",16);
					GUI_ShowNum(25,6,ID_NUM,3,16);
					FINGERPRINT_Cmd_Img_To_Buffer2();
					FPM10A_Receive_Data(12);
					Delay_Ms(1000);
					FPM10A_Cmd_Reg_Model();//ת����������
					FPM10A_Receive_Data(12); 
					FPM10A_Cmd_Save_Finger(ID_NUM);                		         
	        FPM10A_Receive_Data(12);
					Delay_Ms(2000);
					OLED_Clear(0); //����
					GUI_ShowCHinese(48,0,"�ɹ�");
					Delay_Ms(1000);	
				}
				else {
					OLED_Clear(0); //����
					GUI_ShowCHinese(48,0,"ʧ��ʧ��");
					//while(1){};
				}
			
				
										
}

//ˢָ�ƿ���
void press_FR(void)
{	
		do{
				GUI_ShowCHinese(24,0,"��ʼ����");
				GUI_ShowCHinese(24,2,"�����ָ��");Delay_Ms(1000);			
				FPM10A_Cmd_Get_Img(); //���ָ��ͼ��
				FPM10A_Receive_Data(12);
				if(FPM10A_RECEICE_BUFFER[9]==0){//�жϽ��յ���ȷ����,����0ָ�ƻ�ȡ�ɹ�
					FINGERPRINT_Cmd_Img_To_Buffer1();
					FPM10A_Receive_Data(12);		
					FPM10A_Cmd_Search_Finger();
					FPM10A_Receive_Data(16);
					if(FPM10A_RECEICE_BUFFER[9]==0){//�����ɹ�
						OLED_Clear(0);
						GUI_ShowCHinese(24,0,"���ڳɹ�");	

					//ƴ��ָ��ID��
				ID_NUM = FPM10A_RECEICE_BUFFER[10]*256 + FPM10A_RECEICE_BUFFER[11];											
						GUI_ShowString(0,6,"ID:",16);
						GUI_ShowNum(25,6,ID_NUM,3,16);			
						Delay_Ms(4000);
						OLED_Clear(0);}
					else{OLED_Clear(0);GUI_ShowCHinese(24,0,"����ʧ��");					
							Delay_Ms(1000);
					}
				}else{OLED_Clear(0);GUI_ShowCHinese(48,4,"��ָ��");						
						GUI_ShowString(0,6,"ID:",16);
						GUI_ShowNum(25,6,0,1,16);
						Delay_Ms(1500);
						OLED_Clear(0);
				}				
		}while(key_num!=5); 
	}





//ɾ������ָ��
void Del_FR(void)
{
		uint ID_NUM=0;
		key_num=0;
		KEY_Scan(0);
	
		GUI_ShowString(0,6,"ID:",16);Delay_Ms(100);
		while(key_num!=4)
		{
			key_num=0;
			KEY_Scan(0);Delay_Ms(100);
			if(key_num==3){
				key_num=0;
				if(ID_NUM>0)ID_NUM--;
			}
			if(key_num==2){
				key_num=0;
				if(ID_NUM<99)ID_NUM++;
			}
			if(key_num==4){
				
				//rdata[9]=FINGERPRINT_Cmd_Delete_Model(ID_NUM,1);//ɾ������ָ��
	if(FINGERPRINT_Cmd_Delete_Model(ID_NUM,1)==0)
	{
		OLED_Clear(0);
				GUI_ShowCHinese(0,2,"ɾ");GUI_ShowCHinese(17,2,"���ɹ�");
				Delay_Ms(3000);
				key_num=0;
				break;		
	}
				
		/*	OLED_Clear(0);
				GUI_ShowCHinese(0,2,"ɾ");GUI_ShowCHinese(17,2,"���ɹ�");
				Delay_Ms(3000);
				key_num=0;
				break;	*/			
			}
			if(key_num==5){
				OLED_Clear(0);
				key_num=0;
				break;
			}	
			GUI_ShowNum(25,6,ID_NUM,3,16);
			Delay_Ms(100);
		}	
}
//���ָ�ƿ�
void Del_ALL(void)
{	
	OLED_Clear(0);
				FINGERPRINT_Cmd_Delete_All_Model();FPM10A_Receive_Data(12);
				OLED_Clear(0);
				GUI_ShowCHinese(32,2,"��ճɹ�");	
				Delay_Ms(2000); 
}



//�ж�����AS608�Լ����ؿ�ʼ����
void Device_Check(void){
		unsigned char i=0;
		FPM10A_RECEICE_BUFFER[9]=1;				           //��������ھ�λ���ж��Ƿ�ͨ������
		GUI_ShowCHinese(0,0,"��");//�豸�����н���
	GUI_ShowCHinese(0,3,"��");//�豸�����н���
	GUI_ShowCHinese(0,5,"��");//�豸�����н���
		for(i=0;i<5;i++){
			Delay_Ms(100);						           
		}									
		GUI_ShowCHinese(56,0,"��");//�豸�����н���	
		GUI_ShowCHinese(56,3,"��");//�豸�����н���	
GUI_ShowCHinese(56,5,"��");//�豸�����н���		
		FPM10A_Cmd_Check();									//��Ƭ����ָ��ģ�鷢��У������
		FPM10A_Receive_Data(12);							//�����ڽ��յ�������ת��
 		if(FPM10A_RECEICE_BUFFER[9] == 0){					//�ж����ݵ͵�9λ�Ƿ���յ�0		
			GUI_ShowCHinese(80,0,"��");//�豸�����н���
			GUI_ShowCHinese(80,3,"��");//�豸�����н���
GUI_ShowCHinese(80,5,"��");//�豸�����н���			
		}				
}






