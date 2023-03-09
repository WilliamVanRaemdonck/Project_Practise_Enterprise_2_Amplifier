/*
* Display.c
*
* Created: 03/03/2023 12:40:01
*  Author: William VR
*	https://cdn-shop.adafruit.com/datasheets/TC2004A-01.pdf
*	file:///G:/Mijn%20Drive/1.%20Thomas%20More/2021-2022/1.%20Embedded%20Hardware/sem%201/Embedded%20Fundementals/XC888/datasheets/GDM2004d.pdf

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

void initDisplay(void){
	//Step 0
	clearEnable();
	clearRS();
	_delay_ms(15);
	
	//step 1
	setEnable();
	PORTA = 0b11001111;
	_delay_us(100);
	clearEnable();
	
	_delay_ms(5); //>4.1ms
	
	
	//step 2
	setEnable();
	PORTA = 0b11001111;
	_delay_us(100);
	clearEnable();
	
	_delay_us(150); //> 100us
	

	//step 3
	setEnable();
	PORTA = 0b11001111;
	_delay_us(100);
	clearEnable();
	
	_delay_us(150); //> 100us
	
	
	//step 4
	setEnable();
	PORTA = 0b11000111;	//function set
	_delay_us(100);
	clearEnable();

	setEnable();
	PORTA = 0b11110111;	//display off
	_delay_us(100);
	clearEnable();

	setEnable();
	PORTA = 0b11111110;	//display clear
	_delay_us(100);
	clearEnable();

	setEnable();
	PORTA = 0b11111000;	//entry mode set
	_delay_us(100);
	clearEnable();





	_delay_ms(5000);
	//of the books
	setEnable();
	PORTA = 0b11110000;	//display on
	_delay_us(150);
	clearEnable();
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