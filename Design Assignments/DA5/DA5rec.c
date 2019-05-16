/*
 * GccApplication4.c
 *
 * Created: 5/1/2019 11:30:16 AM
 * Author : cdani
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <string.h>
#include "nrf24l01.h"
#include "nrf24l01-mnemonics.h"
#define F_CPU 8000000UL
#define UBRR_9600 51
#include <util/delay.h>

nRF24L01 *setup_rf(void);
volatile bool rf_interrupt = false;

void spi_init()
{
	DDRB	&=	~((1<<2)|(1<<3)|(1<<5));			//	SCK,	MOSI	and	SS	as	inputs
	DDRB	|=	(1<<4);								//	MISO	as	output
	SPCR	&=	~(1<<MSTR);							//	Set	as	slave
	SPCR	|=	(1<<SPR0)|(1<<SPR1);				//	divide	clock	by	128
	SPCR	|=	(1<<SPE);							//	Enable	SPI
}

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


void ADC_init ()
{
	ADMUX = 0;
	ADMUX |= (1<<REFS0);
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1) |(1<<ADEN);
	ADCSRB = 0;
}

int main(void) {
	init_uart(UBRR_9600);
	spi_init();
	_delay_ms(500);
	uint8_t address[5] = { 0x01, 0x01, 0x01, 0x01, 0x01 };
	sei();
	USART_tx_string("connected...\r\n");
	nRF24L01 *rf = setup_rf();
	nRF24L01_listen(rf, 0, address);
	uint8_t addr[5];
	nRF24L01_read_register(rf, 0x00, addr, 1);
	while (true) {
		if (rf_interrupt) {
			rf_interrupt = false;
			while (nRF24L01_data_received(rf)) {
				nRF24L01Message msg;
				nRF24L01_read_received_data(rf, &msg);
				USART_tx_string((char *)msg.data);
				USART_tx_string("F\r\n");
			}
			nRF24L01_listen(rf, 0, address);
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



// nRF24L01 interrupt
ISR(INT0_vect) {
	rf_interrupt = true;
	EIFR |= (INTF0);
}