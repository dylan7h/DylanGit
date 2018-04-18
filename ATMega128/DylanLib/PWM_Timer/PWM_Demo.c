/*
 *  File Name           :	PWM_Demo.c
 *
 *  Created		:	2018-04-16 P.M 1:22:52
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
#include <util/delay.h>
#include "PWM_Timer.h"

int main(void) {
	/*
    *  idx             : The index variable to use for the loop.
    *  hTimer1/hTimer3 : Handles for Timer1 and Timer3.
    */
	uint8_t idx;
	hPWM hTimer1, hTimer3;
	
	/*
	*  [ PWM timer initialization ]
	* Target FRQ of Timer 1 : 100Hz
	* Target FRQ of Timer 3 : 1000Hz
	*/
	InitPWM(&hTimer1, TIMER_ID1, F_CPU, 100);
	InitPWM(&hTimer3, TIMER_ID3, F_CPU, 1000);
	
	/*
	* [ Set Duty of Timer1 ]
	* Target Duty: 75%(1% is 100.)
	*/
	M_SetDuty(&hTimer1, CH_A_PWM, 7500);
	M_SetDuty(&hTimer1, CH_B_PWM, 7500);
	M_SetDuty(&hTimer1, CH_C_PWM, 7500);
	
	/*
	* [ Set Duty of Timer3 ]
	* Target Duty: 75%(1% is 100.)
	*/
	M_SetDuty(&hTimer3, CH_A_PWM, 7500);
	M_SetDuty(&hTimer3, CH_B_PWM, 7500);
	M_SetDuty(&hTimer3, CH_C_PWM, 7500);
	
	// Enable PWM of Timer1(Channel A/B/C)
	M_EnablePWM(&hTimer1, CH_A_PWM);
	M_EnablePWM(&hTimer1, CH_B_PWM);
	M_EnablePWM(&hTimer1, CH_C_PWM);
	// Enable PWM of Timer3(Channel A/B/C)
	M_EnablePWM(&hTimer3, CH_A_PWM);
	M_EnablePWM(&hTimer3, CH_B_PWM);
	M_EnablePWM(&hTimer3, CH_C_PWM);
	
	/* Replace with your application code */
	while (1) {
		// Timer 1 increments the frequency by 1 kHz every 1 second, and Timer 3 increments the frequency by 100 Hz.
		// (Duty is fixed at 75%)
		for(idx = 1; idx <= 20; idx++){
			_delay_ms(1000);
			
			M_SetFRQ(&hTimer1, idx*1000);
			M_SetFRQ(&hTimer3, idx*100);
		}
	}
}

