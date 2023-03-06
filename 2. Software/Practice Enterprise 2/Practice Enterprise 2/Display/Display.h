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
//PA8 pin 4 PC6
//PA9 pin 6 PC7

void initDisplay(void);
void setRS(void);
void clearRS(void);
void setEnable(void);
void clearEnable(void);



#endif /* DISPLAY_H_ */