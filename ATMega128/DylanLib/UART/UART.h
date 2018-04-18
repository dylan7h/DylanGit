/*
 * UART.h
 *
 * Created: 2018-04-14 오후 1:02:26
 *  Author: kazam
 */ 

#ifndef UART_H_
#define UART_H_

/* Choose Macro USING_DYNAMIC_MEMORY or USING_STATIC_MEMORY! */
#define USING_DYNAMIC_MEMORY
//#define USING_STATIC_MEMORY

#include "DataQueue.h"
#define RX0_BUF_SIZE 128
#define TX0_BUF_SIZE 128
#define RX1_BUF_SIZE 16
#define TX1_BUF_SIZE 16

typedef struct UART_HANDLE {
	uint8_t Channel;
	uint32_t BaudRate;
	
	hQueue hRx_Buf;
	hQueue hTx_Buf;

	uint8_t(*SerialAvailable)(struct UART_HANDLE* hKey);
	uint8_t(*SerialWrite)(struct UART_HANDLE* hKey, uint8_t* Buf, uint8_t Len);
	uint8_t(*SerialRead)(struct UART_HANDLE* hKey, uint8_t* Buf, uint8_t Size);
	uint8_t(*WhoAmI)(struct UART_HANDLE* hKey);
	uint32_t(*GetBaudRate)(struct UART_HANDLE* hKey);
} hUART;

uint8_t InitSerial(hUART* hKey, uint8_t Channel, uint32_t System_Clock, uint32_t BaudRate);
uint8_t SetUART_Handler(hUART* hKey, uint8_t Channel, uint32_t BaudRate);
hUART* GetUART_Handle(uint8_t Channel);

#define M_SerialAvailable(hKey) (hKey)->SerialAvailable(hKey)
#define M_SerialWrite(hKey, pBuf, Len) (hKey)->SerialWrite(hKey, pBuf, Len)
#define M_SerialRead(hKey, pBuf, Size) (hKey)->SerialRead(hKey, pBuf, Size)
#define M_WhoAmI(hKey) (hKey)->WhoAmI(hKey)
#define M_GetBaudRate(hKey) (hKey)->GetBaudRate(hKey)

#endif /* UART_H_ */