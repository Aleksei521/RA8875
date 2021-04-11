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

#ifndef _EZ80_MD5_H_
#define _EZ80_MD5_H_

#include "ssl_types.h"
#include "ez80_hash.h"

#define MD5_MAX_MESSAGE_SIZE_BYTES     (0xFFFFFFFFL - 0x10) // MK
#define MD5_HASH_SIZE_BYTES            16
#define MD5_BLOCK_SIZE_BYTES           64

#define MD5_INITIAL_A                  0x67452301
#define MD5_INITIAL_B                  0xEFCDAB89
#define MD5_INITIAL_C                  0x98BADCFE
#define MD5_INITIAL_D                  0x10325476



typedef struct 					MD5_BUFFER_S
{
	SSL_DWORD						A;
	SSL_DWORD						B;
	SSL_DWORD						C;
	SSL_DWORD						D;
} MD5_BUFFER_S;

typedef struct                MD5_HASH_S
{
   MD5_BUFFER_S               Hash;
   SSL_BYTE                   Buffer[MD5_BLOCK_SIZE_BYTES];
   SSL_DWORD                   Index; //MK on 25th June,2006
   SSL_DWORD                   MsgLength; //MK on 25th June,2006 
} MD5_HASH_S;





/*
 * Function Prototypes
 */
extern SSL_STATUS
MD5
(
	SSL_BYTE *						pMsg,
	SSL_WORD							MsgLen,
	SSL_BYTE *						pFinalDigest,
	SSL_WORD							DigestLength
);


extern SSL_HASH * MD5_New( 	SSL_BYTE * pKey, 	SSL_WORD KeyLen );
extern SSL_STATUS MD5_Update( SSL_HASH * pHash, SSL_BYTE * pData, SSL_WORD Length );
extern SSL_STATUS MD5_Final(  SSL_HASH * pHash, SSL_BYTE * pData, SSL_WORD Length );
extern void       MD5_Free(   SSL_HASH * pHash );



#endif
