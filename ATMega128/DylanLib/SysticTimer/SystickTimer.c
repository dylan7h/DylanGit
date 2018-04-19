 /*
 *  File Name   :   SysticTimer.c
 *
 *  Created	    :   2018-04-15 A.M 1:29:18
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
#include <stdlib.h>
#include "SystickTimer.h"

#define MAX_SYSTICK 0xFFFFFFFF

hSystick* SysticHandle = NULL;
static uint32_t Systick_Cnt = 0;
static uint16_t CallBackCnt = 0;
static uint16_t Callback_Period = 1000;
static void(*CallBackFunction)(void);

hSystick* GetSystickHandle(){
	return SysticHandle;
}

static inline uint8_t SetTimer(uint16_t Period, void(*CallBackTimer)(void)){
	if(CallBackTimer == NULL)
		return 1;
		
	Callback_Period = Period; 
	CallBackFunction = CallBackTimer;
	
	return 0;
}

static inline uint32_t Times(void){
	return Systick_Cnt;
}

static inline uint32_t Timediff(uint32_t t_end, uint32_t t_begin){
	return (t_end < t_begin) ? (MAX_SYSTICK - t_begin) + t_end : t_end - t_begin;
}

uint8_t InitSystickTimer(hSystick* key, uint32_t System_Clock){
	SysticHandle = key;
	key->SetTimer = SetTimer;
	key->Timediff = Timediff;
	key->Times = Times;
	
	// Division Rate
	TCCR0 |= (1 << CS02);
	TCCR2 |= (1 << CS21) | (1 <<CS20);
	
	// Run Mode
	TCCR0 |= (1 << WGM01);
	TCCR2 |= (1 << WGM21);
	
	// Top Val
	OCR0 = System_Clock / 1000 / 64 / 2 - 1;
	OCR2 = System_Clock / 1000 / 64 / 2 - 1;
	
	// Interrupt Enable
	TIMSK |= (1 << OCIE0) | (1 << OCIE2) | (1 << TOIE0) | (1 << TOIE2);
	
	sei();
	
	return 0;
}

ISR(TIMER0_COMP_vect){
	Systick_Cnt = (Systick_Cnt == MAX_SYSTICK) ? 0 : Systick_Cnt + 1;
}

ISR(TIMER2_COMP_vect){
	if(Callback_Period == ++CallBackCnt){
		CallBackCnt = 0;
		if(CallBackFunction != NULL)
			CallBackFunction();
	}
}