/*
 * Main.h
 * Created: 03/03/2023 12:22:17
 *  Author: William VR
 */ 


#ifndef MAIN_H_
#define MAIN_H_

/*defines*/
#define F_CPU 8000000UL
#define CHIP_ADDRESS 0x88

#include <avr/io.h>
#include <util/delay.h>

/*functions*/

//MISC
void	initIO();
void initclk(void);
uint8_t reverse(uint8_t);

//ADC
void	initADC();
uint8_t	ReadADCPinValue(uint8_t);

//TIMER0
void initTimer1(void);



#endif /* MAIN_H_ */