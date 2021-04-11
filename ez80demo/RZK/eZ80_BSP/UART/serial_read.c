 /***********************************************************************************
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
 ************************************************************************************/
/************************************************************************************
 * File			:	serial_read.c
 *
 * Description	:	Contains implementation of serial read function.
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
 ************************************************************************************/ 

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
UARTRead
(
	RZK_DEVICE_CB_t *				pDev,
	INT8 *							pBuf,
	RZK_DEV_BYTES_t					Len
)
{
	UART_DVIOBLK *					pDevBlk   = (UART_DVIOBLK *) pDev->dvinputoutput;
	UINT8							uart_base = pDevBlk->UARTbase;
	RZK_SEMAPHOREHANDLE_t			RxSem     = pDevBlk->RxSem;
	UINT8 *							pRxBuf    = pDevBlk->pRxBuf;
	UINT16							BytesRead = 0;
	UINT16 							ReadCount = 0;
	UINT16							Avail;
	UINT8 *							pData;
	UINT8 *							pPastBuf;
	UINTRMASK						IntMask ;
	RZK_STATUS_t    status;


	if( pDevBlk->State != SERIAL_STATE_OPENED )
	{
		return UARTDEV_ERR_INVALID_OPERATION ;
	}

	RZKThreadLockForDelete();
	pPastBuf = pRxBuf + pDevBlk->RxBufSize;
	pData = pDevBlk->pReadBuf;

	 RZKAcquireSemaphore(RxSem, INFINITE_SUSPEND) ;

	while( Len )
	{
		if( pDevBlk->State != SERIAL_STATE_OPENED )
		{			
			RZKReleaseSemaphore(pDevBlk->RxSem) ;
			RZKThreadUnLockForDelete();
			return UARTDEV_ERR_INVALID_OPERATION ;
		}

		IntMask = RZKDisableInterrupts();
		Avail = pDevBlk->RxAvail;
		RZKEnableInterrupts(IntMask);
		
		if( Avail )
		{
			BytesRead = 0;
			if( Avail > Len )
			{
				Avail = Len;
			}
			while( Avail-- )
			{
				*pBuf++ = *pData++;
				if( pData == pPastBuf )
				{
					pData = pRxBuf;
				}
				BytesRead++;
			}
			Len -= BytesRead;
			ReadCount += BytesRead;
			IntMask = RZKDisableInterrupts();
			pDevBlk->RxAvail -= BytesRead;
			pDevBlk->pReadBuf = pData;
			RZKEnableInterrupts(IntMask);
		}
		else
		{
			IntMask = RZKDisableInterrupts();
			pDevBlk->pRxTask = RZKGetCurrentThread() ;
			/*CR#6571 fix*/
			status = RZKSuspendThread(RZKGetCurrentThread(), pDevBlk->serparam->readDealy);
			RZKEnableInterrupts(IntMask);
			if (status == RZKERR_TIMEOUT)
			{
			    RZKReleaseSemaphore( pDevBlk->RxSem );
			    return(ReadCount);
			}

			if(pDevBlk->State == SERIAL_STATE_CLOSED)
			{
				RZKReleaseSemaphore( pDevBlk->RxSem );
				RZKThreadUnLockForDelete();
				return (DDFERR_DEVICE_CLOSED);
			}
		}
	}

	/*
	 * Flow back on once the receive buffer is close to empty.
	 */
	if( pDevBlk->serparam->settings & SERSET_RTSCTS )
	{
		IntMask = RZKDisableInterrupts();
		Avail = pDevBlk->RxAvail;
		RZKEnableInterrupts(IntMask);
		if( Avail <= FLOW_ON_LEVEL )
		{
			UART_MCTL(uart_base) |= MCTL_RTS;
		}
	}

	RZKReleaseSemaphore( RxSem );
	RZKThreadUnLockForDelete();
	return( ReadCount );
}



