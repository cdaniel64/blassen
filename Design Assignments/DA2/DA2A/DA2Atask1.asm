//
// DA2Atask1.asm
//
// Created: 3/2/2019 11:34:29 AM
// Author: Blassen
// 

 .ORG 0				//initialize program
	LDI R16, 4		//load immediate value of 4 in register 16
	LDI R22, 0x02	//load immediate 0x02 on R22
	OUT DDRB, R22	//Output to port B from register 
	LDI R17, 0 ;	//Register will be used to toggle LED on and off, declared at toggle 0
	OUT PORTB, R17	//Output from Port B will be displayed on register 17
	LDI R20, 5 ;	//We have to set the pre-scalar for Register 20 hence the use of value 5
	STS TCCR1B, R20 // Here, we display the pre-scalar number>>>> Prescalar: 1024

BEGIN:
	LDI R25, 0x00	//Reset counter
	STS TCNT1H, R25	//force timer to count from 0 on register 25
	STS TCNT1L, R25	//same as above
	EOR R16, R17	//XOR (either or) to toggle LED control.
	OUT PORTB, R16

DH:					//Delay value high
	LDS R29, TCNT1H // Loading upper bit of counter to R29
	LDS R28, TCNT1L // Loading lower bit of counter to R28
	CPI R28, 0x8C	//Compare with Immediate 
	BRSH BODYHIGH	// Branch if same or higher to BODY label
	RJMP DH

BH:					//body High value
	CPI R29, 0x1A	// Compare to immediate
	BRSH DONE
	RJMP DH

DONE:				//Reset HIGH and LOW bytes for counter for LED
	LDI R20, 0x00	//Load immediate 0x00 on R20
	STS TCNT1H, R20 //once again force the timer to count from 0 on register 20 this time.
	STS TCNT1L, R20
	EOR R16, R17	//XOR to toggle LED
	OUT PORTB, R17	//output to port B on register 17

DL:					//Delay Low
	LDS R29, TCNT1H ; Loading upper bit of counter to R29
	LDS R28, TCNT1L ; Loading lower bit of counter to R28
	CPI R28, 0xB2 ; Compare with Immediate 
	BRSH BL ; Branch if same or higher to BODY label
	RJMP DL

BL:					//Body Low
	CPI R29, 0x11	// Compare to immediate
	BRSH BEGIN
	RJMP DL
	RET


