/*
 * Misc.c
 *
 * Created: 17.07.2020 17:22:36
 *  Author: r.sablin
 */ 

#include "Misc.h"

uint8_t div10(const uint8_t _val)
{
	static const uint16_t iDivisor = 0x19A; //load 1/10 * 2^12
	uint16_t ml = (iDivisor * _val);
	return (uint8_t) (ml >> 12);
}

void Char2Bcd(const uint8_t _val, uint8_t * _buf)
{
	uint16_t temp, temp2;
	temp = div10(_val);
	_buf[2] = (uint8_t)(_val - (temp * 10));
	temp2 = div10(temp);
	_buf[1] = (uint8_t)(temp - (temp2 * 10));
	temp = div10(temp2);
	_buf[0] = (uint8_t)(temp2 - (temp * 10));
}

uint8_t Crc8(uint8_t * _buf, uint8_t _size) 
{
    uint8_t crc = 0;

    while (_size--)
    {
	    uint8_t inbyte = *_buf++;
	    for (uint8_t i = 8; i; i--)
	    {
		    uint8_t mix = (crc ^ inbyte) & 0x01;
		    crc >>= 1;
		    if (mix) crc ^= 0x8C;
		    inbyte >>= 1;
	    }
    }
    return crc;
}
