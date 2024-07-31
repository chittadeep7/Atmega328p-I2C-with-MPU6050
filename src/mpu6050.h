#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

#include "i2c.h"

#define PWR_1   		 0x6B
#define acc_config   	 0x1C
#define gyro_config   	 0x1B
#define WHO_AM_I		 0x75
#define MPU60X0_W		 0xD0
#define MPU60X0_R		 0xD1
#define MPU6050_accx	 0x3B


extern uint8_t arr[14];
extern float ACCX, ACCY, ACCZ, T, GYX, GYY, GYZ;
extern float AX, AY, AZ, TEM, GY, GX, GZ;

void MPU_write(uint8_t dev_addr, uint8_t reg, uint8_t instr);
void deviceid();
void MPU_readall();