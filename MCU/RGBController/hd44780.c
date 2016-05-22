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

ST_0;
	unsigned char local = 0x80;
	while(local)
	{
		SH_0;
		if(tempdata & local) DS_1; else DS_0;
		
		SH_1;
		local>>=1;
	}
ST_1;
}

void WriteToLCD (uint8_t v,unsigned char rs)
{
	if(rs==1)RS_1; else RS_0;
	
	E_1;
    if(v&0x10) D4_1; else D4_0;
    if(v&0x20) D5_1; else D5_0;
    if(v&0x40) D6_1; else D6_0;
    if(v&0x80) D7_1; else D7_0;
    E_0;  
    _delay_ms(1.5);
	
	E_1;
	
    if(v&0x01) D4_1; else D4_0;
    if(v&0x02) D5_1; else D5_0;
    if(v&0x04) D6_1; else D6_0;
    if(v&0x08) D7_1; else D7_0;
    E_0;
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
