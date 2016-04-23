#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#include "hd44780.h"
#include "WS2812B.h"
#include "UART.h"

volatile uint8_t intro_flag=1,audio_flag,data_uart,i_data,frame_ready;
volatile uint8_t data[17]={'\0'};

//$a%123%123%123#	- color accelerometer
//$m%12#			- music 
//$h#				- hello
ISR (USART_RXC_vect)
{
	data_uart=UDR;
	if(data_uart=='$')
	{
		i_data=0;
	}
	data[i_data++]=data_uart;
	if(data_uart=='#' && i_data)frame_ready=1;
	
	if(frame_ready)
	{
		frame_ready=0;
		if(data[0]=='$' && data[1]=='h' && data[2]=='#')
		intro_flag=0;
		if( (!audio_flag) && data[0]=='$' && data[1]=='m' && data[2]=='%' && (data[3]>='0' || data[3]<='9') && (data[4]=='#' || data[5]=='#'))
		audio_flag=1;
	}
	
	PORTD^=(1<<PD4);
	
}
void set_equalizer(uint8_t level,uint8_t tab_red[],uint8_t tab_green[])
{
	for(uint8_t i=0;i< level;i++)
	{
		WS2812B_send(tab_red[i],tab_green[i],0);
	}
	for(uint8_t i=level;i<nOfLEDs;i++)
	{
		WS2812B_send(0,0,0);
	}
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
		_delay_ms(50);
	}
	cls;
	lcd("Connected!");
	
	while(r || g || b)
	{
		if(r)r--;
		if(g)g--;
		if(b)b--;
		for(uint8_t i=0;i<nOfLEDs;i++)
		WS2812B_send(r,g,b);
		_delay_ms(40);
	}
	return;
}

int main(void)
{
	lcd_init();
	DDRD|=(1<<PD4);//RS BLINK LED
	
	UART_Init();
	WS2812B_init();	
	uint8_t red_tab[nOfLEDs]={0,0,0,0,0,0,0,0,10,10,10,10,10,10,10,10};
	uint8_t green_tab[nOfLEDs]={10,10,10,10,10,10,10,10,10,10,10,10,0,0,0,0};
	
	sei();
    while (1) 
    {
		if(audio_flag)
		{
			audio_flag=0;
			
			int sound_level;
			char buffer[3]={'\0'};
			
			buffer[0]=data[3];
			if(data[5]=='#')
				buffer[1]=data[4];
			
			sound_level=atoi(buffer);
			
			set_equalizer(sound_level,red_tab,green_tab);
		}
    }
}

//	intro();

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

		//uint8_t x,y,z,t;
	//	x=y=z=t=0;
	// #036h#035