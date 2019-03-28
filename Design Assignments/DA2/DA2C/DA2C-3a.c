/*
 * GccApplication5.c
 *
 * Created: 3/24/2019 8:47:06 PM
 * Author : cdani
 */ 

#include <avr/io.h>				//calls main library
#define F_CPU 16000000UL		//initializes cpu
#include <util/delay.h>			//includes delay library
#include <avr/interrupt.h>		//includes interrupt 
volatile uint8_t tof_detected;	//this will count overflow units

ISR( TIMER0_COMPA_vect )		//generate the overflow interrupt
{	
	TCNT0 = 0;					//initializes counter timer.
	int START = 0;				//integer variable start equals to 0
	while(START != 27)			//while loop starts checking if START is not equal to 27
	{			
		while(TCNT0 != 255);	//while loop inside while loop testing if timer is not equal to 255
		TCNT0 = 0;				//if timer is equal to 255, then resets timer
		START++;				//adds counter on START
	}
	PORTB ^= (1<<2);			//toggle LED if statement is not true
	START = 0;					//set START variable to 0
	TCNT0 = 0;					//set timer to 0
	while(START != 18)			//generate another while loop asking if START is not equal to 18
	{			
		while(TCNT0 != 255);	//while loop inside another while loop checking if TCNT0 is not equal to 255
		TCNT0 = 0;				//sets TCNT0 equal to 0
		START++;				//initializes counter 
	}
	PORTB ^= (1<<2);			//Turns LED on
	START = 0;					//sets START to 0
	TCNT0 = 0;					//sets timer to 0
}
void timer0_init()				//main timer function called and initialized
{		
	TCCR0B |= (1 << CS02)|(1 << CS00);	//sets prescalar
	TCNT0 = 0;					//starts counter
	TIMSK0 |= (1 << OCIE0A);	
	sei();						//global interrupt flag
	tof_detected = 0;			//count overflow units starting from 0
}
int main(void){					//initializes main program
	DDRB |= (1 << 2);
	DDRB |= (1 << 5);
	PORTB ^= (1 << 5);			//turns LED off
	timer0_init();				//timer reset
	while(1);					//while loop
}	