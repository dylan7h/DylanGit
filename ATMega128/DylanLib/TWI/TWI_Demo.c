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

/*
*	[ Note ]
*	- This is an example of TWI (I2C) communication with ATMega128 using IMU MPU6050.
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

// TWI(I2C) Device Address of The MPU6050 Inertial Measurement Unit(IMU)
#define MPU6050_ADDRESS					0x68

// MPU 6050 Config Register Address.
#define MPU6050_RA_PWR_MGMT_1			0x6B
#define MPU6050_RA_PWR_MGMT_2			0x6C
#define MPU6050_RA_CONFIG				0x1A
#define MPU6050_RA_GYRO_CONFIG			0x1B
#define MPU6050_RA_ACCEL_CONFIG			0x1C

// MPU 6050 Accele_X/Y/Z, Gyro_X/Y/Z Register Address.
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

// The value to be used in the Config Register.
#define WAKE_UP_MPU6050					0x00
#define DLPF_CFG_LV6					0x06
#define GYRO_CFG_250DEG_PER_SEC			0x00
#define ACC_CONFIG_2G					0x00
uint8_t RegData[5] = { WAKE_UP_MPU6050, 0x00, DLPF_CFG_LV6, GYRO_CFG_250DEG_PER_SEC, ACC_CONFIG_2G };

int main(void){
	// Buffer to be used to receive data from the MPU6050.
	uint8_t buf[14];
	// Check the TWI operation of MPU6050 via UART.
	hUART Serial0;
	// Buffer of data to be transmitted via UART.
	char stringBuf[100];
	// TWI Handle Key.
	hTWI hMPU6050;
	
	// Initialize UART Channel 0, Baud rate 115200bit/sec.
	InitSerial(&Serial0, 0, F_CPU, 115200);
	// Initialize TWI(SCL: 400 kHz)
	Init_TWI(&hMPU6050, F_CPU, F_SCL, MPU6050_ADDRESS);	
	
	// Using TWI to wake up the MPU6050 and initialize the settings.		
	M_TWI_MT_Register(&hMPU6050, MPU6050_RA_PWR_MGMT_1, &RegData[0], 1);		
	M_TWI_MT_Register(&hMPU6050, MPU6050_RA_PWR_MGMT_2, &RegData[1], 1);		
	M_TWI_MT_Register(&hMPU6050, MPU6050_RA_CONFIG, &RegData[2], 1);			
	M_TWI_MT_Register(&hMPU6050, MPU6050_RA_GYRO_CONFIG, &RegData[3], 1);			
	M_TWI_MT_Register(&hMPU6050, MPU6050_RA_ACCEL_CONFIG, &RegData[4], 1);	
	
	while(1){
		// Using TWI, we receive Accel X / Y / Z, temperature, and Gyro X/Y/Z values from MPU 6050.
		// (Receive data from consecutive registers with MPU6050_RA_ACCEL_XOUT_H as the start address. 
		// Receive and save data from 14 registers, including MPU6050_RA_ACCEL_XOUT_H to buf. )
		M_TWI_MR_Register(&hMPU6050, MPU6050_RA_ACCEL_XOUT_H, buf, 14);
		
		// It characterizes the data of buf.
		sprintf(stringBuf, "AccX: %d, AccY: %d, AccZ: %d, GryX: %d, GryY: %d, GryZ: %d\r\n"
		, (int16_t)((buf[0] << 8) | buf[1]), (int16_t)((buf[2] << 8) | buf[3]), (int16_t)((buf[4] << 8) | buf[5])
		, (int16_t)((buf[8] << 8) | buf[9]), (int16_t)((buf[10] << 8) | buf[11]), (int16_t)((buf[12] << 8) | buf[13]));
		
		// The data of the stringed MPU6050 is transferred to PC via UART.
		M_SerialWrite(&Serial0, (uint8_t*)stringBuf, strlen(stringBuf));
		_delay_ms(200);
	}
	
	return 0;
}