/*
 * PWM_Demo.c
 *
 * Created: 2018-04-15 오후 1:22:52
 *  Author: kazam
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "PWM_Timer.h"

int main(void) {
	uint8_t idx;
	hPWM hTimer1, hTimer3;
	
	InitPWM(&hTimer1, TIMER_ID1, F_CPU, 100);
	InitPWM(&hTimer3, TIMER_ID3, F_CPU, 1000);
	
	M_SetDuty(&hTimer1, CH_A_PWM, 7500);
	M_SetDuty(&hTimer1, CH_B_PWM, 7500);
	M_SetDuty(&hTimer1, CH_C_PWM, 7500);
	M_SetDuty(&hTimer3, CH_A_PWM, 7500);
	M_SetDuty(&hTimer3, CH_B_PWM, 7500);
	M_SetDuty(&hTimer3, CH_C_PWM, 7500);
	
	M_EnablePWM(&hTimer1, CH_A_PWM);
	M_EnablePWM(&hTimer1, CH_B_PWM);
	M_EnablePWM(&hTimer1, CH_C_PWM);
	
	M_EnablePWM(&hTimer3, CH_A_PWM);
	M_EnablePWM(&hTimer3, CH_B_PWM);
	M_EnablePWM(&hTimer3, CH_C_PWM);
	
	/* Replace with your application code */
	while (1) {
		for(idx = 1; idx <= 20; idx++){
			_delay_ms(1000);
			
			M_SetFRQ(&hTimer1, idx*1000);
			M_SetFRQ(&hTimer3, idx*100);
		}
	}
}

