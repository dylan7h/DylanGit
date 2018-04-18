/*
 * PWM_Timer.h
 *
 * Created: 2018-04-15 오전 1:28:48
 *  Author: kazam
 */ 


#ifndef PWM_TIMER_H_
#define PWM_TIMER_H_

#define TIMER_ID0	0x00
#define TIMER_ID1	0x01
#define TIMER_ID2	0x02
#define TIMER_ID3	0x03

#define CH_A_PWM 0x01
#define CH_B_PWM 0x02
#define CH_C_PWM 0x04

#define NONE		0x00
#define FALSE		0x00
#define TRUE		0x01
#define FRQ_ERROR	0x03
#define WRONG_ID	0x02
#define NULL_KEY	0x04
#define DUTY_ERROR	0x05

typedef struct PWM_HANDLE {
	uint8_t Timer_ID;
	uint16_t Target_FRQ;
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
	
	uint8_t(*SetFRQ)(struct PWM_HANDLE* key, uint16_t FRQ);
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

uint8_t InitPWM(hPWM* key, uint8_t ID, uint32_t System_Clock, uint16_t FRQ);

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