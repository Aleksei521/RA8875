;/********************************************************************************
;* File		:	spiisr.asm
;*
;* Description	:	SPI ISR routine
;*					
;* 
;* Copyright 2016 ZiLOG Inc.  ALL RIGHTS RESERVED.
;*
;* This file contains unpublished confidential and proprietary information
;* of ZiLOG, Inc.
;* NO PART OF THIS WORK MAY BE DUPLICATED, STORED, PUBLISHED OR DISCLOSED 
;* IN ANY FORM WITHOUT THE PRIOR WRITTEN CONSENT OF ZiLOG, INC.
;* This is not a license and no use of any kind of this work is authorized
;* in the absence of a written license granted by ZiLOG, Inc. in ZiLOG's 
;* sole discretion 
;*******************************************************************************/



	.include "ez80F91.inc"
	.sect "CODE"
	.extern _spiresume
	.extern _RZKISRProlog
	.extern _RZKISREpilog
	.assume	adl=1

	.def _spiisr
   NOP
_spiisr:
	PUSH	AF
	PUSH	BC

	IN0  A,(%9A)
	SET  %2,A
	OUT0 (%9A),A

	IN0  A,(%9A)
	RES  %2,A
	OUT0 (%9A),A

	PUSH	DE
	PUSH	HL		
	PUSH	IX
	PUSH	IY

	CALL	_RZKISRProlog
	CALL	_spiresume
	CALL	_RZKISREpilog
	POP		IY
	POP     IX
	POP		HL
	POP		DE
	POP		BC
	POP		AF
	EI
	RET
