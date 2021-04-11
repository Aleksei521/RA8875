 /******************************************************************************
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
 *******************************************************************************/
/*******************************************************************************
 * File			:	serial_write.c
 *
 * Description	:	Contains implementation of serial write function.
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
 *******************************************************************************/
 
#include <stdio.h>
#include "serial.h"
#include "_ez80.h"
#include "ZTypes.h"
#include "ZSysgen.h"
#include "ZDevice.h"
#include "ZThread.h"
#include "ZSemaphore.h"
#include "ZInterrupt.h"
extern UINT8 eZ80ProductRevision;

DDF_STATUS_t
UARTWrite
(
	RZK_DEVICE_CB_t *				pDev,
	INT8 *							pBuf,
	RZK_DEV_BYTES_t					Len
)
{
	UINT8								Data;
	UART_DVIOBLK *					pDevBlk = (UART_DVIOBLK *) pDev->dvinputoutput;
	UINT8								uart_base = pDevBlk->UARTbase;
	UINT8								do_crlf_check = pDevBlk->serparam->settings & SERSET_ONLCR;
	UINT8								do_cts_check = pDevBlk->serparam->settings & SERSET_RTSCTS;
	UINT8								do_sync =  pDevBlk->serparam->settings & SERSET_SYNC;
	UINT8								send_lf = RZK_FALSE;
	UINT8								msr;
	UINTRMASK						IntMask ;
	RZK_DEV_BYTES_t					txSize = Len ;
	UINT8  tempFCTL = 0;
       static UINT8  setupFlag = 0;

	if( pDevBlk->State != SERIAL_STATE_OPENED )
	{
		return UARTDEV_ERR_INVALID_OPERATION ;
	}
	
	RZKThreadLockForDelete();

	RZKAcquireSemaphore(pDevBlk->TxSem , INFINITE_SUSPEND ) ;

	while( Len )
	{
		if( pDevBlk->State != SERIAL_STATE_OPENED )
		{
			RZKReleaseSemaphore(pDevBlk->TxSem) ;
			RZKThreadUnLockForDelete();
			return UARTDEV_ERR_INVALID_OPERATION ;
		}

		/*
		 * Defer sending if the peer flows off.  If this happens the current task
		 * will block until the interrupt handler detects CTS has been reasserted.
		 */
		if( do_cts_check )
		{
			IntMask = RZKDisableInterrupts();
			msr = UART_MSR( uart_base );
			if( !(msr&MSR_CTS) )
			{
				pDevBlk->pTxTask = RZKGetCurrentThread();
				UART_IER(uart_base) |= IER_TIE;
				RZKSuspendInterruptThread();

				if(pDevBlk->State == SERIAL_STATE_CLOSED)
				{
					RZKReleaseSemaphore( pDevBlk->TxSem );
					RZKThreadUnLockForDelete();
					return (DDFERR_DEVICE_CLOSED);
				}
			}
			RZKEnableInterrupts(IntMask);
			
		}

		IntMask = RZKDisableInterrupts();
		if( pDevBlk->TxCount )
		{
			pDevBlk->TxCount--;
			RZKEnableInterrupts(IntMask);

			// Put the next byte in the Tx fifo
			if( send_lf == RZK_TRUE )
			{
				send_lf = RZK_FALSE;
				UART_THR(uart_base) = '\n';
				Len--;
			}
			else
			{
				Data = *pBuf++;
				if( (do_crlf_check) && (Data == '\n') ) 
				{
					send_lf = RZK_TRUE;
					UART_THR(uart_base) = '\r';
				}
				else
				{
					Len--;				
				UART_THR(uart_base) = Data;
                                }
			}
		}
		else
		{
			RZKEnableInterrupts(IntMask);
			Data = UART_LSR(uart_base);
			if( do_sync ) 
			{	// Synchronous Tx
				while( !(Data & LSR_THRE) )
				{
					Data = UART_LSR(uart_base);	
				}
				pDevBlk->TxCount = UART_FIFODEPTH;
			}
			else
			{	
				if( Data & LSR_THRE )
				{
					pDevBlk->TxCount = UART_FIFODEPTH;
				}
				else
				{	// Interrupt-driven Tx
					IntMask = RZKDisableInterrupts();
					pDevBlk->pTxTask = RZKGetCurrentThread() ;
					UART_IER(uart_base) |= IER_TIE;
					
					RZKSuspendInterruptThread();
					RZKEnableInterrupts(IntMask);
					if(pDevBlk->State == SERIAL_STATE_CLOSED)
					{
						RZKReleaseSemaphore( pDevBlk->TxSem );
						RZKThreadUnLockForDelete();
						return (DDFERR_DEVICE_CLOSED);
					}
				}
			}
		}
	}
	RZKReleaseSemaphore(pDevBlk->TxSem );
	RZKThreadUnLockForDelete();
	return  txSize ;
}		

