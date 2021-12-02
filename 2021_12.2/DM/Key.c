#include "stc89c52.h"
#include "key.h"
#include "delay.h"
#include "led.h"

//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY_sw2_up按下//加一键->向上选择*****2
//2，KEY_sw3_down按下//减一键->向下选择****3
//3，KEY_sw4_ok按下//确认键*****4
//4，KEY_sw5_back按下//返回键****5
//注意此函数有响应优先级,KEY2>KEY3>KEY4>KEY5
uchar key_num;
uchar KEY_Scan(uchar mode)
{	 
	static uchar key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY_sw2_up==0||KEY_sw3_down==0||KEY_sw4_ok==0||KEY_sw5_back==0))
	{
		Led_A2_Off();
		Delay_Ms(50);//去抖动 
		key_up=0;
		if(KEY_sw2_up==0)return key_num=2;
		else if(KEY_sw3_down==0)return key_num=3;
		else if(KEY_sw4_ok==0)return key_num=4;
		else if(KEY_sw5_back==0)return key_num=5;
		Led_Init();
	}else if(KEY_sw2_up==1&&KEY_sw3_down==1&&KEY_sw4_ok==1&&KEY_sw5_back==1)key_up=1; 	    
 	return 0;// 无按键按下
}