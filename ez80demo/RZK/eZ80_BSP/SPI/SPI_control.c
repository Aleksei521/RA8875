/************************************************************************************
* File       : spi_control.c
* Description: This file implements the SPI control function
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
***********************************************************************************/



#include "ZSysgen.h"
#include "ztypes.h"
#include "ZThread.h"
#include "ZMessageQ.h"
#include "ZSemaphore.h"
#include "ZInterrupt.h"
#include "ZDevice.h"
#include "spi.h"

INT8 g_cAlt1,g_cAlt2,g_cDdr,g_cDr;
extern UINT32 SYSTEM_CLOCK;

void SPISlaveSel(INT8 cPort,UINT8 cPortByte) 


{
	UINT8 cMask;
	UINTRMASK mInterruptMask;

	cMask = 0xFF & (~cPortByte);

	mInterruptMask = RZKDisableInterrupts();
	switch(cPort)
	{
#ifdef _EZ80F91
		case 'A':
		{
			/* save port register contents when slave is selected*/
			g_cAlt2 = paalt2;
			g_cAlt1 = paalt1;
			g_cDdr = paddr;
			g_cDr = padr;
	
			/* select some particular port pin as slave device based on user input */
			paalt2 &= cMask;
			paalt1 &= cMask;
			paddr &= cMask;
			padr &= cMask;
			break;
		}
#endif
		case 'B':
		{
			/* save port register contents when slave is selected*/
			g_cAlt2 = pbalt2;
			g_cAlt1 = pbalt1;
			g_cDdr = pbddr;
			g_cDr = pbdr;
	
			/* select some particular port pin as slave device based on user input */
			pbalt2 &= cMask;
			pbalt1 &= cMask;
			pbddr &= cMask;
			pbdr &= cMask;
			break;
		}

		case 'C':
		{
			/* save port register contents when slave is selected*/
			g_cAlt2 = pcalt2;
			g_cAlt1 = pcalt1;
			g_cDdr = pcddr;
			g_cDr = pcdr;
	
			/* select some particular port pin as slave device based on user input */
			pcalt2 &= cMask;
			pcalt1 &= cMask;
			pcddr &= cMask;
			pcdr &= cMask;
			break;
		}
		case 'D':
		{
			/* save port register contents when slave is selected*/
			g_cAlt2 = pdalt2;
			g_cAlt1 = pdalt1;
			g_cDdr = pdddr;
			g_cDr = pddr;
	
			/* select some particular port pin as slave device based on user input */
			pdalt2 &= cMask;
			pdalt1 &= cMask;
			pdddr &= cMask;
			pddr &= cMask;
			break;
		}
	}
	RZKEnableInterrupts(mInterruptMask);
}

DDF_STATUS_t SPISlaveDeSel(INT8 cPort,INT8 cPortByte)
{
	UINTRMASK mInterruptMask;
	mInterruptMask = RZKDisableInterrupts();
	switch(cPort)
	{
#ifdef _EZ80F91
		case 'A':
		{
			paalt2 = g_cAlt2;
			paalt1 = g_cAlt1;
			paddr = g_cDdr;
			padr = g_cDr;
			break;
		}
#endif
		case 'B':
		{
			pbalt2 = g_cAlt2;
			pbalt1 = g_cAlt1;
			pbddr = g_cDdr;
			pbdr = g_cDr;
			break;
		}
		case 'C':
		{
			pcalt2 = g_cAlt2;
			pcalt1 = g_cAlt1;
			pcddr = g_cDdr;
			pcdr = g_cDr;
			break;
		}

		case 'D':
		{
			pdalt2 = g_cAlt2;
			pdalt1 = g_cAlt1;
			pdddr = g_cDdr;
			pddr = g_cDr;
			break;
		}
	}
	RZKEnableInterrupts(mInterruptMask);
	return RZKERR_SUCCESS;
}

void SPISetBaud(UINT32 ubaudRate)
{
	/* Assign Baud rate generator divisor value to BRG registers */
	UINTRMASK mInterruptMask;
	UINT uBaud;
	uBaud = ( SYSTEM_CLOCK / ( 2 * ( ubaudRate ) ) ) ;
	mInterruptMask = RZKDisableInterrupts();
	
	spi_brg_h = ( 0xFF00 & uBaud) >> 8 ;
	spi_brg_l = ( uBaud & 0x00FF ) ;
	
	RZKEnableInterrupts(mInterruptMask);
}

DDF_STATUS_t SPI_Ioctl(RZK_DEVICE_CB_t *pDCB,RZK_DEV_BYTES_t uOperation,INT8 *addr1,INT8 *addr2)
{
	switch(uOperation)
	{
	case SET_BAUD :
		SPISetBaud((*(UINT32 *)addr1));
		break ;
	case LOAD_CONTROL :
		spi_ctl |= (*addr1);
		break ;
	case SLAVE_SELECT :
		SPISlaveSel(*addr1,*addr2);
		break ;
	case SLAVE_DESELECT :
		SPISlaveDeSel(*addr1,*addr2);
		break ;
	case READ_STATUS :
		return spi_sr;
	}
	return 0;
}