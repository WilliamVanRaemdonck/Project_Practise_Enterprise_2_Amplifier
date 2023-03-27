/*
* Display.c
*
* Created: 03/03/2023 12:40:01
*  Author: William VR
*	https://www.farnell.com/datasheets/50586.pdf
*	pg 48
*
*   pin 4 RS		-	PINC6
*   0 instruction register
*   1 data register
*
*   pin 5 always 0
*   0	writing
*
*   pin 6 enable	-	PINC7
*   X/0 enable writing
*   X/1 enable reading
*
*   data sheet X = 1
*/

#include "../main.h"
#include "Display.h"

void initDisplay(void){
	//init
	setRS();
	setEnable();
	PORTA = 0x00;
	//power on delay
	_delay_ms(50);
	
	clearEnable();
	clearRS();
	//send function set 3 times
	sendByte(0b00110000);//Function set
	_delay_ms(5);	//>4.1
	
	sendByte(0b00110000);//Function set
	_delay_ms(2);	//>1.67ms
	
	sendByte(0b00110000);//Function set
	_delay_ms(2);	//>1.67ms
	
	sendByte(0b00100000);//Function set	- 4bit mode
	_delay_ms(2);	//>1.67ms
	
	sendNibble(FUNCTION_SET);	// interface length and Character font	- 4 bit en 5*8
	_delay_ms(2);	//>1.67ms
	
	sendNibble(CURSOR_ON_BLINK);// display on, cursor on, blink on
	_delay_ms(2);	//>1.67ms
	
	sendNibble(CLEAR_DISPLAY);	// clear display
	_delay_ms(2);	//>1.67ms
	
	sendNibble(ENTRY_MODE);		// ready to write
	_delay_ms(2);	//>1.67ms
	
	sendNibble(RETURN_HOME);	// return home
	_delay_ms(2);	//>1.67ms
	
	_delay_ms(1000);
}

void writeToDisplay(uint8_t data){
	cursorHome();
	clearLCD();
	
	sendNibble(0b10000000);		//address = 0 DDRAM
	_delay_ms(2);	//>1.67ms
	
	setRS();
	sendNibble(data);
	clearRS();
		
	_delay_ms(1000);
}

void sendByte(uint8_t data){
	//send data
	PORTA = data;
	
	//pulse enable
	setEnable();
	_delay_us(500);
	clearEnable();
	_delay_us(500);
	
	//default position
	clearEnable();
	clearRS();
	PORTA = 0x00;
}

void sendNibble(uint8_t data){
	//data shift
	uint8_t input = data;
	uint8_t leftNibble = (input & 0b11110000);
	input = (data << 4);
	uint8_t rightNibble = (input & 0b11110000);
	
	//data send
	PORTA = leftNibble;
	
	//pulse
	setEnable();
	_delay_ms(2);
	clearEnable();
	_delay_ms(2);
	
	//data send
	PORTA = rightNibble;
	
	//pulse
	setEnable();
	_delay_ms(2);
	clearEnable();
	_delay_ms(2);
	
	//default position
	clearEnable();
	clearRS();
	PORTA = 0x00;
}

void clearLCD(void){
	sendNibble(CLEAR_DISPLAY);	// clear display
	_delay_ms(2);	//>1.67ms
}

void cursorHome(void){
	sendNibble(RETURN_HOME);	// return home
	_delay_ms(2);	//>1.67ms
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