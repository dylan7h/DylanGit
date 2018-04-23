/*
 *  File Name   :   TWI.h
 *
 *  Created	    :   2018-04-15 P.M 2:28:38
 *  belong      :   Korea Polytechnic University
 *	                , Department of Energy-Electrical Engineering Student.
 *  Author      :   KOR DYLAN( Korean name: Jun Ki, Hong)
 *  YouTube     :   https://www.youtube.com/channel/UC9DTd1Rv730XKmWRTpqY8Rg?view_as=subscriber
 *  e-mail      :   dylan.7h@gmail.com
 *  IDE Software:   Atmel Studio 7
 *  Hardware    :   ATmega128, tested on ATmega128(jmod-128-1) at 16MHz.
 *  Note        :   This library is distributed in the hope that it will be useful. However, no warranty is given.
 *  place       :   In April 2018 at the Korea Polytechnic University, Technology Innovation Park 401 ...
 */ 

#ifndef TWI_H_
#define TWI_H_

#include "DataQueue.h"

#define NO_ERROR		0
#define TIME_OUT		1

#define MASTER_MODE		0
#define SLAVE_MODE		1

#define BLOCKING		0
#define NON_BLOCKING	1

#define WRITE			0
#define READ			1

#define ACK_READ		0
#define NACK_READ		1

typedef struct TWI_HANDLE {
	uint8_t		Address;
	uint8_t		Status;
	uint32_t	System_Clock;	// System Frequency.
	uint32_t	Comm_FRQ;		// Communication Frequency.
} hTWI;

uint8_t SetSCL(hTWI* hKey, uint32_t Comm_FRQ);
uint8_t TWI_Start(void);
uint8_t TWI_Stop(void);
uint8_t TWI_Check(uint8_t Status);
uint8_t TWI_Call_Slave(hTWI* hKey, uint8_t RW_Mode);
uint8_t TWI_Call_Master(hTWI* hKey, uint8_t RW_Mode);
uint8_t TWI_MT_Mode(uint8_t data);
uint8_t TWI_MR_Mode(uint8_t* buf, uint8_t Ack_Mode);
uint8_t TWI_ST_Mode(uint8_t data);
uint8_t TWI_SR_Mode(uint8_t* buf, uint8_t Ack_Mode);

uint8_t TWI_MT_Register(hTWI* hKey, uint8_t StartAddr, uint8_t* Buf, uint8_t Length);
uint8_t TWI_MR_Register(hTWI* hKey, uint8_t StartAddr, uint8_t* Buf, uint8_t Length);

uint8_t Init_TWI(hTWI* hKey, uint32_t System_Clock, uint32_t Comm_FRQ, uint8_t Address);

#endif /* TWI_H_ */