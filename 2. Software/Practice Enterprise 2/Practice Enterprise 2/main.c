/*
* Created: 25/10/2022 11:19:27
* Author : William VR
*/
/*includes*/
#include "main.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "I2C/I2C.h"
#include "TDA/TDA.h"
#include "Display/Display.h"
#include "EEPROM/EEPROM.h"

//variables
uint8_t mux = 0x00;
uint8_t muxInput = 0x00;

//input values
uint8_t volume = 0x00;
uint8_t gain = 0x00;
uint8_t bass = 0x00;
uint8_t midRange = 0x00;
uint8_t treble = 0x00;

int main(void)
{
	//IO
	initIO();
	
	//ADC
	initADC();
	
	//TIMER1
	initTimer1();

	//I2C
	initI2C();
	
	//DISPLAY
	initDisplay();
	
	//ENABLE INTERRUPTS
	sei();
	
	//read volume value
	volume = EEPROM_read(0x00);
	
	//unmute TDA IC
	setTDAValue(CHIP_ADDRESS, SubAdr_Speaker_attenuation_L, 0x00);
	setTDAValue(CHIP_ADDRESS, SubAdr_Speaker_attenuation_R, 0x00);
	setTDAValue(CHIP_ADDRESS, SubAdr_Volume, volume);
	
	while (1)
	{
		//read inputs
		gain = ReadADCPinValue(0b00001000);		//PB0
		bass = ReadADCPinValue(0b00001011);		//PB3
		midRange = ReadADCPinValue(0b00001100);	//PB4
		treble = ReadADCPinValue(0b00001101);	//PB5
		
		//mux PC2 PC3 PC4 PC5
		muxInput = PORTC;
		muxInput = muxInput & 0b00111100;
		switch (muxInput)
		{
			case 0b00000100:
			mux = 0x00;
			break;
			case 0b00001000:
			mux = 0x01;
			break;
			case 0b00010000:
			mux = 0x02;
			break;
			case 0b00100000:
			mux = 0x03;
			break;
		}
		
		//TDA update
		setTDAValue(CHIP_ADDRESS, SubAdr_Input_selector, mux);
		setTDAValue(CHIP_ADDRESS, SubAdr_Input_gain, gain);
		setTDAValue(CHIP_ADDRESS, SubAdr_Volume, volume);
		setTDAValue(CHIP_ADDRESS, SubAdr_Bass_gain, bass);
		setTDAValue(CHIP_ADDRESS, SubAdr_Mid_range_gain, midRange);
		setTDAValue(CHIP_ADDRESS, SubAdr_Treble_gain, treble);
		
		//Display update -> parallel
		updateDisplay(volume, mux);
		/*
		for(int i = 0; i < 4;i++){
		updateDisplay((i*50), i);
		}*/
	
		//write to EEPROM
		EEPROM_write(0x00, volume);
	}
}

//-----------------------------------------------------------------------------------------	IO
void initIO(){
	//PORT A
	DDRA = 0b11111111;	//output
	PUEA = 0xff;		//Set pull ups
	PORTA = 0x00;		//write zero
	
	//PORT B
	DDRB = 0b11000000;	//output
	PUEB = 0xff;		//Set pull ups
	PORTB = 0b00000000;	//write zero
	
	//PORT C
	DDRC = 0b11000000;	//output
	PUEC = 0xff;		//Set pull ups
	PORTC = 0b00000000;	//write zero
}

//-----------------------------------------------------------------------------------------	ADC
void initADC(){
	PRR		&= 0b11111110;		//turn power saving off
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

//-----------------------------------------------------------------------------------------	Timer1
void initTimer1(void){
	PRR	   &= 0b11110111;						//power saving off

	TCCR1B |= (1 << WGM12 );					// Configure timer 1 for CTC mode
	OCR1A = 0;
	
	TIMSK1 |= (1 << OCIE1A );					// Enable CTC interrupt
	TCCR1B |= (1 << CS11 ) | (1 << CS10 );		// prescaler of 64
	//TCCR1B |= (1 << CS00 );						// no prescaling
}

//-----------------------------------------------------------------------------------------	MISC
uint8_t reverse(uint8_t b) {
	b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
	b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
	b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
	return b;
}

//-----------------------------------------------------------------------------------------	ISR
ISR(TIMER1_COMPA_vect, ISR_BLOCK){
	static uint8_t volumeSwitchState = 0x00;
	/*
	0x00	READ
	0x01	IDLE	*/

	// Toggle LED on each interrupt
	PORTA ^= 1 << PORTA1;

	//rotary encoder
	switch (volumeSwitchState)
	{
		case 0x00:
		if(PINB & (1<<PINB2)){					// clk = 0?
			if(PINB & (1<<PINB1)){				// data = 0
				if(volume <= 250){
					volume += 5;
				}
				volumeSwitchState = 0x01;
			}
			else{								// data = 1
				if(volume >= 5){
					volume -= 5;
				}
				volumeSwitchState = 0x01;
			}
		}
		break;
		
		case 0x01:
		if(PINB & (1<<PINB2)){					// clk still low?
			//stay
		}
		else{
			volumeSwitchState = 0x00;
		}
		break;
	}
	
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