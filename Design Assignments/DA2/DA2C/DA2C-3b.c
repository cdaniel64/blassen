/*
 * GccApplication6.c
 *
 * Created: 3/24/2019 9:05:01 PM
 * Author : cdani
 */ 

#include <avr/io.h>						//reaches out to main library
#define F_CPU 16000000UL				//initializes cpu
#include <util/delay.h>					//calls out delay library
#include <avr/interrupt.h>				//calls out interrupt library

volatile uint8_t tof_detected;			//declares variable tof_detected volatile

ISR( TIMER0_COMPA_vect ){
	TCNT0 = 0;
	int count = 0;
	if(!(PINC & (1<<PINC1) ) ){
		while(count < 77){
			while((TIFR0 & (1<<OCF0A) ) == 0){
				PORTB &= ~(1<<2);
			}
			TCNT0 = 0;
			TIFR0 |= (1<<OCF0A);
			count++;
		}
		count = 0;
	}
	else{
		PORTB |= (1<<2);
	}
}
void timer0_init()
{
	TCCR0B |= (1 << CS02)|(1 << CS00);	//set up prescalar
	TCNT0 = 0;							//starts counter initialized at 0
	TIMSK0 |= (1 << OCIE0A);
	sei();								//flags up timer
	tof_detected = 0;					
}
int main(void)							//main program 
{
	DDRB |= (1 << 2);
	DDRB |= (1 << 5);
	PORTB ^= (1 << 5);					// Turns LED Off
	timer0_init();						//initializes timer again
	while(1);							//loops generation again
}