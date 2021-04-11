/*
 * File       :     smtp.h
 *
 * Description:  This file contains smtp related APIs.
 *
 * Copyright 2016 ZiLOG Inc.  ALL RIGHTS RESERVED.
 *
 * This file contains unpublished confidential and proprietary information
 * of ZiLOG, Inc.
 * NO PART OF THIS WORK MAY BE DUPLICATED, STORED, PUBLISHED OR DISCLOSED 
 * IN ANY FORM WITHOUT THE PRIOR WRITTEN CONSENT OF ZiLOG, INC.
 * This is not a license and no use of any kind of this work is authorized
 * in the absence of a written license granted by ZiLOG, Inc. in ZiLOG's 
 * sole discretion 
 */
/*
 * Copyright 2004, ZiLOG Inc.
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

#ifndef _smtp_h
#define _smtp_h
#include "ZTypes.h"

#ifndef SSL_OS_RZK
#define SSL_OS_RZK		//VG for MD5 defines of RZK
#endif

#define SMTP_PORT	25 

//extern SYSCALL mail(char *Addr, WORD port, char *subject, char *to, char *from,
//			char *data, char **error, WORD errorlen);
extern INT16 mail(INT8 *Addr, UINT16 port, INT8 *subject, INT8 *to, INT8 *from,INT8 *usrname ,INT8 *passwd,
	INT8 *data, INT8 *error, UINT16 errorlen);
extern INT16 smtp_authentication ( INT8 *in_out_buf, UINT16 in_out_bufsz, INT8 * fromDomain,INT8 *usrname, INT8 *passwrd ,
	 INT16 device );
extern	INT16 authenticate_smtp ( INT8 *in_out_buf, UINT16 in_out_bufsz,INT8 * fromDomain,INT8 *usrname, INT8 *passwrd ,
	 INT16 device );

#endif
