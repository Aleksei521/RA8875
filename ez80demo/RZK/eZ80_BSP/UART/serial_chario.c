/*********************************************************************************
 * File			:	serial_chario.c
 *
 * Description	:	Contains implementation of serial character IO functions.
 *					
 * Copyright 2016, ZiLOG Inc.
 * All Rights Reserved
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of ZiLOG Inc., and might
 * contain proprietary, confidential and trade secret information of
 * ZiLOG, our partners and parties from which this code has been licensed.
 * 
 * The contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of ZiLOG Inc.
 ********************************************************************************/
 
#include <stdio.h>
#include "serial.h"
#include "_ez80.h"
#include "ZTypes.h"
#include "ZSysgen.h"
#include "ZDevice.h"
#include "ZThread.h"
#include "ZSemaphore.h"
#include "ZInterrupt.h"


DDF_STATUS_t
UARTGetc
(
	RZK_DEVICE_CB_t *					pDev
)
{
	INT8								Data;
	INT16								Status;

	
	Status = UARTRead( pDev, (INT8 *)&Data, 1 );
	if( Status == 1 )
	{
		return( Data );
	}
	else
	{
		return( Status );
	}
}



DDF_STATUS_t
UARTPutc
(
	RZK_DEVICE_CB_t *				pDev,
	INT8							Data
)
{
	return( UARTWrite(pDev, &Data, 1) );
}



