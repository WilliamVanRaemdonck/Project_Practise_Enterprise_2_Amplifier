/*
* I2C.h
* Created: 09/01/2023 15:32:19
*  Author: William VR
*/


#ifndef I2C_H_
#define I2C_H_

#define I2CSpeed 52		//9600BAUD

/*function declaration*/
void	initI2C();
void	setTDAValue(uint8_t, uint8_t, uint8_t); //chip address, sub address, data
void	sendI2C(uint8_t);
void	finishCom(void);
void	clkLow(void);
void	clkHigh(void);
void	sdaLow(void);
void	sdaHigh(void);

#endif /* I2C_H_ */