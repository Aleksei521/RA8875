/*
 * File       :     smtp_conf.c
 *
 * Description:  This file contains SMTP protocol implementation.
 *
 * Copyright 2004 ZiLOG Inc.  ALL RIGHTS RESERVED.
 *
 * This file contains unpublished confidential and proprietary information
 * of ZiLOG, Inc.
 * NO PART OF THIS WORK MAY BE DUPLICATED, STORED, PUBLISHED OR DISCLOSED 
 * IN ANY FORM WITHOUT THE PRIOR WRITTEN CONSENT OF ZiLOG, INC.
 * This is not a license and no use of any kind of this work is authorized
 * in the absence of a written license granted by ZiLOG, Inc. in ZiLOG's 
 * sole discretion 
 */
#include "smtp.h"

extern INT16 smtp_authentication ( INT8 *in_out_buf, UINT16 in_out_bufsz, INT8 * fromDomain, INT8 *usrname, 
							INT8 *passwrd , INT16 device );

INT16 authenticate_smtp ( INT8 *in_out_buf, UINT16 in_out_bufsz, INT8 * fromDomain, INT8 *usrname, INT8 *passwrd ,
						 INT16 device )
{
	return smtp_authentication ( in_out_buf, in_out_bufsz,fromDomain, usrname, passwrd ,  device );
}
