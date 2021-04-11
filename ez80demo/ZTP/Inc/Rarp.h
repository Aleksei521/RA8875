/*
 * File       :     rarp.h
 *
 * Description:  This file contains the rarp defines and API declarations.
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
#include "ZTPtcp.h"

#define RA_REQUEST 0x03
#define RA_REPLY 0x04

extern INT16 rarpsend(UINT8 ifn);

#ifdef MULTIHOMING
extern INT _rarp_handler( arp_Header *in,UINT8 iface);
#else
extern int _rarp_handler( arp_Header *in);
#endif

extern RZK_MESSAGEQHANDLE_t RarpMQHandle;

