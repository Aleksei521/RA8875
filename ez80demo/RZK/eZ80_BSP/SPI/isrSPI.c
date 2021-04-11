/********************************************************************************
* File			:	isrspi.c
*
* Description	:	SPI interrupt task
*					
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
*******************************************************************************/

#include "ZSysgen.h"
#include "ztypes.h"
#include "ZThread.h"
#include "ZInterrupt.h"
#include "ZDevice.h"
#include "spi.h"

extern DEV_SPI_CB_t spiDevStruct ; 
/* SPI Interrupt task */
void spi_task(void)
{
	RZK_STATE_t stat ;

	while(1)
	{
		stat = RZKDisablePreemption() ;
		/* Re enabling spi interrupts */
		RZKResumeThread(spiDevStruct.hSPIThread);
		RZKSuspendInterruptThread() ;
		RZKRestorePreemption(stat) ;
	}
}/*! End of File */
