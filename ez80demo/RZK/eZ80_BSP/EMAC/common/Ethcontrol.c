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
/*
* File			:	Ethcontrol.c
*
* Description	:	Ethernet IO control manager function. Interface between hardware
*                   and upper layers
*					
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

#include <stdio.h>
#include "ZDevice.h"
#include "EtherMgr.h"

void EmacEnableIrq( void );
void EmacDisableIrq( void );
void EthMAddFunc(INT8 * pAddr) ;
void EthMDelFunc(INT8 *	pAddr) ;
void EmacReset( void );


DDF_STATUS_t EmacControl(RZK_DEVICE_CB_t *pDev, RZK_DEV_BYTES_t func, INT8 *arg1, INT8 *arg2)
{

if( (pDev->devMode & RZKDEV_OPENED) != RZKDEV_OPENED )
{
	return EMACDEV_ERR_INVALID_OPERATION ;
}

switch((INT)func) {
	case EPC_MADD:
	{
		EthMAddFunc((INT8 *)arg1);
		break;
	}		
		
	case EPC_MDEL:
	{
		EthMDelFunc((INT8 *)arg1);
		break;
	}
		
	case EPV_IRQ_ENABLE:
	{
		EmacEnableIrq();
		break;
	}
	case EPV_IRQ_DISABLE:
	{
		EmacDisableIrq();
		break;
	}
	case EPV_RESET:
	{
		EmacReset();
		break;
	}
	default:
		printf("emac_control(): Unknown function %d\n", func );
	}	
return EMACDEV_ERR_SUCCESS ;
}








































