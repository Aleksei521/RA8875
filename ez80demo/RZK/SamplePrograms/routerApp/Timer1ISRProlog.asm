;/********************************************************************************************
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
; **********************************************************************************************/
		
;	.sect ".isr"
	.extern _RZKISRProlog
	.extern _RZKISREpilog
	.extern _Timer1ISR
	.extern _g_hTimer1Thread
	.extern _RZKResumeInterruptThread
	.extern _RZKSuspendInterruptThread
	.extern _Tmr3IntTask
	.assume adl=1

	.def _Timer1ISRProlog
   NOP
_Timer1ISRProlog:
	PUSH	AF
.ifdef _EZ80F91	
	; clear timer0 interrupt flag
	IN0     A, (67H)	; read interrupt identification register

	; Disable Timer1 interrupt
	IN0     A,(66H)
	RES     0, A
	OUT0    (66H), A
.else
	; Disable Timer1 interrupt
	IN0	A,(83H)
	RES	6, A
	OUT0	(83H), A
.endif	
	
	PUSH	BC
	PUSH	DE
	PUSH	HL		; scratch reg for function call from isr
	PUSH	IX
	PUSH	IY

	CALL	_RZKISRProlog
	LD HL,	(_g_hTimer1Thread)
	PUSH	HL
	CALL	_RZKResumeInterruptThread
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

;/* Function		:	_RZKSystemTimerIsrEntry
;*
;* Description		:	ISR entry point function
;* 
;* Inputs			:	None.
;*							
;* Outputs			:	None.
;*							
;*
;* Dependencies		:	None.
;*/
	.def _Timer1ISREntry
_Timer1ISREntry:
  rzksystem:
	    Call _Timer1ISR
		DI
.ifdef _EZ80F91
		IN0	A,(66H)
		SET	0, A
		OUT0 (66H), A
.else
		IN0	A,(83H)
		SET	6, A
		OUT0 (83H), A
.endif                     
		Call	_RZKSuspendInterruptThread
		EI
		jp  rzksystem
