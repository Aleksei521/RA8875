 /*******************************************************************************************
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
 *******************************************************************************************/
/*******************************************************************************************
* File			:	Ethopen.c
*
* Description	:	Ethernet device initializing manager function. 
*                   Interface between hardware and upper layers
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
*******************************************************************************************/

#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZThread.h"
#include "ZMessageQ.h"
#include "ZDevice.h"
#include "EtherMgr.h"


extern ETH_DEV_STRUCT_t emac[] ;
extern INT8 RZK_STANDALONE ;
extern void GetEtheraddr(ETH_ADDR *);
extern DDF_STATUS_t EthInitFunc(INT8 * , void (*)(void)) ;
extern void RxFunc(void);
extern INT _interface_init(void) ;	//Added for CR#5128


DDF_STATUS_t EmacOpen(RZK_DEVICE_CB_t *pDev, RZK_DEV_NAME_t *devName, RZK_DEV_MODE_t * mode)
{


    ETH_DEV_STRUCT_t  *pEthDev ;
    ETH_ADDR emacAddr ;
    UINT8 i ; // IAR port, changed from char to UINT8
    DDF_STATUS_t status ;
    
            if(pDev->devMode == RZKDEV_OPENED)
                    return EMACDEV_ERR_SUCCESS ;
    
    /* check this routine */
    GetEtheraddr((ETH_ADDR*)&emacAddr[0]);
    
    pEthDev = &emac[0] ;
    
    pDev->dvinputoutput = (UINT8 *)pEthDev ;
    
    pEthDev->pDev = pDev ;
    //pEthDev->edcPNif = NULL ;
    pEthDev->edcDescr = (INT8 * )  "F91 EMAC" ;


/* initialize mac address */

	for (i=0; i<6 ; ++i) {
		pEthDev->edcPAddr[i] = emacAddr[i];
		pEthDev->edcBCast[i] = 0xff;
	}
	status = EthInitFunc((INT8 *)&emacAddr[0], &RxFunc );


		if( status == EMACDEV_ERR_PEMAC_INIT_DONE )
		{
			pDev->devMode = RZKDEV_OPENED ;
		}

			/* This call is made only for stand alone RZK */
			if(RZK_STANDALONE)
					_interface_init() ;	//Added for CR#5128

			return EMACDEV_ERR_SUCCESS ;
}








