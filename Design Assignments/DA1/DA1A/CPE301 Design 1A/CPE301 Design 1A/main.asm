;
; CPE301 Design 1A.asm
;
; Created: 2/10/2019 11:31:48 AM
; Author : Blassen
	 .org 00
		ldi R22, 0x05; decimal number 5 on register 22
		ldi R23, 0x05; decimal number 5 on register 23
		ldi R24, 0x3C; setting a low of 60 on register 24
		ldi R25, 0x3C; setting a high of 60 on register 25 
	
	L1: add R19, R25; This will be added to column 2
		dec R22; this states that Register 22 = R22-1
		brcs OVERFLOW1
		brne L1
	
	L2: add R18, R24; This will be added to column 1
		dec R23; Register 23 = R23-1
		brcs OVERFLOW2
		brne L2

	L3: rjmp L3

	Overflow1: subi R20, -1; this will be the carryover for column 3
		clc
		brcs OVERFLOW3
		cpi R22,0
		breq L2
		rjmp L1
	OVERFLOW2: subi R17, -1; this will be the carryover for column 2
		clc
		add R19, R17; add carryovers from column 1 into column 2
		brcs OVERFLOW3
		cpi R23, 0
		breq L3
		rjmp L2
	OVERFLOW3: subi R20, -1; this will finally add the carry into column 3
		clc
		rjmp L3
		
