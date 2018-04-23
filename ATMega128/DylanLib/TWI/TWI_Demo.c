/*
 *  File Name   :   TWI_Demo.c
 *
 *  Created     :   2018-04-21 P.M 5:45:34
 *  belong      :   Korea Polytechnic University
 *					, Department of Energy-Electrical Engineering Student.
 *  Author      :   KOR DYLAN( Korean name: Jun Ki, Hong)
 *  YouTube     :   https://www.youtube.com/channel/UC9DTd1Rv730XKmWRTpqY8Rg?view_as=subscriber
 *  e-mail      :   dylan.7h@gmail.com
 *  IDE Software:   Atmel Studio 7
 *  Hardware    :   ATmega128, tested on ATmega128(jmod-128-1) at 16MHz.
 */ 

#define F_CPU 16000000UL
#define F_SCL 400000UL
#include <avr/io.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include "TWI.h"
#include "UART.h"

#define MPU6050_ADDRESS		0x68
#define MPU6050_RA_WHO_AM_I	0x75

#define MPU6050_RA_PWR_MGMT_1			0x6B
#define MPU6050_RA_PWR_MGMT_2			0x6C
#define MPU6050_RA_CONFIG				0x1A
#define MPU6050_RA_GYRO_CONFIG			0x1B
#define MPU6050_RA_ACCEL_CONFIG			0x1C

#define MPU6050_RA_ACCEL_XOUT_H			0x3B
#define MPU6050_RA_ACCEL_XOUT_L			0x3C
#define MPU6050_RA_ACCEL_YOUT_H			0x3D
#define MPU6050_RA_ACCEL_YOUT_L			0x3E
#define MPU6050_RA_ACCEL_ZOUT_H			0x3F
#define MPU6050_RA_ACCEL_ZOUT_L			0x40
#define MPU6050_RA_TEMP_OUT_H			0x41
#define MPU6050_RA_TEMP_OUT_L			0x42
#define MPU6050_RA_GYRO_XOUT_H			0x43
#define MPU6050_RA_GYRO_XOUT_L			0x44
#define MPU6050_RA_GYRO_YOUT_H			0x45
#define MPU6050_RA_GYRO_YOUT_L			0x46
#define MPU6050_RA_GYRO_ZOUT_H			0x47
#define MPU6050_RA_GYRO_ZOUT_L			0x48
/*
I2C_WriteReg(&MPU6050, MPU6050_RA_PWR_MGMT_1, &RegData[0], 1);
I2C_WriteReg(&MPU6050, MPU6050_RA_PWR_MGMT_2, &RegData[1], 1);
I2C_WriteReg(&MPU6050, MPU6050_RA_CONFIG, &RegData[2], 1);
I2C_WriteReg(&MPU6050, MPU6050_RA_GYRO_CONFIG, &RegData[3], 1);
I2C_WriteReg(&MPU6050, MPU6050_RA_ACCEL_CONFIG, &RegData[4], 1);
*/

#define WAKE_UP_MPU6050					0x00
#define DLPF_CFG_LV6					0x06
#define GYRO_CFG_250DEG_PER_SEC			0x00
#define ACC_CONFIG_2G					0x00

uint8_t RegData[5] = { WAKE_UP_MPU6050, 0x00, DLPF_CFG_LV6, GYRO_CFG_250DEG_PER_SEC, ACC_CONFIG_2G };

int main(void){
	uint8_t buf[14];
	hUART Serial0;
	hTWI hI2C;
	char stringBuf[100];
	
	InitSerial(&Serial0, 0, F_CPU, 115200);
	Init_TWI(&hI2C, F_CPU, F_SCL, BLOCKING, 0x68);	
						
	TWI_MT_Register(&hI2C, MPU6050_RA_PWR_MGMT_1, &RegData[0], 1);		
	TWI_MT_Register(&hI2C, MPU6050_RA_PWR_MGMT_2, &RegData[1], 1);		
	TWI_MT_Register(&hI2C, MPU6050_RA_CONFIG, &RegData[2], 1);			
	TWI_MT_Register(&hI2C, MPU6050_RA_GYRO_CONFIG, &RegData[3], 1);			
	TWI_MT_Register(&hI2C, MPU6050_RA_ACCEL_CONFIG, &RegData[4], 1);	
	
	while(1){
		TWI_MR_Register(&hI2C, MPU6050_RA_ACCEL_XOUT_H, buf, 14);
		sprintf(stringBuf, "AccX: %d, AccY: %d, AccZ: %d, GryX: %d, GryY: %d, GryZ: %d\r\n"
		, (int16_t)((buf[0] << 8) | buf[1]), (int16_t)((buf[2] << 8) | buf[3]), (int16_t)((buf[4] << 8) | buf[5])
		, (int16_t)((buf[8] << 8) | buf[9]), (int16_t)((buf[10] << 8) | buf[11]), (int16_t)((buf[12] << 8) | buf[13]));
		
		M_SerialWrite(&Serial0, (uint8_t*)stringBuf, strlen(stringBuf));
		_delay_ms(200);
	}
	
	return 0;
}