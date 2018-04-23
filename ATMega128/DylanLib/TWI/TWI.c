/*
 *  File Name   :   TWI.c
 *
 *  Created	    :   2018-04-15 P.M 3:27:24
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

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include "TWI.h"

// (0x78 == (1 << TWS7) | (1 << TWS6) | (1 << TWS5) | (1 << TWS4) | (1 << TWS3))
#define TWI_STATUS						(TWSR & 0xF8)	
#define TW_MASTER_ARB_LOST				0x38		//(TW_MT_ARB_LOST or TW_MR_ARB_LOST)

#define TWI_INTERRUPT_FLAG				0x80		// = (1 << TWINT)
#define TWI_ENABLE_ACK					0x40		// = (1 << TWEA)
#define TWI_START						0x20		// = (1 << TWSTA)
#define TWI_REP_START					0x10		// = (1 << TWSTO)
#define TWI_STOP						0x10		// = (1 << TWSTO)
#define TWI_WRITE_COLLISION				0x08		// = (1 << TWWC)
#define TWI_ENABLE						0x04		// = (1 << TWEN)
#define TWI_INTERRUPT_ENABLE			0x01		// = (1 << TWIE)

#define TWI_START_CONDITION				0xA4		// = (TWI_INTERRUPT_FLAG | TWI_START | TWI_ENABLE)
#define TWI_STOP_CONDITION				0x94		// = (TWI_INTERRUPT_FLAG | TWI_STO | TWI_ENABLE)
#define TWI_ADDR_TRANSMIT_CONDITION		0x84		// = (TWI_INTERRUPT_FLAG | TWI_ENABLE)
#define TWI_DATA_TRANSMIT_CONDITION		0x84		// = (TWI_INTERRUPT_FLAG | TWI_ENABLE)
#define TWI_DATA_RECEIVE_ACK_CONDITION	0xC4		// = (TWI_INTERRUPT_FLAG | TWI_ENABLE_ACK | TWI_ENABLE)
#define TWI_DATA_RECEIVE_NACK_CONDITION	0x84		// = (TWI_INTERRUPT_FLAG | TWI_ENABLE)



static inline uint8_t TWI_Check(uint8_t Status){
	register uint8_t t_out;

	for(t_out = 0; t_out < 0xFF; t_out++){
		if(TWI_STATUS == Status)
		return NO_ERROR;
	}

	return TIME_OUT;
}

static uint8_t TWI_Start(void){
	// Clear TWI Control Register.
	TWCR = 0;
	TWCR = TWI_START_CONDITION;			// Set TWI Start Condition.
	return TWI_Check(TW_START);
}

static inline uint8_t TWI_Stop(void){
	TWCR = TWI_STOP_CONDITION;			// Set TWI Stop Condition.
	return NO_ERROR;
}

static inline uint8_t TWI_Call_Slave(hTWI* hKey, uint8_t RW_Mode){
	// Set Address.
	TWDR = (hKey->Address | RW_Mode);
	// Call Slave.
	TWCR = TWI_ADDR_TRANSMIT_CONDITION;
	
	return (RW_Mode == WRITE) ? TWI_Check(TW_MT_SLA_ACK) : TWI_Check(TW_MR_SLA_ACK);
}

static inline uint8_t TWI_Call_Master(hTWI* hKey, uint8_t RW_Mode){
	// Set Address.
	TWDR = (hKey->Address | RW_Mode);
	// Call Slave.
	TWCR = TWI_ADDR_TRANSMIT_CONDITION;
	
	return (RW_Mode == WRITE) ? TWI_Check(TW_ST_SLA_ACK) : TWI_Check(TW_SR_SLA_ACK);
}

static inline uint8_t TWI_MT_Mode(uint8_t data){
	// Set Transmit Data.
	TWDR = data;
	// Transmit Data To Slave.
	TWCR = TWI_DATA_TRANSMIT_CONDITION;
	
	return TWI_Check(TW_MT_DATA_ACK);
}

static inline uint8_t TWI_MR_Mode(uint8_t* buf, uint8_t Ack_Mode){
	uint8_t retVal;
	
	// Receive Data From Slave.
	TWCR = (Ack_Mode == ACK_READ) ? TWI_DATA_RECEIVE_ACK_CONDITION : TWI_DATA_RECEIVE_NACK_CONDITION;
	
	retVal = (Ack_Mode == ACK_READ) ? TWI_Check(TW_MR_DATA_ACK) : TWI_Check(TW_MR_DATA_NACK);
	if(retVal == TIME_OUT)
		return retVal;
		
	(*buf) = TWDR;
	return retVal;
}

static inline uint8_t TWI_ST_Mode(uint8_t data){
	// Set Transmit Data.
	TWDR = data;
	// Transmit Data To Slave.
	TWCR = TWI_DATA_TRANSMIT_CONDITION;
	
	return TWI_Check(TW_ST_DATA_ACK);
}

static inline uint8_t TWI_SR_Mode(uint8_t* buf, uint8_t Ack_Mode){
	uint8_t retVal;
	
	// Receive Data From Slave.
	TWCR = ((Ack_Mode == ACK_READ) ? TWI_DATA_RECEIVE_ACK_CONDITION : TWI_DATA_RECEIVE_NACK_CONDITION);
	
	retVal = (Ack_Mode == ACK_READ) ? TWI_Check(TW_SR_DATA_ACK) : TWI_Check(TW_SR_DATA_NACK);
	if(retVal == TIME_OUT)
		return retVal;
	
	(*buf) = TWDR;
	return retVal;
}

static inline uint8_t TWI_MT_Slave(struct TWI_HANDLE* hKey, uint8_t* Buf, uint8_t Length){
	register uint8_t idx;
	
	if(TWI_Start() == TIME_OUT)						return TW_START;
	if(TWI_Call_Slave(hKey, WRITE) == TIME_OUT)		return TW_MT_SLA_ACK;
	
	for(idx = 0; idx < Length; idx++){
		if(TWI_MT_Mode(Buf[idx]) == TIME_OUT)
			return TW_MT_DATA_ACK;
	}
	
	TWI_Stop();
	
	return NO_ERROR;
}

static inline uint8_t TWI_MR_Slave(struct TWI_HANDLE* hKey, uint8_t* Buf, uint8_t Length){
	register uint8_t idx;
	
	// Start
	if(TWI_Start() == TIME_OUT)						return TW_START;
	if(TWI_Call_Slave(hKey, READ) == TIME_OUT)		return TW_MR_SLA_ACK;
	
	// Receive Data the slave.
	for(idx = 0; idx < (Length - 1); idx++){
		if(TWI_MR_Mode(&Buf[idx], ACK_READ) == TIME_OUT)
			return TW_MR_DATA_ACK;
	}
	
	if(TWI_MR_Mode(&Buf[Length - 1], NACK_READ) == TIME_OUT)
		return TW_MR_DATA_NACK;
	
	TWI_Stop();
	
	return NO_ERROR;
}

static inline uint8_t TWI_MT_Register(hTWI* hKey, uint8_t StartAddr, uint8_t* Buf, uint8_t Length){
	register uint8_t idx;
	
	if(TWI_Start() == TIME_OUT)						return TW_START;
	if(TWI_Call_Slave(hKey, WRITE) == TIME_OUT)		return TW_MT_SLA_ACK;
	if(TWI_MT_Mode(StartAddr) == TIME_OUT)			return TW_MT_DATA_ACK;
	
	for(idx = 0; idx < Length; idx++){
		if(TWI_MT_Mode(Buf[idx]) == TIME_OUT)	
			return TW_MT_DATA_ACK;
	}
	
	TWI_Stop();
	
	return NO_ERROR;
}

static inline uint8_t TWI_MR_Register(hTWI* hKey, uint8_t StartAddr, uint8_t* Buf, uint8_t Length){
	register uint8_t idx;
	
	// First Start.(Call the register of the slave.)
	if(TWI_Start() == TIME_OUT)						return TW_START;
	if(TWI_Call_Slave(hKey, WRITE) == TIME_OUT)		return TW_MT_SLA_ACK;
	if(TWI_MT_Mode(StartAddr) == TIME_OUT)			return TW_MT_DATA_ACK;
	
	// Repeated Start.(Access to the register address of the slave.)
	if(TWI_Start() == TIME_OUT)						return TW_START;
	if(TWI_Call_Slave(hKey, READ) == TIME_OUT)		return TW_MR_SLA_ACK;
	
	// Receive Data from Register of the slave.
	for(idx = 0; idx < (Length - 1); idx++){
		if(TWI_MR_Mode(&Buf[idx], ACK_READ) == TIME_OUT)
			return TW_MR_DATA_ACK;
	}
		
	if(TWI_MR_Mode(&Buf[Length - 1], NACK_READ) == TIME_OUT)
		return TW_MR_DATA_NACK;
	
	TWI_Stop();
	
	return NO_ERROR;
}

static inline uint8_t SetSCL(hTWI* hKey, uint32_t Comm_FRQ){
	uint16_t TWBR_Temp;
	
	TWBR_Temp = ((hKey->System_Clock / Comm_FRQ) - 16) / 2;
	if((TWBR_Temp > 10) && (TWBR_Temp < 0xFF)){
		TWBR = (uint8_t)TWBR_Temp;
		
		return NO_ERROR;
	}
	
	return 1;
}

uint8_t Init_TWI(hTWI* hKey, uint32_t System_Clock, uint32_t Comm_FRQ, uint8_t Address){	
	// Set Member Variable.
	hKey->System_Clock = System_Clock;
	hKey->Comm_FRQ = Comm_FRQ;
	hKey->Address = (Address << 1);
	
	// Member Function.
	hKey->TWI_Check = TWI_Check;
	hKey->TWI_Start = TWI_Start;
	hKey->TWI_Stop = TWI_Stop;
	hKey->TWI_Call_Slave = TWI_Call_Slave;
	hKey->TWI_Call_Master = TWI_Call_Master;
	hKey->TWI_MT_Mode = TWI_MT_Mode;
	hKey->TWI_MR_Mode = TWI_MR_Mode;
	hKey->TWI_ST_Mode = TWI_ST_Mode;
	hKey->TWI_SR_Mode = TWI_SR_Mode;
	hKey->TWI_MT_Register = TWI_MT_Register;
	hKey->TWI_MR_Register = TWI_MR_Register;
	hKey->SetSCL = SetSCL;
	
	hKey->TWI_MT_Slave = TWI_MT_Slave;
	hKey->TWI_MR_Slave = TWI_MR_Slave;
	hKey->TWI_MT_Register = TWI_MT_Register;
	hKey->TWI_MR_Register = TWI_MR_Register;
	
	// Set SCL.
	return SetSCL(hKey, Comm_FRQ);
}