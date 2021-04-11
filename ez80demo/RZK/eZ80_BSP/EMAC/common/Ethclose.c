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
* File			:	Ethclose.c
*
* Description	:	Ethernet device close manager function. Interface between hardware
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


extern RZK_THREADHANDLE_t emacInterruptThdHdl ;
extern RZK_MESSAGEQHANDLE_t rxQueueHandle ;

DDF_STATUS_t EmacClose(RZK_DEVICE_CB_t *pdev)
{

  RZK_STATUS_t status ;

/* Disable all Emac interrupts */
//
//
//
//
//
/* Not sure if it is necessary?? */

  /* Delete the resources created */
  status = RZKDeleteThreadEnhanced( emacInterruptThdHdl) ;
  if(status != RZKERR_SUCCESS )
  {
          return EMACDEV_ERR_KERNEL_ERROR ;
  }
  
  status = RZKDeleteQueue(rxQueueHandle) ;
  if(status != RZKERR_SUCCESS )
  {
          return EMACDEV_ERR_KERNEL_ERROR ;
  }
  
  pdev->devMode = ~(RZKDEV_OPENED) ;
  
  return EMACDEV_ERR_SUCCESS ;

}













































