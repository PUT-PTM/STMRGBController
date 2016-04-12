#ifndef LCD_HD44780_H
#define LCD_HD44780_H

#include <avr/pgmspace.h> //PSTR

#define DDRn DDRD
#define PORTn PORTD

#define DS_PIN PD5
#define ST_PIN PD6
#define SH_PIN PD7

#define DS0 PORTn&=~(1<<DS_PIN) 
#define DS1 PORTn|=(1<<DS_PIN)
#define ST0 PORTn&=~(1<<ST_PIN) 
#define ST1 PORTn|=(1<<ST_PIN)
#define SH0 PORTn&=~(1<<SH_PIN)
#define SH1 PORTn|=(1<<SH_PIN) 


/*
*	MODE
*	1 SLOW while >>=1
*	2 FAST 8x if
*/

#define MODE 2

/*
*	ENABLE_Q7_Q6
*	0 Q7 Q6 DISABLED hi-Z
*	1 Q7 Q6 ENABLED
*/

#define ENABLE_Q7_Q6 1

/*
*+--------------------------+---------+--+---------------------+---------+--+--+
*| while local              | 232 212 |  | if                  | 230 68  |  |  |
*+--------------------------+---------+  +---------------------+---------+--+--+
*| while local +2 pin       | 234 252 |  | if + 2 pin          | 231 140 |  |  |
*+--------------------------+---------+  +---------------------+---------+--+--+
*| while                    | 234 248 |  | if PSTR             | 230 64  |  |  |
*|   local PSTR +2 pin      |         |  |                     |         |  |  |
*+--------------------------+---------+  +---------------------+---------+--+--+
*| while local PSTR         | 232 206 |  | if + 2 pin PSTR     | 231 136 |  |  |
*+--------------------------+---------+  +---------------------+---------+--+--+
*| while                    | 307 158 |  | if 2ms PSTR         | 304 228 |  |  |
*|   local PSTR 2ms         |         |  |                     |         |  |  |
*+--------------------------+---------+  +---------------------+---------+--+--+
*| while                    | 309 198 |  | if + 2 pin 2ms PSTR | 306 86  |  |  |
*|   local PSTR 2ms + 2 pin |         |  |                     |         |  |  |
*+--------------------------+---------+  +---------------------+---------+--+--+
*| while                    | 309 202 |  | if 2ms + 2 pin      | 306 90  |  |  |
*|   local 2ms + 2 pin      |         |  |                     |         |  |  |
*+--------------------------+---------+  +---------------------+---------+--+--+
*| while                    | 307 162 |  | if 2ms              | 304 232 |  |  |
*|   local 2ms              |         |  |                     |         |  |  |
*+--------------------------+---------+--+---------------------+---------+--+--+
*
*/

/* RS */

#define RS1      rejestr(0x01,LCDWRITE)
#define RS0      rejestr(0x01,LCDERASE)


/* RW */
//RW CONNECT TO GND
//#define RW1      rejestr(0x02,LCDWRITE)
//#define RW0      rejestr(0x02,LCDERASE)


/* E */

#define E1       rejestr(0x02,LCDWRITE)
#define E0       rejestr(0x02,LCDERASE)


/* D4 */

#define D41      rejestr(0x04,LCDWRITE)
#define D40      rejestr(0x04,LCDERASE)


/* D5 */

#define D51      rejestr(0x08,LCDWRITE)
#define D50      rejestr(0x08,LCDERASE)


/* D6 */

#define D61      rejestr(0x10,LCDWRITE)
#define D60      rejestr(0x10,LCDERASE)


/* D7 */

#define D71      rejestr(0x20,LCDWRITE)
#define D70      rejestr(0x20,LCDERASE)

#define LCDWRITE 1
#define LCDERASE 0

#define LCDCOMMAND 0
#define LCDDATA    1

//#define USE_TEMP_CHAR
#ifdef USE_TEMP_CHAR

	#define lcd_stopien_celsjusza WriteToLCD(0,LCDDATA);lcd("C")

	void definiuj_stopien_Celsjusza(void);

#endif

#define pos(x,y)  WriteToLCD(0x80|((x)+((y)*0x40)), LCDCOMMAND)

#define cls             WriteToLCD(0x01, LCDCOMMAND)

#define LCD_DISPLAY(DCB) WriteToLCD(0x08|(DCB),LCDCOMMAND)

#define LCD_WRITE_DATA(D)      WriteToLCD((D),LCDDATA)

void rejestr(unsigned char data,unsigned char WriteOrErase);
volatile static uint8_t tempdata;
void lcd_RAM(char *str);
void lcd_init(void);
void WriteToLCD(uint8_t v,unsigned char rs);
void lcd_FLASH(const char *str);
void lcd_int(uint8_t val, uint8_t system);
void lcd_int16(uint16_t val, uint8_t system);
#define lcd(tekst) lcd_FLASH(PSTR(tekst))

#endif
