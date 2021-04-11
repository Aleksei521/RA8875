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

#include "ez80_hash.h"
#include "ez80_md5.h"

#ifdef	MD5_HTTP
   HASH_NEW				   			HashGen[ SSL_MAX_HASH ] =
   {
      NULL,
      MD5_New,
      NULL,
      NULL,
      NULL
   };
#else
   #include "ez80_sha1.h"
   #include "ez80_hmac.h"
   #include "michael.h"

   HASH_NEW				   			HashGen[ SSL_MAX_HASH ] =
   {
      NullHash_New,	
      MD5_New,
      HMAC_MD5_New,
      SHA1_New,
      HMAC_SHA1_New,
      MICHAEL_New
   };
#endif
