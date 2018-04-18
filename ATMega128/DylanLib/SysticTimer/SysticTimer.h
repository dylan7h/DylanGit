/*
 * SysticTimer.h
 *
 * Created: 2018-04-15 오전 1:29:06
 *  Author: kazam
 */ 


#ifndef SYSTICTIMER_H_
#define SYSTICTIMER_H_

typedef struct Systic_Handle{
	uint8_t(*SetTimer)(uint16_t Period, void(*CallBackTimer)(void));
	uint32_t(*Times)(void);
	uint32_t(*Timediff)(uint32_t t_end, uint32_t t_begin);
} hSystic;

uint8_t InitSysticTimer(hSystic* key, uint32_t System_Clock);
hSystic* GetSysticHandle();

#define M_SetTimer(key, Period, CallBackTimer) (key)->SetTimer(Period, CallBackTimer)
#define M_Times(key) (key)->Times()
#define M_Timediff(key, t_end, t_begin) (key)->Timediff(t_end, t_begin)
 
#endif /* SYSTICTIMER_H_ */