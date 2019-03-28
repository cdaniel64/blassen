/*
 * GccApplication4.c
 *
 * Created: 3/24/2019 6:37:05 PM
 * Author : cdani
 */ 


#include <avr/io.h>						//calls main library
#define F_CPU 16000000UL				//initializes cpu
#include <util/delay.h>					//includes delay library
#include <avr/interrupt.h>				//includes interrupt library
volatile uint8_t tof_detected;			//this will count overflow units

ISR(TIMER0_OVF_vect)					//generate the overflow interrupt
{
	tof_detected++;						//overflow counter setup
}
void timer0_init()						//main timer function called and initialized
{
	TCCR0B |= (1 << CS02)|(1 << CS00);	//set up the pre-scalar
	TCNT0 = 0;							//initializes counter
	TIMSK0 |= (1 << TOIE0);				//timer 1 interrupt
	sei();								//global interrupt flag
	tof_detected = 0;					//value to overflow set
}
void func(unsigned int of_detection_num, unsigned int tcount) //function of unsigned integer detection number, if it is unsigned, count up.
{
	while(tof_detected != 0){			//if statement initialized. if overflow not equal to 0, then continue.
		if (tof_detected >= of_detection_num){	//if value of overflow is equal ot greater than detection number.
			if (TCNT0 >= tcount){				//check if timer reaches the count of the variable tcount
				PORTB ^= (1 << 2);				// Toggle LED if statement is not true
				TCNT0 = 0;						// Reset counter
				tof_detected = 0;				// Reset overflow
			}
		}
	}
}
int main(void)						//initializes main program
{
	DDRB |= (1 << 2);				//sets LED to pin PB2
	DDRB |= (1 << 5);
	PORTB ^= (1 << 5);				// LED off
	timer0_init();					//starts timer
	while(1)						//infinite loop generated
	{
		if(!(PINC & (1<<PINC1))){	//if statement to turn LED off
			PORTB ^= (1 << 2);		// LED OFF
			func(78, 165);
		}
		else
		PORTB |= (1<<2);			// LED off
	}
}