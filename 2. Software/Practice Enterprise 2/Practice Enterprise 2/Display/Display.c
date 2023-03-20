/*
* Display.c
*
* Created: 03/03/2023 12:40:01
*  Author: William VR
*	https://www.farnell.com/datasheets/50586.pdf
*	pg 48

pin 4 RS	-	PINC6
0 instruction register
1 data register

pin 5 always 0
0	writing

pin 6 enable	-	PINC7
X/0 enable writing
X/1 enable reading


data sheet X = 1
*/

#include "../main.h"
#include "Display.h"

void initDisplay(void){
	//power on delay
	_delay_ms(50);
	//set to 0
	clearRS();
	clearEnable();
	sendNibble(0xff);
	
	//send function set times
	clearRS();
	clearEnable();
	sendNibble(0b11001111);//Function set 
	
	//send function set 2 times
	for(uint8_t functionCounter = 0; functionCounter < 2; functionCounter++){
		sendNibble(0b11011111);//Function set 
		sendNibble(0b10111111);//Function set 
	}
	
	sendNibble(0b11111111);//display on/off
	sendNibble(0b01111111);
	
	sendNibble(0b11111111);//display clear
	sendNibble(0b11101111);
	
	sendNibble(0b11111111);//Entry mode set
	sendNibble(0b10011111);
}

void writeToDisplay(uint8_t data){	
	
}

void sendNibble(uint8_t data){
	setEnable();
	_delay_us(50);
	PORTA = data;
	_delay_us(50);
	clearEnable();
	_delay_us(50);
}

void setRS(void){
	PORTC |= 0b01000000;
}

void clearRS(void){
	PORTC &= 0b10111111;
}

void setEnable(void){
	PORTC |= 0b10000000;
}

void clearEnable(void){
	PORTC &= 0b01111111;
}