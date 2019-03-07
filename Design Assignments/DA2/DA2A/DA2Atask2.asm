//DA2Atask2asm
//Created: 3/2/2019 4:11:25 PM
//Author : Blassen 
 .ORG 0				//initialize program
	LDI R22, 0		//load immediate value 0 on register 22
	LDI R16, 0xFF	//load immediate value 0xFF on register 16
	OUT DDRB, R16	//output port B on register 16
	OUT DDRC, R22	//output port C on register 22
	OUT PORTC, R16	//output R16 to port C
	LDI R17, 0		// Register needed to toggle the LED just like in task 1
	OUT PORTB, R16	//output R16 to port B
	LDI R20, 5		//To set the prescalar by loading immediate value of 5 in register 20
	STS TCCR1B, R20	//Prescalar: 1024
	
CHECK:				//check for value
	IN R26, PINC	
	COM R26
	ANDI R26, 0x04	//adds immediate value to R26
	CPI R26, 0x04	//compares immediate value to R26
	BRNE CHECK	

BEGIN:
	LDI R25, 0x00	//Reset Counter on register 25
	STS TCNT1H, R25	//forces counter to count from 1
	STS TCNT1L, R25//forces counter to count from 1
	OUT PORTB, R22	//output R22 value on Port B

DH:
	LDS R29, TCNT1H // Loading upper bit value of counter to Regiser 29
	LDS R28, TCNT1L // Loading lower bit of value in counter to Register 28
	CPI R28, 0x4A	// Compare with Immediate 
	BRSH BH	// Branch if same or higher to BODY label
	RJMP DH	//jumps to branch

BH:
	CPI R29, 0x4C	//Compares value to immediate
	BRSH DONE		//done if the same
	RJMP DH			//jumps branch

DONE:				// Reset HIGH and LOW bytes for counter once program is done
	LDI R25, 0x00	//load immediate value of 0x00 on register 25
	STS TCNT1H, R25	//forces counter to count from 1
	STS TCNT1L, R25	//forces counter to count from 1
	OUT PORTB, R16	//output value of R16 in port B
	RJMP CHECK

