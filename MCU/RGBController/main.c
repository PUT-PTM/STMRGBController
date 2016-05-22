#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#include "hd44780.h"
#include "WS2812B.h"
#include "UART.h"

volatile uint8_t intro_flag=1,data_uart,i_data,frame_ready,acc_flag,bye_flag=1;
volatile uint8_t data[17]={'\0'};


//$a%123%123%123#	- color accelerometer
//$hi#				- hello
//$bye#
ISR (USART_RXC_vect)
{
	data_uart=UDR;
	PORTD&=~(1<<PD4);
	if(data_uart=='$')
	{
		i_data=0;
	}
	data[i_data++]=data_uart;
	if(data_uart=='#' && i_data)frame_ready=1;
	
	if(frame_ready)
	{
		frame_ready=0; 
		if(data[0]=='$' && data[1]=='h' &&  data[2]=='i' && data[3]=='#')
			intro_flag=0;
		if(data[0]=='$' && data[1]=='a' && data[2]=='%')
			acc_flag=1;
		if(data[0]=='$' && data[1]=='b' )// data[2]=='y' && data[3]=='e' && data[4]=='#'
			bye_flag=1;
	}
	
	PORTD|=(1<<PD4);
	
}

void intro(void)
{
	uint8_t red_array[16]={10,10,20,10,0,20,15,5,20,10,0,15,5,10,20,10};
	uint8_t green_array[16]={10,15,10,20,5,20,0,15,20,10,20,0,15,20,10,15};
	uint8_t blue_array[16]={20,10,20,15,20,15,10,20,15,10,5,10,15,20,20,15};
	uint8_t r=0,g=0,b=0;
	uint8_t r_next=0,g_next=0,b_next=0;
	cls;
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

void analyse_frame(char bufferred[], char buffergreen[], char bufferblue[])
{
	uint8_t inc=0;
	
	uint8_t next_start=0;
	
	for(uint8_t i=3;i<6;i++)
	{
		if(data[i]!='%')
		bufferred[inc++] = data[i];
		else
		break;
		next_start = i;
	}
	
	inc=0;
	
	for(uint8_t i=next_start+2;i<next_start+5;i++)
	{
		if(data[i]!='%')
		buffergreen[inc++] = data[i];
		else
		break;
		next_start=i;
	}
	
	inc =0;
	
	for(uint8_t i=next_start+2;i<next_start+5;i++)
	{
		if(data[i]!='%')
		bufferblue[inc++] = data[i];
		else
		break;
	}
}
int main(void)
{
	lcd_init();
	DDRD|=(1<<PD4);//RS BLINK LED
	
	UART_Init();
	WS2812B_init();	
	
	sei();
	
	uint8_t red,green,blue;
	char buffergreen[4];
	char bufferred[4];
	char bufferblue[4];
	

    while (1) 
    {
		if(bye_flag)
		{
			bye_flag=0;
			intro_flag=1;
			intro();
		}
		
		if(acc_flag)
		{
			acc_flag=0;
			
			for(uint8_t i=0;i<3;i++)
				bufferred[i]=buffergreen[i]=bufferblue[i]='\0';
			
			analyse_frame(bufferred,buffergreen,bufferblue);
			
			red=atoi(bufferred);
			green=atoi(buffergreen);
			blue=atoi(bufferblue);
			
			pos(0,0);
			lcd_int(red,10);			lcd("  ");
			lcd_int(green,10);			lcd("  ");
			lcd_int(blue,10);			lcd("  ");
			pos(0,1);
			lcd_RAM(data);
			
			for(uint8_t i=0;i<nOfLEDs;i++)
				WS2812B_send(red,green,blue);
		}
		
    }
}

