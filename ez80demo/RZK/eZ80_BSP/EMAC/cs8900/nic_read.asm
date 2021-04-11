;********************************************************************************
; Copyright 2001, Metro Link, Inc.												*
; All Rights Reserved															*
;																				*
; This is UNPUBLISHED PROPRIETARY SOURCE CODE of Metro Link, Inc. and may		*
; contain proprietary, confidential and trade secret information of				*
; Metro Link, Inc. and/or its partners.											*
;																				*
; The contents of this file may not be disclosed to third parties, copied or	*
; duplicated in any form, in whole or in part, without the prior written		*
; permission of Metro Link, Inc.												*
;
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
;																				*
;********************************************************************************

	.assume ADL=1
	.extern p_emac_base
	.def	_nic_read
	.def	_nic_discard

_nic_read:
	push 	ix
	ld		ix, 0
	add		ix, sp

	.if 	!ZDS1
	ld 		de, 0
	ld 		e, (ix+6)
	ld 		d, (ix+7)
	ld		hl, (ix+9)
	ld		bc, (ix+12)
	.endif
	;***	Input Parameters:
	;		DE	Copy length in bytes
	;		HL Destination buffer
	;		BC IOBase (Ext 16-bit IO device)
	

	;***	Determine number of Word transfers
	;		= (DE+1)/2
	inc		de
	or		a, a
	rr		d
	rr		e


	;***	Check for read length of 0
	ld		a, e
	or		a, d
	jr		z, done_read

read_again:
									; Number of Cycles
	ini2							; 5
	inc		b						; 1
	in		a, (bc)					; 3
	ld		(hl), a					; 2
	dec		c						; 1
	inc		hl						; 1	= 13

	dec		de						; 1
	ld		a, e					; 1
	or		a, d					; 1
	jr		nz, read_again			; 3

done_read:
	ld		sp, ix
	pop		ix
	ret
	


_nic_discard:
	push 	ix
	ld		ix, 0
	add	ix, sp

	.if 	!ZDS1
	ld 		de, 0
	ld 		e, (ix+6)
	ld 		d, (ix+7)
	ld		bc, (ix+9)
	.endif
	;***	Input Parameters:
	;		DE	Copy length in bytes
	;		BC IOBase (Ext 16-bit IO device)
	

	;***	Determine number of Word transfers
	;		= (DE+1)/2
	inc	de
	or		a, a
	rr		d
	rr		e


	;***	Check for read length of 0
	ld		a, e
	or		a, d
	jr		z, done_discard

discard_again:
									; Number of Cycles
	in		a, (bc)				; 3
	inc	c						; 1
	in		a, (bc)				; 3
	dec	c						; 1

	dec	de						; 1
	ld		a, e					; 1
	or		a, d					; 1
	jr		nz, discard_again	; 3

done_discard:
	ld		sp, ix
	pop	ix
	ret


