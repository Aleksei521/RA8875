 /***************************************************************************************
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
 **************************************************************************************/
/**************************************************************************************
* File			:	Ethaddr.c
*
* Description	:	Gets the ethernet MAC address.
*					
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
**************************************************************************************/

/*
 * Revision History:
 * MKC: CR# 5449: The GetEtherAddr function will return whatever address
 * is present in f91_mac_addr variable. The previous implementation of 
 * looking at a location in flash to determine the serialization is removed
 * and now serialization is made through the data persistence.
 */

#include "EtherMgr.h"


extern void* RZKMemcpy(void *, const void *, RZK_LENGTH_t); // IAR port, UINT changed to RZK_LENGTH_t

UINT8 defmacaddr[ETHPKT_ALEN] = {0x00,0x90,0x23,0x00,0x09,0x09} ;


extern const INT8	f91_mac_addr[ETHPKT_ALEN] ;

INT8 *		p_mac_addr = (INT8 *) f91_mac_addr;


/**
 * @memo Get ethernet physical address.
@doc This function Determines the MAC address for the system, and places it in
the location specified by it's parameter.
@param eaddr is a pointer to the location in which it place the MAC address.
 */
void
GetEtheraddr(ETH_ADDR *eaddr)
{

// CR 5449
	//unsigned char *flptr=(unsigned char *)0x3FFA;
//	UINT8 *	flptr = (UINT8 *)p_mac_addr;
	
	
	/* Look in the flash memory to see if a MAC address is present */
//	if( flptr[0] == 0xFF && flptr[1] == 0xFF && flptr[2] == 0xFF &&
//	    flptr[3] == 0xFF && flptr[4] == 0xFF && flptr[5] == 0xFF ) {
		/*  No MAC set in flash, use the default */
//		flptr=defmacaddr;
//		}
	
	RZKMemcpy((UINT8 *)eaddr,p_mac_addr,ETHPKT_ALEN);
}