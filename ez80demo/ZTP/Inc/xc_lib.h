/*
 * Copyright 2016, ZiLOG Inc.
 * All Rights Reserved
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of ZiLOG Inc., and might
 * contain proprietary, confidential and trade secret information of
 * ZiLOG, our partners and parties from which this code has been licensed.
 * 
 * The contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of ZiLOG Inc.
 */

#ifndef _XC_LIB_H_
#define _XC_LIB_H_
#include <stdlib.h>
#include <string.h>
#include "ZTypes.h"
#include "zdevice.h"




/*
 * Copyright 2001, Metro Link, Inc.
 * All Rights Reserved
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Metro Link, Inc. and may
 * contain proprietary, confidential and trade secret information of
 * Metro Link, Inc. and/or its partners.
 *  
 * The contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of Metro Link, Inc.
 *
 */

INT16 xc_ascdate(UINT32 time, INT8 *str);
void _doprnt(INT8 *fmt, void * *args, INT16 (*func)(void * ,INT8, UINT16 *), void * farg, UINT16 * pIndex);
INT16 xc_fprintf(RZK_DEVICE_CB_t * dev, INT8 *fmt, ...);
INT8 * xc_index(register INT8 *sp, INT8 c);
INT16 xc_printf(INT8 *fmt, ...);
INT8 * xc_sprintf(INT8 *str, INT8 *fmt, ...);
INT16 xc_strcasecmp(register INT8 *s1, register INT8 *s2);
//void xc_memset( HANDLE p_dest, BYTE Data, WORD Length );


//#define	min(a,b)	( (a) < (b) ? (a) : (b) )
//#define	max(a,b)	( (a) > (b) ? (a) : (b) )



#endif
