 /*************************************************************************************
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
 *************************************************************************************/
/*************************************************************************************
 * File			:	serial.h
 *
 * Description	:	Contains serial driver macro and structure definitions.
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
 ***********************************************************************************/
 

#ifndef _SERIAL_H_
#define _SERIAL_H_

#include "ZTypes.h"
#include "ZTypes.h"
#include "ZSysgen.h"
#include "ZDevice.h"

typedef enum { PAREVEN, PARODD, PARNONE } serparity;

typedef struct serialparam {
	UINT32	baud;
	UINT16	databits;
	UINT16	stopbits;
	serparity parity;
	UINT16	settings;
	TICK_t  readDealy; /*Venkat - CR#6571 fix */
}SERIAL_PARAMS;

/* Serial Port Settings */
/* Flow control bits */
#define SERSET_DTR_ON	0x8	/* Assert DTR on open, reset it on close */
#define SERSET_RTSCTS	0x4	/* Use RTS/CTS HW flow control */
#define SERSET_DTRDSR	0x2	/* Use DTR/DSR HW flow control */
#define SERSET_XONXOFF	0x1	/* Use XON/XOFF SW flow control */

/* Misc. settings */
#define SERSET_ONLCR	0x10	/* Map NL to CR-NL on output */
#define SERSET_SYNC		0x20	/* Use Synchronous routines instead of interrupts */
#define SERSET_IGNHUP	0x40	/* Ignore Hangup (CD drop) */

#define UART_FIFODEPTH			16
#define FLOW_ON_LEVEL			128
#define UART_RX_BUF_SIZE		4096
#define FLOW_OFF_LEVEL			(UART_RX_BUF_SIZE-(UART_FIFODEPTH<<2))

extern struct serialparam serparams[];


typedef struct						UART_BLK_S
{
	UINT8							State;
	RZK_DEVICE_CB_t *				pDev;
	SERIAL_PARAMS *					serparam;
	UINT8							TxCount;
	RZK_THREADHANDLE_t				pTxTask;
	RZK_SEMAPHOREHANDLE_t			TxSem;
	RZK_SEMAPHOREHANDLE_t			RxSem;
	RZK_THREADHANDLE_t				pRxTask;
	UINT16							RxBufSize;
	UINT16							RxAvail;
	UINT8 *							pRxBuf;
	UINT8 *							pReadBuf;
	UINT8 *							pWriteBuf;
	UINT8							UARTbase;
} UART_DVIOBLK;


typedef enum 
{
	SERIAL_STATE_UNINITIALIZED,
	SERIAL_STATE_CLOSED,
	SERIAL_STATE_OPENED
} SERIAL_STATE_E;


/* UART status values */
typedef enum {
	UARTDEV_ERR_SUCCESS = 0,
	UARTDEV_ERR_OVERRUN = -20,
	UARTDEV_ERR_INIT_FAILED,
	UARTDEV_ERR_INVALID_OPERATION,
	UARTDEV_ERR_INVALID_ARGS,
	UARTDEV_ERR_TEMP_BUSY,
	UARTDEV_ERR_KERNEL_ERROR,
	UARTDEV_ERR_INVALID_BAUDRATE,
	UARTDEV_ERR_INVALID_DATABITS,
	UARTDEV_ERR_INVALID_STOPBITS,
	UARTDEV_ERR_INVALID_PARITY_TYPE,
	UARTDEV_ERR_INVALID_PARITY_OP
} UARTDEV_ERR_NUMS ;


typedef enum {
	DISABLE_PARITY,
	ENABLE_PARITY,
	SET_BAUD,
	SET_DATABIT,
	SET_PARITY,
	SET_STOPBITS,
	SET_FIFO_TRIG_LEVEL,
	SET_ALL,
	DISABLE_UART_INTR,
	ENABLE_UART_INTR,
	GET_FIFO_COUNT,
	SET_READ_DELAY /* CR#6571 */
	
} UARTDEV_IOCTL_FUNC ; 


DDF_STATUS_t UARTInit(RZK_DEVICE_CB_t *	pDev);
DDF_STATUS_t UARTOpen(RZK_DEVICE_CB_t *pDev, RZK_DEV_NAME_t *pName, RZK_DEV_MODE_t *pMode);
DDF_STATUS_t UARTClose(RZK_DEVICE_CB_t *pDev);
DDF_STATUS_t UARTRead(RZK_DEVICE_CB_t *pDev,INT8 *pBuf, RZK_DEV_BYTES_t	Len);
DDF_STATUS_t UARTWrite(RZK_DEVICE_CB_t *pDev,INT8 *pBuf, RZK_DEV_BYTES_t Len);
DDF_STATUS_t UARTControl(RZK_DEVICE_CB_t *pDCB, RZK_DEV_BYTES_t uOperation,	void *addr1, void *addr2);
DDF_STATUS_t UARTGetc(RZK_DEVICE_CB_t *pDev);
DDF_STATUS_t UARTPutc(RZK_DEVICE_CB_t *pDev, INT8 Data);
DDF_STATUS_t UARTPeek(RZK_DEVICE_CB_t *pDev);

DDF_STATUS_t AddUART0(void);
DDF_STATUS_t AddUART1(void);

#endif
