/*
 * uart.h
 *
 * Created: 1/11/2016 11:30:46 AM
 *  Author: Atnock
 */ 


#ifndef UART_H_
#define UART_H_

void uart_init(void);
void uart_putchar(char c, FILE *stream);
char uart_getchar(FILE *stream);



#endif /* UART_H_ */