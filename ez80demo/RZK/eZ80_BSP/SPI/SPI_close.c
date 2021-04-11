/**********************************************************************************
* File       : spi_close.c
* Description: This file implements the SPI close function
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
********************************************************************************/


#include "ZSysgen.h"
#include "ztypes.h"
#include "ZThread.h"
#include "ZInterrupt.h"
#include "ZDevice.h"
#include "spi.h"

DDF_STATUS_t SPI_Close( RZK_DEVICE_CB_t * pDCB )
{
	DEV_SPI_CB_t *pDev = (DEV_SPI_CB_t *)pDCB->dvinputoutput ;
	RZK_STATE_t stat;
	RZK_STATUS_t status;
	if( (pDCB->devMode & RZKDEV_OPENED)!= RZKDEV_OPENED)
		return SPIDEV_ERR_INVALID_OPERATION ;

	stat = RZKDisablePreemption() ;

	status = RZKDeleteThreadEnhanced( pDev->spiIntThreadHdl ) ;
	if( status != RZKERR_SUCCESS )
	{
		RZKRestorePreemption(stat) ;
		return status ;
	}
	pDCB->devMode &= ~RZKDEV_OPENED;
	RZKRestorePreemption(stat) ;
	return RZKERR_SUCCESS ;
}
