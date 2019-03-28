/*
 * GccApplication7.c
 *
 * Created: 3/27/2019 9:37:33 PM
 * Author : cdani
 */ 

#define F_CPU 16000000UL			//initialize CPU
#include <avr/io.h>					//call main library
#include <util/delay.h>				//call delay library
#include <stdio.h>					//call studio library
#include <math.h>					//call math functions library

#define BAUDRATE 9600				//sets the rate at which the device will transmit information
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)


void USART_init( unsigned int ubrr );	//void function so it doesnt return value while transmitting
void USART_tx_string( char *data );		//and receiving data
char outs[25];							//function declaration for number of characters displayed out
int number;								//function declaration for number to be generated
char str[] = "SUP";						//initialize string characters 
char empty[] = " ";						//an empty space
float othernumber = 27.3653;			//float number random from source code 27.3653
int main(void)							//main function call
{
										// Initialize three desired values
	USART_init(BAUD_PRESCALLER);		// Initialize the USART
	USART_tx_string("Connected!\r\n");  // Send initial connection print for verification to the programmer
	_delay_ms(125);						// Delay first print commands by 125ms
	while(1)							// while loop, it will get three desired values, if first desired value, print a string.
	{
		USART_tx_string(str);			//Print SUP message
		USART_tx_string(empty);			// Print empty space to have some room
		number = rand();				//if second desired value, system will print a random integer number
		snprintf(outs,sizeof(outs),"%3d\r\n", number);  // print the value
		USART_tx_string(outs);			//print again the number
		USART_tx_string(empty);			//print empty space
		sprintf(outs, "%g", othernumber);//third value will print a floating number
		//snprintf(outs,sizeof(outs),"%3d\r\n", othernumber);  // print it other way to write it
		USART_tx_string(outs);			// Print random integer number
		USART_tx_string(empty);			// Print empty space
		_delay_ms(1000);				// wait a bit
	}
}


void USART_init( unsigned int ubrr )	// INIT USART (RS-232)
{
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0B = (1 << TXEN0);				// Enable RX, TX & RX interrupt
	UCSR0C = (3 << UCSZ00);				//asynchronous 8 N 1
}


void USART_tx_string( char *data )		//send a string through RS-232 by FTDI board
{
	while ((*data != '\0'))				//while loop initialized
	{
		while (!(UCSR0A & (1 <<UDRE0)));
		UDR0 = *data;
		data++;							//adds value to data
	}
}