/*
 * DA2Bc.c
 *
 * Created: 3/9/2019 2:02:27 PM
 * Author : cdani
 */ 

#define F_CPU 16000000UL	//initializes CPU
#include <avr/io.h>			//Includes AVR library
#include <util/delay.h>		//Includes utility for delays
#include <avr/interrupt.h>	//includes interrupts

int main(void)				//initializes code
{
	DDRB = 0xFF;			//Set Port B output
	DDRD = 0x00;			//sets port D as output
	PORTD = 0x02 ;			// Shifts UP
	PORTB = 0xFF;			//sets LED OFF
	EICRA = 0x02;			//this command will make it falling edge "triggered"
	EIMSK = (1<<INT0);		//commands enables the interrupt that will be created from outside sources
	sei();					//enables interrupt
	while(1);
}
ISR (INT0_vect)				//handles the external interrupt
{
	PORTB ^= 0x05;			// toggle PORT B delay of 1.25s
}