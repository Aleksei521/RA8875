/**********************************************************************************
* File       : spi_readwrite.c
* 
* Description: This file implements the SPI_readwrite function
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
*********************************************************************************/

#include "ZSysgen.h"
#include "ztypes.h"
#include "ZThread.h"
#include "ZMessageQ.h"
#include "ZSemaphore.h"
#include "ZInterrupt.h"
#include "ZDevice.h"
#include "spi.h"

DDF_STATUS_t SPI_TRANSFER(RZK_DEVICE_CB_t *pDCB,UINT8 cReadOp,RZK_DEV_BUFFER_t *buf,RZK_DEV_BYTES_t nBytes)
{
	UINT8 l_cAlt2,l_cAlt1,l_cDdr,l_cDr,cError = 0; 
	RZK_DEV_BYTES_t nBytesTransfered = 0;
	UINTRMASK intrMask;
	RZK_STATE_t uState;
	DEV_SPI_CB_t *pDev = (DEV_SPI_CB_t *)pDCB->dvinputoutput ;
	UINT nLoop = 0;
	INT8 cTemp = spi_ctl;

	if( (pDCB->InUse & SPI_BUSY) == SPI_BUSY)
		return SPIDEV_ERR_BUSY ;

	/* Disable Interrupts */
	intrMask = RZKDisableInterrupts( ) ;
	pDCB->InUse = SPI_BUSY ;
	/* Enable Interrupts */
	RZKEnableInterrupts( intrMask ) ;

	/* Program PortB Alternate routines to work as a SPI device*/
	/* Four bits of port B are used for SPI. So they are made as alternate registers 
	PB7,PB6,PB3,PB2 are made as alternate registers */
	l_cAlt2 = pbalt2;
	l_cAlt1 = pbalt1;
	l_cDdr = pbddr;
	l_cDr = pbdr;

	pbalt2 |= 0xCC;
	pbalt1 &= 0x33;
	pbddr  |= 0xCC;
	pbdr   &= 0x33;
	
	if(!(cTemp & MASTER_EN))
		pbalt1 |= 0x04;

	pDev->nBytesTransfered = nBytesTransfered;
	pDev->nBytes = nBytes;
	pDev->buf = buf;
	pDev->cReadOp = cReadOp;
	pDev->hSPIThread = RZKGetCurrentThread();

	/* Actual read/write to to SPI */
	uState = RZKDisablePreemption() ;

	/* Actual read/write to to SPI */
	while(pDev->nBytesTransfered < pDev->nBytes)
	{
		/* To read any byte from slave a write has to be done. So this read call below does a write 
		to slave. */
		/* If it is Read operation then dump zero into TSR so as to read from slave */
		spi_tsr = pDev->buf[pDev->nBytesTransfered];

		while(1)
		{
			if(nLoop)
			{
				l_cDr = pbdr;
				/* If slave is deselected return from routine */
				if(l_cDr & 0x04)
				if(!(cTemp & MASTER_EN))
					return(pDev->nBytesTransfered - 1);
				else
				{
					INT i = 0;
					for(i = 0;i < 100; i++);
				}
			}
			
			if((cError = spi_sr) == 0x00)
			{
				if(cTemp & MASTER_EN)
					continue;
				else
				{
					/* Only for the first time suspend the current thread */
					if(!nLoop)
					{
						RZKSuspendThread(pDev->hSPIThread, MAX_INFINITE_SUSPEND);
						pbalt1 &= 0xFB;
						break;
					}
					continue;
				}
			}
			else if(cError == SPIF)
			{
				/* If it is Read operation then read RBR register */
				if(pDev->cReadOp)
					pDev->buf[pDev->nBytesTransfered] = spi_rbr;
				pDev->nBytesTransfered++;
				break;
			}
			else 
			{
				break;
			}
		}
		/* If any error had occured during data transfer report to user*/
		/* Also inform how many number of bytes were actually read/written */
		nLoop++;
		if((cError == WCOL) || (cError == MODF) )
			break;
		
		cError = spi_sr;
	}
	RZKRestorePreemption(uState);

	/* Reassign the old values of Port B registers after SPI operation is completed */
	pbalt2 = l_cAlt2;
	pbalt1 = l_cAlt1;
	pbddr = l_cDdr;
	pbdr = l_cDr;

	intrMask = RZKDisableInterrupts() ;
	pDCB->InUse &= ~(SPI_BUSY) ;
	RZKEnableInterrupts(intrMask) ;

	if(cError & WCOL)
		return(SPIDEV_ERR_WCOL);
	else if(cError & MODF)
		return(SPIDEV_ERR_MULTIMASTER);
	else
		return(pDev->nBytesTransfered - 1);
	/* Inform how many number of bytes were actually read/written */
}

DDF_STATUS_t SPI_Read(RZK_DEVICE_CB_t *pdev,RZK_DEV_BUFFER_t *buf,RZK_DEV_BYTES_t nBytes)
{
	UINT8 cReadOp = 1;
	/* Call generic spi read routine with user specified parameters*/
	return(SPI_TRANSFER(pdev,cReadOp,buf,nBytes));
}

DDF_STATUS_t SPI_Write(RZK_DEVICE_CB_t *pdev,RZK_DEV_BUFFER_t *buf,RZK_DEV_BYTES_t nBytes)
{
	UINT8 cReadOp = 0;
	/* Call generic spi read routine with user specified parameters*/
	return(SPI_TRANSFER(pdev,cReadOp,buf,nBytes));
}
