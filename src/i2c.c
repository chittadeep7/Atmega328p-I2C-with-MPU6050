#include "i2c.h"

void i2c_init(void)
{
	TWBR = 32;                    // prescaler for sck  F_CPU/16+2*TWBR *1 =100kHz  i2c clock
	TWCR |= (1 << TWEN);         //enables i2c interface
}
void wait(void)
{
	while(!( TWCR & (1 << TWINT)));    //twi interrupt flag, set when current opn is done
	
}
void start(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);   //clears interrupt flag ->enables i2c->start condition
	wait();
}
void stop()
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	
}
uint8_t Ack(void)                                     //generates acknowledgement
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	wait();
	return TWDR;
}
uint8_t Nack(void)                                    //generates NACK
{
	TWCR = (1 << TWINT) | (1 << TWEN);
	wait();
	return TWDR;
}
void send(uint8_t data)
{
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);               //clears interrupt flag and enables twi interface
	wait();
}

