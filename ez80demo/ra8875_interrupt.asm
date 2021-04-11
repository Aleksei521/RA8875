;	.include "kernel.inc"


	.assume ADL=1
	.extern _RZKResumeInterruptThread
	.extern _RZKISRProlog
	.extern _RZKISREpilog
	.extern _i_text_ptr
	.extern _i_isFont
	
	;.extern _ghRA8875wait_isr
	.extern _ghRA8875int_isr
	.def	_ra8875int_isr
	;.def	_ra8875wait_isr
	
	
   NOP
_ra8875int_isr:
	PUSH	AF
	PUSH	HL
	;font - 01
	;touch panel 04
	; Disable PD3 interrupt @Modified by Ermakov
	;IN0		A,	(0A5H)
	;RES		3, A
    ;OUT0	(0A5H), A

	; Clear  PD3 interrupt @Added by Ermakov
	LD      A,	08H
	OUT0     (0CFH), A 
	LD		A,(_i_isFont)
	OR		A,A
	JP		Z,_ret_int
	;
	LD		HL,0D00001H;CMD
	LD		(HL),0F1H
	LD		HL,0D00000H;DATA
	LD		A,(HL)
	TST		A,01H
	JP		Z,_NotText
	;LD		HL,0D00001H;CMD
	;LD		(HL),0F1H
	LD		HL,0D00000H;DATA
	LD		(HL),01H
	;
	LD		HL,(_i_text_ptr)
	LD		A,(HL)
	TST		A,0FFH
	JP		Z,_clr_int
	INC		HL
	LD		(_i_text_ptr),HL
	LD		HL,0D00001H;CMD
	LD		(HL),02H
	LD		HL,0D00000H;DATA
	LD		(HL),A
	; Enable PD3 interrupt @Modified by Ermakov
	;IN0		A,	(0A5H)
	;SET		3, A
	;OUT0	(0A5H), A
_ret_int:
	POP		HL
	POP		AF
	EI
	RETI
_clr_int:
	LD		HL,0D00001H;CMD
	LD		(HL),0F0H
	LD		HL,0D00000H;DATA
	LD		A,(HL)
	AND		A,0FEH
	LD		(HL),A
_NotText:	
    PUSH 	BC
	PUSH	DE
	PUSH	IX
	PUSH	IY
	CALL	_RZKISRProlog
	LD		HL, (_ghRA8875int_isr)
	PUSH	HL
	CALL	_RZKResumeInterruptThread	;Interrupt specific C code
	POP		HL
	CALL	_RZKISREpilog
    POP		IY
	POP     IX
	POP		DE
 	POP		BC
	POP		HL
	POP		AF
	EI
	RETI


   ;NOP
;_ra8875wait_isr:
	;PUSH	AF
	
	; Disable PD6 interrupt @Modified by Ermakov
	;IN0		A,	(0A5H)
	;RES		6, A
    ;OUT0	(0A5H), A

	; Clear  PD3 interrupt @Added by Ermakov
	;LD      A,	040H
	;OUT0     (0CFH), A 

    ;PUSH 	BC
	;PUSH	DE
	;PUSH	HL
	;PUSH	IX
	;PUSH	IY
	;CALL	_RZKISRProlog
	;LD		HL, (_ghRA8875wait_isr)
	;PUSH	HL
	;CALL	_RZKResumeInterruptThread	;Interrupt specific C code
	;POP		HL
	;CALL	_RZKISREpilog
    ;POP		IY
	;POP     IX
	;POP		HL
	;POP		DE
 	;POP		BC
	; Enable PD3 interrupt @Modified by Ermakov
;	IN0		A,	(0A5H)
;	SET		6, A
;	OUT0	(0A5H), A
	;POP		AF
	;EI
	;RETI
