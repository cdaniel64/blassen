/*
 * GccApplication9.c
 *
 * Created: 4/5/2019 10:34:48 PM
 * Author : Blassen
 */ 

#define F_CPU 8000000UL						//initialize CPU
#define UBRR_115200 3						// defined for 8Mhz with 8.5% error

#define Domain "api.thingspeak.com"			//create domain to communicate with thingspeak	
#define API_Write_Key "K6HINW5F75HTUPI8"	//API ID Key Number
#define Channel_ID "467279"					//ID of channel used for the project
#define SSID ""								
#define Password ""


#include <avr/io.h>							//initialize libraries to be used
#include <util/delay.h>					
#include <stdio.h>
#include <avr/interrupt.h>
#include <stdint.h>


											// Function Declarations that will not be displaying an output
void read_adc(void);
void adc_init(void);
void USART_init( unsigned int ubrr );
void USART_tx_string( char *data );
volatile unsigned int adc_temp;
char outs[30];


int main(void)										//main program display
{
	adc_init(); 									//Initialize the ADC (Analog / Digital Converter)
	USART_init(UBRR_115200); 						//Initialize the USART (RS232 interface)
	_delay_ms(125); 								//create some delay for the system to boot up (0.125s)
	snprintf(outs,sizeof(outs),"AT\r\n");
	USART_tx_string(outs);
	_delay_ms(2000);								//another delay of 2s
	snprintf(outs,sizeof(outs),"AT+CWMODE=3\r\n");  //wifi mode will be enabled
	USART_tx_string(outs);
	_delay_ms(2000);								//another delay of 2s
	snprintf(outs,sizeof(outs),"AT+CIPMUX=0\r\n");	//single connection being set
	USART_tx_string(outs);
	_delay_ms(2000);								//another delay of 2s
	snprintf(outs,sizeof(outs),"AT+CIPMODE=0\r\n");	//normal mode activated
	USART_tx_string(outs);							
	_delay_ms(2000);								//one more delay of 2s to make sure system works properly
	snprintf(outs,sizeof(outs),"AT+CWJAP=\"%s\",\"%s\"\r\n", SSID, Password);	//connect to wifi network
	USART_tx_string(outs);
	_delay_ms(2000);								//one more delay of 2s
	snprintf(outs,sizeof(outs),"AT+CPISTART=\"TCP\",\"%s\",80\r\n",Domain);		//connect to thingspeak
	USART_tx_string(outs);
	_delay_ms(2000);								//one more delay of 2s

	
	
	while(1)										//while loop initialized
	{
		read_adc();									//initialize read adc command
		snprintf(outs,sizeof(outs),"GET /update?api_key=%s&field1=1\r\n",API_Write_Key);	//send value to flash
		USART_tx_string(outs);
		_delay_ms(150);
		snprintf(outs,sizeof(outs),"AT+CISEND=%3d\r\n",adc_temp);
		USART_tx_string(outs);
		_delay_ms(15000);			//wait 15 seconds
	}
}

void adc_init(void)
{
	/*Setup and enable ADC **/
	ADMUX = 0;				//select ADC0 Pin as input
	ADMUX = (0<<REFS1)| 	//Reference Selection Bits
	(1<<REFS0)| 			//AVcc - external cap at AREF
	(1<<ADLAR); 			//ADC left Adjust Result
	
	ADCSRA = (1<<ADEN)| 	//ADC ENable
	(1<<ADSC)| 				//ADC Start Conversion
	(1<<ADATE)| 			//ADC Auto Trigger Enable
	(0<<ADIF)| 				//ADC Interrupt Flag
	(0<<ADIE)| 				//ADC Interrupt Enable
	(1<<ADPS2)| 			//ADC Pre-scaler of 64
	(1<<ADPS1)|
	(0<<ADPS0);

}

/* READ ADC PINS*/
void read_adc(void)
{
	unsigned char i = 4;					//set for 4 ADC reads
	adc_temp = 0;							//initialize temp to 0
	while (i--)
	{
		ADCSRA |= (1<<ADSC);				//start the conversion
		while((ADCSRA & (1<<ADIF)) == 0);	//wait for conversion to finish
		adc_temp += ADCH*2;					//get temp value
		_delay_ms(50);						//wait a bit
	}
	adc_temp = adc_temp / 4; 				// Average a few samples

}

/* INIT USART (RS-232) */
void USART_init( unsigned int ubrr )
{
	UBRR0H = (unsigned char)(ubrr>>8);			//set baud rate
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << TXEN0) | (1 <<RXEN0); 		// Enable receiver, transmitter
	UCSR0C = (1 << UCSZ00) | (1 << UCSZ01); 	//asynchronous 8-bit data 1 stop bit
}

/* SEND A STRING TO THE RS-232*/
void USART_tx_string( char *data )
{
	while ((*data != '\0'))
	{
		while (!(UCSR0A & (1 <<UDRE0)));	//wait for the transmit buffer to empty
		UDR0 = *data;						//put the data into the empty buffer, which sends the data
		_delay_ms(125); 					// wait a bit
		data++;
	}
}

