/*
 *	File Name	:	UART.h
 *
 *  Created		:	2018-04-14 P.M 1:02:26
 *	belong		:	Korea Polytechnic University
 *					, Department of Energy-Electrical Engineering Student.
 *  Author		:	KOR DYLAN( Korean name: Jun Ki, Hong)
 *  YouTube		:	https://www.youtube.com/channel/UC9DTd1Rv730XKmWRTpqY8Rg?view_as=subscriber
 *	e-mail		:	dylan.7h@gmail.com
 *  Software	:	Atmel Studio 7
 *	Hardware	:	ATmega128, tested on ATmega128(jmod-128-1) at 16MHz.
 */ 

#ifndef UART_H_
#define UART_H_
 
// [ Choose Macro USING_DYNAMIC_MEMORY or USING_STATIC_MEMORY! ]
// - select  static or dynamic memory to use transmit and receive buffer memory.
#define USING_DYNAMIC_MEMORY
//#define USING_STATIC_MEMORY

#include "DataQueue.h"

// [ Set the transmit and receive buffer memory size at here. ]
// - Channel 1 Buf Size.
#define RX0_BUF_SIZE 128
#define TX0_BUF_SIZE 128
// - Channel 2 Buf Size.
#define RX1_BUF_SIZE 16
#define TX1_BUF_SIZE 16

/*
*  [ UART(Serial) Communication Handle Structure ]
*	- Member variable
*	1. Channel  : Channel number to use.
*	2. BaudRate : Communication speed.
*	3. hRx_Buf	: Receive Queue Buffer Handle.
*	4. hTx_Buf	: Transmit Queue Buffer Handle.
*
*	- Member Function.
*	1. SerialAvailable  : Returns 1 if there is data in the receive buffer.
*	>> hKey	:	Handle Address.
*
*	2. SerialWrite		: Load the data into the transmit buffer.
*	>> hKey	: Handle Address, Buf : Block of data to transfer, Len : Block data Len.
*
*	3. SerialRead		: Pull the data in the receive buffer.
*	>> hKey	: Handle Address, Buf : Receive memory block, Size : Memory block size.
*
*	4. WhoAmI			: Returns the channel number being used by the handle.
*	>> hKey	: Handle Address
*
*	5. GetBaudRate		: Returns the communication speed being used by the handle.
*	>> hKey	: Handle Address
*/
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

/*	[ Initialize UART Function ]
*	>> hKey			: Non Initialized  hUART type Handle variable Address.
*	>> Channel		: To use UART Channel.
*	>> System_Clock	: System Frequency.
*	>> BaudRate		: Communication Speed.
*/
uint8_t InitSerial(hUART* hKey, uint8_t Channel, uint32_t System_Clock, uint32_t BaudRate);

/*
*	[ GetUART_Handle ]
* - If you have initialized the handle for that channel 
*	, return the address of the handle for that channel.
*	>> Channel	: The channel number of the handle address to return.
*/
hUART* GetUART_Handle(uint8_t Channel);

/* [ Macro for ease of use ] */
#define M_SerialAvailable(hKey)			(hKey)->SerialAvailable(hKey)
#define M_SerialWrite(hKey, pBuf, Len)	(hKey)->SerialWrite(hKey, pBuf, Len)
#define M_SerialRead(hKey, pBuf, Size)	(hKey)->SerialRead(hKey, pBuf, Size)
#define M_WhoAmI(hKey)					(hKey)->WhoAmI(hKey)
#define M_GetBaudRate(hKey)				(hKey)->GetBaudRate(hKey)

#endif /* UART_H_ */