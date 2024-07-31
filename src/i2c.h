#include <avr/io.h>

void i2c_init(void);

void wait(void);

void start(void);

void stop(void);

void send(uint8_t data);

uint8_t Ack(void);

uint8_t Nack(void);
