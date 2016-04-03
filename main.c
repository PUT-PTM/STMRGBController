#include <avr/io.h>
#include <avr/interrupt.h>
#include "hd44780.h"
#include "WS2812B.h"
#include "UART.h"

void intro(void)
{
	//do stuff effects
}
int main(void)
{
	lcd_init();
	UART_Init();
	WS2812B_init();	
	
    while (1) 
    {
		
    }
}

