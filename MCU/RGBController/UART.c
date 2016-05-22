#include <avr/io.h>
#include "UART.h"

void UART_Init(void)
{
	UBRRH=0;
	UBRRL=8;
	UCSRA=(1<<RXC);
	UCSRB=(1<<TXEN)|(1<<RXEN)|(1<<RXCIE);
	UCSRC=(1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);		//8Mhz M8 9600/8/n/1

}

void UART_Char(char data)
{
	while(!(UCSRA & (1<<UDRE)));
	UDR=data;
}

void UART_Write(const char *string)
{
	while(*string)
	{
	UART_Char(*string++);
	}	
}
char UART_Read(void)
{
	while(!(UCSRA & (1<<RXC)));
	return UDR;
}