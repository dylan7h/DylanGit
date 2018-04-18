/*
 * PWM_Timer.c
 *
 * Created: 2018-04-15 오전 1:28:56
 *  Author: kazam
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "PWM_Timer.h"

#define CHECK_KEY(key) if(key == NULL) return NULL_KEY;

static hPWM* hTimer1;
static hPWM* hTimer3;

static inline uint8_t SetOVF_ISR(hPWM* key, uint8_t(*New_OVF_ISR)(void)){
	CHECK_KEY(key);
	key->OVF_ISR = New_OVF_ISR;
	
	return 0;
}

static inline uint8_t SetCOMP_ISR(hPWM* key, uint8_t Channel, uint8_t(*COMP_ISR)(void)){
	CHECK_KEY(key);
	
	switch(Channel){
	case CH_A_PWM:
		key->CH_A_COMP_ISR = COMP_ISR;
		break;
	case CH_B_PWM:
		key->CH_B_COMP_ISR = COMP_ISR;
		break;
	case CH_C_PWM:
		key->CH_C_COMP_ISR = COMP_ISR;
		break;
	}
	
	return 0;
}

static inline uint16_t GetFRQ(hPWM* key){
	return key->Target_FRQ;
}

static inline uint16_t GetDuty(hPWM* key, uint8_t Channel){
	switch(Channel){
	case CH_A_PWM:
		return key->CH_A_Duty;
	case CH_B_PWM:
		return key->CH_B_Duty;
	case CH_C_PWM:
		return key->CH_C_Duty;
	}
	
	return DUTY_ERROR;
}

static inline uint8_t IsEnablePWM(hPWM* key, uint8_t Channel){
	return (key->Enable_Channel & Channel) ? TRUE : FALSE;
}

static inline uint8_t WhoAmI(hPWM* key){
	return key->Timer_ID;
}

static inline uint8_t SetDuty(hPWM* key, uint8_t Channel, uint16_t Duty){
	register uint16_t DstVal;
	CHECK_KEY(key);
	
	if(Duty >= 10000)
	return DUTY_ERROR;
	
	DstVal = key->Top_Value - (uint16_t)((uint32_t)key->Top_Value * (uint32_t)Duty / 10000);
	if(DstVal == key->Top_Value)
	DstVal -= 1;
	else if(DstVal == 0)
	DstVal = 1;
	
	switch(Channel){
	case CH_A_PWM:
		switch(key->Timer_ID){
		case TIMER_ID1:
			OCR1AH = (DstVal >> 8) & 0xFF;
			OCR1AL = DstVal & 0xFF;
			break;
		case TIMER_ID3:
			OCR3AH = (DstVal >> 8) & 0xFF;
			OCR3AL = DstVal & 0xFF;
			break;
		}
		key->CH_A_Duty = Duty;
		break;
	case CH_B_PWM:
		switch(key->Timer_ID){
		case TIMER_ID1:
			OCR1BH = (DstVal >> 8) & 0xFF;
			OCR1BL = DstVal & 0xFF;
			break;
		case TIMER_ID3:
			OCR3BH = (DstVal >> 8) & 0xFF;
			OCR3BL = DstVal & 0xFF;
			break;
		}
		key->CH_B_Duty = Duty;
		break;
	case CH_C_PWM:
		switch(key->Timer_ID){
		case TIMER_ID1:
			OCR1CH = (DstVal >> 8) & 0xFF;
			OCR1CL = DstVal & 0xFF;
			break;
		case TIMER_ID3:
			OCR3CH = (DstVal >> 8) & 0xFF;
			OCR3CL = DstVal & 0xFF;
			break;
		}
		key->CH_C_Duty = Duty;
		break;
	}
	
	return 0;
}

static inline uint8_t SetFRQ(hPWM* key, uint16_t FRQ){
	register uint16_t TopVal;
	CHECK_KEY(key);
	if(FRQ > 20000)
		return FRQ_ERROR;
	
	if(FRQ > 130){
		TopVal = key->System_Clock / (2 * FRQ);
		switch(key->Timer_ID){
		case TIMER_ID1:
			TCCR1B &= 0xFF - ((1 << CS12) | (1 << CS11) | (1 << CS10));
			TCCR1B |= (1 << CS10);
			ICR1H = (TopVal >> 8) & 0xFF; ICR1L = TopVal & 0xFF;
			TCNT1H = 0; TCNT1L = 0;
			break;
		case TIMER_ID3:
			TCCR3B &= 0xFF - ((1 << CS32) | (1 << CS31) | (1 << CS30));
			TCCR3B |= (1 << CS30);
			ICR3H = (TopVal >> 8) & 0xFF;
			ICR3L = TopVal & 0xFF;
			TCNT3H = 0; TCNT3L = 0;
			break;
		}
	}
	else{
		TopVal = key->System_Clock / (2 * 8 * FRQ);
		switch(key->Timer_ID){
		case TIMER_ID1:
			TCCR1B &= 0xFF - ((1 << CS12) | (1 << CS11) | (1 << CS10));
			TCCR1B |= (1 << CS11);
			ICR1H = (TopVal >> 8) & 0xFF; ICR1L = TopVal & 0xFF;
			TCNT1H = 0; TCNT1L = 0;
			break;
		case TIMER_ID3:
			TCCR3B &= 0xFF - ((1 << CS32) | (1 << CS31) | (1 << CS30));
			TCCR3B |= (1 << CS31);
			ICR3H = (TopVal >> 8) & 0xFF; ICR3L = TopVal & 0xFF;
			TCNT3H = 0; TCNT3L = 0;
		break;
		}
	}
	
	key->Top_Value = TopVal;
	key->Target_FRQ = FRQ;
	
	if(IsEnablePWM(key, CH_A_PWM))
		SetDuty(key, CH_A_PWM, key->CH_A_Duty);
		
	if(IsEnablePWM(key, CH_B_PWM))
		SetDuty(key, CH_B_PWM, key->CH_B_Duty);
	
	if(IsEnablePWM(key, CH_C_PWM))
		SetDuty(key, CH_C_PWM, key->CH_C_Duty);
	
	return 0;
}

static inline uint8_t EnablePWM(hPWM* key, uint8_t Channel){
	CHECK_KEY(key);
	
	switch(Channel){
	case CH_A_PWM:
		key->Enable_Channel |= CH_A_PWM;
		switch(key->Timer_ID){
		case TIMER_ID1:
			TCCR1A |= (1 << COM1A1) | (1 << COM1A0);
			break;
		case TIMER_ID3:
			TCCR3A |= (1 << COM3A1) | (1 << COM3A0);
			break;
		}
		break;
	case CH_B_PWM:
		key->Enable_Channel |= CH_B_PWM;
		switch(key->Timer_ID){
		case TIMER_ID1:
			TCCR1A |= (1 << COM1B1) | (1 << COM1B0);
			break;
		case TIMER_ID3:
			TCCR3A |= (1 << COM3B1) | (1 << COM3B0);
			break;
		}
		break;
	case CH_C_PWM:
		key->Enable_Channel |= CH_C_PWM;
		switch(key->Timer_ID){
		case TIMER_ID1:
			TCCR1A |= (1 << COM1C1) | (1 << COM1C0);
			break;
		case TIMER_ID3:
			TCCR3A |= (1 << COM3C1) | (1 << COM3C0);
			break;
		}
		break;
	}
	return 0;
}

static inline uint8_t DisablePWM(hPWM* key, uint8_t Channel){
	CHECK_KEY(key);
	
	switch(Channel){
	case CH_A_PWM:
		key->Enable_Channel &= 0xFF - CH_A_PWM;
		switch(key->Timer_ID){
		case TIMER_ID1:
			TCCR1A &= 0xFF - ((1 << COM1A1) | (1 << COM1A0));
			break;
		case TIMER_ID3:
			TCCR1A &= 0xFF - ((1 << COM3A1) | (1 << COM3A0));
			break;
		}
		break;
	case CH_B_PWM:
		key->Enable_Channel &= 0xFF - CH_B_PWM;
		switch(key->Timer_ID){
		case TIMER_ID1:
			TCCR1A &= 0xFF - ((1 << COM1B1) | (1 << COM1B0));
			break;
		case TIMER_ID3:
			TCCR1A &= 0xFF - ((1 << COM3B1) | (1 << COM3B0));
			break;
		}
		break;
	case CH_C_PWM:
		key->Enable_Channel &= 0xFF - CH_C_PWM;
		switch(key->Timer_ID){
		case TIMER_ID1:
			TCCR1A &= 0xFF - ((1 << COM1C1) | (1 << COM1C0));
			break;
		case TIMER_ID3:
			TCCR1A &= 0xFF - ((1 << COM3C1) | (1 << COM3C0));
			break;
		}
		break;
	}
	return 0;
}

uint8_t InitPWM(hPWM* key, uint8_t ID, uint32_t System_Clock, uint16_t FRQ){
	CHECK_KEY(key);
	
	key->System_Clock = System_Clock;
	key->Target_FRQ = FRQ;
	key->CH_A_Duty = 1;	
	key->CH_B_Duty = 1;	
	key->CH_C_Duty = 1;	
	key->Timer_ID = ID;
	
	key->OVF_ISR = NULL;
	key->CH_A_COMP_ISR = NULL;
	key->CH_B_COMP_ISR = NULL;
	key->CH_C_COMP_ISR = NULL;
	
	key->SetFRQ = SetFRQ;
	key->SetDuty = SetDuty;
	key->SetOVF_ISR = SetOVF_ISR;
	key->SetCOMP_ISR = SetCOMP_ISR;
	key->EnablePWM = EnablePWM;
	key->DisablePWM = DisablePWM;
	key->GetFRQ = GetFRQ;
	key->GetDuty = GetDuty;
	key->IsEnablePWM = IsEnablePWM;
	key->WhoAmI = WhoAmI;
	
	switch(ID){
	case TIMER_ID1:
		TCCR1B |= (1 << WGM13);
		DDRB |= (1 << PB5) | (1 << PB6) | (1 << PB7);
		PORTB &= 0xFF - ((1 << PB5) | (1 << PB6) | (1 << PB7));
		TIMSK |= (1 <<TOIE1) | (1 << OCIE1A) | (1 << OCIE1B); 
		ETIMSK |= (1 << OCIE1C);
		hTimer1 = key;
		break;
	case TIMER_ID3:
		TCCR3B |= (1 << WGM33);
		DDRE |= (1 << PE3) | (1 << PE4) | (1 << PE5);
		PORTB &= 0xFF - ((1 << PE3) | (1 << PE4) | (1 << PE5));
		ETIMSK |= (1 <<TOIE3) | (1 << OCIE3A) | (1 << OCIE3B) | (1 << OCIE3C); 
		hTimer3 = key;
		break;
	default:
		return WRONG_ID;
	}
	
	SetFRQ(key, FRQ);
	
	sei();
	
	return 0;
}

ISR(TIMER1_OVF_vect){
	if(hTimer1->OVF_ISR != NULL)
		hTimer1->OVF_ISR();
}

ISR(TIMER1_COMPA_vect){
	if(hTimer1->CH_A_COMP_ISR != NULL)
		hTimer1->CH_A_COMP_ISR();
}

ISR(TIMER1_COMPB_vect){
	if(hTimer1->CH_B_COMP_ISR != NULL)
		hTimer1->CH_B_COMP_ISR();
}

ISR(TIMER1_COMPC_vect){
	if(hTimer1->CH_C_COMP_ISR != NULL)
		hTimer1->CH_C_COMP_ISR();
}

ISR(TIMER3_OVF_vect){
	if(hTimer3->OVF_ISR != NULL)
		hTimer3->OVF_ISR();
}

ISR(TIMER3_COMPA_vect){
	if(hTimer3->CH_A_COMP_ISR != NULL)
		hTimer3->CH_A_COMP_ISR();
}

ISR(TIMER3_COMPB_vect){
	if(hTimer3->CH_B_COMP_ISR != NULL)
		hTimer3->CH_B_COMP_ISR();
}

ISR(TIMER3_COMPC_vect){
	if(hTimer3->CH_C_COMP_ISR != NULL)
		hTimer3->CH_C_COMP_ISR();
}