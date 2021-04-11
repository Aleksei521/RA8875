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
 **********************************************************************************/
/**********************************************************************************
 * File			:	serial_task.c
 *
 * Description	:	Contains implementation of serial interrupt thread function.
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
 **********************************************************************************/ 

#include <stdio.h>
#include "serial.h"
#include "_ez80.h"
#include "ZTypes.h"
#include "ZSysgen.h"
#include "ZDevice.h"
#include "ZThread.h"
#include "ZSemaphore.h"
#include "ZInterrupt.h"


//#define SERIAL_DEBUG

UINT16				spill_count = 0;
UINT16 uart_rdr_no_rd =0 ;
RZK_THREADPARAMS_t ThdParams ;
RZK_THREADPARAMS_t *pThdParams = &ThdParams ;

void Uart_IntTask( UART_DVIOBLK * pDevBlk );
extern RZK_SEMAPHOREHANDLE_t hsem;
extern UINT8 eZ80ProductRevision;
void
Uart_IntTask
(
	UART_DVIOBLK *					pDevBlk
)
{
	UINT8							uart_base = pDevBlk->UARTbase;
	volatile UINT8					ier;
	volatile UINT8					lsr;
	volatile UINT8					msr;
	UINT8							ch;
	UINT8 *							pRxBuf = pDevBlk->pRxBuf;
	UINT8 *							pBuf = pDevBlk->pWriteBuf;
	UINT16							BufLen = pDevBlk->RxBufSize;
	UINT8 *							pPastBuf = pBuf + BufLen;
	UINT16							Avail;
//	RZK_SEMAPHOREHANDLE_t			Sem = pDevBlk->RxSem; // Commented during IAR Porting
	RZK_THREADHANDLE_t				pTask;
	UINT8							CritFlag;
	UINT16 							BytesRead;
	UINT8							do_cts_check = pDevBlk->serparam->settings & SERSET_RTSCTS;
	UINT8							do_dcd_check = !(pDevBlk->serparam->settings & SERSET_IGNHUP);
	UINT8							TxFlowOn = RZK_TRUE;
	UINTRMASK						IntMask ;
    UINT8  tempFCTL = 0;
	



	IntMask = RZKDisableInterrupts() ;

	lsr = UART_LSR(pDevBlk->UARTbase);
	msr = UART_MSR(pDevBlk->UARTbase);
	ier = UART_IIR(pDevBlk->UARTbase);

	while( 1 )
	{
		ier = UART_SPR( uart_base ) | UART_IER( uart_base );
		RZKEnableInterrupts(IntMask) ;
		lsr = UART_LSR( uart_base );
		msr = UART_MSR( uart_base );

		if( lsr& LSR_OE)
		{
		#ifdef SERIAL_DEBUG
			printf( "Serial Overrun \n" );
		#endif
		}

		if( ier & (IIR_RDR<<4) )
		{
			if( !(lsr&LSR_DR) )
			{
				/*
				 * In odd situations the Uart will trigger an Rx interrupt,
				 * but the LSR indicates that no data is available...
				 *
				 * The problem seems most likely to occur in cases where
				 * the UART interrupts were disabled while the remote was
				 * sending data.  
				 *
				 * This situation will immediately clear itself once the
				 * remote sends some data.  However, we can't be sure 
				 * this will ever happen.  Therefore to clear this condition,
				 * we momentarily put the Uart in loopback mode and transmit
				 * a single character.
				 */
				UART_MCTL(uart_base) |= MCTL_LOOP;
				UART_THR(uart_base) = 'Z';
				lsr=UART_LSR(uart_base);
				while( !(lsr&LSR_DR) )
				{
					/*
					 * CR# 2168
					 * Wait until data reaches the RBR before emptying FIFO
					 */
					lsr=UART_LSR(uart_base);
				}
				while(lsr&LSR_DR) 
				{
					// Toss everything we read - should just be 1 byte.
					lsr = UART_RBR(uart_base);
					lsr = UART_LSR(uart_base);
				}
		    	UART_MCTL( uart_base ) &= ~MCTL_LOOP;
				uart_rdr_no_rd++;
			}
		}
		ier &= 0x1F;

		if( lsr & LSR_DR )
		{
		   BytesRead = 0;
			IntMask = RZKDisableInterrupts() ;
			Avail = pDevBlk->RxAvail;
			RZKEnableInterrupts(IntMask) ;
			while( lsr & LSR_DR )
			{
				ch=UART_RBR(uart_base);

				if( (lsr & (LSR_BI | LSR_FE | LSR_PE | LSR_OE | LSR_DR)) == LSR_DR )
				{
					if( (Avail+BytesRead) < BufLen )
					{
						*pBuf++ = ch;
						if( pBuf == pPastBuf )
						{
							pBuf = pRxBuf;
						}
						BytesRead++;
					}
					else
					{
						spill_count++;
						#ifdef SERIAL_DEBUG
						printf( "Spill count %x\n", spill_count );
						#endif
					}
                            }
				lsr=UART_LSR(uart_base);
			}
			CritFlag= RZKDisablePreemption();
			pDevBlk->RxAvail += BytesRead;
			Avail = pDevBlk->RxAvail;

			/*
			 * If the Rx buffer is close to filling, flow off
			 */
			if( do_cts_check )
			{
				if( Avail >= FLOW_OFF_LEVEL )
				{
					UART_MCTL(uart_base) &= ~MCTL_RTS;
				}
			}

			pTask = pDevBlk->pRxTask;
			if( pTask && BytesRead )
			{
				pDevBlk->pRxTask = NULL;
				{	
				/* CR6571 */
					
						RZKResumeThread(pTask) ;
					
				}
			}
			RZKRestorePreemption(CritFlag);
		}

		if( (lsr & LSR_THRE) && (ier & IER_TIE) )
		{
			ier &= ~IER_TIE;
			if( TxFlowOn == RZK_TRUE )
			{
				CritFlag= RZKDisablePreemption();
				pDevBlk->TxCount = UART_FIFODEPTH;
				pTask = pDevBlk->pTxTask;
				if( pTask )
				{
					pDevBlk->pTxTask = NULL;
					{	
						RZKGetThreadParameters( pTask, pThdParams );
						if((ThdParams.uState & THREAD_INFINITESUSPEND) == THREAD_INFINITESUSPEND )
							RZKResumeThread(pTask) ;
					}
				}
				RZKRestorePreemption(CritFlag);
			}
		}

		if( (msr&MSR_DDCD) && !(msr&MSR_DCD) && (do_dcd_check) )
		{
			ier = 0;
			#ifdef SERIAL_DEBUG
                        printf( "DCD Dropped\n" );
			#endif
			pDevBlk->pDev->fnClose(pDevBlk->pDev);
		}

		if( do_cts_check )
		{
			if( msr&MSR_DCTS )
			{
				if( msr&MSR_CTS )
				{
					TxFlowOn = RZK_TRUE;
					ier |= IER_TIE;
				}
				else
				{
					TxFlowOn = RZK_FALSE;
				}
			}
		}

		IntMask = RZKDisableInterrupts() ;

		/*
		 * Re-enable HW interrupts
		 */
      UART_IER(uart_base) |= ier;
		RZKSuspendInterruptThread();
	}
}




