/*
 * GccApplication3.c
 *
 * Created: 5/2/2019 11:31:34 AM
 * Author : cdani
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#define UBRR_9600 51
#include <util/delay.h>
#include <stdbool.h>
#include <string.h>
#include "nrf24l01.h"

void setup_timer(void);
nRF24L01 *setup_rf(void);

volatile unsigned int adc_temp;
char outs[20];
volatile bool rf_interrupt = false;
volatile bool send_message = false;

void init_uart(unsigned int ubrr)
{
	UBRR0H = (unsigned char)(ubrr>>8);		//set baud rate
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1<<TXEN0) | (1<<RXEN0);
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
	
}

void USART_tx_string( char *data ) {
	while ((*data != '\0')) {
		while (!(UCSR0A & (1 <<UDRE0)));
		UDR0 = *data;
		data++;
	}
}

void adc_init(void)
{
	/** Setup and enable ADC **/
	ADMUX = (0<<REFS1)| 	// Reference Selection Bits
	(1<<REFS0)|				// AVcc - external cap at AREF
	(1<<ADLAR)| 			// ADC left Adjust Result
	(0<<MUX2)| 				// Analog Channel Selection Bits
	(0<<MUX1)| 				// ADC0 Pin
	(0<<MUX0);

	ADCSRA = (1<<ADEN)| 	// ADC ENable
	(1<<ADSC)| 				// ADC Start Conversion
	(1<<ADATE)| 			// ADC Auto Trigger Enable
	(0<<ADIF)| 				// ADC Interrupt Flag
	(0<<ADIE)| 				// ADC Interrupt Enable
	(1<<ADPS2)| 			// ADC Prescaler Select Bits
	(0<<ADPS1)|
	(1<<ADPS0);

}

/* READ ADC PINS*/
void read_adc(void)
{
	unsigned char i =4;							//set i to 4- make 4 readings
	adc_temp = 0;								//initialize ADC_TEMP
	while (i--)
	{
		ADCSRA |= (1<<ADSC);
		while((ADCSRA & (1<<ADIF)) == 0);
		adc_temp += ADCH;						//sum up 4 readings
		_delay_ms(50);
	}
	adc_temp = adc_temp / 4; 					// Average of four samples

}

int main(void) {
	uint8_t to_address[5] = { 0x01, 0x01, 0x01, 0x01, 0x01 };
	bool on = false;
	adc_init();
	sei();
	nRF24L01 *rf = setup_rf();
	setup_timer();

	while (true) {
		read_adc();
		if (rf_interrupt) {
			rf_interrupt = false;
			int success = nRF24L01_transmit_success(rf);
			if (success != 0)
			nRF24L01_flush_transmit_message(rf);
		}

		if (send_message) {
			send_message = false;
			on = !on;
			nRF24L01Message msg;
			if (on)
			{
				snprintf(outs,sizeof(outs),"%3d\r\n", adc_temp);	// print ADC value
				memcpy(msg.data, outs , 3);
				USART_tx_string((char *)msg.data);
				USART_tx_string("F\r\n");
			}
			else memcpy(msg.data, "OFF", 4);
			msg.length = strlen((char *)msg.data) + 1;
			nRF24L01_transmit(rf, to_address, &msg);
		}
	}

	return 0;
}

nRF24L01 *setup_rf(void) {
	nRF24L01 *rf = nRF24L01_init();
	rf->ss.port = &PORTB;
	rf->ss.pin = PB2;
	rf->ce.port = &PORTB;
	rf->ce.pin = PB1;
	rf->sck.port = &PORTB;
	rf->sck.pin = PB5;
	rf->mosi.port = &PORTB;
	rf->mosi.pin = PB3;
	rf->miso.port = &PORTB;
	rf->miso.pin = PB4;
	// interrupt on falling edge of INT0 (PD2)
	EICRA |= _BV(ISC01);
	EIMSK |= _BV(INT0);
	nRF24L01_begin(rf);
	return rf;
}

// setup timer to trigger interrupt every second when at 1MHz
void setup_timer(void) {
	TCCR1B |= _BV(WGM12);
	TIMSK1 |= _BV(OCIE1A);
	OCR1A = 15624;
	TCCR1B |= _BV(CS10) | _BV(CS11);
}

// each one second interrupt
ISR(TIMER1_COMPA_vect) {
	send_message = true;
}

// nRF24L01 interrupt
ISR(INT0_vect) {
	rf_interrupt = true;
}