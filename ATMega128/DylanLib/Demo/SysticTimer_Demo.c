/*
 * SysticTimer_Demo.c
 *
 * Created: 2018-04-16 오후 7:36:54
 *  Author: kazam
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include "SysticTimer.h"
#include "UART.h"

hUART Serial0;
hSystic SystemTimer;

void TimCallBack(void);

int main(void) {
	InitSerial(&Serial0, 0, F_CPU, 115200);
	InitSysticTimer(&SystemTimer, F_CPU);
	
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
		result = M_Timediff(&SystemTimer, M_Times(&SystemTimer), t_begin);
		sprintf(Buf, "%d [ms]\n", (int)result);
		M_SerialWrite(&Serial0, (uint8_t*)Buf, strlen(Buf));
		bFlag = 0;
	}
	else{
		t_begin = M_Times(&SystemTimer);
		bFlag = 1;
	}
}