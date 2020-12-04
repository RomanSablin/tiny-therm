/* 
* C7Segment3.h
*
* Created: 01.07.2020 17:23:37
* Author: r.sablin
*/


#ifndef __C7SEGMENT3_H__
#define __C7SEGMENT3_H__

#include <avr/io.h>

#define	simA  PORTB2
#define	simB  PORTB0
#define simC  PORTB4
#define simD  PORTB6
#define simE  PORTB5
#define simF  PORTB1
#define simG  PORTB3
	
#define simDP PORTA6
#define simDIG1 PORTA2
#define simDIG2 PORTA1
#define simDIG3 PORTA0

enum
{
	mA = (1<<simA),
	mB = (1<<simB),
	mC = (1<<simC),
	mD = (1<<simD),
	mE = (1<<simE),
	mF = (1<<simF),
	mG = (1<<simG)
};

enum
{
	dig_0 = (mA|mB|mC|mD|mE|mF),//  ; 0
	dig_1 = (mB|mC),// ; 1
	dig_2 = (mA|mB|mD|mE|mG),// ; 2
	dig_3 = (mA|mB|mC|mD|mG),// ; 3
	dig_4 = (mB|mC|mF|mG),// ; 4
	dig_5 = (mA|mC|mD|mF|mG),// ; 5
	dig_6 = (mA|mC|mD|mE|mF|mG),// ; 6
	dig_7 = (mA|mB|mC),// ; 7
	dig_8 = (mA|mB|mC|mD|mE|mF|mG),// ; 8
	dig_9 = (mA|mB|mC|mD|mF|mG), // ; 9
	dig_minus = mG,// ; -
	dig_off = 0
};
	
void C7Segment3Init();
void C7Segment3SetDigit(uint8_t _dig);
void C7Segment3Show();
void C7SegmentSet(uint8_t _val, uint8_t _sign);
void SetSegmentWait();

#endif //__C7SEGMENT3_H__
