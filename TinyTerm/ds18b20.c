/*
ds18b20 lib 0x02

copyright (c) Davide Gironi, 2012

Released under GPLv3.
Please refer to LICENSE file for licensing information.
*/
#define F_CPU 1000000UL

#include <math.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "ds18b20.h"

#include "Misc.h"

/*
 * ds18b20 init
 */
uint8_t ds18b20_reset() {
	uint8_t i;

	//low for 480us
	DS18B20_PORT &= ~ (1<<DS18B20_DQ); //low
	DS18B20_DDR |= (1<<DS18B20_DQ); //output
	_delay_us(480);

	//release line and wait for 60uS
	DS18B20_DDR &= ~(1<<DS18B20_DQ); //input
	_delay_us(70);

	//get value and wait 420us
	i = (DS18B20_PIN & (1<<DS18B20_DQ));
	_delay_us(410);

	DS18B20_PORT |= (1<<DS18B20_DQ);

	//return the read value, 0=ok, 1=error
	return i;
}

/*
 * write one bit
 */
void ds18b20_writebit(uint8_t bit){
	cli();
	//low for 1uS
	DS18B20_PORT &= ~ (1<<DS18B20_DQ); //low
	if(bit)
		_delay_us(10);
	else
		_delay_us(55);

	DS18B20_PORT |= (1<<DS18B20_DQ);
	sei();
	
	if(bit)
		_delay_us(65);
	else
		_delay_us(5);
}

/*
 * read one bit
 */
uint8_t ds18b20_readbit(void){
	uint8_t bit=0;
	cli();
	DS18B20_PORT &= ~ (1<<DS18B20_DQ); //low
	DS18B20_DDR |= (1<<DS18B20_DQ); //output
	_delay_us(1);

	//release line and wait for 14uS
	DS18B20_DDR &= ~(1<<DS18B20_DQ); //input
	_delay_us(14);

	//read the value
	if(DS18B20_PIN & (1<<DS18B20_DQ))
		bit=1;
	
	sei();
	//wait 45uS and return read value
	_delay_us(45);
	DS18B20_PORT |= (1<<DS18B20_DQ); //
	return bit;
}

/*
 * write one byte
 */
//void ds18b20_writebyte(uint8_t byte){
//	uint8_t i=8;
//	while(i--){
//		ds18b20_writebit(byte&1);
//		byte >>= 1;
//	}
//}

/*
 * read one byte
 */
//uint8_t ds18b20_readbyte(void){
//	uint8_t i=8, n=0;
//	while(i--){
//		n >>= 1;
//		n |= (ds18b20_readbit()<<7);
//	}
//	return n;
//}
void ds18b20_writebyte(uint8_t value)
{
	uint8_t bitMask;
	DS18B20_DDR |= (1<<DS18B20_DQ); //output
	for (bitMask = 0x01; bitMask; bitMask <<= 1)
		ds18b20_writebit((bitMask & value)?1:0);
}

uint8_t ds18b20_readbyte()
{
	uint8_t r = 0;
//	mGpioPort->OTYPER |= mGpioPin; // opendrain on
	for (uint8_t bitMask = 0x01; bitMask; bitMask <<= 1)
	{
		if ( ds18b20_readbit())
		r |= bitMask;
	}
	return r;
}

/*
 * get temperature
 * return 0 - Ok, 1 - error
 */
uint8_t ds18b20_gettemp(uint8_t *_temperature, uint8_t *_sign) 
{
	static uint8_t powerOn = 1;
	uint8_t temperature_l;
	uint8_t temperature_h;
	uint8_t retd = 0;

	if(ds18b20_reset() != 0) //reset
		return 1;
		
	ds18b20_writebyte(DS18B20_CMD_SKIPROM); //skip ROM
	ds18b20_writebyte(DS18B20_CMD_CONVERTTEMP); //start temperature conversion

	_delay_ms(800);
//	while(!ds18b20_readbit()); //wait until conversion is complete

	if(ds18b20_reset() != 0) //reset
		return 1;
		
	ds18b20_writebyte(DS18B20_CMD_SKIPROM); //skip ROM
	ds18b20_writebyte(DS18B20_CMD_RSCRATCHPAD); //read scratchpad

	uint8_t data[9];
	for(int i=0; i<9; i++)
		data[i] = ds18b20_readbyte();

	if(Crc8(data, 8) == data[8])
	{
		uint16_t raw;
		raw = data[1];
		raw <<= 8;
		raw &= 0xff00;
		raw |= data[0];
		
		if(raw < 0x7d0)
		{
			raw >>= 4;
			*_sign = 0;
			*_temperature = (uint8_t) raw & 0x7f;
		}
		else 
		{
			raw = ((uint16_t)0xffff-(uint16_t)raw)>>4; 
			*_sign = 1;
			int8_t st = (int8_t) raw & 0xff;
			*_temperature = abs(st);
		}

		return 0;
	}

	return 1;
}

