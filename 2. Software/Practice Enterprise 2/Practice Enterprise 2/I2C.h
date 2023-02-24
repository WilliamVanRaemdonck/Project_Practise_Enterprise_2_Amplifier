/*
* I2C.h
*
* Created: 09/01/2023 15:32:19
*  Author: William VR
*/


#ifndef I2C_H_
#define I2C_H_

#define I2CSpeed 50		//9600BAUD

/*function declaration*/
void	initI2C();
void	sendI2C(uint8_t, uint8_t, uint8_t);	//chip address, sub address, data
static void	clkLow(void);
static void	clkHigh(void);
static void	sdaLow(void);
static void	sdaHigh(void);


void initI2C(){
	/*
	PB7	SDA
	PB6	SCL
	DDRB	|= 0b11000000;	//output
	PUEB	|= 0b11000000;	//Set pull ups
	PORTB	|= 0b11000000;	//Default op 1
	*/
	sdaHigh();
	clkHigh();
	_delay_us(I2CSpeed);
}

void sendI2C(uint8_t chipAddress, uint8_t subAddress, uint8_t data){
	uint8_t shift;
	uint8_t mask = 0b00000001;
	uint8_t result = 0;
	
	shift = chipAddress;
	for(int index = 0; index < 8; index++){			//send address
		result = shift & mask;
		clkLow();
		_delay_us(I2CSpeed);
		if(result == 1){
			sdaHigh();
			_delay_us(I2CSpeed);
		}
		else{
			sdaLow();
			_delay_us(I2CSpeed);
		}
		shift = (shift >> 1);	//shift right by one
		clkHigh();
		_delay_us(I2CSpeed);
	}
	sdaHigh();					// Acknowledge
	clkLow();
	_delay_us(I2CSpeed);
	clkHigh();
	_delay_us(I2CSpeed);
	clkLow();
	_delay_us(I2CSpeed);
	clkHigh();
	_delay_us(I2CSpeed);
	
	shift = subAddress;
	for(int index = 0; index < 8; index++){			//send sub address
		result = shift & mask;
		clkLow();
		_delay_us(I2CSpeed);
		if(result == 1){
			sdaHigh();
			_delay_us(I2CSpeed);
		}
		else{
			sdaLow();
			_delay_us(I2CSpeed);
		}
		shift = (shift >> 1); //shift right by one
		clkHigh();
		_delay_us(I2CSpeed);
	}
	sdaHigh();					// Acknowledge
	clkLow();
	_delay_us(I2CSpeed);
	clkHigh();
	_delay_us(I2CSpeed);
	clkLow();
	_delay_us(I2CSpeed);
	clkHigh();
	_delay_us(I2CSpeed);
	
	shift = data;
	for(int index = 0; index < 8; index++){			//send data
		result = shift & mask;
		clkLow();
		_delay_us(I2CSpeed);
		if(result == 1){
			sdaHigh();
			_delay_us(I2CSpeed);
		}
		else{
			sdaLow();
			_delay_us(I2CSpeed);
		}
		shift = (shift >> 1); //shift right by one
		clkHigh();
		_delay_us(I2CSpeed);
	}
	sdaHigh();					// Acknowledge
	clkLow();
	_delay_us(I2CSpeed);
	clkHigh();
	_delay_us(I2CSpeed);
	clkLow();
	_delay_us(I2CSpeed);
	clkHigh();
	_delay_us(I2CSpeed);
	
	sdaHigh();					// finish com
	clkHigh();
}

static void clkLow(void){
	PORTB &= (0<<PORTB7);
}

static void clkHigh(void){
	PORTB |= (1<<PORTB7);
}

static void sdaLow(void){
	PORTB &= (0<<PORTB6);
}

static void sdaHigh(void){
	PORTB |= (1<<PORTB6);
}


#endif /* I2C_H_ */