/*
 *  File Name   :   SysticTimer_Demo.c
 *
 *  Created     :   2018-04-16 P.M 7:36:54
 *  belong      :   Korea Polytechnic University
 *                  , Department of Energy-Electrical Engineering Student.
 *  Author      :   KOR DYLAN( Korean name: Jun Ki, Hong)
 *  YouTube     :   https://www.youtube.com/channel/UC9DTd1Rv730XKmWRTpqY8Rg?view_as=subscriber
 *  e-mail      :   dylan.7h@gmail.com
 *  IDE Software:   Atmel Studio 7
 *  Hardware    :   ATmega128, tested on ATmega128(jmod-128-1) at 16MHz.
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include "SystickTimer.h"
#include "UART.h"

/*
*  Serial0     : Handle of UART0.(The demo example is used with UART.)
*  SystemTimer : Systick Timer Handle
*/
hUART Serial0;
hSystick SystemTimer;

//Declaration of the function to be registered in the CallBack function.
void TimCallBack(void);

int main(void) {
	// Initialization of serial communication and systick timer.
	InitSerial(&Serial0, 0, F_CPU, 115200);
	InitSystickTimer(&SystemTimer, F_CPU);
	
	// Registration of CallBack function. (100ms cycle.)
	M_SetTimer(&SystemTimer, 100, TimCallBack);
	
	/* Replace with your application code */
	while (1) {
		// Empty...
	}
}

void TimCallBack(void){
	static uint8_t bFlag;
	static uint32_t t_begin;
	uint32_t result;
	char Buf[30];
	
	if(bFlag){
		// Time measurement end and elapsed time calculation.
		result = M_Timediff(&SystemTimer, M_Times(&SystemTimer), t_begin);
		// Create a string form and send it via serial communication.
		sprintf(Buf, "%d [ms]\n", (int)result);
		M_SerialWrite(&Serial0, (uint8_t*)Buf, strlen(Buf));
		bFlag = 0;
	}
	else{
		// Start measuring time.
		t_begin = M_Times(&SystemTimer);
		bFlag = 1;
	}
}