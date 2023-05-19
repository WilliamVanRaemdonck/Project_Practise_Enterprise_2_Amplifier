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
//PA8 pin - 4 PC6	RS
//PA9 pin  -6 PC7 ENABLE
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

#define FUNCTION_SET		0b00110000
#define FUNCTION_SET_4BIT	0b00100000

#define CURSOR_ON_BLINK	0b00001100
#define DISPLAY_ON		0b00000001
#define ENTRY_MODE		0b00000110

#define RETURN_HOME		0b00000010
#define CLEAR_DISPLAY	0b00000001

#define SET_DDRAM_ADDRESS	0b10000000

void initDisplay(void);
void updateDisplay(uint8_t, uint8_t);
void writeToDisplay(char[], uint8_t, uint8_t); // data; length, DDRAM address
void loadingScreen(void);

void clearLCD(void);
void cursorHome(void);

void sendByteByNibble(char);

void setRS(void);
void clearRS(void);
void setEnable(void);
void clearEnable(void);



#endif /* DISPLAY_H_ */