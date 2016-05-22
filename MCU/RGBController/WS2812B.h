#ifndef WS2812B_H_
#define WS2812B_H_

#define WS2812B_DDR DDRB
#define WS2812B_PORT PORTB
#define WS2812B_PIN PB3

#define nOfLEDs 16
void WS2812B_init(void);
void WS2812B_sendone(void);
void WS2812B_sendzero(void);
void WS2812B_none(void);
void WS2812B_send(uint8_t r, uint8_t g, uint8_t b);

#endif