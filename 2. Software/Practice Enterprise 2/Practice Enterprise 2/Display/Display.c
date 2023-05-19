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
*/

#include "../main.h"
#include "Display.h"
#include <string.h>
#include <stdio.h>

//vars
char inpStrText[] = "INPUT:              ";
char volStrText[] = "VOLUME:";

char loadingStrText[] = "       Loading       ";
char emptyStrText[] = "                   ";
char fullbarStrText[] = "YYYYYYYYYYYYYYYYYYY_";

char* muxTable[4] = {
	" [X]  [_]  [_]  [_] ",
	" [_]  [X]  [_]  [_] ",
	" [_]  [_]  [X]  [_] ",
	" [_]  [_]  [_]  [X] "
};

char barStr[20];
char* muxStr;

uint8_t bars = 0x00;
uint8_t blanks = 0x00;
uint8_t first = 0x00;

void initDisplay(void){
	//init
	setRS();
	setEnable();
	PORTA = 0x00;
	//power on delay
	_delay_ms(500);
	
	clearEnable();
	clearRS();
	_delay_ms(500);
	
	//send function set 3 times
	sendByteByNibble(FUNCTION_SET);	//Function set
	_delay_ms(15);	//>4.1
	
	sendByteByNibble(FUNCTION_SET);	//Function set
	_delay_ms(5);	//>1.67ms
	
	sendByteByNibble(FUNCTION_SET);	//Function set
	_delay_ms(2);	//>1.67ms
	
	sendByteByNibble(FUNCTION_SET_4BIT);	//Function set	- 4bit mode
	_delay_ms(2);	//>1.67ms
	
	sendByteByNibble(CURSOR_ON_BLINK);	// display on, cursor on, blink on
	_delay_ms(2);	//>1.67ms
	
	sendByteByNibble(ENTRY_MODE);	// ready to write
	_delay_ms(2);	//>1.67ms
	
	sendByteByNibble(RETURN_HOME);	// return home
	_delay_ms(2);	//>1.67ms
	
	clearLCD();
}

void updateDisplay(uint8_t displayValueF, uint8_t mux){
	//1 VOLUME:					address: 0x80
	//2 ??????_______________	address: 0xc0
	//3 INPUT:					address: 0x94
	//4 [] [] [?] []			address: 0xd4

	// volume string
	bars = displayValueF / 12; // 0 - 255 => 0 - 20 => / 12.75 ~ 12
	blanks = 20 - bars;
	
	//assemble string
	strcpy(barStr, "");
	for(uint8_t i = 0; i < bars; i++){
		strcat(barStr, "Y");
	}
	for(uint8_t i = 0; i < blanks; i++){
		strcat(barStr, "_");
	}
	
	//mux string
	muxStr = muxTable[mux];
	
	cursorHome();
	
	writeToDisplay(volStrText, strlen(volStrText), 0x80);
	
	// write entire bar on reset
	if(first == 0x00){
		writeToDisplay(fullbarStrText, strlen(fullbarStrText), 0xc0);
		first = 0x01;
	}
	else{
		writeToDisplay(barStr, strlen(barStr), 0xc0);
	}
	
	writeToDisplay(inpStrText, strlen(inpStrText), 0x94);
	writeToDisplay(muxStr, strlen(muxStr), 0xd4);
}

void writeToDisplay(char data[], uint8_t length, uint8_t DDRAMaddress){
	uint8_t addressF = DDRAMaddress;
	
	cursorHome();
	
	for(int i = 0; i < length;i++){
		sendByteByNibble(addressF);
		_delay_us(2);
		
		setRS();
		sendByteByNibble(data[i]);
		clearRS();
		
		addressF++;
	}
}

void loadingScreen(void){	
	sendByteByNibble(CURSOR_ON_BLINK);	// display on, cursor on, blink on
	_delay_ms(2);	//>1.67ms
	
	sendByteByNibble(ENTRY_MODE);		// ready to write
	_delay_ms(2);	//>1.67ms
	
	clearLCD();
	cursorHome();
	
	writeToDisplay(emptyStrText, strlen(emptyStrText), 0x80);
	writeToDisplay(loadingStrText, strlen(loadingStrText), 0xc0);
	
	for(uint8_t i = 0; i < 20; i++){
		strcat(barStr, "Y");
		writeToDisplay(barStr, strlen(barStr), 0x94);
	}
	
	clearLCD();
	cursorHome();
}

void sendByteByNibble(char data){
	uint8_t input = 0x00;
	uint8_t leftNibble = 0x00;
	uint8_t rightNibble = 0x00;
	uint8_t mask = 0b11110000;
	
	//X to blok
	if(data == 0b01011001){		//data == Y ?
		leftNibble = 0xFF;
		rightNibble = 0xFF;
	}
	else{
		input = (uint8_t)data;
		
		leftNibble = (input & mask);
		
		input = (((uint8_t)data) << 4);
		rightNibble = (input & mask);
	}
	
	//data send
	PORTA = leftNibble;
	
	//pulse
	setEnable();
	_delay_us(50);
	clearEnable();
	_delay_us(50);
	
	//data send
	PORTA = rightNibble;
	
	//pulse
	setEnable();
	_delay_us(50);
	clearEnable();
	_delay_us(50);
	
	//default position
	clearEnable();
	clearRS();
	PORTA = 0x00;
}

void clearLCD(void){
	sendByteByNibble(CLEAR_DISPLAY);	// clear display
	_delay_ms(2);	//>1.67ms
}

void cursorHome(void){
	sendByteByNibble(RETURN_HOME);	// return home
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