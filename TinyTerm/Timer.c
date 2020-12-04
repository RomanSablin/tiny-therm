/*
 * Timer.c
 *
 * Created: 17.07.2020 17:06:38
 *  Author: r.sablin
 */ 
#include "Timer.h"

void (* sInterrupt) (void);

void Timer1Init(void * _interrupt)
{
	TCCR1A = 0x00;
	TCCR1B |= _BV(CS10)|_BV(CS11)|_BV(CS12);    //64
	TIMSK = _BV(OCIE1A);
	OCR1A = 100;
	sInterrupt = _interrupt;
}

ISR(TIMER1_COMPA_vect)
{
	sInterrupt();
	TCNT1 = 0;		// Reset counter cos no CTC
}

