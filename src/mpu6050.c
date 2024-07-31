#include "mpu6050.h"

uint8_t arr[14];
float ACCX, ACCY, ACCZ, T, GYX, GYY, GYZ;
float AX, AY, AZ, TEM, GY, GX, GZ;

//simple write operation to MPU6050
void MPU_write(uint8_t dev_addr, uint8_t reg, uint8_t instr)        
{
	start();
	send(MPU60X0_W);
	send(reg);
	send(instr);
	stop();
}

//reads the device id 0x68 from the who_am_i register of MPU6050
void deviceid()
{
    uint8_t id;
	start();
	send(MPU60X0_W);
	send(WHO_AM_I);
	start();
	send(MPU60X0_R);
	id = Nack();
	stop();
	if(id == 0x68)
	{
		printf("Connected to MPU6050 with device id: 0x%x\n\n", id);
		}
	else{
		printf("Unknown device id: $x\n\n", id);
	}
}

//reads 7bytes of data starting from address 0x3B
//data is stored in a buffer for conversion
void MPU_readall()
{
	uint16_t data;
	start();
	send(MPU60X0_W);
	send(MPU6050_accx);
	_delay_ms(500);
	start();
	send(MPU60X0_R);
	
	for(uint8_t i = 0; i < 13; i++)
	{
		arr[i] = Ack();
	}
	    arr[13] =Nack();
	stop();
	
	ACCX = ((arr[0] << 8) | arr[1]);
	ACCY = ((arr[2] << 8) | arr[3]);
	ACCZ = ((arr[4] << 8) | arr[5]);
	T = ((arr[6] << 8) | arr[7]);
	GYX = ((arr[8] << 8) | arr[9]);
	GYY = ((arr[10] << 8) | arr[11]);
	GYZ = ((arr[12] << 8) | arr[13]);
	
}

//MPU6050 raw data conversion
void MPU_conv(void)
{
	MPU_readall();
	AX = ACCX / 4096;               //raw accel / accel sensitivity ->>acceleration in g
	AY = ACCY /4096;
	AZ = ACCZ / 4096;
	TEM = T /340 +36.53;             //celcius conversion
	GX = GYX / 65.5;                   
	GY = GYY / 65.5;                  //convert raw gyro data to degree/seconds
	GZ = GYZ / 65.5;
}