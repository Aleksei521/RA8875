;********************************************************************************
; Copyright 2004, ZiLOG Inc.													*
; All Rights Reserved															*
;																				*
; This is UNPUBLISHED PROPRIETARY SOURCE CODE of ZiLOG Inc., and might			*
; contain proprietary, confidential and trade secret information of			*
; ZiLOG, our partners and parties from which this code has been licensed.		*
;																				*
; The contents of this file may not be disclosed to third parties, copied or		*
; duplicated in any form, in whole or in part, without the prior written		*
; permission of ZiLOG Inc.														*
;********************************************************************************

	.include "ez80F91.inc"
	.sect "CODE"
	.extern _RZKISRProlog
	.extern _RZKISREpilog
	.extern _SerIntTasks
	.extern _RZKResumeInterruptThread
	.assume	adl=1

	.def _uart0isr
   NOP
_uart0isr:
	PUSH	AF
	PUSH	BC
	PUSH	DE
	PUSH	HL		
	PUSH	IX
	PUSH	IY
	
	ld	bc, UART0_IIR
	in	a, (bc)
	rla
	rla
	rla
	rla
	and	a, 0F0h
	ld	d, a

	ld	bc, UART0_IER
	in	a, (bc)
	or	a, d
	ld	d, a
	xor	a, a
	out	(bc), a
	ld	c, UART0_SPR
	ld	a, d
	out	(bc), a
	
	CALL	_RZKISRProlog
	ld      DE, (_SerIntTasks)
	PUSH	DE
	CALL    _RZKResumeInterruptThread
	POP		DE
	CALL	_RZKISREpilog
	
	POP	IY
	POP IX
	POP	HL
	POP	DE
	POP	BC
	POP	AF
	EI
	RETI
