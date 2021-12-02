#ifndef __KEY_H__
#define __KEY_H__
#include "stc89c52.h"

#define uchar unsigned char 
#define uint unsigned int
	
sbit KEY_sw2_up=P2^4;         
sbit KEY_sw3_down=P2^5;
sbit KEY_sw4_ok=P2^6;
sbit KEY_sw5_back=P2^7;

extern uchar key_num;
uchar KEY_Scan(uchar mode);


#endif