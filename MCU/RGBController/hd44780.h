#ifndef LCD_HD44780_H
#define LCD_HD44780_H

#include <avr/pgmspace.h> //PSTR

#define DDRn DDRD
#define PORTn PORTD

#define DS_PIN PD5
#define ST_PIN PD6
#define SH_PIN PD7

#define DS_0 PORTn&=~(1<<DS_PIN) 
#define DS_1 PORTn|=(1<<DS_PIN)
#define ST_0 PORTn&=~(1<<ST_PIN) 
#define ST_1 PORTn|=(1<<ST_PIN)
#define SH_0 PORTn&=~(1<<SH_PIN)
#define SH_1 PORTn|=(1<<SH_PIN) 

/* RS */

#define RS_1      rejestr(0x01,LCDWRITE)
#define RS_0      rejestr(0x01,LCDERASE)


/* RW */
//RW CONNECT TO GND
//#define RW1      rejestr(0x02,LCDWRITE)
//#define RW0      rejestr(0x02,LCDERASE)


/* E */

#define E_1       rejestr(0x02,LCDWRITE)
#define E_0       rejestr(0x02,LCDERASE)


/* D4 */

#define D4_1      rejestr(0x04,LCDWRITE)
#define D4_0      rejestr(0x04,LCDERASE)


/* D5 */

#define D5_1      rejestr(0x08,LCDWRITE)
#define D5_0      rejestr(0x08,LCDERASE)


/* D6 */

#define D6_1      rejestr(0x10,LCDWRITE)
#define D6_0      rejestr(0x10,LCDERASE)


/* D7 */

#define D7_1      rejestr(0x20,LCDWRITE)
#define D7_0      rejestr(0x20,LCDERASE)

#define LCDWRITE 1
#define LCDERASE 0

#define LCDCOMMAND 0
#define LCDDATA    1

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
#define lcd(tekst) lcd_FLASH(PSTR(tekst))

#endif
