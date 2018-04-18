/*
 * Atmega128Lib.c
 *
 * Created: 2018-04-14 오후 12:47:31
 * Author : kazam
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include "UART.h"

int main(void) {
	uint8_t retVal, temp[50];
	hUART Serial0;
	
	InitSerial(&Serial0, 0, F_CPU, 115200);
	
    /* Replace with your application code */
    while (1) {
		if(M_SerialAvailable(&Serial0)){
			retVal = M_SerialRead(&Serial0, temp, 50);
			M_SerialWrite(&Serial0, temp, retVal);
		}
    }
}