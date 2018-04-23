/*
 * TWI.c
 *
 * Created: 2018-04-15 오전 1:28:31
 *  Author: kazam
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include <string.h>
#include <stdlib.h>

#include "TWI.h"
#include "UART.h"

// (0x78 == (1 << TWS7) | (1 << TWS6) | (1 << TWS5) | (1 << TWS4) | (1 << TWS3))
#define TWI_STATUS				(TWSR & 0xF8)	
#define TW_MASTER_ARB_LOST		0x38	//(TW_MT_ARB_LOST or TW_MR_ARB_LOST)

#define TWI_INTERRUPT_FLAG			0x80		// = (1 << TWINT)
#define TWI_ENABLE_ACK				0x40		// = (1 << TWEA)
#define TWI_START					0x20		// = (1 << TWSTA)
#define TWI_REP_START				0x10		// = (1 << TWSTO)
#define TWI_STOP					0x10		// = (1 << TWSTO)
#define TWI_WRITE_COLLISION			0x08		// = (1 << TWWC)
#define TWI_ENABLE					0x04		// = (1 << TWEN)
#define TWI_INTERRUPT_ENABLE		0x01		// = (1 << TWIE)

#define TWI_START_CONDITION				0xA4		// = (TWI_INTERRUPT_FLAG | TWI_START | TWI_ENABLE)
#define TWI_STOP_CONDITION				0x94		// = (TWI_INTERRUPT_FLAG | TWI_STO | TWI_ENABLE)
#define TWI_ADDR_TRANSMIT_CONDITION		0x84		// = (TWI_INTERRUPT_FLAG | TWI_ENABLE)
#define TWI_DATA_TRANSMIT_CONDITION		0x84		// = (TWI_INTERRUPT_FLAG | TWI_ENABLE)
#define TWI_DATA_RECEIVE_ACK_CONDITION	0xC4		// = (TWI_INTERRUPT_FLAG | TWI_ENABLE_ACK | TWI_ENABLE)
#define TWI_DATA_RECEIVE_NACK_CONDITION	0x84		// = (TWI_INTERRUPT_FLAG | TWI_ENABLE)
hUART* Serial;

static void SendStatus(uint8_t Status){
	char Buf[10];
	itoa(Status, Buf, 16);
	M_SerialWrite(Serial, (uint8_t*)Buf, strlen(Buf));
}

void SendMsg(char* Msg){
	M_SerialWrite(Serial, (uint8_t*)Msg, strlen(Msg));
}

uint8_t SetSCL(hTWI* hKey, uint32_t Comm_FRQ){
	uint16_t TWBR_Temp;
	
	TWBR_Temp = ((hKey->System_Clock / Comm_FRQ) - 16) / 2;
	if((TWBR_Temp > 10) && (TWBR_Temp < 0xFF)){
		TWBR = (uint8_t)TWBR_Temp;
		if(hKey->Mode == NON_BLOCKING){
			TWCR |= TWI_INTERRUPT_ENABLE;
			sei();
		}
		
		return NO_ERROR;
	}
	
	return 1;
}

uint8_t TWI_Start(hTWI* hKey){
	// Clear TWI Control Register.
	TWCR &= (hKey->Mode == BLOCKING) ? 0 : TWI_INTERRUPT_ENABLE;
	TWCR = TWI_START_CONDITION;	// Set TWI Start Condition.
	return TWI_Check(hKey, TW_START);
}

uint8_t TWI_Stop(hTWI* hKey){
	TWCR = TWI_STOP_CONDITION;		// Set TWI Stop Condition.
	return NO_ERROR;
}

uint8_t TWI_Check(hTWI* hKey, uint8_t Status){
	register uint8_t t_out;

	for(t_out = 0; t_out < 0xFF; t_out++){
		
		if(TWI_STATUS == Status)
			return NO_ERROR;
	}
	SendStatus(TWI_STATUS);
	return TIME_OUT;
}

uint8_t TWI_Call_Slave(hTWI* hKey, uint8_t RW_Mode){
	// Set Address.
	TWDR = (hKey->Address | RW_Mode);
	// Call Slave.
	TWCR = TWI_ADDR_TRANSMIT_CONDITION;
	
	return (RW_Mode == WRITE) ? TWI_Check(hKey, TW_MT_SLA_ACK) : TWI_Check(hKey, TW_MR_SLA_ACK);
}

uint8_t TWI_Call_Master(hTWI* hKey, uint8_t RW_Mode){
	// Set Address.
	TWDR = (hKey->Address | RW_Mode);
	// Call Slave.
	TWCR = TWI_ADDR_TRANSMIT_CONDITION;
	
	return (RW_Mode == WRITE) ? TWI_Check(hKey, TW_ST_SLA_ACK) : TWI_Check(hKey, TW_SR_SLA_ACK);
}

uint8_t TWI_MT_Mode(hTWI* hKey, uint8_t data){
	// Set Transmit Data.
	TWDR = data;
	// Transmit Data To Slave.
	TWCR = TWI_DATA_TRANSMIT_CONDITION;
	
	return TWI_Check(hKey, TW_MT_DATA_ACK);
}

uint8_t TWI_MR_Mode(hTWI* hKey, uint8_t* buf, uint8_t Ack_Mode){
	uint8_t retVal;
	
	// Receive Data From Slave.
	TWCR = (Ack_Mode == ACK_READ) ? TWI_DATA_RECEIVE_ACK_CONDITION : TWI_DATA_RECEIVE_NACK_CONDITION;
	
	retVal = (Ack_Mode == ACK_READ) ? TWI_Check(hKey, TW_MR_DATA_ACK) : TWI_Check(hKey, TW_MR_DATA_NACK);
	if(retVal == TIME_OUT)
		return retVal;
		
	(*buf) = TWDR;
	return retVal;
}

uint8_t TWI_ST_Mode(hTWI* hKey, uint8_t data){
	// Set Transmit Data.
	TWDR = data;
	// Transmit Data To Slave.
	TWCR = TWI_DATA_TRANSMIT_CONDITION;
	
	return TWI_Check(hKey, TW_ST_DATA_ACK);
}

uint8_t TWI_SR_Mode(hTWI* hKey, uint8_t* buf, uint8_t Ack_Mode){
	uint8_t retVal;
	
	// Receive Data From Slave.
	TWCR = ((Ack_Mode == ACK_READ) ? TWI_DATA_RECEIVE_ACK_CONDITION : TWI_DATA_RECEIVE_NACK_CONDITION);
	
	retVal = (Ack_Mode == ACK_READ) ? TWI_Check(hKey, TW_SR_DATA_ACK) : TWI_Check(hKey, TW_SR_DATA_NACK);
	if(retVal == TIME_OUT)
		return retVal;
	
	(*buf) = TWDR;
	return retVal;
}

uint8_t TWI_MT_Register(hTWI* hKey, uint8_t StartAddr, uint8_t* Buf, uint8_t Length){
	register uint8_t idx;
	
	if(TWI_Start(hKey) == TIME_OUT)					return TW_START;
	if(TWI_Call_Slave(hKey, WRITE) == TIME_OUT)		return TW_MT_SLA_ACK;
	if(TWI_MT_Mode(hKey, StartAddr) == TIME_OUT)	return TW_MT_DATA_ACK;
	
	for(idx = 0; idx < Length; idx++){
		if(TWI_MT_Mode(hKey, Buf[idx]) == TIME_OUT)	
			return TW_MT_DATA_ACK;
	}
	
	TWI_Stop(hKey);
	
	return NO_ERROR;
}

uint8_t TWI_MR_Register(hTWI* hKey, uint8_t StartAddr, uint8_t* Buf, uint8_t Length){
	register uint8_t idx;
	
	// First Start.(Call the register of the slave.)
	if(TWI_Start(hKey) == TIME_OUT)					return TW_START;
	if(TWI_Call_Slave(hKey, WRITE) == TIME_OUT)		return TW_MT_SLA_ACK;
	if(TWI_MT_Mode(hKey, StartAddr) == TIME_OUT)	return TW_MT_DATA_ACK;
	
	// Repeated Start.(Access to the register address of the slave.)
	if(TWI_Start(hKey) == TIME_OUT)					return TW_START;
	if(TWI_Call_Slave(hKey, READ) == TIME_OUT)		return TW_MR_SLA_ACK;
	
	// Receive Data from Register of the slave.
	for(idx = 0; idx < (Length - 1); idx++){
		if(TWI_MR_Mode(hKey, &Buf[idx], ACK_READ) == TIME_OUT)
			return TW_MR_DATA_ACK;
	}
		
	if(TWI_MR_Mode(hKey, &Buf[Length - 1], NACK_READ) == TIME_OUT)
		return TW_MR_DATA_NACK;
	
	TWI_Stop(hKey);
	
	return NO_ERROR;
}

uint8_t Init_TWI(hTWI* hKey, uint32_t System_Clock, uint32_t Comm_FRQ, uint8_t Mode, uint8_t Address){
	Serial = GetUART_Handle(0);
	
	// Set Member Variable.
	hKey->System_Clock = System_Clock;
	hKey->Comm_FRQ = Comm_FRQ;
	hKey->Mode = Mode;
	hKey->Address = (Address << 1);
	
	// Set SCL.
	return SetSCL(hKey, Comm_FRQ);
}

ISR(TWI_vect){
	switch(TWI_STATUS){
	/* [ TWI Common ] */
	case TW_START:					// [ start condition transmitted ]
		
		break;
	case TW_REP_START:				// [ repeated start condition transmitted ]
		break;
	case TW_SR_STOP:				// [ stop or repeated start condition received while selected ]
		break;
	case TW_NO_INFO:				// [ no state information available ]
		break;
	case TW_BUS_ERROR:				// [ illegal start or stop condition ]
		break;
		
	case TW_MASTER_ARB_LOST:		// [ arbitration lost in (SLA+W or data) or (SLA+R or NACK) ]
		break;	
		
	/* [ Master Transmit ] */		
	case TW_MT_SLA_ACK:				// [ SLA+W transmitted, ACK received ]
		break;			
	case TW_MT_SLA_NACK:			// [ SLA+W transmitted, NACK received ]
		break;			
	case TW_MT_DATA_ACK:			// [ data transmitted, ACK received ]
		break;			
	case TW_MT_DATA_NACK:			// [ data transmitted, NACK received ]
		break;			
	
	/* [ Master Receive ] */					
	case TW_MR_SLA_ACK:				// [ SLA+R transmitted, ACK received ]
		break;			
	case TW_MR_SLA_NACK:			// [ SLA+R transmitted, NACK received ]
		break;			
	case TW_MR_DATA_ACK:			// [ data received, ACK returned ]
		break;			
	case TW_MR_DATA_NACK:			// [ data received, NACK returned ]
		break;
		
	/* [ Slave Transmit ] */		
	case TW_ST_SLA_ACK:				// [ SLA+R received, ACK returned ]
		break;	
	case TW_ST_ARB_LOST_SLA_ACK:	// [ arbitration lost in SLA+RW, SLA+R received, ACK returned ]
		break;			
	case TW_ST_DATA_ACK:			// [ data transmitted, ACK received ]
		break;			
	case TW_ST_DATA_NACK:			// [ data transmitted, NACK received ]
		break;			
	case TW_ST_LAST_DATA:			// [ last data byte transmitted, ACK received ]
		break;
		
	/* [ Slave Receive ] */
	// def TW_SR_SLA_ACK			
	case TW_SR_SLA_ACK:				// [ SLA+W received, ACK returned ]
		break;	
	case TW_SR_ARB_LOST_SLA_ACK:	// [ arbitration lost in SLA+RW, SLA+W received, ACK returned ]
		break;			
	case TW_SR_GCALL_ACK:			// [ general call received, ACK returned ]
		break; 
	case TW_SR_ARB_LOST_GCALL_ACK:	// [ arbitration lost in SLA+RW, general call received, ACK returned ]
		break;			
	case TW_SR_DATA_ACK:			// [ data received, ACK returned ]
		break;			
	case TW_SR_DATA_NACK:			// [ data received, NACK returned ]
		break;	
	case TW_SR_GCALL_DATA_ACK:		// [ general call data received, ACK returned ]
		break;	
	case TW_SR_GCALL_DATA_NACK:		// [ general call data received, NACK returned ]
		break;
	}
}