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

;	.sect	".isr"	
	.extern _RZKResumeInterruptThread	
	.extern _i2cIntThreadHdl
	.extern _I2CStatus
	.extern _RZKISRProlog
	.extern _RZKISREpilog
	.assume	adl=1

	.def	_i2cisr
   NOP
_i2cisr:	
	PUSH	AF
	PUSH	BC
	PUSH	DE
	PUSH	HL		; scratch reg for function call from isr
	PUSH	IX
	PUSH	IY
	
;	IN0  A,(%CB)
;	RES  %3,A   
;	OUT0 (%CB),A

;	IN0	A, (I2C_SR)		; Save the status register value
;	LD 	(_I2CStatus), A
	IN0 A, (I2C_CTL)	
	AND A, %7F
	OUT0 (I2C_CTL), A		; Disabling the I2C interrupts
	CALL	_RZKISRProlog
	LD HL, (_i2cIntThreadHdl)
	PUSH HL
	CALL _RZKResumeInterruptThread
	POP HL
	CALL	_RZKISREpilog
	POP		IY
	POP    	IX
	POP		HL
	POP		DE
	POP		BC
	POP		AF
	EI
	RETI





















