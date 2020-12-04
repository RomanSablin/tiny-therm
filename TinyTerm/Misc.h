/*
 * Misc.h
 *
 * Created: 17.07.2020 17:22:48
 *  Author: r.sablin
 */ 


#ifndef MISC_H_
#define MISC_H_

#include <stdint.h>

uint8_t div10(const uint8_t _val);
void Char2Bcd(const uint8_t _val, uint8_t * _buf);
uint8_t Crc8(uint8_t * _buf, uint8_t _size);


#endif /* MISC_H_ */