 /*******************************************************************************
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
 ********************************************************************************/
/********************************************************************************
 * File			:	serial_seek.c
 *
 * Description	:	Contains implementation of serial seek function.
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
UARTPeek
( 
	RZK_DEVICE_CB_t * 						pDev 
)
{
	UART_DVIOBLK *					pDevBlk   = (UART_DVIOBLK *) pDev->dvinputoutput;
	RZK_DEV_BYTES_t Count;
	UINT8								CritFlag;


	CritFlag = RZKDisablePreemption();
	if( pDevBlk->State == SERIAL_STATE_OPENED )
	{
		Count = pDevBlk->RxAvail;
	}
	else
	{
		Count = UARTDEV_ERR_INVALID_OPERATION ;		// Set proper error value
  	}
	RZKRestorePreemption(CritFlag);
	return( Count );
}



