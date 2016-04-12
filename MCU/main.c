#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "hd44780.h"
#include "WS2812B.h"
#include "UART.h"
volatile uint8_t intro_flag=1;
ISR (USART_RXC_vect)
{
	
	PORTD^=(1<<PD4);
	
}
void intro(void)
{
	uint8_t red_array[16]={10,10,20,10,0,20,15,5,20,10,0,15,5,10,20,10};
	uint8_t green_array[16]={10,15,10,20,5,20,0,15,20,10,20,0,15,20,10,15};
	uint8_t blue_array[16]={20,10,20,15,20,15,10,20,15,10,5,10,15,20,20,15};
	uint8_t r=0,g=0,b=0;
	uint8_t r_next=0,g_next=0,b_next=0;
	
	lcd(" STM RGB Controller");
	pos(3,1);
	lcd("Norbert Hanysz");
	pos(21,0);
	lcd("Mikolaj Owczarczak");
	pos(20,1);
	lcd("BT wait 4 connect...");
	
	while(intro_flag)
	{
		if(r==red_array[r_next] && g==green_array[g_next] && b==blue_array[b_next])
		{
			r_next=g_next=b_next++;
		}
		if(r<red_array[r_next])
			r++;
		else if(r>red_array[r_next])
				r--;
	
		if(g<green_array[g_next])
			g++;
		else if(g>green_array[g_next])
				g--;
	
		if(b<blue_array[b_next])
			b++;
		else if(b>blue_array[b_next])
				b--;
	
		r_next&=0x0f;
		g_next&=0x0f;
		b_next&=0x0f;
		for(uint8_t i=0;i<nOfLEDs;i++)
		WS2812B_send(r,g,b);
		_delay_ms(60);
	}
	cls;
	lcd("Connected!");
	return;
}
int main(void)
{
	lcd_init();
	DDRD|=(1<<PD4);//RS BLINK LED
	UART_Init();
	WS2812B_init();	
	
	
	
	
	
	intro();
/*	uint8_t a=240,b=10;
	uint8_t roznica;
	uint8_t tmp;
		roznica = (a-b)/nOfLEDs;
		tmp=a;
		for(uint8_t i=0;i<nOfLEDs;i++)
		{
			WS2812B_send(0,tmp-roznica,0);
			tmp-=roznica;
		}
		_delay_ms(100);*/
		uint8_t x,y,z,t;
		x=y=z=t=0;
    while (1) 
    {
		t++;
		if(t<=85){x++;	x= t*3;}
		if(t>85 && t<=170){y++;y= (t-85)*3;}
		if(t>170){z++;	z=(t-170)*3;}
	pos(0,1);
		lcd_int(x,10);
		lcd(" ");
	
	
	lcd_int(y,10);
	lcd(" ");
	
	
	lcd_int(z,10);
	lcd(" ");
		WS2812B_send(x,y,z);
		_delay_ms((10));
    }
}

