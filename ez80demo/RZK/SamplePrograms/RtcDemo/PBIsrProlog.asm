;/************************************************************************************
; * File       : Timer1ISRProlog.s
; *
; * Description: This sample program is an interrupt prologue routine 
; *              that calls the "C" ISR function for timer1.
; *
; * Copyright 2004 ZiLOG Inc.  ALL RIGHTS RESERVED.
; *
; * This file contains unpublished confidential and proprietary information
; * of ZiLOG, Inc.
; * NO PART OF THIS WORK MAY BE DUPLICATED, STORED, PUBLISHED OR DISCLOSED 
; * IN ANY FORM WITHOUT THE PRIOR WRITTEN CONSENT OF ZiLOG, INC.
; * This is not a license and no use of any kind of this work is authorized
; * in the absence of a written license granted by ZiLOG, Inc. in ZiLOG's 
; * sole discretion 
; *************************************************************************************/
		
	.extern _RZKISRProlog
	.extern _RZKISREpilog
	.extern _pb0_isr
	.extern _pb1_isr
	.extern _g_hpb1 ;
	.extern _g_hpb2 ;
	.extern _RZKResumeThread
	.assume adl=1

	.def _Pb0_isr_prolog
   NOP
_Pb0_isr_prolog:
	; Disable Port B's interrupt
	PUSH	AF
	ld      A,	FFH
	OUT0     (00A7H), A 
	IN0		A,	(009AH)
	SET		0, A
	OUT0	(009AH), A
	PUSH	BC
	PUSH	DE
	PUSH	HL		
	PUSH	IX
	PUSH	IY
	CALL	_RZKISRProlog
	LD HL,	(_g_hpb1)
	PUSH	HL
	CALL	_RZKResumeThread
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

	.def _Pb1_isr_prolog
   NOP
_Pb1_isr_prolog:
	PUSH	AF
	ld      A,	FFH
	OUT0     (00A7H), A 
	IN0		A,	(009AH)
	SET		1, A
	OUT0	(009AH), A
	PUSH	BC
	PUSH	DE
	PUSH	HL		
	PUSH	IX
	PUSH	IY
	CALL	_RZKISRProlog
	LD HL,	(_g_hpb2)
	PUSH	HL
	CALL	_RZKResumeThread
	POP HL
	CALL	_RZKISREpilog
	POP		IY
	POP    		IX
	POP		HL
	POP		DE
	POP		BC
	POP		AF
	EI
	RETI
