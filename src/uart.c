#include <avr/io.h>
#include "uart.h"
#include <util/setbaud.h>

void init_uart()
{
	UBRR0H = UBRRH_VALUE;                        // UBRRn = F_CPU/16BAUD -1
	UBRR0L = UBRRL_VALUE;
	
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);        //enables the USART receiver and transmitter
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);      // 8-bit data 1 stop
}
void transmit(uint8_t info)
{
	while(!(UCSR0A & (1 << UDRE0)));             //waits for UDRE0 flag to set and data is written to UDR0 buffer when 1
	UDR0 = info;
}
uint8_t receive(void)
{
	while(!(UCSR0A & (1 << RXC0)));              //waits for RXC0 flag to set means there is unread data
	return UDR0;
}

int usart_putchar(char var, FILE *stream)
{
	if(var =='\n') transmit('\r');
	transmit(var);
	return 0;
}
void print_str(const char mystring[])
{
	uint8_t i = 0;
	while(mystring[i])
	{
		transmit(mystring[i]);
		i++;
	}
}

