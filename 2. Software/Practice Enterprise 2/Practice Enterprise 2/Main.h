/*
 * Main.h
 *
 * Created: 03/03/2023 12:22:17
 *  Author: William VR
 */ 


#ifndef MAIN_H_
#define MAIN_H_

/*defines*/
#define F_CPU 1000000UL
#define CHIP_ADDRESS 0x88

#include <avr/io.h>
#include <util/delay.h>


/*typedefs*/
typedef unsigned char uint8_t;

/*functions*/

//MISC
void	initIO();
void	printl(uint8_t);

//ADC
void	initADC();
uint8_t	ReadADCPinValue(uint8_t);





#endif /* MAIN_H_ */