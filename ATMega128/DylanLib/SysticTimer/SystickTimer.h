/*
 *  File Name   :   SystickTimer.h
 *
 *  Created	    :   2018-04-15 A.M 1:29:06
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

#ifndef SYSTICKTIMER_H_
#define SYSTICKTIMER_H_

/*
*  [ Systick Timer Handle Structure ]
*   - Member Function.
*   1. SetTimer  : A feature created by reminding the SetTimer of the Windows API.
*   - The function address that you entered is registered in the CallBack function and called by the CallBack function every input cycle.
*   >> Period : CallBack cycle, CallBackTimer : The address of the function to register in the CallBack function.
*
*   2. Times     : Returns the current clock count.
*
*   3. Timediff  : Calculates and returns the time difference using the number of clocks returned by two Times functions.
*   >> t_end : Time to end, t_begin : Start time.
*/
typedef struct Systick_Handle{
	uint8_t(*SetTimer)(uint16_t Period, void(*CallBackTimer)(void));
	uint32_t(*Times)(void);
	uint32_t(*Timediff)(uint32_t t_end, uint32_t t_begin);
} hSystick;

/*  [ Initialize Systick Timer Function ]
*   >> hKey         : Non Initialized hSystic type Handle variable Address.
*   >> System_Clock : System Frequency.
*--------------------------------------------------------------------------
* [ Note ]
* The Systick Timer is based on 8-bit timers, Timer 0 and Timer 2.
* Timer 0 is used to return the number of clocks
* , and Timer 2 is used to perform the CallBack function.
* Never reinitialize Systick Timer Handle!
*--------------------------------------------------------------------------
*/
uint8_t InitSystickTimer(hSystick* key, uint32_t System_Clock);

/*
*   [ GetSysticHandle ]
*   - If you have initialized the Systick Timer handle
*   , return the address of the Systick Timer handle.
*/
hSystick* GetSystickHandle();

/* [ Macro for ease of use ] */
#define M_SetTimer(key, Period, CallBackTimer) (key)->SetTimer(Period, CallBackTimer)
#define M_Times(key) (key)->Times()
#define M_Timediff(key, t_end, t_begin) (key)->Timediff(t_end, t_begin)
 
#endif /* SYSTICTIMER_H_ */