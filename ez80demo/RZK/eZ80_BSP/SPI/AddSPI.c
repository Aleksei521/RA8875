/********************************************************************************
* File			:	AddSpi.c
*
* Description	:	Adds a SPI device to the DDF device table.
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
*******************************************************************************/


#include "ZTypes.h"
#include "ZSysgen.h"
#include "ZDevice.h"

extern RZK_DEVICE_CB_t SpiDev ;


DDF_STATUS_t AddSpi(void)
{
	return RZKDevAttach( &SpiDev ) ;

}

