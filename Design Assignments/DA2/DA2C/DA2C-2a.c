/*
 * GccApplication3.c
 *
 * Created: 3/24/2019 6:20:35 PM
 * Author : cdani
 */ 

#include <avr/io.h>					//includes main library
#define F_CPU 16000000UL			//defines CPU
#include <util/delay.h>				//calls out delay library
#include <avr/interrupt.h>			//calls out interrupt library

volatile uint8_t tof_detected;		//will count overflow
ISR(TIMER0_OVF_vect)				//this section will create an overflow interrupt for TIMER0
{
	tof_detected++;					//overflow addition
}
void timer0_init()					//timer flag 0 is raised when timer starts
{
	TCCR0B |= (1 << CS02)|(1 << CS00);	//prescalar initiated
	TCNT0 = 0;						//counter is initiated
	TIMSK0 |= (1 << TOIE0);			//timer 1 interrupt
	sei();							//this is the global interrupt flag
	tof_detected = 0;				//self explanatory
}
void func(unsigned int of_detection_num, unsigned int tcount)
{
	if (tof_detected >= of_detection_num)	//initiation of if statement 
	{
		if (TCNT0 >= tcount)		//checks value if timer reaches count
		{
			PORTB ^= (1 << 2);    // Toggles LED ON
			TCNT0 = 0;            // initializes counter with value of 0
			tof_detected = 0;     //initializes overflow variable stack 
		}
	}
}
int main(void)
{
	DDRB |= (1 << 2);			//sets LED port to a new one
	DDRB |= (1 << 5);
	PORTB ^= (1 << 5);			//turns LED off
	timer0_init();				//starts timer again	
	while(1)					//loop keeps going forever
	{
		func(26, 165);
		func(17, 195);
	}
}