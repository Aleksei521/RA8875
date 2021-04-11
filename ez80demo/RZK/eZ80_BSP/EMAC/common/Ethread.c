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
* File			:	Ethread.c
*
* Description	:	Ethernet read manager function. Interface between hardware
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
#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZThread.h"
#include "ZMessageQ.h"
#include "ZDevice.h"
#include "EtherMgr.h"
#include "common.h"

#include <stdio.h>

#ifdef PPPoE
#include "PPPoE_Globals.h"
#endif

extern void					ReceiveEthPkt(EMAC_FRAME_t * ) ;
extern RZK_MESSAGEQHANDLE_t rxQueueHandle ;
extern TICK_t				rxBlockTime ;
extern ETH_DEV_STRUCT_t		emac[];


extern UINT16 intel16( UINT16 );

DDF_STATUS_t EmacRead(RZK_DEVICE_CB_t *pDev, ETH_PKT_t **pep, RZK_DEV_BYTES_t len)
{
  
  RZK_PTR_t pktrecvbuff ;
  COUNT_t size = sizeof(RZK_PTR_t) ;  // IAR changed
  RZK_STATUS_t status = EMACDEV_ERR_SUCCESS ;
  
  if( (pDev->devMode & RZKDEV_OPENED) != RZKDEV_OPENED )
  {
          return EMACDEV_ERR_INVALID_OPERATION ;
  }
  
  /* Network interface not supported Yet */
  
  /********************************/
	  /* Receive the packet pointer */
	  status = RZKReceiveFromQueue( rxQueueHandle, (RZK_PTR_t) &pktrecvbuff, &size, rxBlockTime) ;  // IAR changed
	  if(status != RZKERR_SUCCESS)
	  {
	      return EMACDEV_ERR_KERNEL_ERROR ;
	  }

 	  *pep = (ETH_PKT_t *)pktrecvbuff ;
	  len = (*pep)->ethPktLen ;
	  
	  return len ;
}
