/************************************************************************************
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
 ************************************************************************************/
 /************************************************************************************
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
 ************************************************************************************/

/* lexan.c - lexan */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZDevice.h"
#include "shell.h"

/*------------------------------------------------------------------------
 *  lexan  -  ad hoc lexical analyzer to divide command line into tokens
 *------------------------------------------------------------------------
 */
INT16 lexan(struct shvars *Shl, INT8	*line)
{
	INT8	**tokptr;
	UINT16	ntok;
	INT8	*p;
	INT8	ch;
	INT8	*to;
	INT8	quote;

	to = Shl->shargst;		/* area to place token strings */

/* Ungh.. DOS based compilers treat '\n' as 0x0a instead of 0x0d */
	for(p=line; *p!='\0';p++)
		if( *p == (INT8)0x0d )
			*p = (INT8)0x0a;

	tokptr = &Shl->shtok[ntok = 0];	/* array of ptrs to tokens */
	for  (p = line ; *p!='\0' && *p!='\n' && ntok < SHMAXTOK ;) {
		while ( (ch = *p) == ' ')	/* skip leading blanks	*/
			p++;
		while ( (ch = *p) == (UINT8)'\377') {	/* skip telnet command	*/
			p++;p++;p++;
			}
		if (ch == '\0' || ch == '\n')	/* end of line or string*/
			return(ntok);
		*tokptr++ = to;			/* save start of token	*/
		Shl->shtktyp[ntok++] = ch;
		if (ch == '"' || ch == '\'') {	/* check for quoted str.*/
			quote = ch;
			for (p++ ; (ch = *p++) != quote && ch != '\n'
				&& ch != '\0' ; )
				*to++ = ch;
			if (ch != quote)
				return(-1);
		} else {		/* other possible tokens	*/
			*to++ = *p++;
			if (ch!='>' && ch!='<' && ch!='&')
				while ((ch = *p)!='\n' && ch !='\0' &&
					ch!='<' && ch!='>' && ch!=' ' &&
					ch!='"' && ch!='\'' && ch !='&')
					*to++= *p++; /* copy alphamerics*/
		}
		*to++ = 0;		/* terminate token string	*/
	}
	return(ntok);
}
