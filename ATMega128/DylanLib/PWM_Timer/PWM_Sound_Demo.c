/*
 *  File Name   :   PWM_Sound_Demo.c
 *
 *  Created     :   2018-04-28 A.M :11:42
 *  belong      :   Korea Polytechnic University
 *					, Department of Energy-Electrical Engineering Student.
 *  Author      :   KOR DYLAN( Korean name: Jun Ki, Hong)
 *  YouTube     :   https://www.youtube.com/channel/UC9DTd1Rv730XKmWRTpqY8Rg?view_as=subscriber
 *  e-mail      :   dylan.7h@gmail.com
 *  IDE Software:   Atmel Studio 7
 *  Hardware    :   ATmega128, tested on ATmega128(jmod-128-1) at 16MHz.
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "PWM_Timer.h"

extern const uint8_t PROGMEM Sound[22468] ;
hPWM hTimer1;
uint16_t idx;
uint8_t SojuSound();

int main(void) {
	InitPWM(&hTimer1, TIMER_ID1, F_CPU, 16000);
	
	M_SetDuty(&hTimer1, CH_C_PWM, 7500);
	
	M_EnablePWM(&hTimer1, CH_A_PWM);
	M_EnablePWM(&hTimer1, CH_B_PWM);
	M_EnablePWM(&hTimer1, CH_C_PWM);

	M_SetOVF_ISR(&hTimer1, SojuSound);
	
	while (1){
		
	} 
}

uint8_t SojuSound(){
	register uint16_t Duty = 0;
	
	if(idx == 22468){
		idx = 0;
		return 0;
	}
	
	Duty =  pgm_read_byte(&Sound[idx]) * ( 10000 / 0xFF );
	M_SetDuty(&hTimer1, CH_A_PWM, Duty);
	M_SetDuty(&hTimer1, CH_B_PWM, Duty);
	M_SetDuty(&hTimer1, CH_C_PWM, Duty);
	idx += 1;
	
	return 0;
}