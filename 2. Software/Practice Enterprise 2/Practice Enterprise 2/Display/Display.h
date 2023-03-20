/*
 * display.h
 *
 * Created: 03/03/2023 12:40:13
 *  Author: William VR
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_

//port A DATA pins
//port C
//PA8 pin 4 PC6	RS
//PA9 pin 6 PC7 ENABLE
/*
	1. Display clear

	2. Function set:
	DL = 1; 8-bit interface data
	N = 0; 1-line display
	F = 0; 5x8 dot character font

	3. Display on/off control:
	D = 0; Display off
	C = 0; Cursor off
	B = 0; Blinking off

	4. Entry mode set:
	I/D = 1; Increment by 1
	S = 0; No shift
*/
void initDisplay(void);
void writeToDisplay(uint8_t);
void sendNibble(uint8_t);

void setRS(void);
void clearRS(void);
void setEnable(void);
void clearEnable(void);



#endif /* DISPLAY_H_ */