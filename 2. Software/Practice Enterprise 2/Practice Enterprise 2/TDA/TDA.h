/*
 * TDA.h
 *
 * Created: 15/12/2022 16:21:01
 *  Author: William VR
 */ 


#ifndef TDA_H_
#define TDA_H_

#define CHIP_ADDRESS 0x88

//Sub-address byte
#define SubAdr_Input_selector		0b00000000
#define SubAdr_Input_gain			0b00000001
#define SubAdr_Volume				0b00000010
#define SubAdr_Bass_gain			0b00000011
#define SubAdr_Mid_range_gain		0b00000100
#define SubAdr_Treble_gain			0b00000101
#define SubAdr_Speaker_attenuation_L 0b00000110
#define SubAdr_Speaker_attenuation_R 0b00000111
#define AUTO_INCREMENT				0b00001000

uint8_t convert4bits(uint8_t);
uint8_t convert6bits(uint8_t);

#endif /* TDA_H_ */