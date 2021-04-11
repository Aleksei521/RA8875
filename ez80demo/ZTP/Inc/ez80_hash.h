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
#ifndef _EZ80_HASH_H_
#define _EZ80_HASH_H_

#include "ssl_types.h"



/*
 * Hash algorithms supported by this implementation.
 */
#define SSL_HASH_NULL						0
#define SSL_HASH_MD5							1
#define SSL_HASH_HMAC_MD5					2
#define SSL_HASH_SHA1						3
#define SSL_HASH_HMAC_SHA1					4
#define SSL_HASH_MICHAEL               5
#define SSL_MAX_HASH							6	// Always 1 more than last defined SSL_HASH_xxx value


typedef struct SSL_HASH_S *	(* HASH_NEW)   ( SSL_BYTE * pKey, SSL_WORD KeyLen );
typedef SSL_STATUS 				(* HASH_UPDATE)( struct SSL_HASH_S * pHash, SSL_BYTE * pData, SSL_WORD Length );
typedef SSL_STATUS 				(* HASH_FINAL) ( struct SSL_HASH_S * pHash, SSL_BYTE * pData, SSL_WORD Length );
typedef void   					(* HASH_FREE)  ( struct SSL_HASH_S * pHash );


typedef struct						SSL_HASH_S
{
	SSL_BYTE							HashType;
   SSL_WORD                   DigestSize;
	SSL_WORD							BlockSize;
	HASH_UPDATE						Update;
	HASH_FINAL						Final;
	HASH_FREE						Delete;
} SSL_HASH;



/*
 * Function Prototypes
 */
extern SSL_HASH *
NullHash_New
(
	SSL_BYTE 					 * pKey,	
	SSL_WORD							KeyLen 
);

extern SSL_HASH *
InvalidHash_New
(
	SSL_BYTE 					 * pKey,	
	SSL_WORD							KeyLen 
);



/*
 * Globals
 */
extern HASH_NEW				   HashGen[ SSL_MAX_HASH ];



#endif
