#include "stc89c52.h"
#include "led.h"
sbit led_A2=P1^1;
void Led_Init(void)
{
	
	led_A2=0;
}

void Led_A2_Off(void)
{
		led_A2=1;
}
