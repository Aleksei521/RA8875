;	.include "kernel.inc"


	.assume ADL=1
	.extern _PB0IntTaskPID
	.extern _RZKResumeInterruptThread
	.extern _RZKISRProlog
	.extern _RZKISREpilog
	.extern _g_hPb0Thread
	.extern _PB0IntTask
	.def	_pb0_isr
	
   NOP
_pb0_isr:
	PUSH	AF
	
	; Disable PB0 interrupt @Modified by Venkat
;	IN0		A,	(009AH)
;	SET		0, A
	ld      A,00H
    OUT0	(009DH), A

	; Clear  PB0 interrupt @Added by Venkat
	ld      A,	01H
	OUT0     (00A7H), A 

    PUSH 	BC
	PUSH	DE
	PUSH	HL
	PUSH	IX
	PUSH	IY
	CALL	_RZKISRProlog
	ld		HL, (_g_hPb0Thread)
	PUSH	HL
	CALL	_RZKResumeInterruptThread	;Interrupt specific C code
	POP		HL
	CALL	_RZKISREpilog
;	CALL	_PB0IntTask	;Interrupt specific C code
    POP		IY
	POP     IX
	POP		HL
	POP		DE
 	POP		BC
	POP		AF
	EI
	RETI
