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
	//Step 0
	_delay_ms(50);
	clearEnable();
	clearRS();
	//1
	setEnable();
	PORTA = Function_set;
	_delay_ms(4.5);	//>4.1ms
	clearEnable();
	//2
	setEnable();
	PORTA = Function_set;
	_delay_ms(4.5);	//>4.1ms
	clearEnable();
	//3
	setEnable();
	PORTA = Function_set;
	_delay_ms(4.5);	//>4.1ms
	clearEnable();
	//4
	setEnable();
	PORTA = Function_set;
	clearEnable();
	
	
	
}

void writeToDisplay(uint8_t address, uint8_t data){
	
	setEnable();
	PORTA = Display_ON;
	_delay_us(50);	//?
	clearEnable();
	
	_delay_ms(1000);
	
	setEnable();
	PORTA = Display_OFF;
	_delay_us(50);	//?
	clearEnable();
	
	_delay_ms(1000);

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