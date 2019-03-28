/*
 * GccApplication1.c
 *
 * Created: 3/24/2019 5:54:14 PM
 * Author : cdani
 */ 
#include <avr/io.h>			//calls out library to be used
#define F_CPU 16000000UL	//defines CPU for future functions
#include <util/delay.h>		//includes library to be used

int main(void){				//initializes main program
	DDRB = 0xFF;			//sets port B as output
	PORTB = 0xFF;			//command sets LED off
	TCCR0B = 0b0000101;		//declares scalar 1024
	while(1)				//initializes while loop
	{
		TCNT0 = 0;			//holds timer count
		int START = 0;		//integer defines numeric variable
		while(START != 18)	//continuation of while loop stating if value not equal to 18, continue.
		{
			while(TCNT0 != 255);	//if value of timer not equal to 255 then continue
			TCNT0 = 0;		//if value equal to 255 then set timer to 0
			START++;		//start 
		}
		PORTB ^= (1<<2);	// Toggle on LED
		START = 0;			//starts with value 0
		TCNT0 = 0;			//sets timer to 0
		while(START !=27)	//if number is not equal to 27
		{
			while(TCNT0 != 255);	//if number is not equal to 255
			TCNT0 = 0;				//if number is equal to 255, set timer to 0 and start again
			START++;
		}
		PORTB ^= (1<<2);	//toggle LED off
		START = 0;			//timer starts again from 0
		TCNT0 = 0;
	}
}