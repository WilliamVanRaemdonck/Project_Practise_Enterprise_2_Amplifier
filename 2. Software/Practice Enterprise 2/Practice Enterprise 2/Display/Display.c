/*
* Display.c
*
* Created: 03/03/2023 12:40:01
*  Author: William VR
*	https://cdn-shop.adafruit.com/datasheets/TC2004A-01.pdf
*	file:///G:/Mijn%20Drive/1.%20Thomas%20More/2021-2022/1.%20Embedded%20Hardware/sem%201/Embedded%20Fundementals/XC888/datasheets/GDM2004d.pdf
*	https://www.farnell.com/datasheets/50586.pdf

pin 4 RS	-	PINC6
0 instruction register
1 data register

pin 5 always 0
0	writing

pin 6 enable	-	PINC7
X/0 enable writing
X/1 enable reading
*/

#include "../main.h"
#include "Display.h"

//from https://www.farnell.com/datasheets/50586.pdf
void initDisplay(void){
	//init
	clearRS();
	clearEnable();
	
	//Wait time >40ms
	_delay_ms(50);
	
	//first time function set
	setEnable();
	setRS();
	_delay_us(Tsu1);
	PORTA = Function_set;
	_delay_us(Tsu2);
	clearEnable();
	_delay_us(Th2);
	
	//Wait time >4.1ms
	_delay_ms(5);
	
	//second time function set
	setEnable();
	_delay_us(Tsu1);
	PORTA = Function_set;
	_delay_us(Tsu2);
	clearEnable();
	_delay_us(Th2);
	
	//Wait time >100us
	_delay_us(250);
	
	//third time function set
	setEnable();
	_delay_us(Tsu1);
	PORTA = Function_set;
	_delay_us(Tsu2);
	clearEnable();
	_delay_us(Th2);
	
	//Wait time >39us
	_delay_us(250);
	
	//display on
	setEnable();
	_delay_us(Tsu1);
	PORTA = Display_Control;
	_delay_us(Tsu2);
	clearEnable();
	_delay_us(Th2);
	
	//Wait time >39us
	_delay_us(250);
	
	//display on
	setEnable();
	_delay_us(Tsu1);
	PORTA = Clear_Display;
	_delay_us(Tsu2);
	clearEnable();
	_delay_us(Th2);
	
	//Wait time >1.52ms
	_delay_ms(2);
	
	//entry mode set
	setEnable();
	_delay_us(Tsu1);
	PORTA = Entry_Mode_Set;
	_delay_us(Tsu2);
	clearEnable();
	clearRS();
	_delay_us(Th2);
	
	//Wait time >39us
	_delay_us(250);
}

void writeToDisplay(uint8_t address, uint8_t data){
	//write to DDRam
	setEnable();
	_delay_us(Tsu1);
	PORTA = Set_DDRAM_Address;
	_delay_us(Tsu2);
	clearEnable();
	_delay_us(Th2);
	
	//Wait time >39us
	_delay_us(250);
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