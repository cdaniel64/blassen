;
; DA2B.asm
;
; Created: 3/9/2019 1:52:10 PM
; Author : Blassen
;

.ORG 0
	JMP MAIN				//jumps to main
.ORG 0X02					//will be set as original location for external interrupt
	JMP EX0_ISR				//once main is executed properly, it will jump to EX0_ISR
MAIN: 
	LDI R22, HIGH(RAMEND)	//Loads high 8 bits of R22
	OUT SPH, R22			//Set Pointer Stack high
	LDI R22, LOW(RAMEND)	//loads low 8 bits of R22
	OUT SPL, R22			//set pointer stack low
	LDI R22, 0X02			//load value 0x02 immediate on R22 to make it into falling edge triggered 
	STS	EICRA, R22			//generates external interrupt control
	SBI DDRB, 2				//creates Port B.2 as output
	CBI	PORTB, 2			//clears bit 2 in port B
	SBI PORTD, 2			//pulls value up
	LDI R22, 1<<INT0		//enables INT0 function
	OUT EIMSK, R22			//external interrupt mask
	SEI						//enables interrupts
STABLE: JMP STABLE			//jumps to STABLE function which will serve to run the loops
EX0_ISR:
	SBI		PORTB, 2		//creates Port B.2 as output
	RCALL	T1				//Recall subfunction T1
	CBI		PORTB, 2		//clears bit 2 in port B	
	RETI
T1:							//Timer 
	LDI R19, 100			//Load immediate in R19 value of 100
L1:							//Loop one for purposes of timer and interrupt
	LDI R20, 200			//load immediate value of 200 in register 20
L2:							//second loop for purposes of timer and interrupt
	LDI R21, 200			//load immediate value of 200 in register 21
L3:							//third loop for purposes of timer and interrupt
	NOP						//this command basically does nothing, timer filler
	NOP						//""""""""""""""""""""""""""""""""""""""""""""""""
	DEC R21					//Decreases value of R21 by one
	BRNE L3					// Branch if not equal to Loop 3
	DEC R20					//Decreases value of R20 by one
	BRNE L2					//Checks if equal value to loop 2, if not it continues
	DEC R19					//decreases value of R19 by one
	BRNE L1					//it branches out if equal to loop one, if not, it continues.
	RET						//returns from procedure where function was originally called