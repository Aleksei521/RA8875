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
* File			:	Ethwrite.c
*
* Description	:	Ethernet write manager function. Interface between hardware
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



#define ep_type ethPktHeader.ethType
#define hs2net(x) ez80_hs2net( (x) )

extern DDF_STATUS_t EthPktTransmit (ETH_PKT_t * pep );


DDF_STATUS_t EmacWrite(RZK_DEVICE_CB_t *pDev, ETH_PKT_t *pep, RZK_DEV_BYTES_t len)
{
  RZK_DEV_BYTES_t size = len ;
//  ETH_DEV_STRUCT_t  *pEthDev ;  // IAR port
  
  if( (pDev->devMode & RZKDEV_OPENED) != RZKDEV_OPENED )
  {
          return EMACDEV_ERR_INVALID_OPERATION ;
  }
  
//  pEthDev = (ETH_DEV_STRUCT_t  *)pDev->dvinputoutput ;  // IAR port commented, dummy
  
  if( size > ETHPKT_MAXLEN)
  {
          /* free buffer Not needed*/
          return EMACDEV_ERR_INVALID_ARGS ;
  }
/*  
  size -= (INT8 *)&(pep->ethPktHeader) - (INT8*)pep;
  if(size < ETHPKT_MINLEN)
          size = ETHPKT_MINLEN ;*/
  
  
  pep->ethPktLen = len + sizeof( ETH_HEADER_t );
  //pep->ep_type = hs2net(pep->ep_type); //changed upon request from Ajay
  
  return EthPktTransmit (pep) ;
  

}






































