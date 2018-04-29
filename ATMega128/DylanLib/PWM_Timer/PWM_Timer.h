/*
 *  File Name   :   PWM_Timer.h
 *
 *  Created	    :   2018-04-15 A.M 1:28:48
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

#ifndef PWM_TIMER_H_
#define PWM_TIMER_H_

/*
*  [ Timer number macro used for PWM ]
* Note : The 16-bit timer is used as the timer for PWM, and the 16-bit timers for ATMega128 are Timer1 and Timer 3.
*/
#define TIMER_ID1	0x01
#define TIMER_ID3	0x03

/*
* [ Channel macro of PWM timer n ]
* Note : The ATmega128 16-bit timer has three ports that can output PWM signals.
*/
#define CH_A_PWM	0x01
#define CH_B_PWM	0x02
#define CH_C_PWM	0x04

#define NONE		0x00
#define FALSE		0x00
#define TRUE		0x01
#define FRQ_ERROR	0x03
#define WRONG_ID	0x02
#define NULL_KEY	0x04
#define DUTY_ERROR	0x05

/*
*  [ PWM Handle Structure ]
*	- Member variable
*   1. Timer_ID       : Timer number used for PWM
*   2. Target_FRQ     : Target PWM Frequency
*   3. Top_Value      : The maximum value of the TCNT for setting the target PWM frequency.
*   4. CH_A/B/C_Duty  : Duty value for each PWM channel A / B / C (1% → 100)
*   5. Enable_Channel : Current operating PWM channel.
*   6. System_Clock   : System Frequency.
*
*   - Member Function.
*   1. OVF_ISR           : Interrupt service routine to be executed when TCNTn of timer n overflows.
*
*   2. CH_A/B/C_COMP_ISR : Interrupt service routine to be executed when output compare interrupt occurs on channel A / B / C of timer n.
*
*   3. SetFRQ            : Set the target frequency of the PWM.
*   >> key : Handle Address, FRQ : the target frequency of the PWM.
*
*   4. SetDuty           : Set the target Duty of the PWM.
*   >> key : Handle Address, Channel : , Duty : the target Duty of the PWM
*
*   5. SetOVF_ISR        : It links the function of the task that you want to perform when the corresponding interrupt occurs.
*   >> key : Handle Address, New_OVF_ISR : The function address of the operation to be performed.
*
*   6. SetCOMP_ISR       : It links the function of the task that you want to perform when the corresponding interrupt occurs.
*   >> key : Handle Address, Channel : Target PWM Channel, COMP_ISR : The function address of the operation to be performed.
*
*   7. EnablePWM         : Allow PWM on this channel.
*   >> key : Handle Address, Channel : Target Channel.
*
*   8. DisablePWM        : Prohibit the PWM of this channel.
*   >> key : Handle Address, Channel : Target Channel.
*
*   9. GetFRQ            : Returns the current target PWM frequency.
*   >> key : Handle Address
*
*   10. GetDuty          : Returns the current target PWM Duty.
*   >> key : Handle Address, Channel : 
*
*   11. IsEnablePWM      : Returns 1 if this channel's PWM is allowed, 0 otherwise.
*   >> key : Handle Address, 
*
*   12. WhoAmI           : Returns the timer number registered on the handle.
*   >> key : Handle Address

*/
typedef struct PWM_HANDLE {
	uint8_t Timer_ID;
	uint32_t Target_FRQ;
	uint16_t Top_Value;
	uint16_t CH_A_Duty;
	uint16_t CH_B_Duty;
	uint16_t CH_C_Duty;
	uint8_t	Enable_Channel;
	uint32_t System_Clock;
	
	uint8_t(*OVF_ISR)(void);
	uint8_t(*CH_A_COMP_ISR)(void);
	uint8_t(*CH_B_COMP_ISR)(void);
	uint8_t(*CH_C_COMP_ISR)(void);
	
	uint8_t(*SetFRQ)(struct PWM_HANDLE* key, uint32_t FRQ);
	uint8_t(*SetDuty)(struct PWM_HANDLE* key, uint8_t Channel, uint16_t Duty);
	uint8_t(*SetOVF_ISR)(struct PWM_HANDLE* key, uint8_t(*New_OVF_ISR)(void));
	uint8_t(*SetCOMP_ISR)(struct PWM_HANDLE* key, uint8_t Channel, uint8_t(*COMP_ISR)(void));
	uint8_t(*EnablePWM)(struct PWM_HANDLE* key, uint8_t Channel);
	uint8_t(*DisablePWM)(struct PWM_HANDLE* key, uint8_t Channel);
	uint16_t(*GetFRQ)(struct PWM_HANDLE* key);
	uint16_t(*GetDuty)(struct PWM_HANDLE* key, uint8_t Channel);
	uint8_t(*IsEnablePWM)(struct PWM_HANDLE* key, uint8_t Channel);
	uint8_t(*WhoAmI)(struct PWM_HANDLE* key);
} hPWM;

/*  [ Initialize PWM Function ]
*   >> hKey         : Non Initialized hPWM type Handle variable Address.
*   >> ID           : To use Timer num.
*   >> System_Clock : System Frequency.
*   >> FRQ          : Target PWM Frequency.
*/
uint8_t InitPWM(hPWM* key, uint8_t ID, uint32_t System_Clock, uint32_t FRQ);

/* [ Macro for ease of use ] */
#define M_SetFRQ(key, FRQ) (key)->SetFRQ(key, FRQ)
#define M_SetDuty(key, Channel, Duty) (key)->SetDuty(key, Channel, Duty)
#define M_SetOVF_ISR(key, New_OVF_ISR) (key)->SetOVF_ISR(key, New_OVF_ISR)
#define M_SetCOMP_ISR(key, Channel, COMP_ISR) (key)->SetCOMP_ISR(key, Channel, COMP_ISR)
#define M_EnablePWM(key, Channel) (key)->EnablePWM(key, Channel)
#define M_DisablePWM(key, Channel) (key)->DisablePWM(key, Channel)
#define M_GetFRQ(key) (key)->GetFRQ(key)
#define M_GetDuty(key, Channel) (key)->GetDuty(key, Channel)
#define M_IsEnablePWM(key, Channel) (key)->IsEnablePWM(key, Channel)
#define M_WhoAmI(key) (key)->WhoAmI(key)

#endif /* PWM_TIMER_H_ */