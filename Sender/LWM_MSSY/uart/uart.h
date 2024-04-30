/*
 * uart.h
 *
 * Created: 2/12/2024 12:13:47
 *  Author: Student
 */ 
#include <stdio.h>

#ifndef UART_H_
#define UART_H_
void UART_init(uint16_t Baudrate,uint8_t interrupt);
void UART_SendChar(uint8_t data);
uint8_t UART_GetChar( void );
void UART_SendString(char *text);
int printCHAR(char character, FILE *stream);
#endif /* UART_H_ */