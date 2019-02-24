;
; DA1B.asm
;
; Created: 2/23/2019 2:45:50 PM
; Author : cdani

.EQU	STARTADDS =	0X0200		; code sets address of memory
.EQU	COUNTONE = 99			;this will be the counter

.ORG 0x0000						;origin at 0x0000
	CLR		r0					;clear register R0
	LDI		xh, high(STARTADDS) ;set the high byte of register X
	LDI		xl, Low(STARTADDS)	;sets the low byte of register X
	LDI		yh, high(0x0400)	;sets high byte of register Y
	LDI		yl, low(0x0400)		;sets low byte of register X
	LDI		zl, low(0x0600)		;sets low byte of register Z
	LDI		zh, high(0x0600)	;sets high byte of register Z
	LDI		r21, COUNTONE		;loads the function counter to register R21
	LDI		r23, 11				;loads immediate value
	

START:					;program branch that runs the counter
	MOV		r22, r23	;it moves data from R22 to R23
	ST		X+, r23		;stores the number in register and updates the value
	RJMP	CHECK		; code jumps to checking section of code number

CHECK:					;branch of code that will check if the number is divisible by 3,
	SUBI	r22, 0x03;	;subtracts the number in register 22 by value of 3
	BRLT	NDIV		;if number is NOT divisible by 3, then it will jump back to START to check for another number.
	BREQ	YDIV		;if number is divisible by 3, then it will proceed to branch YDIV
	RJMP	CHECK

YDIV:					;on this branch of code, if the number is divisible by 3, it will store it in its designated slot
	ST		Y+, r23		;store the value in its current location and update such location
	ADD		r16, r23	;adds the value of register to the one we have set as accumulator
	ADC		r17, r0		;adds the carry bit to the accumulator as well
	RJMP	END			;program is done and will jump to end so that we can start the cycle again.

NDIV:					;on this branch of the code, if the number is NOT divisible by 3, then the number gets stored in its designated slot
	ST		Z+, r23		;this command stores the value of current location and updates as such.
	ADD		r18, r23	;adds to the accumulator
	ADC		r19, r0		;adds the carry bit to such accumulator again
	RJMP	END			;program is done and will jump to end so that we can start the cycle again.

END:
	INC		r23			;once code has ended, this will increase the value of R23
	DEC		r21			;once the code has ended, this will decrease the value of R21
	BRNE	START		;this will send the loop to start
	BREAK				;breakpoint of program.

