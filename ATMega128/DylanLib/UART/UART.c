/*
 * UART.c
 *
 * Created: 2018-04-14 오후 1:02:32
 *  Author: kazam
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "UART.h"

hUART* hSerial0;
hUART* hSerial1;

/* [ Static Memory Version ] */
#ifdef USING_STATIC_MEMORY
static inline Node* Get_RX0_Buf() { static Node Buf[RX0_BUF_SIZE]; return Buf; }
static inline Node* Get_RX1_Buf() { static Node Buf[RX1_BUF_SIZE]; return Buf; }
static inline Node* Get_TX0_Buf() { static Node Buf[TX0_BUF_SIZE]; return Buf; }
static inline Node* Get_TX1_Buf() { static Node Buf[TX1_BUF_SIZE]; return Buf; }
#endif

/* [ Dynamic Memory Version ] */
#ifdef USING_DYNAMIC_MEMORY
static inline Node* Get_RX0_Buf() { return (Node*)malloc(sizeof(Node)*RX0_BUF_SIZE); }
static inline Node* Get_RX1_Buf() { return (Node*)malloc(sizeof(Node)*RX1_BUF_SIZE); }
static inline Node* Get_TX0_Buf() { return (Node*)malloc(sizeof(Node)*TX0_BUF_SIZE); }
static inline Node* Get_TX1_Buf() { return (Node*)malloc(sizeof(Node)*TX1_BUF_SIZE); }
#endif

static inline uint8_t SerialAvailable(hUART* hKey){ return !(uint8_t)M_IsEmpty(&hKey->hRx_Buf); }

static inline uint8_t SerialWrite(hUART* hKey, uint8_t* Buf, uint8_t Len){
	register uint8_t idx;
	
	if(hKey == NULL) return NULL_KEY;
	if(Buf == NULL) return NULL_BUF;
	
	for(idx = 0; idx < Len; idx++)
		M_EnQueue(&hKey->hTx_Buf, Buf[idx]);
	
	switch(hKey->Channel){
	case 0:
		UCSR0B |= (1 << UDRIE0);
		break;
	case 1:
		UCSR1B |= (1 << UDRIE1);
		break;
	}
	
	return NONE;
}

static inline uint8_t SerialRead(hUART* hKey, uint8_t* Buf, uint8_t Size){
	register uint8_t idx = 0;
	
	if(hKey == NULL) return NULL_KEY;
	if(Buf == NULL) return NULL_BUF;
	
	while(M_DeQueue(&hKey->hRx_Buf, &Buf[idx]) != EMPTY_BUF) {
		if(++idx >= Size) break;
	}
	
	return idx;
}

static inline uint8_t WhoAmI(hUART* hKey){
	return hKey->Channel;
}
static inline uint32_t GetBaudRate(hUART* hKey){
	return hKey->BaudRate;
}

static inline uint8_t SetUART_Handler(hUART* hKey, uint8_t Channel, uint32_t BaudRate){
	if(hKey == NULL)
	return NULL_KEY;
	
	hKey->BaudRate = BaudRate;
	hKey->Channel = Channel;
	switch(Channel){
		case 0:
		hSerial0 = hKey;
		InitQueue(&hKey->hRx_Buf, Get_RX0_Buf(), RX0_BUF_SIZE);
		InitQueue(&hKey->hTx_Buf, Get_TX0_Buf(), TX0_BUF_SIZE);
		break;
		case 1:
		hSerial1 = hKey;
		InitQueue(&hKey->hRx_Buf, Get_RX1_Buf(), RX1_BUF_SIZE);
		InitQueue(&hKey->hTx_Buf, Get_TX1_Buf(), TX1_BUF_SIZE);
		break;
	}
	sei();
	
	return NONE;
}

uint8_t InitSerial(hUART* hKey, uint8_t Channel, uint32_t System_Clock, uint32_t BaudRate){
	uint16_t UBRR = (int16_t)(System_Clock/(BaudRate << 3) - 1);
	if(hKey == NULL)
		return 1;
	
	switch(Channel){
	/*
	* - Double the UART Transmission Speed Enable.
	* - RX/Data Register Ready Complete Interrupt Enable.
	* - Receiver/Transmitter Enable.
	* - Set Baud Rate.
	*/
	case 0:	// Channel 0
		UCSR0A |= (1 << U2X0);
		UCSR0B |= (1 << RXCIE0) | (1 << UDRIE0) | (1 << RXEN0) | (1 << TXEN0);
		UBRR0H = (uint8_t)((UBRR >> 8) & 0xFF);
		UBRR0L = (uint8_t)(UBRR & 0xFF);
		
		break;
	case 1:	// Channel 1
		UCSR1A |= (1 << U2X1);
		UCSR1B |= (1 << RXCIE1) | (1 << UDRIE1) | (1 << RXEN1) | (1 << TXEN1);
		UBRR1H = (UBRR >> 8) & 0xFF;
		UBRR1L = (uint8_t)(UBRR & 0xFF);
		break;
	default:
		return 2;
	}
	
	hKey->SerialAvailable = SerialAvailable;
	hKey->SerialRead = SerialRead;
	hKey->SerialWrite = SerialWrite;
	
	return SetUART_Handler(hKey, Channel, BaudRate);
}

hUART* GetUART_Handle(uint8_t Channel){
	hUART* retHandle = NULL;
	
	switch(Channel){
	case 0:
		retHandle = hSerial0;
		break;
	case 1:
		retHandle = hSerial1;
		break;
	}
	
	return retHandle;
}

ISR(USART0_UDRE_vect){
	uint8_t Data;
	if(M_DeQueue(&hSerial0->hTx_Buf, &Data) != EMPTY_BUF)
		UDR0 = Data;
	else
		UCSR0B ^= (1 << UDRIE0);
}
ISR(USART0_RX_vect){
	register uint8_t Data = UDR0;
	M_EnQueue(&hSerial0->hRx_Buf, Data);
}
ISR(USART1_UDRE_vect){
	uint8_t Data;
	if(M_DeQueue(&hSerial1->hTx_Buf, &Data) != EMPTY_BUF)
		UDR1 = Data;
	else
		UCSR1B ^= (1 << UDRIE1);
}
ISR(USART1_RX_vect){
	uint8_t Data = UDR1;
	M_EnQueue(&hSerial1->hRx_Buf, Data);
}