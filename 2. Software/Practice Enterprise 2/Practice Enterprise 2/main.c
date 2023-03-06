/*
 * Created: 25/10/2022 11:19:27
 * Author : William VR
 */
/*includes*/
#include "main.h"

#include <avr/io.h>
#include <util/delay.h>

#include "I2C/I2C.h"
#include "TDA/TDA.h"
#include "Display/Display.h"

int main(void)
{
	//IO
	initIO();
	
	//ADC
	initADC();
	
	//I2C
	initI2C();
	
	//display
	initDisplay();
    
	//variables
	uint8_t mux = 0x00;
	uint8_t gain = 0x00;
	
	//shown on display
	uint8_t volume = 0x00;
	uint8_t bass = 0x00;
	uint8_t midRange = 0x00;
	uint8_t treble = 0x00;
	
    while (1) 
    {
		//read inputs
		gain = ReadADCPinValue(0b00001000);		//PB0
		bass = ReadADCPinValue(0b00001011);		//PB3
		midRange = ReadADCPinValue(0b00001100);	//PB4
		treble = ReadADCPinValue(0b00001101);	//PB5
		
		//mux
		
		//Volume -> rotary encoder
		
		//TDA update
		//debug
		setTDAValue(0b10000000, 0b00000000, 0b00000000);
		_delay_ms(10);
		/*
		setTDAValue(CHIP_ADDRESS, SubAdr_Input_selector, mux);
		setTDAValue(CHIP_ADDRESS, SubAdr_Input_gain, gain);
		setTDAValue(CHIP_ADDRESS, SubAdr_Input_Volume, volume);
		setTDAValue(CHIP_ADDRESS, SubAdr_Bass_gain, bass);
		setTDAValue(CHIP_ADDRESS, SubAdr_Mid_range_gain, midRange);
		setTDAValue(CHIP_ADDRESS, SubAdr_Treble_gain, treble);		
		*/

		//Display update -> parallel
		
		// we crash?
		/*
		printl(0b11111111);
		_delay_ms(500);
		printl(0b11111110);
		_delay_ms(500);
		*/
	}
}

//-----------------------------------------------------------------------------------------	IO
void initIO(){
	//PORT A
	DDRA = 0xff;	//output
	PUEA = 0xff;	//Set pull ups
	PORTA = 0xff;	//write zero
	
	//PORT B
	DDRB = 0b11000000;	//output
	PUEB = 0xff;		//Set pull ups
	PORTB = 0b11000000;	//write zero
	
	//PORT C
	DDRC = 0b11000000;	//output
	PUEC = 0xff;		//Set pull ups
	PORTC = 0b11000000;	//write zero
}

//-----------------------------------------------------------------------------------------	ADC
void initADC(){
	PRR		&= 0b11111110;		//turn power saving of
	ADCSRA	|= 0b10000000;		//turn ADEN on
	ADMUXB	&= 0b00000000;		//Voltage reference VCC
	ADCSRA	|= 0b00000100;		//set Scaler 0100 => /16
	ADCSRB	|= 0b00001000;		//left adjusted
}

uint8_t ReadADCPinValue(uint8_t ADCReadPin){
	ADMUXA	|= ADCReadPin;		
	ADMUXB	&= 0b11111110;		
	ADCSRA	|= 0b01000000;		//ADSC on
	return ADCH;
}

//-----------------------------------------------------------------------------------------	MISC
void printl(uint8_t input){
	PORTA = input;
}

//-----------------------------------------------------------------------------------------	back burner
/*
int ADCreadPin = 0b00010000;
int var = 0;
var = ReadADCPinValue(ADCreadPin);
PORTA = var;

PORTB &= ~(1 << PINB7);
PORTB |= (1 << PINB7);

PORTB &= ~(1 << PINB6);
PORTB |= (1 << PINB6);
*/