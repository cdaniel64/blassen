/*
 * GccApplication2.c
 *
 * Created: 3/24/2019 6:06:20 PM
 * Author : cdani
 */ 

#include <avr/io.h>				//calls out library to be used
#define F_CPU 16000000UL		//defines CPU for future functions
#include<util/delay.h>			//includes library to be used
int overflow = 0;				//declares overflow variable
int main()						//initializes main program
{
	DDRB |= (1<<2); 			//sets port B as output
	PORTB |= (1<<2); 			//command sets LED off
	DDRC &= (0<<2);				//commands port C switch
	PORTC |= (1<<2); 			//enables the port to pull up

	TCCR0A = 0;					//time delay at beginning of main loop
	TCCR0B = (1 << CS02) | (1 << CS00);	//Setting up the pre-scalar 1024
	while (1) {					//initialization of while loop
		if (!(PINC & (1<<PINC1)))		//if statement
		{
			overflow = 0;		//value added to overflow variable
			TCNT0 = 0;			//value added to timer
		}
		while ((TIFR0 & 0x01) == 0);	//if statement continuation
		TCNT0 = 0x00;					//resets the counter
		TIFR0 = 0x01;					//resets the main overflow
		overflow++;						//increases counter in overflow
		if (overflow <= 67)				//if statement to turn on LED
		PORTB = (0 << DDB2);			//port B turning on LED
		else PORTB = (1 << DDB2);		//if statement to turn off LED
	}
}