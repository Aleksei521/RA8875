 /**********************************************************************************
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
/*********************************************************************************
 * File			:	serial_open.c
 *
 * Description	:	Contains implementation of serial open function.
 *					
 * Copyright 2017, ZiLOG Inc.
 * All Rights Reserved
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of ZiLOG Inc., and might
 * contain proprietary, confidential and trade secret information of
 * ZiLOG, our partners and parties from which this code has been licensed.
 * 
 * The contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of ZiLOG Inc.
 **************************************************************************************/

#include <stdio.h>
#include "serial.h"
#include "_ez80.h"
#include "ZTypes.h"
#include "ZSysgen.h"
#include "ZDevice.h"
#include "ZThread.h"
#include "ZSemaphore.h"
#include "ZInterrupt.h"

extern UINT8 UART0_TASK_PRIO ;
extern UINT8 UART0_Task_Stack[] ;
extern UINT8 UART1_TASK_PRIO ;
extern UINT8 UART1_Task_Stack[] ;
extern UINT	UART0_TASK_STACK_SIZE ;
extern UINT	UART1_TASK_STACK_SIZE ;
extern UINT32 SYSTEM_CLOCK ;
extern void Uart_IntTask( UART_DVIOBLK * pDevBlk );

RZK_THREADHANDLE_t SerIntTasks[2];

void
UART_setparams
( 
	UART_DVIOBLK *					pDevBlk
)
{
	UINT16			 					bgen; /* baud rate generator valu */
	UINT8			 					tmp=LCTL_5DATABITS;


	/* Set the baud rate */
	UART_LCTL(pDevBlk->UARTbase) |= LCTL_DLAB; /* DLAB */
   bgen = SYSTEM_CLOCK/pDevBlk->serparam->baud;
   bgen = (bgen + 8) >> 4;
	BRG_DLRL(pDevBlk->UARTbase) = (UINT8)bgen;
	BRG_DLRH(pDevBlk->UARTbase) = (bgen&0xff00)>>8;
	UART_LCTL(pDevBlk->UARTbase) &= ~LCTL_DLAB; /* DLAB */

	switch( pDevBlk->serparam->databits ) 
	{
		case 6:
			tmp|=LCTL_6DATABITS;
			break;
		case 7:
			tmp|=LCTL_7DATABITS;
			break;
		case 8:
			tmp|=LCTL_8DATABITS;		break;
		default:
			printf("Unsupported value for databits %d\n",
			pDevBlk->serparam->databits);
	}

	switch( pDevBlk->serparam->stopbits ) 
	{
		case 1:
			break;
		case 2:
			tmp|=LCTL_2STOPBITS;
			break;
		default:
			printf("Unsupported value for stopbits %d\n",
			pDevBlk->serparam->stopbits);
	}

	switch( pDevBlk->serparam->parity ) 
	{
		case PAREVEN:
			tmp|=(LCTL_PEN|LCTL_EPS);
			break;
		case PARODD:
			tmp|=(LCTL_PEN);
			break;
		case PARNONE:
			break;
		default:
			printf("Unsupported value for parity %d\n",
			pDevBlk->serparam->parity);
	}

	/* Set the port attributes */
	UART_LCTL(pDevBlk->UARTbase)=tmp;
}


DDF_STATUS_t
UARTOpen
(
	RZK_DEVICE_CB_t *				pDev,
	RZK_DEV_NAME_t *				pName,
	RZK_DEV_MODE_t *							pMode
)
{
	UART_DVIOBLK *					pDevBlk = (UART_DVIOBLK *) pDev->dvinputoutput;
	UINT8								CritFlag;
   UINT8                      ier = IER_RIE;


	if( pDevBlk->State == SERIAL_STATE_OPENED )
	{
		return UARTDEV_ERR_SUCCESS ;
	}

	CritFlag = RZKDisablePreemption();
	if( pDevBlk->State == SERIAL_STATE_CLOSED )
	{
		pDevBlk->TxSem    = RZKCreateSemaphore((RZK_NAME_t*)"SERTX", 1, RECV_ORDER_PRIORITY) ;//KE_SemCreate( 1 );
		if(pDevBlk->TxSem == NULL)
			printf("\nUART Tx Semaphore creation error");
		pDevBlk->RxSem    = RZKCreateSemaphore((RZK_NAME_t*)"SERRX", 1, RECV_ORDER_PRIORITY) ;
		if(pDevBlk->RxSem == NULL)
			printf("\nUART Rx Semaphore creation error");
		pDevBlk->RxAvail   = 0;
		pDevBlk->RxBufSize = UART_RX_BUF_SIZE;// To be made a configurable parameter-- Sridhar
		pDevBlk->pReadBuf  = pDevBlk->pRxBuf;
		pDevBlk->pWriteBuf = pDevBlk->pRxBuf;
		pDevBlk->TxCount   = UART_FIFODEPTH;

		/*
		 * Disable Uart interrupts, while HW is configured.
		 */
   	UART_IER(pDevBlk->UARTbase)  = 0;
	UART_LCTL(pDevBlk->UARTbase) = 0;
	UART_MCTL(pDevBlk->UARTbase) = 0;
	UART_setparams(pDevBlk);

	/* Set up and flush the FIFOs */
	UART_FCTL(pDevBlk->UARTbase) = FCTL_TRIG_8 | FCTL_FIFOEN | FCTL_CLRTXF | FCTL_CLRRXF;

	 /* Flow control: tell sender we're ready for data */
   if(serparams[pDev->dvminor].settings & SERSET_RTSCTS)
	{
      ier |= IER_MIIE;
		UART_MCTL(pDevBlk->UARTbase) |= MCTL_RTS;
	}
   else
   {
      /* Enable modem status to detect changes in DCD */
      if( !(serparams[pDev->dvminor].settings & SERSET_IGNHUP) )
      {
         ier = ier |= IER_MIIE;
      }
   }

	if(serparams[pDev->dvminor].settings & SERSET_DTR_ON)
	{
		UART_MCTL(pDevBlk->UARTbase) |= MCTL_DTR;
	}

	/*
	 * Install the interrupt vector and create the interrupt task.
	 */

	if(pDev->dvminor == 0)
	{

		SerIntTasks[pDev->dvminor] = RZKCreateThreadEnhanced((RZK_NAME_t *)"SER0IT", (RZK_PTR_t)Uart_IntTask,
										NULL, UART0_TASK_STACK_SIZE,
										UART0_TASK_PRIO, 2,
										RZK_THREAD_INTERRUPT|RZK_THREAD_PREEMPTION,
										1, pDevBlk );
		if(SerIntTasks[pDev->dvminor] == NULL)
		{
			RZKRestorePreemption(CritFlag);
			return UARTDEV_ERR_KERNEL_ERROR ;
		}
	}
	else
	{

		SerIntTasks[pDev->dvminor] = RZKCreateThreadEnhanced((RZK_NAME_t *)"SER1IT", (RZK_PTR_t)Uart_IntTask,
										NULL, UART1_TASK_STACK_SIZE,
										UART1_TASK_PRIO, 2,
										RZK_THREAD_INTERRUPT|RZK_THREAD_PREEMPTION,
										1, pDevBlk );
		if(SerIntTasks[pDev->dvminor] == NULL)
		{
			RZKRestorePreemption(CritFlag);
			return UARTDEV_ERR_KERNEL_ERROR ;
		}
	}
									
	RZKInstallInterruptHandler((RZK_FNP_ISR) pDev->dvintvector, pDev->dvivec) ;
   UART_IER(pDevBlk->UARTbase) = ier;

	pDevBlk->State = SERIAL_STATE_OPENED;
	}
	else
	{
		RZKRestorePreemption(CritFlag);
		return UARTDEV_ERR_INVALID_OPERATION ;
	}
	RZKRestorePreemption(CritFlag);

	return UARTDEV_ERR_SUCCESS ;
}



