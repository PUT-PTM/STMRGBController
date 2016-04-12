#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
#include "hd44780.h"

void rejestr(unsigned char data, unsigned char WriteOrErase)
{
if(WriteOrErase)
tempdata=(tempdata|data);
else
tempdata&=~(data);

ST0;
#if MODE==1
	#if ENABLE_Q7_Q6==1
		unsigned char local = 0x80;
	#elif ENABLE_Q7_Q6==0
		unsigned char local = 0x20;
	#endif
	while(local)
	{
	SH0;
	if(tempdata & local)DS1; else DS0;
	SH1;
	local>>=1;
	}
#elif MODE==2
	#if ENABLE_Q7_Q6==1
	SH0;
	if(tempdata & 0x80)DS1; else DS0;
	SH1;
	SH0;
	if(tempdata & 0x40)DS1; else DS0;
	SH1;	
	# endif
	
	SH0;
	if(tempdata & 0x20)DS1; else DS0;
	SH1;
	SH0;
	if(tempdata & 0x10)DS1; else DS0;
	SH1;
	SH0;
	if(tempdata & 0x08)DS1; else DS0;
	SH1;
	SH0;
	if(tempdata & 0x04)DS1; else DS0;
	SH1;
	SH0;
	if(tempdata & 0x02)DS1; else DS0;
	SH1;
	SH0;
	if(tempdata & 0x01)DS1; else DS0;
	SH1;
	
#endif
ST1;
}

void WriteToLCD (uint8_t v,unsigned char rs)
{
if(rs==1)RS1; else RS0;
	
	E1;

    if(v&0x10) D41; else D40;
    if(v&0x20) D51; else D50;
    if(v&0x40) D61; else D60;
    if(v&0x80) D71; else D70;
    E0;  
    _delay_ms(1.5);
	
	E1;
	
    if(v&0x01) D41; else D40;
    if(v&0x02) D51; else D50;
    if(v&0x04) D61; else D60;
    if(v&0x08) D71; else D70;
    E0;
	_delay_ms(1.5);
	
	}
void lcd_init(void)
{

DDRn|=(1<<DS_PIN)|(1<<ST_PIN)|(1<<SH_PIN);

WriteToLCD(0x02,LCDCOMMAND);
WriteToLCD(0x28,LCDCOMMAND);
   LCD_DISPLAY(0x04);
   cls;
}
void lcd_RAM(char *str)
{
    unsigned char i =0;

    while(str[i])
        LCD_WRITE_DATA(str[i++]) ;
}
void lcd_FLASH(const char *str)
{
unsigned char znak=0;
	while((znak=pgm_read_byte(str++)))
		LCD_WRITE_DATA(znak);
}
void lcd_int(uint8_t val,uint8_t system)
{
	char bufor[9];
	itoa(val,bufor,system);
	lcd_RAM(bufor);
}
void lcd_int16(uint16_t val,uint8_t system)
{
	char bufor[17];
	itoa(val,bufor,system);
	lcd_RAM(bufor);
}

#ifdef USE_TEMP_CHAR
void definiuj_stopien_Celsjusza(void)
{
	unsigned char tab[8]={6,9,9,6,0,0,0,0};//°
	WriteToLCD(0x40,LCDCOMMAND);
	for(uint8_t i=0;i<8;i++)
	{
		WriteToLCD(tab[i],LCDDATA);
	}
	WriteToLCD(0x00,LCDDATA);
}
/*
	definiuj_stopien_Celsjusza();
	pos(0,0);
	WriteToLCD(0,LCDDATA);
*/

#endif
