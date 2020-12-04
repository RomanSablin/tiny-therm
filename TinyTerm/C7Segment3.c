/* 
* C7Segment3.cpp
*
* Created: 01.07.2020 17:23:37
* Author: r.sablin
*/

#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "C7Segment3.h"
#include "Timer.h"
#include "Misc.h"

#define TABLE_SIZE 12
volatile uint8_t mMemory[3];
volatile uint8_t mCur;

static const uint8_t mDigitTable[TABLE_SIZE] = {dig_0, dig_1, dig_2, dig_3, dig_4, dig_5, dig_6, dig_7, dig_8, dig_9, dig_minus, dig_off};

void C7Segment3Init()
{
	mCur = 0;
	mMemory[0] = 0;
	mMemory[1] = 0;
	mMemory[2] = 0;
	DDRB = 0x7f;
	PORTB = 0;
	DDRA |= ((1<<simDP)|(1<<simDIG1)|(1<<simDIG2)|(1<<simDIG3));
	PORTA |= ((1<<simDIG1)|(1<<simDIG2)|(1<<simDIG3));
	Timer1Init(C7Segment3Show);
}

void C7SegmentSet(uint8_t _val, uint8_t _sign)
{
	Char2Bcd(_val, mMemory);
	if(_sign != 0)
		mMemory[0] = 10;
}

void C7Segment3SetDigit(uint8_t _dig)
{
	if(_dig < TABLE_SIZE)
		PORTB = mDigitTable[_dig];
	else
		PORTB = mDigitTable[10];
}

void C7Segment3Show()
{
	switch (mCur)
	{
		case 0:
			mCur=1;
			PORTA |= (1<<simDIG3);
			C7Segment3SetDigit(mMemory[0]);
			PORTA &= ~(1<<simDIG1);
		break;
		case 1:
			mCur=2;
			PORTA |= (1<<simDIG1);
			C7Segment3SetDigit(mMemory[1]);
			PORTA &= ~(1<<simDIG2);
		break;
		default: 
			mCur=0;
			PORTA |= (1<<simDIG2);
			C7Segment3SetDigit(mMemory[2]);
			PORTA &= ~(1<<simDIG3);
		break;
	}
}

void SetSegmentWait()
{
	mMemory[0] = 11;
	mMemory[1] = 11;
	mMemory[2] = 11;
	_delay_ms(200);
	mMemory[0] = 10;
	mMemory[1] = 11;
	mMemory[2] = 11;
	_delay_ms(200);
	mMemory[0] = 11;
	mMemory[1] = 10;
	mMemory[2] = 11;
	_delay_ms(200);
	mMemory[0] = 11;
	mMemory[1] = 11;
	mMemory[2] = 10;
	_delay_ms(200);
}