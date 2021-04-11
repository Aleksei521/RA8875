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
/*********************************************************************************
 * File			:	serial_control.c
 *
 * Description	:	Contains implementation of serial IO control function.
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

extern UINT32 SYSTEM_CLOCK ;

DDF_STATUS_t UARTControl( 
					 RZK_DEVICE_CB_t *pDCB,	// device id
					 RZK_DEV_BYTES_t uOperation,
					 void *addr1,
					 void *addr2
						 )
{

	UART_DVIOBLK *	pDevBlk = (UART_DVIOBLK *)pDCB->dvinputoutput ;
	UINT16			 					bgen; /* baud rate generator valu */
	UINT8			 					tmp=LCTL_5DATABITS;
	TICK_t*    pReadDelay = addr1;

	if( pDCB == NULL )
		return UARTDEV_ERR_INVALID_ARGS ;


	RZKThreadLockForDelete();	//Lock Thread for deletion
	RZKAcquireSemaphore(pDevBlk->TxSem , INFINITE_SUSPEND ) ;

	RZKAcquireSemaphore(pDevBlk->RxSem , INFINITE_SUSPEND ) ;

	switch(uOperation)
	{

	case SET_ALL:
	case SET_BAUD :
			/* Set the baud rate */
			UART_LCTL(pDevBlk->UARTbase)|=LCTL_DLAB; /* DLAB */
			bgen=SYSTEM_CLOCK/((UINT32)pDevBlk->serparam->baud<<4);
			BRG_DLRL(pDevBlk->UARTbase)=(bgen&0x00ff);
			BRG_DLRH(pDevBlk->UARTbase)=(bgen&0xff00)>>8;
			UART_LCTL(pDevBlk->UARTbase)&=~LCTL_DLAB; /* DLAB */

			if(uOperation != SET_ALL)
				break ;

	case SET_DATABIT :
		{

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
		if(uOperation != SET_ALL)			
			break ;
		}

	case SET_PARITY :
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

			if(uOperation != SET_ALL)
				break ;

	case SET_STOPBITS :
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
			break ;

	 /* CR#6571 */
	 case SET_READ_DELAY:
	        pDevBlk->serparam->readDealy = *pReadDelay;
 			break;
	}

	if(uOperation != SET_BAUD && uOperation != SET_READ_DELAY)
		UART_LCTL(pDevBlk->UARTbase)=tmp;

	RZKReleaseSemaphore(pDevBlk->TxSem );
	RZKReleaseSemaphore(pDevBlk->RxSem );
	RZKThreadUnLockForDelete();	//Unlock thread 
	return UARTDEV_ERR_SUCCESS ;



}


