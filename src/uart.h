#include <stdio.h>
#define F_CPU 8000000UL
#ifndef BAUD
#define BAUD 9600
#endif

void init_uart();

void transmit(uint8_t info);

uint8_t receive(void);

int usart_putchar(char var, FILE *stream);

void print_str(const char mystring[]);



