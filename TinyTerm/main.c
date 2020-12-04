/*
 * TinyTerm.cpp
 *
 * Created: 01.07.2020 12:06:07
 * Author : r.sablin
 */ 
#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "C7Segment3.h"
#include "ds18b20.h"

int main(void)
{
	uint8_t temp;
	uint8_t sign;

	C7Segment3Init();

	sei();

	while (1) 
    {
		if(ds18b20_gettemp(&temp, &sign) == 0)
		{
			C7SegmentSet(temp, sign);
			_delay_ms(1000);
		}
		else 
		{
			SetSegmentWait();
		}
    }
}

