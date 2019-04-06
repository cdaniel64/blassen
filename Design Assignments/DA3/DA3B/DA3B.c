/*
 * GccApplication8.c
 *
 * Created: 3/30/2019 3:10:26 PM
 * Author : cdani
 */ 
#define F_CPU 16000000UL				//initialize CPU
#include <avr/io.h>						//call main library
#include <util/delay.h>					//call delay library
#include <stdio.h>						//include stdio library for further analysis

#define BAUDRATE 9600											//defines the rate at which the microcontroller will transmit information
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)		//sets up scalar 1024

void read_adc(void);					// Declaration of functions that we do not want the processor to output results,
void adc_init(void);					//this function initializes the ADC module					
void USART_init( unsigned int ubrr );	//function initializes USART
void USART_tx_string( char *data );		//function initializes tx trsings of USART
volatile unsigned int adc_temp;			//declares unsigned integer for adc_temp
char outs[20];							//number of characters out
int main(void)							//initializes main program
{
	adc_init();							//Initializes the ADC from past declared function
	USART_init(BAUD_PRESCALLER);		//initializes USART from past declared function
	USART_tx_string("Online\r\n");		//connects program with confirmation message	
	_delay_ms(130);						//creates delay for .125s so there are no internal problems with processes
	while(1)							//while loop declaration
	{
		read_adc();						//reads ADC
		snprintf(outs,sizeof(outs),"%3d\r\n", adc_temp);	//Prints result from reading
		USART_tx_string(outs);		
		_delay_ms(130);					//delay so program doesnt crash
	}
}

void adc_init(void)						//Program initialize for ADC
{
										// Setup of ADC for enable
	ADMUX = (0<<REFS1)|					//Selection Bits (reference table)
	(1<<REFS0)|							// AVcc 
	(0<<ADLAR)|							// ADC result adjusted to left side
	(1<<MUX2)|							// Analog Channel Selection Bits
	(0<<MUX1)|							// ADC4-PC4 PIN27
	(0<<MUX0);
	ADCSRA = (1<<ADEN)|					// Enable of ADC
	(0<<ADSC)|							// ADC Start Conversion
	(0<<ADATE)|							// ADC Auto Trigger Enable
	(0<<ADIF)|							// ADC Interrupt Flag
	(0<<ADIE)|							// ADC Interrupt Enable
	(1<<ADPS2)|							// ADC Prescaler Select Bits
	(0<<ADPS1)|
	(1<<ADPS0);
}
void read_adc(void)						//section program reads the pins designated for ADC
{
	unsigned char i = 5;				//sets unsigned character i equal to 5
	adc_temp = 0;						//sets adc_temp to 0
	while (i--)							//generates while loop, if true, subtract from i
	{
		ADCSRA |= (1<<ADSC);
		while(ADCSRA & (1<<ADSC));
		adc_temp+= ADC;
		_delay_ms(50);					//creates a delay of .05 seconds
	}
	adc_temp = adc_temp / 5;			// Averages a portion of samples by 5
}
void USART_init( unsigned int ubrr )	//initiates USART RS-232
{
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << TXEN0);				// Enable RX, TX & RX interrupt
	UCSR0C = (3 << UCSZ00);				//generates an asynchronous (8 N 1)
}

void USART_tx_string( char *data )		//sends a complete string to the RS-232 point
{
	while ((*data != '\0'))				//initialize while loop to record data.
	{
		while (!(UCSR0A & (1 <<UDRE0)));
		UDR0 = *data;
		data++;
	}
}