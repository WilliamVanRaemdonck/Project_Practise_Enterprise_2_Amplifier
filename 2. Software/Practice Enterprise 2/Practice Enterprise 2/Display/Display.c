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
#include <string.h>

//vars
char inpStrText[] = "INPUT:";
char volStrText[] = "VOLUME:   ";

char* muxTable[4] = {
	" [X]  [_]  [_]  [_] ",
	" [_]  [X]  [_]  [_] ",
	" [_]  [_]  [X]  [_] ",
	" [_]  [_]  [_]  [X] "
};

char barStr[20];
char* muxStr;

uint8_t bars = 0;
uint8_t blanks = 0;

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
	
	writeToDisplay(volStrText, strlen(volStrText), 0x80);
	writeToDisplay(inpStrText, strlen(inpStrText), 0x94);
}

void updateDisplay(uint8_t displayValueF, uint8_t mux){
	//1 VOLUME:					address: 0x80
	//2 ??????_______________	address: 0xc0
	//3 INPUT:					address: 0x94
	//4 [] [] [?] []			address: 0xd4

	// volume string
	bars = displayValueF / 12; // 0 - 255 => 0 - 20 => / 12.75 ~ 13
	blanks = 20 - bars;
	//assemble string
	strcpy(barStr, "");
	for(int i = 0; i < bars; i++){
		strcat(barStr, "X");
	}
	for(int i = 0; i < blanks; i++){
		strcat(barStr, "_");
	}
	
	//mux string
	muxStr = muxTable[mux];

	writeToDisplay(barStr, strlen(barStr), 0xc0);
	writeToDisplay(muxStr, strlen(muxStr), 0xd4);
}

void writeToDisplay(char data[], uint8_t length, uint8_t DDRAMaddress){
	uint8_t address = DDRAMaddress;
	cursorHome();
	for(int i = 0; i < length;i++){
		sendNibble(address);		//address = 0 DDRAM
		//_delay_ms(2);	//>1.67ms	//tested not neccesary
		
		setRS();
		sendNibble(data[i]);
		clearRS();
		
		address++;
	}
	
	address = DDRAMaddress;
}

void sendByte(char data){
	//send data
	PORTA = data;
	
	//pulse enable
	setEnable();
	_delay_us(50);
	clearEnable();
	_delay_us(50);
	
	//default position
	clearEnable();
	clearRS();
	PORTA = 0x00;
}

void sendNibble(char data){
	//data shift
	char input = (char)data;
	char leftNibble = (input & 0b11110000);
	input = (data << 4);
	char rightNibble = (input & 0b11110000);
	
	//data send
	PORTA = leftNibble;
	
	//pulse
	setEnable();
	//_delay_ms(2);
	_delay_us(50);
	clearEnable();
	//_delay_ms(2);
	_delay_us(50);
	
	//data send
	PORTA = rightNibble;
	
	//pulse
	setEnable();
	//_delay_ms(2);
	_delay_us(50);
	clearEnable();
	//_delay_ms(2);
	_delay_us(50);
	
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