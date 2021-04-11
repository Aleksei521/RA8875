;/***********************************************************************************
; * File       : PBISRProlog.s
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
; ************************************************************************************/
		
	.extern _RZKISRProlog
	.extern _RZKISREpilog
	.extern _pb3_isr
	.extern _g_hpb3 ;
	.extern _RZKResumeThread
	.assume adl=1

	.def _Pb3_isr_prolog
   NOP
_Pb3_isr_prolog:
	
	PUSH	AF
; Disable PB0 interrupt @Modified by Venkat
;	IN0		A,	(009AH)
;	SET		3, A
    ld      A,00H
    OUT0	(009DH), A

	; Clear  PB0 interrupt @Added by Venkat
	ld      A,	08H
	OUT0     (00A7H), A 


	PUSH	BC
	PUSH	DE
	PUSH	HL		
	PUSH	IX
	PUSH	IY
	CALL	_RZKISRProlog
	LD HL,	(_g_hpb3)
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
	RET

