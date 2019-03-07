//  DA2Atask2.c
//Created: 3/2/2019 12:54:15 PM
//Author: Blassen
  

#include <avr/io.h>			//include library
#define F_CPU 16000000UL	//initialize CPU
#include <util/delay.h>		//inculde library

int main(void)				//initialize program
{
	DDRB = 0xFF;			//Set Port B as an output 
	DDRC = 0x00;			//Set Port C as an input
	PORTC = 0xFF;			//Set switches that we will use ON 
		while (1)			//conditional initialized
	{	PORTB = 0xFF;		// Port B is off while value is 1
		if(PINC == 0x7D)	//if statement 
		{ PORTB = 0x00;		//if pinc equals to 0xFF, then port B will output 0x00
			_delay_ms(1250);//if port B does not equal 0xFF, then create delay of 1250 ms 
	}
	
	}
}

