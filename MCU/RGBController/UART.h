#ifndef UART_H
#define UART_H


void UART_Init(void);

void UART_Char(char data);

void UART_Write(const char *string);

char UART_Read(void);


#endif