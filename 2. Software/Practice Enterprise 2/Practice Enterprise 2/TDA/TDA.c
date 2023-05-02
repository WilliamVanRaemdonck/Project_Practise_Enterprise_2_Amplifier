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

uint8_t convert4bits(uint8_t input) {
	uint8_t shift = ((input >> 5) & 0b00000111);
	if(input > 128){
		shift |= 0b00001000;
	}
	return shift;
}

uint8_t convert6bits(uint8_t input) {
	return ((input >> 2) & 0x3F); 
}