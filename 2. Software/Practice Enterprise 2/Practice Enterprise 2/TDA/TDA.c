/*
 * TDA.c
 * Created: 03/03/2023 12:39:33
 *  Author: William VR
 */ 

#include "../main.h"
#include "TDA.h"

/*
input = 0 -> 255

1111 1111 => XXXX 1111 shift 4
1111 1111 => XX11 1111 shift 6

*/

uint8_t steps[15] = {	0b00000000,
						0b00000001,
						0b00000010,
						0b00000011,
						0b00000100,
						0b00000101,
						0b00000110,
						0b00000111,
						0b00001110,
						0b00001101,
						0b00001100,
						0b00001011,
						0b00001010,
						0b00001001,
						0b00001000};
						

uint8_t convert4bits(uint8_t input) {
	uint8_t shift = ((input >> 4) & 0b00001111);
	
	return steps[shift] ;
}

uint8_t convert6bits(uint8_t input) {
	uint8_t shift = ((input >> 2) & 0x3F);
	
	if(shift > 0b00101000){
		shift = 0b00111000;	//mute
	}
	
	return shift; 
}