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

#ifndef _BASE64_H_
#define _BASE64_H_


#include "ssl_types.h"



#define INVALID_BASE64_CHAR				0xFF


extern SSL_WORD Base64Decode(	SSL_BYTE * pEncoded, SSL_WORD	EncLength );


#endif
