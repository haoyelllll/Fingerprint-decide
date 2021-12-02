#include "stc89c52.h"
#include "delay.h"
#include "buzz.h"


sbit buzz =P1^2;
void Buzz_Init(void)
{
	buzz=0;
}
void buzz_times(unsigned char times)
{
	unsigned char i=0;
	for(i=0;i<times;i++)
	{
		 buzz=1;
		 Delay_Ms(200);
		 buzz=0;
		 Delay_Ms(200);
	}
}