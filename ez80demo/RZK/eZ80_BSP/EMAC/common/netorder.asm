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
;																				*
;********************************************************************************

	.assume	adl=1
	.def	_ez80_hs2net
	.def	_ez80_hl2net

_ez80_hs2net:
	push 	ix
	ld		ix, 0
	add		ix, sp

	.if !ZDS1
	ld		de, (ix+6)
	.endif
	ld		h, e
	ld		l, d

	pop		ix
	ret

_ez80_hl2net:
	push 	ix
	ld		ix, 0
	add		ix, sp

	.if !ZDS1
	ld		e, (ix+6)
	ld		hl, (ix+5)
	ld		h, (ix+8)
	ld		l, (ix+9)
	.else
	ld		b, (ix+6)
	ld		de, (ix+5)
	ld		d, (ix+8)
	ld		e, (ix+9)
	.endif

	pop		ix
	ret

