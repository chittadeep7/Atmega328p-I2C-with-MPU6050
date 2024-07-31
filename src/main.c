#include <stdio.h>
#include <math.h>
#include <avr/wdt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "i2c.h"
#include "mpu6050.h"

static FILE mystdout = FDEV_SETUP_STREAM(usart_putchar, NULL, _FDEV_SETUP_WRITE);            //for printf function

float roll_angle, pitch_angle;
//watchdog timer off
//c;ear the watchdog system Reset Flag (WDRF) and the WDE
//disbale gloal interrupt during the process
void wdt_off(void)                       
{
	cli();
	__asm__ __volatile__ ("wdr");
	MCUSR &= ~(1 << WDRF);
	WDTCSR |= (1 << WDCE) | (1 << WDE);
	WDTCSR = 0x00;
	sei();                                   //enable global interrupts
	
}
//watchdog timer in interrupt mode with 8sec timeout period
void watchdog_enable(void)
{
	cli();
	__asm__ __volatile__ ("wdr");
	WDTCSR = (1 << WDCE) | (1 << WDE);                     
	WDTCSR = 0x29;                             //enable WDE with WDCE cleared from previous write and set prescaler for 8sec
	WDTCSR |= (1 << WDIE);                     //enable interrupt mode
	sei();
}
void MPU_sleep(void)
{
	SMCR |= (1 << SM1);                        //power down mode enabled
	SMCR |= (1 << SE);                         //enable sleep
}
int main(void)
{
	wdt_off();
	char array[28], buf[17];
	init_uart();
	stdout = &mystdout;
	printf("\n\n");
	printf("MPU6050 initializing\n\n");
	//MPU6050 initialization set power_management1 register first
	
	MPU_write(MPU60X0_W, PWR_1, 0x01);
	
	MPU_write(MPU60X0_W, acc_config, 0x10);                     //set accelerometer range to 8g
	                                                       
	MPU_write(MPU60X0_W, gyro_config, 0x01);                   //set gyroscope range to 500 degree /sec
	//MPU_write(MPU60X0_W, 0x19, 0x07);
	deviceid();
	printf("==============================================\n\n");
	_delay_ms(1000);

	watchdog_enable();                              //start the watchdog timer if wdt is not rest in 8sec watchdog interrupt will be called
	//WDTCSR |= (1 << WDIE);
	sei();
	printf("Reading from MPU6050\n\n\n");
	while (1)
	{
		//MPU_write(MPU60X0_W, PWR_1, 0x01);
		
		MPU_readall();                               //MPU6050 reads 7bytes of data
		MPU_conv();                                  //conversion of raw data
		
		dtostrf(AX, 3, 2, buf);
		sprintf(array, "%s", buf);
		printf("Ax: %s\n", array);
		
		dtostrf(AY, 3, 2, buf);
		sprintf(array, "%s", buf);
		printf("Ay: %s\n", array);
		
		dtostrf(AZ, 3, 2, buf);
		sprintf(array, "%s", buf);
		printf("Az: %s\n", array);
		
		dtostrf(TEM, 3, 2, buf);
		sprintf(array, "%s", buf);
		printf("T:  %s\n", array);
		
		dtostrf(GX, 3, 2, buf);
		sprintf(array, "%s", buf);
		printf("Gx: %s\n", array);
		
		dtostrf(GY, 3, 2, buf);
		sprintf(array, "%s", buf);
		printf("Gy: %s\n", array);
		
		dtostrf(GZ, 3, 2, buf);
		sprintf(array, "%s", buf);
		printf("Gz: %s\n\n", array);
		
		roll_angle = atan(AY/sqrt(AX * AX + AZ * AZ)) * 1/(3.142/ 180);                          //calculates the roll angle ->> rotation about x-axis
		pitch_angle = -atan(AX/sqrt(AY * AY + AZ * AZ)) * 1/(3.142/ 180);                        //calculates the pitch angle ->> rotaion about y-axis
		
		dtostrf(roll_angle, 3, 2, buf);
		sprintf(array, "%s", buf);
		printf("Roll: %s\n", array);
		
		dtostrf(pitch_angle, 3, 2, buf);
		sprintf(array, "%s", buf);
		printf("pitch: %s\n\n", array);
		
		printf("==============================================\n\n");
		printf("Atmega328p is entering power down sleep mode\n\n");
		
		
	    MPU_sleep();
		for(uint8_t i = 0; i < 2; i++)
		{
			MCUSR |= (1 << 3 << 5);
			MCUSR = (MCUSR & ~(1 << BODSE) | (1 << BODS));
			__asm__ __volatile__ ("sleep");    
		}                                                                            //MCU goes to sleep mode for 2 * 8 secs
	    SMCR &= ~(1 << SE);                                                         //programme resumes here after interrupt
		printf("Waking up from sleep mode\n\n");
		printf("===================================================\n\n");
			
	}
	return 0;
}

//watchdog interrupt
//set interrupt flag or atemega328p will undergo system reset
ISR(WDT_vect)
{
	WDTCSR |= (1 << WDIE);
	
}