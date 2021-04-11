/*
 * File       :     tftp.h
 *
 * Description:  This file contains the tftp defines and API declarations.
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


#ifndef _tftp_h
#define _tftp_h
#include "ZTypes.h"
#include "shell.h"//Meenakshi included this header file during IAR porting, CR####

#define TFTP_PORT		69 
#define TIMEOUTCOUNT		5
typedef struct _req_init
  {
    INT16 type;
    INT8 data[512];
  } req_init;
typedef struct _req_data
  {
    INT16 type;
    INT16 block;
    INT8 data[512];
  } req_data;
typedef struct _req_ack
  {
    INT16 type;
    INT16 block;
  } req_ack;
/* TFTP request types */
#define	TFTP_RRQ	1	/* Read Request */
#define	TFTP_WRQ	2	/* Write Request */
#define	TFTP_DATA	3	/* Data */
#define	TFTP_ACK	4	/* Acknowledgement */
#define	TFTP_ERROR	5	/* Error */

/* TFTP Error Types */
#define TFTP_ILL_OP 	4 /* Illegal TFTP operation */
#define TFTP_INV_TID	 	5 /* Illegal TFTP operation */


extern UINT32 ztpTftpTimeout;
/* tftp.c */
extern INT32 tftp_get(INT8 *Addr, INT8 *filename);
extern INT32 tftp_put(INT8 *Addr, INT8 *filename);

INT16 cmd_tftp_get (struct shvars * Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout,RZK_DEVICE_CB_t * stderr,UINT16 nargs,INT8* args[]);

INT16 cmd_tftp_put (  struct shvars * Shl, 
                        RZK_DEVICE_CB_t * stdin, 
                        RZK_DEVICE_CB_t * stdout,
                        RZK_DEVICE_CB_t * stderr,
                        UINT16 nargs,
                        INT8* args[]
                     );

#endif
