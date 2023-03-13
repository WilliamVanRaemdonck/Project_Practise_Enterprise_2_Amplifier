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

//x = 1 op pin 0
#define Clear_Display	0b11111110	//1.52ms exec time
#define Return_Home		0b11111101	//1.52ms exec time
#define Entry_Mode_Set	0b11111010	//shift met cursor
#define Display_ON		0b11110000	//display on with cursor
#define Display_OFF		0b11110100	//display off with cursor
#define Cursor_OFF		0b11110011	//display on cursor hidden
#define Cursor_Shift	0b11101011	//shift cursor right
#define	Display_Shift	0b11100011	//shift display right cursor follows
#define Function_set	0b11000111	//8bits, 8x5, 2 lines
#define Set_CGRAM_Address	0b10111111	//DB5<=DB0 = data
#define Set_DDRAM_Address	0b01111111	//DB6<=DB0 = data


void initDisplay(void);
void writeToDisplay(uint8_t, uint8_t);	//address, data
void setRS(void);
void clearRS(void);
void setEnable(void);
void clearEnable(void);



#endif /* DISPLAY_H_ */