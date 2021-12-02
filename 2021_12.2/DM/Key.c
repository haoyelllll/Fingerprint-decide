#include "stc89c52.h"
#include "key.h"
#include "delay.h"
#include "led.h"

//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY_sw2_up����//��һ��->����ѡ��*****2
//2��KEY_sw3_down����//��һ��->����ѡ��****3
//3��KEY_sw4_ok����//ȷ�ϼ�*****4
//4��KEY_sw5_back����//���ؼ�****5
//ע��˺�������Ӧ���ȼ�,KEY2>KEY3>KEY4>KEY5
uchar key_num;
uchar KEY_Scan(uchar mode)
{	 
	static uchar key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY_sw2_up==0||KEY_sw3_down==0||KEY_sw4_ok==0||KEY_sw5_back==0))
	{
		Led_A2_Off();
		Delay_Ms(50);//ȥ���� 
		key_up=0;
		if(KEY_sw2_up==0)return key_num=2;
		else if(KEY_sw3_down==0)return key_num=3;
		else if(KEY_sw4_ok==0)return key_num=4;
		else if(KEY_sw5_back==0)return key_num=5;
		Led_Init();
	}else if(KEY_sw2_up==1&&KEY_sw3_down==1&&KEY_sw4_ok==1&&KEY_sw5_back==1)key_up=1; 	    
 	return 0;// �ް�������
}