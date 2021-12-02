#include "stc89c52.h"
#include "delay.h"
#include "buzz.h"
#include "key.h"
#include "OLED.h"
#include <stdio.h>
#include <string.h>
#include "AS608.h"
#include "led.h"


 
//as608通讯协议定义
volatile unsigned char FPM10A_RECEICE_BUFFER[32];
code unsigned char FPM10A_Get_Device[10] ={0x01,0x00,0x07,0x13,0x00,0x00,0x00,0x00,0x00,0x1b};//口令验证
code unsigned char FPM10A_Pack_Head[6] = {0xEF,0x01,0xFF,0xFF,0xFF,0xFF};  //协议包头
code unsigned char FPM10A_Get_Img[6] = {0x01,0x00,0x03,0x01,0x00,0x05};    //获得指纹图像
code unsigned char FPM10A_Get_Templete_Count[6] ={0x01,0x00,0x03,0x1D,0x00,0x21 }; //获得模版总数
code unsigned char FPM10A_Search[11]={0x01,0x00,0x08,0x04,0x01,0x00,0x00,0x03,0xE7,0x00,0xF8}; //搜索指纹搜索范围0 - 999,使用BUFFER1中的特征码搜索
code unsigned char FPM10A_Search_0_9[11]={0x01,0x00,0x08,0x04,0x01,0x00,0x00,0x00,0x13,0x00,0x21}; //搜索0-9号指纹
code unsigned char FPM10A_Img_To_Buffer1[7]={0x01,0x00,0x04,0x02,0x01,0x00,0x08}; //将图像放入到BUFFER1
code unsigned char FPM10A_Img_To_Buffer2[7]={0x01,0x00,0x04,0x02,0x02,0x00,0x09}; //将图像放入到BUFFER2
code unsigned char FPM10A_Reg_Model[6]={0x01,0x00,0x03,0x05,0x00,0x09}; //将BUFFER1跟BUFFER2合成特征模版
code unsigned char FPM10A_Delete_All_Model[6]={0x01,0x00,0x03,0x0D,0x00,0x11};//删除指纹模块里所有的模版

//volatile:系统总是重新从它所在的内存读取数据，即使它前面的指令刚刚从该处读取过数据
volatile unsigned char  FPM10A_Save_Finger[9]={0x01,0x00,0x06,0x06,0x01,0x00,0x0B,0x00,0x19};//将BUFFER1中的特征码存放到指定的位置

//as608默认模块地址
ulong AS608Addr = 0XFFFFFFFF;

uint ID_NUM=0;

char *rdata;



/****串口初始化****/
void Uart_Init(void){//9600bps@11.0592MHz
    SCON=0x50;   //UART方式1:8位UART;   REN=1:允许接收 
    PCON=0x00;   //SMOD=0:波特率不加倍 
    TMOD=0x20;   //T1方式2,用于UART波特率 
    TH1=0xFD; 
    TL1=0xFD;   //UART波特率设置:FDFD，9600;FFFF,57600
    TR1=1;	 //允许T1计数 
    EA=1;
}
void Uart_Send_Byte(unsigned char c){//发送数据
	SBUF = c;
	while(!TI);		//发送完为1 
	TI = 0;
}
unsigned char Uart_Receive_Byte(){//接收数据
	unsigned char dat;
	while(!RI);	 //接收完为1 
	RI = 0;
	dat = SBUF;
	return (dat);
}







/********************** FINGERPRINT命令字 *********************/
//发送包头
void FPM10A_Cmd_Send_Pack_Head(void){
	int i;	
	for(i=0;i<6;i++){ //包头 
     Uart_Send_Byte(FPM10A_Pack_Head[i]);   
    }		
}
//发送指令
void FPM10A_Cmd_Check(void){
	int i=0;
	FPM10A_Cmd_Send_Pack_Head(); //发送通信协议包头
	for(i=0;i<10;i++){		
		Uart_Send_Byte(FPM10A_Get_Device[i]);
	  }
}
//接收反馈数据缓冲
void FPM10A_Receive_Data(unsigned char ucLength){
  unsigned char i;
  for (i=0;i<ucLength;i++)
     FPM10A_RECEICE_BUFFER[i] = Uart_Receive_Byte();
}
//FINGERPRINT_获得指纹图像命令
char FPM10A_Cmd_Get_Img(void){
    unsigned char i;
    FPM10A_Cmd_Send_Pack_Head(); //发送通信协议包头
    for(i=0;i<6;i++){ //发送命令 0x1d
		Uart_Send_Byte(FPM10A_Get_Img[i]);
	}
		return 1;
}
//讲图像转换成特征码存放在Buffer1中
void FINGERPRINT_Cmd_Img_To_Buffer1(void){
 	unsigned char i;
	FPM10A_Cmd_Send_Pack_Head(); //发送通信协议包头      
   	for(i=0;i<7;i++){   //发送命令 将图像转换成 特征码 存放在 CHAR_buffer1     
		Uart_Send_Byte(FPM10A_Img_To_Buffer1[i]);
	}
}
//将图像转换成特征码存放在Buffer2中
void FINGERPRINT_Cmd_Img_To_Buffer2(void){
     unsigned char i;
     for(i=0;i<6;i++){    //发送包头	
		Uart_Send_Byte(FPM10A_Pack_Head[i]);   
   	 }
     for(i=0;i<7;i++){   //发送命令 将图像转换成 特征码 存放在 CHAR_buffer1     
		Uart_Send_Byte(FPM10A_Img_To_Buffer2[i]);
   	  }
}
//搜索全部用户999枚
void FPM10A_Cmd_Search_Finger(void){
	unsigned char i;	   	    
	FPM10A_Cmd_Send_Pack_Head(); //发送通信协议包头
	for(i=0;i<11;i++){         
		Uart_Send_Byte(FPM10A_Search[i]);   
	}
}
void FPM10A_Cmd_Reg_Model(void){
	unsigned char i;	   	    
	FPM10A_Cmd_Send_Pack_Head(); //发送通信协议包头
	for(i=0;i<6;i++){          
		Uart_Send_Byte(FPM10A_Reg_Model[i]);   
	}
}
//删除指纹模块里的所有指纹模版
void FINGERPRINT_Cmd_Delete_All_Model(void){
	unsigned char i;    
    for(i=0;i<6;i++){		//包头
		Uart_Send_Byte(FPM10A_Pack_Head[i]); 
	} 		  
    for(i=0;i<6;i++){ //命令合并指纹模版	
		Uart_Send_Byte(FPM10A_Delete_All_Model[i]);   
	}	
}
//删除指纹模块里的某一个指纹模版
char FINGERPRINT_Cmd_Delete_Model(uint PageID,uint N){
	uint temp;
	uchar str[8];
	str[0]=0xef;str[1]=0x01;str[2]=AS608Addr>>24;
	str[3]=AS608Addr>>16;str[4]=AS608Addr>>8;
	str[5]=AS608Addr;str[6]=0x07;str[7]='\0'; 
	Uart_Send_Byte(0xEF);//发送包头
	Uart_Send_Byte(0x01);//发送包头
	Uart_Send_Byte(AS608Addr>>24);//发送地址
	Uart_Send_Byte(AS608Addr>>16);//发送地址
	Uart_Send_Byte(AS608Addr>>8);//发送地址
	Uart_Send_Byte(AS608Addr);//发送地址
	Uart_Send_Byte(0x01);//命令包标识
	Uart_Send_Byte(0x07>>8);//发送包长度
	Uart_Send_Byte(0x07);//发送包长度
	Uart_Send_Byte(0x0C);//发送指令码
	Uart_Send_Byte(PageID>>8);
	Uart_Send_Byte(PageID);
	Uart_Send_Byte(N>>8);
	Uart_Send_Byte(N);
	temp = 0x01+0x07+0x0C
	+(PageID>>8)+(uchar)PageID
	+(N>>8)+(uchar)N;
	Uart_Send_Byte(temp>>8);//发送校验和
	Uart_Send_Byte(temp);//发送校验和
	rdata=str;
	return 0x00;
}
//保存指纹
void FPM10A_Cmd_Save_Finger( unsigned int storeID ){
	unsigned long temp = 0;
	unsigned char i;
	FPM10A_Save_Finger[5] =(storeID&0xFF00)>>8;
	FPM10A_Save_Finger[6] = (storeID&0x00FF);
	for(i=0;i<7;i++){
		temp = temp + FPM10A_Save_Finger[i];
	}   //计算校验和			 
	FPM10A_Save_Finger[7]=(temp & 0x00FF00) >> 8; //存放校验数据
	FPM10A_Save_Finger[8]= temp & 0x0000FF;		   
	FPM10A_Cmd_Send_Pack_Head(); //发送通信协议包头	
	for(i=0;i<9;i++){
		Uart_Send_Byte(FPM10A_Save_Finger[i]);      //发送命令 将图像转换成 特征码 存放在 CHAR_buffer1
	}  		
}











//录入指纹
void Add_FR(void)
{

	Led_Init();
					GUI_ShowCHinese(0,3,"请输入指纹");
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
					OLED_Clear(0); //清空显示屏
					GUI_ShowCHinese(19,0,"请输入指纹");
					
					
					
					FPM10A_Cmd_Get_Img(); //获得指纹图像
					
					GUI_ShowCHinese(19,2,"读取");
					FPM10A_Receive_Data(12);
			
					GUI_ShowCHinese(19,4,"指纹");
				//判断接收到的确认码,等于0指纹获取成功
				if(FPM10A_RECEICE_BUFFER[9]==0){ 
					
						FINGERPRINT_Cmd_Img_To_Buffer1();
				
						FPM10A_Receive_Data(12);//生成特征
						OLED_Clear(0); //清屏
						GUI_ShowCHinese(48,2,"成功");//1111111111111
						
						GUI_ShowCHinese(0,2,"请再次放入指纹");
						/************************/		
						FPM10A_Cmd_Get_Img(); //获得指纹图像
						FPM10A_Receive_Data(12);Delay_Ms(2000);
					Delay_Ms(3000);
				}
				//判断接收到的确认码,等于0指纹获取成功
				if(FPM10A_RECEICE_BUFFER[9]==0){					
					Delay_Ms(200);
					OLED_Clear(0); //清屏
					GUI_ShowCHinese(0,0,"确认成功");
					GUI_ShowString(0,6,"ID:",16);
					GUI_ShowNum(25,6,ID_NUM,3,16);
					FINGERPRINT_Cmd_Img_To_Buffer2();
					FPM10A_Receive_Data(12);
					Delay_Ms(1000);
					FPM10A_Cmd_Reg_Model();//转换成特征码
					FPM10A_Receive_Data(12); 
					FPM10A_Cmd_Save_Finger(ID_NUM);                		         
	        FPM10A_Receive_Data(12);
					Delay_Ms(2000);
					OLED_Clear(0); //清屏
					GUI_ShowCHinese(48,0,"成功");
					Delay_Ms(1000);	
				}
				else {
					OLED_Clear(0); //清屏
					GUI_ShowCHinese(48,0,"失败失败");
					//while(1){};
				}
			
				
										
}

//刷指纹考勤
void press_FR(void)
{	
		do{
				GUI_ShowCHinese(24,0,"开始考勤");
				GUI_ShowCHinese(24,2,"请放入指纹");Delay_Ms(1000);			
				FPM10A_Cmd_Get_Img(); //获得指纹图像
				FPM10A_Receive_Data(12);
				if(FPM10A_RECEICE_BUFFER[9]==0){//判断接收到的确认码,等于0指纹获取成功
					FINGERPRINT_Cmd_Img_To_Buffer1();
					FPM10A_Receive_Data(12);		
					FPM10A_Cmd_Search_Finger();
					FPM10A_Receive_Data(16);
					if(FPM10A_RECEICE_BUFFER[9]==0){//搜索成功
						OLED_Clear(0);
						GUI_ShowCHinese(24,0,"考勤成功");	

					//拼接指纹ID数
				ID_NUM = FPM10A_RECEICE_BUFFER[10]*256 + FPM10A_RECEICE_BUFFER[11];											
						GUI_ShowString(0,6,"ID:",16);
						GUI_ShowNum(25,6,ID_NUM,3,16);			
						Delay_Ms(4000);
						OLED_Clear(0);}
					else{OLED_Clear(0);GUI_ShowCHinese(24,0,"考勤失败");					
							Delay_Ms(1000);
					}
				}else{OLED_Clear(0);GUI_ShowCHinese(48,4,"无指纹");						
						GUI_ShowString(0,6,"ID:",16);
						GUI_ShowNum(25,6,0,1,16);
						Delay_Ms(1500);
						OLED_Clear(0);
				}				
		}while(key_num!=5); 
	}





//删除单个指纹
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
				
				//rdata[9]=FINGERPRINT_Cmd_Delete_Model(ID_NUM,1);//删除单个指纹
	if(FINGERPRINT_Cmd_Delete_Model(ID_NUM,1)==0)
	{
		OLED_Clear(0);
				GUI_ShowCHinese(0,2,"删");GUI_ShowCHinese(17,2,"除成功");
				Delay_Ms(3000);
				key_num=0;
				break;		
	}
				
		/*	OLED_Clear(0);
				GUI_ShowCHinese(0,2,"删");GUI_ShowCHinese(17,2,"除成功");
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
//清空指纹库
void Del_ALL(void)
{	
	OLED_Clear(0);
				FINGERPRINT_Cmd_Delete_All_Model();FPM10A_Receive_Data(12);
				OLED_Clear(0);
				GUI_ShowCHinese(32,2,"清空成功");	
				Delay_Ms(2000); 
}



//判断连接AS608以及加载开始界面
void Device_Check(void){
		unsigned char i=0;
		FPM10A_RECEICE_BUFFER[9]=1;				           //串口数组第九位可判断是否通信正常
		GUI_ShowCHinese(0,0,"加");//设备加载中界面
	GUI_ShowCHinese(0,3,"加");//设备加载中界面
	GUI_ShowCHinese(0,5,"加");//设备加载中界面
		for(i=0;i<5;i++){
			Delay_Ms(100);						           
		}									
		GUI_ShowCHinese(56,0,"载");//设备加载中界面	
		GUI_ShowCHinese(56,3,"载");//设备加载中界面	
GUI_ShowCHinese(56,5,"载");//设备加载中界面		
		FPM10A_Cmd_Check();									//单片机向指纹模块发送校对命令
		FPM10A_Receive_Data(12);							//将串口接收到的数据转存
 		if(FPM10A_RECEICE_BUFFER[9] == 0){					//判断数据低第9位是否接收到0		
			GUI_ShowCHinese(80,0,"中");//设备加载中界面
			GUI_ShowCHinese(80,3,"中");//设备加载中界面
GUI_ShowCHinese(80,5,"中");//设备加载中界面			
		}				
}






