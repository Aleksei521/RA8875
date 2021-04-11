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
 * File			:	serial_init.c
 *
 * Description	:	Contains implementation of serial initialization function.
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

#include "serial.h"
#include "_ez80.h"
#include "ZTypes.h"
#include "ZSysgen.h"
#include "ZDevice.h"
#include "ZThread.h"
#include "ZSemaphore.h"
#include "ZInterrupt.h"

extern UINT8	b_uart0_iv;
extern UINT8	b_uart1_iv;
extern DDF_STATUS_t IONULL( void ) ;
extern DDF_STATUS_t IOERR( void );

UINT8	UART0RxBuff[UART_RX_BUF_SIZE] ;
UINT8	UART1RxBuff[UART_RX_BUF_SIZE] ;


UART_DVIOBLK						Uart0_Blk = 
{
	SERIAL_STATE_UNINITIALIZED,
	NULL,
  	&serparams[0],
	0,
	NULL,
	NULL,
	NULL,
	NULL,
	0,
	0,
	NULL,
	NULL,
	NULL,
	UART0
};
UART_DVIOBLK						Uart1_Blk = 
{
	SERIAL_STATE_UNINITIALIZED,
	NULL,
  	&serparams[1],
	0,
	NULL,
	NULL,
	NULL,
	NULL,
	0,
	0,
	NULL,
	NULL,
	NULL,
	UART1
};


DDF_STATUS_t
UARTInit
(
	RZK_DEVICE_CB_t *		pDev
)
{
	UART_DVIOBLK *					pDevBlk = (UART_DVIOBLK *) pDev->dvinputoutput;
	UINT8								b_serial_iv;


	switch( pDev->dvminor )
	{
		case( 0 ):
		{
			b_serial_iv = b_uart0_iv;
			break;
		}
		case( 1 ):
		{
			b_serial_iv = b_uart1_iv;
			break;
		}
		default:
		{
			
			return( UARTDEV_ERR_INIT_FAILED );
		}
	}

	if( pDevBlk->State == SERIAL_STATE_UNINITIALIZED )
	{
		pDevBlk->State 	= SERIAL_STATE_CLOSED;
		pDevBlk->pDev		= pDev;
		if(pDev->dvminor == 0)
		{
			pDevBlk->pRxBuf   = UART0RxBuff ;	
		}
		else
		{
			pDevBlk->pRxBuf   = UART1RxBuff ;	
		}
			

		pDev->dvivec = b_serial_iv;

		return( UARTDEV_ERR_SUCCESS );
	}
	else
	{
		return( UARTDEV_ERR_INIT_FAILED );
	}
}

