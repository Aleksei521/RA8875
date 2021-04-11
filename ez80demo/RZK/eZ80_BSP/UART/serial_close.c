 /*********************************************************************************
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
 *********************************************************************************/
/********************************************************************************
 * File			:	serial_close.c
 *
 * Description	:	Contains implementation of serial close function.
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

extern void InvalidInterruptHandler(void) ;
extern RZK_THREADHANDLE_t SerIntTasks[];

DDF_STATUS_t
UARTClose
(
	RZK_DEVICE_CB_t *			pDev
)
{
	UART_DVIOBLK *					pDevBlk = (UART_DVIOBLK *) pDev->dvinputoutput;
	INT16								Status;
	UINT8								CritFlag;


	if( pDevBlk->State != SERIAL_STATE_OPENED )
	{
		return UARTDEV_ERR_INVALID_OPERATION ;
	}

	CritFlag = RZKDisablePreemption();
	if( pDevBlk->State == SERIAL_STATE_OPENED )
	{
		/* disable UART interrupts */
		UART_IER( pDevBlk->UARTbase ) = 0;
		RZKInstallInterruptHandler(  (RZK_FNP_ISR) InvalidInterruptHandler, pDev->dvivec );
		UART_MCTL( pDevBlk->UARTbase ) = 0;

		pDevBlk->State = SERIAL_STATE_CLOSED;

		RZKDeleteSemaphore( pDevBlk->TxSem );
		RZKDeleteSemaphore( pDevBlk->RxSem );

		/*
		 * Release any tasks that were suspended on a Read or Write request.
		 */
		if( pDevBlk->pTxTask )
		{
			RZKResumeThread( pDevBlk->pTxTask );
		}
		if( pDevBlk->pRxTask )
		{
			RZKResumeThread( pDevBlk->pRxTask );			
		}

		RZKDeleteThreadEnhanced( SerIntTasks[pDev->dvminor] );
		Status = UARTDEV_ERR_SUCCESS;
	}
	else
	{
		Status = UARTDEV_ERR_INVALID_OPERATION;
	}
	RZKRestorePreemption(CritFlag);

	return( Status );
}


