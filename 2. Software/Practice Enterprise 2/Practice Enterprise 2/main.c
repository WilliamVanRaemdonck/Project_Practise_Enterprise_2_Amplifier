/*
* Created: 25/10/2022 11:19:27
* Author : William VR
*/
/*includes*/
#include "main.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <math.h>

#include "I2C/I2C.h"
#include "TDA/TDA.h"
#include "Display/Display.h"
#include "EEPROM/EEPROM.h"



//variables	input values
volatile uint8_t volume = 0x00;
uint8_t mux = 0x00;
uint8_t gain = 0x00;
uint8_t bass = 0x00;
uint8_t midRange = 0x00;
uint8_t treble = 0x00;
uint8_t muteLeds = 0x00;

int main(void)
{	
	//watch dog timer
	//wdt_enable(WDTO_2S);
	//wdt_reset(); // optional, to prevent the watchdog from triggering an unwanted reset
	//WDTCSR |= (1 << WDE) | (1 << WDCE);
	//WDTCSR = (1 << RESET);
	
	initclk();
	
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
	setTDAValue(CHIP_ADDRESS, SubAdr_Volume, 0x00);
		
	while (1)
	{
		//read inputs
		gain = ReadADCPinValue(0b00001000);			//PB0
		bass = ReadADCPinValue(0b00001011);			//PB3
		midRange = ReadADCPinValue(0b00001100);		//PB4
		treble = 255 - ReadADCPinValue(0b00001101);	//PB5
		
		//mux PC2 PC3 PC4 PC5
		mux = 0;
		if (PINC & (1 << PINC3)) mux = 1;
		if (PINC & (1 << PINC4)) mux = 2;
		if (PINC & (1 << PINC5)) mux = 3;
		
		//TDA update
		setTDAValue(CHIP_ADDRESS, SubAdr_Input_selector, mux);
		setTDAValue(CHIP_ADDRESS, SubAdr_Input_gain, ((gain >> 4) & 0b00001111));
		setTDAValue(CHIP_ADDRESS, SubAdr_Volume, convert6bits((255 - volume)));
		setTDAValue(CHIP_ADDRESS, SubAdr_Bass_gain,  convert4bits(bass));
		setTDAValue(CHIP_ADDRESS, SubAdr_Mid_range_gain,  convert4bits(midRange));
		setTDAValue(CHIP_ADDRESS, SubAdr_Treble_gain,  convert4bits(treble));
			
		//Display update -> parallel
		updateDisplay(volume, mux);
		
		//write to EEPROM
		EEPROM_write(0x00, volume);
	}
}

//-----------------------------------------------------------------------------------------	IO
void initIO(){
	//PORT A
	DDRA = 0xff;	//output			
	PUEA = 0xff;	//Set pull ups
	PORTA = 0x00;	//write zero
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
	ADMUXA = ADCReadPin;
	
	// Start ADC conversion
	ADCSRA |= (1 << ADSC);
	
	// Wait for ADC conversion to complete
	while (ADCSRA & (1 << ADSC));
	
	// Return ADC result
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
// src: https://stackoverflow.com/questions/2602823/in-c-c-whats-the-simplest-way-to-reverse-the-order-of-bits-in-a-byte
uint8_t reverse(uint8_t b) {
	b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
	b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
	b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
	return b;
}

void initclk(void){
	//Set prescaler to 1;
	CCP	= 0xD8;		
	CLKPR = 0x00;
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
				if(volume >= 5){
					volume -= 5;
				}
				volumeSwitchState = 0x01;
			}
			else{								// data = 1
				if(volume <= 250){
					volume += 5;
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

DDRB = 0xff
_delay_ms(500);
PORTB &= 0xff
_delay_ms(500);
PORTB |= 0xff

PORTB &= ~(1 << PINB6);
PORTB |= (1 << PINB6);
*/