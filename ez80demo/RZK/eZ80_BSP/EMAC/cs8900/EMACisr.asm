;********************************************************************************
; Copyright 2004, ZiLOG Inc.													*
; All Rights Reserved															*
;																				*
; This is UNPUBLISHED PROPRIETARY SOURCE CODE of ZiLOG Inc., and might			*
; contain proprietary, confidential and trade secret information of				*
; ZiLOG, our partners and parties from which this code has been licensed.		*
;																				*
; The contents of this file may not be disclosed to third parties, copied or	*
; duplicated in any form, in whole or in part, without the prior written		*
; permission of ZiLOG Inc.														*
;********************************************************************************

	.include "ez80F92.inc"

;	.sect	".isr"	;Added 24-07-02
	.extern _RZKResumeInterruptThread	
	.extern _emacInterruptThdHdl
	.extern _RZKISRProlog
	.extern _RZKISREpilog
	.extern	_pPktPagePtr
	.assume	adl=1

	.def	_emacisr
   NOP
_emacisr:	
	PUSH	AF
	PUSH	BC
	PUSH	DE
	PUSH	HL		; scratch reg for function call from isr
	PUSH	IX
	PUSH	IY

;***	Prevent the CS8900 from generating interrupts
	ld		bc, (_pPktPagePtr)
	xor 	a, a

	;***	PKTPAGEPTR = BUSCTL (0x0116);
	ld		de, 0116h
	out	(bc), e
	inc	bc
	out	(bc), d
	inc	bc

	;***	PKTPAGEDATA0 = ADDR_BUSCTL (0x0017);
	inc	e
	out	(bc), e
	inc	bc
	out	(bc), a

	CALL	_RZKISRProlog

	; Call RZKResumeInterruptThread
	LD HL, (_emacInterruptThdHdl)
	PUSH HL
	CALL _RZKResumeInterruptThread
	POP HL
;	call _F91EmacTask
	CALL	_RZKISREpilog
	POP		IY
	POP     IX
	POP		HL
	POP		DE
	POP		BC
	POP		AF
	EI
	RET


