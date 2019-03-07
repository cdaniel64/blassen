//DA2ATask1
//Created: 3/1/2019 11:55:14 AM
//Author: Blassen
 

#include <avr/io.h>				//adds library
#define F_CPU 16000000UL		//Defines CPU
#include <util/delay.h>			//adds another library

int main(void)
{
    DDRB = 0xFF;				// Set PortB as an output of system
    
    while (1)
    { PORTB = 0x02;				// PortB.2 set high byte value
	    _delay_ms(435);			// Set high for 60% duty cycle of 725ms delay
	    PORTB = 0x00;			// PortB set low byte value 
	    _delay_ms(290);			//Set low for 40% duty cycle of 725ms delay
	}	    
}