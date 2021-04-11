/****************************************************************************
 * File       : uart_conf.c
 *
 * Description: Defines the configurable system parameters for UART0 device
 *
 * Copyright 2016 ZiLOG Inc.  ALL RIGHTS RESERVED.
 *
 * This file contains unpublished confidential and proprietary information
 * of ZiLOG, Inc.
 * NO PART OF THIS WORK MAY BE DUPLICATED, STORED, PUBLISHED OR DISCLOSED 
 * IN ANY FORM WITHOUT THE PRIOR WRITTEN CONSENT OF ZiLOG, INC.
 * This is not a license and no use of any kind of this work is authorized
 * in the absence of a written license granted by ZiLOG, Inc. in ZiLOG's 
 * sole discretion 
 ***************************************************************************/

#include "serial.h"
#include "ZTypes.h"
#include "ZSysgen.h"
#include "ZDevice.h"
#include "_ez80.h"

#define UART0_THD_STACK_SIZEH           	(1024)
#define UART0_TASK_PRIOH                	(6)	/* CR#5167 */

#define UART1_THD_STACK_SIZEH           	(2048)
#define UART1_TASK_PRIOH                	(6)	/* CR#5167  */


extern UART_DVIOBLK Uart0_Blk ;
extern UART_DVIOBLK Uart1_Blk ;
extern DDF_STATUS_t IOERR( void );
extern void uart0isr( void );
extern void uart1isr( void );

/* CR#6571 */
struct serialparam serparams[2] = {{115200, 8, 1, PARNONE, 0, MAX_INFINITE_SUSPEND},
                                   {115200, 8, 1, PARNONE, 0, MAX_INFINITE_SUSPEND}
									};

/* UART0 Device Control Block Initialization Structure */
RZK_DEVICE_CB_t Serial0Dev =
{ 
	RZK_FALSE, "SERIAL0",
	UARTInit, (FNPTR_RZKDEV_STOP)IOERR, UARTOpen, UARTClose,
	(FNPTR_RZKDEV_READ)UARTRead, (FNPTR_RZKDEV_WRITE)UARTWrite, (FNPTR_RZKDEV_SEEK)IOERR,
	(FNPTR_RZKDEV_GETC)UARTGetc, (FNPTR_RZKDEV_PUTC)UARTPutc, (FNPTR_RZKDEV_IOCTL)UARTControl,
	(RZK_PTR_t)uart0isr, 0000, (UINT8*)&Uart0_Blk,
	 0,0
};

/* UART1 Device Control Block Initialization Structure */
RZK_DEVICE_CB_t Serial1Dev =
{ 
	RZK_FALSE, "SERIAL1",
	UARTInit, (FNPTR_RZKDEV_STOP)IOERR, UARTOpen, UARTClose,
	(FNPTR_RZKDEV_READ)UARTRead, (FNPTR_RZKDEV_WRITE)UARTWrite, (FNPTR_RZKDEV_SEEK)IOERR,
	(FNPTR_RZKDEV_GETC)UARTGetc, (FNPTR_RZKDEV_PUTC)UARTPutc, (FNPTR_RZKDEV_IOCTL)UARTControl,
	(RZK_PTR_t)uart1isr, 0000, (UINT8*)&Uart1_Blk,
	 0,1
};


// **************************
// DO NOT CHANGE ANYTHING BELOW IN THIS SECTION
// **************************

UINT8	b_uart0_iv		= IV_UART0 ;
UINT8	b_uart1_iv		= IV_UART1 ;

UINT8   UART0_TASK_PRIO						= UART0_TASK_PRIOH ;
UINT	UART0_TASK_STACK_SIZE	      	        = UART0_THD_STACK_SIZEH ; 


UINT8   UART1_TASK_PRIO      		       		= UART1_TASK_PRIOH ;
UINT	UART1_TASK_STACK_SIZE        	   		= UART1_THD_STACK_SIZEH ; 


