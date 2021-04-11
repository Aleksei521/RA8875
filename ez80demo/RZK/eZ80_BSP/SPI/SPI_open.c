/*****************************************************************************
* File       : spi_open.c
* Description: This file implements the UART_Open function
* Copyright 2016 ZiLOG Inc.  ALL RIGHTS RESERVED.
*
* This file contains unpublished confidential and proprietary information
* of ZiLOG, Inc.
* NO PART OF THIS WORK MAY BE DUPLICATED, STORED, PUBLISHED OR DISCLOSED 
* IN ANY FORM WITHOUT THE PRIOR WRITTEN CONSENT OF ZiLOG, INC.
* This is not a license and no use of any kind of this work is authorized
* in the absence of a written license granted by ZiLOG, Inc. in ZiLOG's 
* sole discretion 
*****************************************************************************/

#include "ZSysgen.h"
#include "ztypes.h"
#include "ZThread.h"
#include "ZMessageQ.h"
#include "ZSemaphore.h"
#include "ZInterrupt.h"
#include "ZDevice.h"
#include "spi.h"

extern void spi_task(void) ;
extern UINT8 SPI_TASK_PRIO ;
extern UINT8 SPI_Task_Stack[] ;
extern UINT SPI_THD_STACK_SIZE;
DEV_SPI_CB_t spiDevStruct ; 

DDF_STATUS_t SPI_Open(RZK_DEVICE_CB_t * pDCB,RZK_DEV_NAME_t * devName,RZK_DEV_MODE_t  *devMode)
{
	DEV_SPI_CB_t *pdev = (DEV_SPI_CB_t *)pDCB->dvinputoutput ;
	RZK_STATE_t stat;
	RZK_STATUS_t status = 0;

	stat = RZKDisablePreemption() ;

	if( (pDCB->devMode & RZKDEV_OPENED)== RZKDEV_OPENED )
	{
		RZKRestorePreemption(stat) ;
		return SPIDEV_ERR_INVALID_OPERATION ;
	}

	/* Check if devMode is not equal to zero */
	if( (*devMode != 0) )
		pDCB->devMode = (*devMode) ;

	/* Install uart interrupt handler */
	RZKInstallInterruptHandler( (RZK_FNP_ISR)pDCB->dvintvector, PB2_IRQ_NUM ) ;

	/* Creating an interrupt thread to service the UART interrupts */
	pdev->spiIntThreadHdl = RZKCreateThread((RZK_NAME_t*)"SPI_TASK",(RZK_PTR_t)spi_task, NULL,(SPI_Task_Stack+SPI_THD_STACK_SIZE), 
						SPI_TASK_PRIO, 2, RZK_THREAD_INTERRUPT|RZK_THREAD_PREEMPTION,0 ) ;

	if( pdev->spiIntThreadHdl == NULL )
	{
		RZKRestorePreemption(stat) ;
		return SPIDEV_ERR_KERNEL_ERROR ;
	}
		
	pDCB->devMode |= RZKDEV_OPENED ;
	RZKRestorePreemption(stat) ;
	
	return status ;	
}
