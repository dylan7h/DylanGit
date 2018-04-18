/*
 *  File Name           :	UART_Demo.c
 *
 *  Created		:	2018-04-15 P.M 12:47:31
 *  belong		:	Korea Polytechnic University
 *					, Department of Energy-Electrical Engineering Student.
 *  Author		:	KOR DYLAN( Korean name: Jun Ki, Hong)
 *  YouTube		:	https://www.youtube.com/channel/UC9DTd1Rv730XKmWRTpqY8Rg?view_as=subscriber
 *  e-mail		:	dylan.7h@gmail.com
 *  IDE Software:   Atmel Studio 7
 *  Hardware    :   ATmega128, tested on ATmega128(jmod-128-1) at 16MHz.
 */ 

/*
* [ This demo code is an example of an echo back using UART. ]
*/

#define F_CPU 16000000UL
#include <avr/io.h>
#include "UART.h"

int main(void) {
    /*
    *  retVal  : The variable to store the return value of the function.
    *  >> The number of data read from the receive buffer.
    *  buf     : Receive and Transmit Data Buf.
    *  Serial0 : UART Handle of Channel 0.
    */
    uint8_t retVal, buf[50];
    hUART Serial0;
	
    // Initialize UART0 and Handle
    InitSerial(&Serial0, 0, F_CPU, 115200);
	
    /* Replace with your application code */
    while (1) {
		// if receive buffer on data being...
		if(M_SerialAvailable(&Serial0)){
			// The data of the receive buffer is stored in buf block.
			// The value returned is the size of the received data.
			retVal = M_SerialRead(&Serial0, buf, sizeof(buf));
			// The received data as it sends back.
			M_SerialWrite(&Serial0, buf, retVal);
		}
    }
}
