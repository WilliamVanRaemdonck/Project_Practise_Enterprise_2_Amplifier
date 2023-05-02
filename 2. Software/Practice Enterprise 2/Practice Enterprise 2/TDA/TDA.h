/*
 * TDA.h
 *	Created: 15/12/2022 16:21:01
 *  Author: William VR
 */ 


#ifndef TDA_H_
#define TDA_H_

#define CHIP_ADDRESS 0x88

//Sub-address byte
#define SubAdr_Input_selector		0x00
#define SubAdr_Input_gain			0x01
#define SubAdr_Volume				0x02
#define SubAdr_Bass_gain			0x03
#define SubAdr_Mid_range_gain		0x04
#define SubAdr_Treble_gain			0x05
#define SubAdr_Speaker_attenuation_L 0x06
#define SubAdr_Speaker_attenuation_R 0x07
#define AUTO_INCREMENT				0b00001000

uint8_t convert4bits(uint8_t);
uint8_t convert6bits(uint8_t);

#endif /* TDA_H_ */