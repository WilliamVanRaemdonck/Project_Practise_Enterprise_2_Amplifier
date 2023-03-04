/*
 * I2C.c
 *
 * Created: 03/03/2023 12:39:19
 *  Author: William VR
 */ 

#include "../main.h"
#include "I2C.h"

void initI2C(){
	/*
	PB7	SDA
	PB6	SCL
	*/
	sdaHigh();
	clkHigh();
	_delay_us(I2CSpeed);
}

void setTDAValue(uint8_t chipAddress, uint8_t subAddress, uint8_t data){
	sendI2C(chipAddress);
	sendI2C(subAddress);
	sendI2C(data);
	finishCom();
}

void sendI2C(uint8_t input){
	uint8_t shift = 0x00;
	uint8_t mask = 0b00000001;
	uint8_t result = 0x00;
	
	//start condition
	sdaLow();
	_delay_us(I2CSpeed);
	clkLow();
	_delay_us(I2CSpeed);
	
	//send input
	shift = input;
	for(uint8_t index = 0; index < 8; index++){
		result = shift & mask;
		if(result == 0x01){
			sdaHigh();
		}
		else{
			sdaLow();
		}
		clkHigh();
		_delay_us(I2CSpeed);
		clkLow();
		_delay_us(I2CSpeed);
		shift = (shift >> 1);	//shift right by one
	}
	//ACK
	sdaLow();
	clkHigh();
	_delay_us(I2CSpeed);
	clkLow();
	_delay_us(I2CSpeed);
	sdaHigh();
	_delay_us(I2CSpeed);

}

void finishCom(){
	// finish communication
	sdaHigh();
	clkHigh();
	_delay_us(I2CSpeed);
}

static void clkLow(void){
	PORTB &= 0b10111111;
}

static void clkHigh(void){
	PORTB |= 0b01000000;
}

static void sdaLow(void){
	PORTB &= 0b01111111;
}

static void sdaHigh(void){
	PORTB |= 0b10000000;
}

