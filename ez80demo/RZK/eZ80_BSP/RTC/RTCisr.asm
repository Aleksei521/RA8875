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

	.include "ez80F91.inc"

;	.sect	".isr"	
	.extern _RZKResumeInterruptThread	
	.extern _rtcInterruptThdHdl
	.extern _RZKISRProlog
	.extern _RZKISREpilog
	.assume	adl=1

	.def	_rtcisr
   NOP
_rtcisr:	
	PUSH	AF
	IN0 A, 	(RTC_CTRL)
	AND A, %BF
	OUT0 (RTC_CTRL), A
	PUSH	BC
	PUSH	DE
	PUSH	HL		; scratch reg for function call from isr
	PUSH	IX
	PUSH	IY
	CALL	_RZKISRProlog
	LD HL, (_rtcInterruptThdHdl)
	PUSH HL
	CALL _RZKResumeInterruptThread
	POP HL
	CALL	_RZKISREpilog
	POP		IY
	POP     IX
	POP		HL
	POP		DE
	POP		BC
	POP		AF
	EI
	RETI
