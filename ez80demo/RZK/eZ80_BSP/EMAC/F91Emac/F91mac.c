 /**********************************************************************************************
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
 **********************************************************************************************/
/********************************************************************************************
* File			:	F91mac.c
*
* Description	:	Driver source file for ez80F91 ethernet controller
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
******************************************************************************************/


#include <stdio.h>
#include <string.h>
#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZThread.h"
#include "ZTimer.h"
#include "ZMessageQ.h"
#include "ZInterrupt.h"
#include "ZDevice.h"
#include "ZSemaphore.h"
#include "_ez80.h"
#include "emacF91.h"
#include "EtherMgr.h"


extern RZK_DEVICE_CB_t*   CONSOLE;

#ifdef PPPoE
#include "PPPoE_Globals.h"
#define	PPP_PPPoE_EMAC_HEADER_LENGTH	22
#define	EMAC_PPPoE_HEADER_LENGTH		20
#define EMAC_HEADER_LENGTH				14
#define	EMAC_SRC_DSTN_LENGTH			12
#define	PPP_PPPoE_HEADER_LENGTH			8
#define	PPPoE_HEADER_LENGTH				6
#define PPP_HEADER_LENGTH				2
extern UINT16 intel16( UINT16 );
#endif

#define ETH_PKT_ADD_LEN 	6

extern	void		emacisr(void) ;
extern	void*	RZKMemcpy(void *, const void *, RZK_LENGTH_t) ;
extern	INT		phyInit(void) ;
extern	UINT	rxBlockTime ;
extern	UINT8	EMAC_Task_Stack[] ;
extern	UINT8	EMAC_TASK_PRIO ;
extern	UINT	EMAC_THD_STACK_SIZE ;
extern	UINT16	EMAC_MQ_SIZE ;
extern	UINT8	EMAC_Message_Buff[] ;

extern	RZK_MESSAGEQHANDLE_t rxQueueHandle ; /* receive queue handle */
extern	const F91_EMAC_CONF_t F91_emac_config ;

extern void emacConnThd(void) ;
extern INT putch(INT nch);

QUEUE_REF_t txQueueReference = { (QUEUE_NODE_t*)NULL, (QUEUE_NODE_t*)NULL}; 
INT8	* pInternalSramBase;	// 24-bit address of internal MAC SRAM

/*
 * Pointers into EMAC shared memory for buffer management.
*/
DESC_TBL_t *			twp;	/* Transmit write pointer */
DESC_TBL_t *			trp;	/* Transmit read pointer */
DESC_TBL_t *			rwp;	/* Receive write pointer */
DESC_TBL_t *			rrp;	/* Receive read pointer */
DESC_TBL_t *			tlbp;	/* Transmit low boundary pointer */
DESC_TBL_t *			bp;	/* Boundary Pointer */
DESC_TBL_t *			rhbp;	/* Receive high boundary pointer */

UINT16					bufMask;
UINT16					bufSize;
UINT16					bufSizeShift;

UINT16					txBufsLeft;	/* counts tx pkt buffers left */
UINT16					rxBufsLeft;	/* counts rx pkt buffers left*/

static INT32 			crc; /* For crc calculation */
static UINT32			polynomial;

/* Common to all drivers */
ETH_DEV_STRUCT_t       	emac[1];
void					(*	rxnotifyfunc)(void);
INT8              		bRxCount = 0; /* Received packet count */

/* Error counters */
UINT16       			crcErrors ;					
UINT16       			runtErrors	;
UINT16       			excessLengthErrors	;
UINT16       			rxMissCount	;
UINT16       			txColCount	;

INT8 					doingTx = RZK_FALSE; /* Transmit busy flag */
ETH_PKT_t*				txPepNext;
QUEUE_NODE_t			emacTxQueue;

/* These can be added to the device structure */
RZK_THREADHANDLE_t		emacInterruptThdHdl ;	/* emac interrupt thread handle */
RZK_THREADHANDLE_t		emacConnChkThdHdl ;	

//UINT8 EMAC_Conn_Stack[512] ;
RZK_SEMAPHOREHANDLE_t g_MacSem;

DDF_STATUS_t EthPktTransmit (ETH_PKT_t *	pep )
{

DDF_STATUS_t  status = pep->ethPktLen ;

	if( pep->ethPktLen <= (MAX_DATA_SIZE + HEADER_SIZE) ) 
	{

//		status = EMACDEV_ERR_TX_WAITING;
//		stat = RZKDisablePreemption();
		RZKAcquireSemaphore(g_MacSem,MAX_INFINITE_SUSPEND);
		PutToQueue(&txQueueReference, &pep->link) ;
		TransmitNextPkt();
		RZKReleaseSemaphore(g_MacSem);
//		RZKRestorePreemption(stat);
	}
	else
	{
		status = EMACDEV_ERR_PKTTOOBIG ;
	}

	return( status );

}


void TransmitNextPkt ()
{


	if( doingTx == RZK_FALSE )
	{
		txPepNext = (ETH_PKT_t*)GetFromQueue(&txQueueReference) ;
		if( txPepNext )
		{
			doingTx = RZK_TRUE;
			SendNow( txPepNext );
		}
	}

}


void SendNow(EMAC_FRAME_t* databuff)
{
	DESC_TBL_t * 	NextPkt;
	UINT16					Len = databuff->ethPktLen;
	UINT16					WrapLen;
	INT8 *				pSrc;
	INT8 *				pDst;


	/*
	 * Calculate the address of the emac packet buffer that will immediately 
	 * follow the one about to transmitted. Before giving the current
	 * packet to the emac HW, set the ownership of the next packet buffer to
	 * Host Owns so the emac HW does not try to transmit the frame.
	 *
	 * Note: this implementation only sends 1 packet at a time and the the 
	 * init routine has already verified that a max sized frame can fit in the 
	 * emac Tx buffer.  Therefore there is no need to track how many ehternet 
	 * buffers remain.
	 */

	NextPkt = (DESC_TBL_t*) ((INT8*)twp + ((Len+bufSize-1+DATA_OS) & (bufMask)));
	if( NextPkt >= bp )
	{
		NextPkt = (DESC_TBL_t*)((INT8*)tlbp + ((INT8*)NextPkt - (INT8*)bp));
	}
	NextPkt->stat = HOST_OWNS;

	twp->np = NextPkt;
	twp->pktsz = Len;
//	asm("\tdi") ;
	/*
	 * Copy packet data into the emac Tx buffer.  Check to see if the data will
	 * wrap around the end of the transmit buffer.
	 */
    pSrc = (INT8*)&databuff->ethPktHeader;
	pDst = (INT8*)twp+DATA_OS;

	WrapLen = (INT8*)bp - pDst;
#ifdef PPPoE
	{
		if( ( databuff->ethPktHeader.ethType == g_PPPoE_EthTypeDiscovery ) ||
			( databuff->ethPktHeader.ethType == g_PPPoE_EthTypeSession )
		  )
		{
//			UINT16 tempLen = Len ; // PPPoE header + Length
			g_PPPoETxHeader.payloadLength = intel16 ( Len - EMAC_PPPoE_HEADER_LENGTH );

			/** Copy the EMAC and PPPoE Headers **/
			RZKMemcpy(  pDst, 
						pSrc, 
						EMAC_HEADER_LENGTH 
					 ); //EMAC Header 

			RZKMemcpy( 	pDst + EMAC_HEADER_LENGTH, 
						&g_PPPoETxHeader, 
						PPPoE_HEADER_LENGTH 
					 ); //PPPoE Header 
			
			if( WrapLen >= Len )
			{	// No wrap
				/** Copy the rest of the data **/
				if( databuff->ethPktHeader.ethType == g_PPPoE_EthTypeSession )
				{
					RZKMemcpy( 	pDst + EMAC_PPPoE_HEADER_LENGTH, 
								&databuff->PPPoEField, 
								PPP_HEADER_LENGTH 
							  ); //PPP header
					RZKMemcpy(  pDst + PPP_PPPoE_EMAC_HEADER_LENGTH, 
								pSrc + EMAC_HEADER_LENGTH, 
								( Len - PPP_PPPoE_EMAC_HEADER_LENGTH ) 
							  ); // rest
				}
				else
				{
					RZKMemcpy(  pDst + EMAC_PPPoE_HEADER_LENGTH , 
								pSrc + EMAC_HEADER_LENGTH, 
								( Len - EMAC_PPPoE_HEADER_LENGTH ) 
							 ); // rest					
				}
			}
			else
			{
				if( databuff->ethPktHeader.ethType == g_PPPoE_EthTypeSession )
				{
					// Wrap
					RZKMemcpy(  pDst + EMAC_PPPoE_HEADER_LENGTH, 
								&databuff->PPPoEField, 
								PPP_HEADER_LENGTH
							 ); //PPP header
					RZKMemcpy(  pDst + PPP_PPPoE_EMAC_HEADER_LENGTH, 
								pSrc+EMAC_HEADER_LENGTH, 
								(WrapLen-PPP_PPPoE_EMAC_HEADER_LENGTH)
							 );
					RZKMemcpy( 	tlbp , 
								&pSrc[WrapLen-PPP_PPPoE_HEADER_LENGTH],
								(Len-WrapLen) 
							 );
				}
				else
				{
					RZKMemcpy( 	pDst + EMAC_PPPoE_HEADER_LENGTH , 
								pSrc+EMAC_HEADER_LENGTH, 
								(WrapLen-EMAC_PPPoE_HEADER_LENGTH)
							 );
					RZKMemcpy( 	tlbp , 
								&pSrc[WrapLen-PPPoE_HEADER_LENGTH], 
								(Len-WrapLen) 
							 );
				}
			}
		}
		else
		{
			if( WrapLen >= Len )
			{	// No wrap
				RZKMemcpy( pDst, pSrc, Len );
			}
			else
			{	// Wrap
				RZKMemcpy( pDst, pSrc, WrapLen );
				RZKMemcpy( tlbp, &pSrc[WrapLen], (Len-WrapLen) );
			}
		}
			
	}
#else
    if( WrapLen >= Len )
	{	// No wrap
		RZKMemcpy( pDst, pSrc, Len );
	}
	else
	{	// Wrap
		RZKMemcpy( pDst, pSrc, WrapLen );
		RZKMemcpy( tlbp, &pSrc[WrapLen], (Len-WrapLen) );
	}
#endif	
//	asm("\tei") ;
	/*
	 * Set the status to Emac owns so the HW will begin trnamitting the frame.
	 */
	twp->stat = EMAC_OWNS;
	twp = NextPkt;
}



void EthTransmit(void)
{
	DESC_TBL_t * 				HW_TRP;


//	stat = RZKDisablePreemption();
	RZKAcquireSemaphore(g_MacSem,MAX_INFINITE_SUSPEND);
	EMAC_ISTAT = TXDONE;

	/*
	 * Determine the location of HW Transmit Read Poiter
	 */
	HW_TRP = (DESC_TBL_t*)(pInternalSramBase + EMAC_TRP_L + ((UINT16)EMAC_TRP_H<<8));

	/*
	 * Verify the current tx frame was sent
	 */
	if( trp->stat & TxAbort )
	{
		// Give the frame back to the MAC
		trp = HW_TRP;
		SendNow( txPepNext );
//		SendNow( (EMAC_FRAME_t *) &txPepNext->ethPktOrder );
	}
	else
	{
		trp = HW_TRP;
		doingTx = RZK_FALSE;
      txPepNext->MacOwns = FALSE;
		TransmitNextPkt();
	}
	RZKReleaseSemaphore(g_MacSem);
//	RZKRestorePreemption(stat);
}

//Receive Packet Processing
void ReceiveEthPkt(EMAC_FRAME_t * databuff)
{
	UINT16        temp;
	INT8*     psrc;
	INT8*     pdst;
	UINT16        txferSz;
	UINT16 recvdFrmType ;

    txferSz = rrp->pktsz - CRCSIZE;

	// set databuff->Flags from rx descriptor status
	//***databuff->flags  = rrp->stat;
	databuff->ethPktLen = txferSz;
      
	// move the data from the rx ring buffers into system buffer
	pdst = (INT8*)&databuff->ethPktHeader;
	psrc = (INT8*)rrp+DATA_OS;

	recvdFrmType = *(( UINT16 *) ( psrc + 12 ));
	databuff->PPPoEField = recvdFrmType ;
#ifdef PPPoE
		
	if( ( recvdFrmType == g_PPPoE_EthTypeDiscovery )||
		( recvdFrmType == g_PPPoE_EthTypeSession )
		)
		{
			
			/*** Reduce PPPoE Header length from the Payload **/
			databuff->ethPktLen = txferSz - PPPoE_HEADER_LENGTH; 

			/** Copy EMAC Header **/
			RZKMemcpy(	pdst, 
						psrc, 
						EMAC_SRC_DSTN_LENGTH
					 );
			
			/** Copy PPPoE Header **/
			RZKMemcpy(	(VOID*)&g_PPPoERxHeader, 
						psrc+EMAC_HEADER_LENGTH, 
						PPPoE_HEADER_LENGTH
					 );

			//printf("\t PPPoE Code 0x%X", g_PPPoERxHeader.code );
			/** If the data length reaches boundary**/
			if (psrc+txferSz > (INT8*)rhbp)
			{
				/** Get the size of the leftover buffer excluding 
				    PPPoE and Ethernet headers **/
				if( recvdFrmType == g_PPPoE_EthTypeSession )
				{
					temp = (INT8*)rhbp - psrc - PPP_PPPoE_EMAC_HEADER_LENGTH;

					/** Copy PPP Header **/
					RZKMemcpy(	pdst + EMAC_SRC_DSTN_LENGTH, 
								psrc+EMAC_PPPoE_HEADER_LENGTH, 
								PPP_HEADER_LENGTH 
							 );

					/** Copy from the leftover buffer **/
					RZKMemcpy(	pdst + EMAC_HEADER_LENGTH, 
								psrc+PPP_PPPoE_EMAC_HEADER_LENGTH, 
								temp
							  );

					/** Copy from the start of the Rx Buffer pointer **/
					RZKMemcpy(	pdst + EMAC_HEADER_LENGTH + temp, 
								bp, 
								( txferSz-temp-PPP_PPPoE_EMAC_HEADER_LENGTH )
							 );
				}
				else
				{
					temp = (INT8*)rhbp - psrc - EMAC_PPPoE_HEADER_LENGTH;

					/** Copy from the leftover buffer **/
					RZKMemcpy(	pdst + EMAC_HEADER_LENGTH, 
								psrc+EMAC_PPPoE_HEADER_LENGTH, 
								temp
							 );

					/** Copy from the start of the Rx Buffer pointer **/
					RZKMemcpy(	pdst + EMAC_HEADER_LENGTH + temp, 
								bp, 
								( txferSz-temp-EMAC_PPPoE_HEADER_LENGTH )
							  );
				}
		    }
			else
			{
				if( recvdFrmType == g_PPPoE_EthTypeSession )
				{
					/** Copy PPP Header **/
					RZKMemcpy(	pdst+EMAC_SRC_DSTN_LENGTH, 
								psrc+EMAC_PPPoE_HEADER_LENGTH, 
								PPP_HEADER_LENGTH
							 ); //MK
					/** Copy the rest of the data **/
					RZKMemcpy(	pdst+EMAC_HEADER_LENGTH, 
								psrc+PPP_PPPoE_EMAC_HEADER_LENGTH, 
								( txferSz-PPP_PPPoE_EMAC_HEADER_LENGTH )
							  );
				}
				else
				{
					/** Copy the rest of the data **/
					RZKMemcpy(	pdst+EMAC_HEADER_LENGTH, 
								psrc+EMAC_PPPoE_HEADER_LENGTH, 
								( txferSz-EMAC_PPPoE_HEADER_LENGTH )
							 );	
				}
			}
		}
		else
		{
			if (psrc+txferSz > (INT8*)rhbp)
			{
				temp = (INT8*)rhbp - psrc;
				RZKMemcpy(pdst, psrc, temp);
				RZKMemcpy(&pdst[temp], bp, txferSz-temp);
		    }
		    else
			{
				RZKMemcpy(pdst, psrc, txferSz);
			}
		}
#else
    if (psrc+txferSz > (INT8*)rhbp)
	{
		temp = (INT8*)rhbp - psrc;
		RZKMemcpy(pdst, psrc, temp);
		RZKMemcpy(&pdst[temp], bp, txferSz-temp);
    }
    else
	{
		RZKMemcpy(pdst, psrc, txferSz);
	}
#endif 		
}







//Receive Interrupt Processing
void EthReceive(void) 
{

//	struct ETH_DEV_STRUCT_t	*	ped=&(emac[0]);
//	ETH_PKT_t *	pep;                    // Commented during IAR Porting
//	ETH_DEV_STRUCT_t	*ped=&emac[0];  // Commented during IAR Porting
	UINT16 temp;
//	RZK_STATUS_t status ;   // Commented during IAR Porting
   

	if( rrp->np == ((DESC_TBL_t*)(pInternalSramBase + EMAC_RWP_L + ((UINT16)EMAC_RWP_H<<8))) )
	{
		EMAC_ISTAT = RXDONE;//rx_stat;
	}

	// update error/event counters
	temp = rrp->stat;

	if( temp & RxCrcError )
		/*CRCErrors++*/;

	if (rrp->pktsz < MINFRAMESIZE)
	{
		/*RuntErrors++;*/
		temp &= ~RxOK;
	}
	if (rrp->pktsz > MAXFRAMESIZE)
	{
		/*ExcessLengthErrors++;*/
		temp &= ~RxOK;
	}

	if( (rxnotifyfunc != 0) && (temp & RxOK) )
	{
		rxnotifyfunc();
	}
   
   if(temp & (UINT16)RxOVR)
   {
//      kprintf( "F91 emac Rx Overrun.\n" );
      EMAC_ISTAT = RXOVRRUN;
   }   
   
	// update rrp to next pkt
	rrp = rrp->np;
	
	// update rrp i/o reg - for h/w rx buffer counting
	// must do this or we'll run outofbufs.
	EmacRrp = (UINT32)rrp;

}

void F91EmacTask( void )
{
   UINTRMASK            intmask;


	while( 1 )
	{
      while( EMAC_ISTAT & (TXFSMERR | RXDONE | TXDONE) )
      {
         if( EMAC_ISTAT & TXFSMERR )
         {//Reset the transmit state machine
            EMAC_RST = 0x14;
            EMAC_ISTAT &= TXFSMERR;
            EMAC_RST = 0;
            //printf( "EmacIsr ISTAT %x MIISTAT %x\r\n", EMAC_ISTAT, EMAC_MIISTAT );
         }
         if( EMAC_ISTAT & RXDONE )
         {
            EthReceive();
         }

         if( EMAC_ISTAT & TXDONE )
         {
            EthTransmit();
         }
      }

		intmask = RZKDisableInterrupts() ;
		EMAC_IEN = TXFSMERRIEN | RXDONEIEN | TXDONEIEN;									
		RZKSuspendInterruptThread() ;
		RZKEnableInterrupts(intmask) ;

	}
}








/***************************************************************************************************/
/* Accessory Functions */



//writing into the AMD 79C874 PHY registers.
UINT16 WritePhyReg(UINT16 phy_reg, UINT16 data)
{
	// write the phy reg
	EmacCtld= data;
	EMAC_RGAD = phy_reg;
	EMAC_MIIMGT |= LCTLD;
	// wait for mii done
	while(EMAC_MIISTAT & MGTBUSY);
	return EMACDEV_ERR_SUCCESS ;
}



//reading from the AMD 79C874 PHY registers.
UINT16 ReadPhyReg(UINT16 phy_reg, UINT16* data)
{
	// read the creg
	EMAC_RGAD = phy_reg;
	EMAC_MIIMGT |= RSTAT;
	// wait for mii done
	while(EMAC_MIISTAT & MGTBUSY);
	*data = (EMAC_PRSD_L + ((UINT16)EMAC_PRSD_H<<8));
	return EMACDEV_ERR_SUCCESS;
}



void EmacReset( void )
{
	EMAC_IEN = 0;
	// Reset all pemac blocks
	EMAC_RST = SRST | HRTFN | HRRFN | HRTMC | HRRMC | HRMGT;
}



void EmacEnableIrq( void )
{
	// enable the tx and rx interrupts
	EMAC_IEN = TXFSMERRIEN | RXDONEIEN | TXDONEIEN;
}



void EmacDisableIrq( void )
{
	// Disable the tx and rx interrupts
	EMAC_IEN = 0;
}


RZK_TIMERHANDLE_t emacTimerHdl ;

DDF_STATUS_t EthInitFunc(INT8 * ieeeaddr, void (*rxnotify)(void))
{
	//
	// ieeeaddr is pointer to a char array containing 6 bytes of
	// EMAC Station Address
	//
	UINT16    i;
	UINT16    temp;
	INT status ;

	/*
	 * Save the interrupt callback addresses
	 */
	rxnotifyfunc = rxnotify;	
	if( rxnotifyfunc == 0 )
	{
		return (ILLEGAL_CALLBACKS);
	}

	/*
	 * Ensure internal EMAC Ram is enabled. Calculate memory base address and clear it.
	 */
	pInternalSramBase = (INT8 *)((RAM_ADDR_U << 16) + (INT8*)EMAC_RAM_OFFSET);
	RAM_CTL |= ERAM_EN;
	memset( (INT8*)pInternalSramBase,0, EMAC_RAM_SIZE );

	EmacReset();
//	CRCErrors = RuntErrors = ExcessLengthErrors = RxMissCount = TxColCount = 0;
	polynomial = 0x04c11db7UL;

	// setup the emac station address
	for (i = 0; i < 6; i++) 
	{
		EmacStad(i) = *(ieeeaddr+i);
	}
	
	// check by reading back station address
	for (i = 0; i < 6; i++) 
	{
		if (EmacStad(i) != *(ieeeaddr+i))
			return(EMACDEV_ERR_PEMAC_NOT_FOUND);
	}

	// disable EMAC interrupts
	EMAC_IEN = 0;
	
	// clear interrupt status flags
	EMAC_ISTAT = 0xff;
	
	// setup buffer size
	if( F91_emac_config.bufSize > BUF32 )
	{
		bufSize = BUF32;
	}
	else
	{
		bufSize = F91_emac_config.bufSize;
	}
	bufSizeShift = 8 - bufSize;
	EMAC_BUFSZ = bufSize << 6;
	bufMask = 0xFFFF << bufSizeShift;
	bufSize = 1 << bufSizeShift;
//	kprintf( "Using packet buffer size %x, Mask %x, Shift %x\n", bufSize, bufMask, bufSizeShift );

	
	// set pause frame control timeout value
	EMAC_TPTV_L = 0x14;
	EMAC_TPTV_H = 0x00;

	/*
	 * Ensure there is enough room in the Tx buffer for at least 1
	 * full sized ethernet frame + one extra emac packet buffer.
	 */
	temp = (MAXDATASIZE + HEADERSIZE + sizeof(DESC_TBL_t) + (bufSize<<1) -1) & bufMask;
	if( ((F91_emac_config.txBufSize & bufMask) >= temp) 
	 && ((F91_emac_config.txBufSize & bufMask) <= EMAC_RAM_SIZE) )
	{
		temp = F91_emac_config.txBufSize & bufMask;
	}
	if( F91_emac_config.txBufSize != temp )
	{
		/*kprintf( "Adjusted F91 Tx buffer size to %u\n", temp )*/;
	}

	// init descriptor tables boundry pointers
	tlbp = (DESC_TBL_t*)(pInternalSramBase);
	bp   = (DESC_TBL_t*)(pInternalSramBase + temp);
	rhbp = (DESC_TBL_t*)(pInternalSramBase + EMAC_RAM_SIZE);
	//kprintf( "TLBP %x,  BP %x,  RHBP %x\n", tlbp, bp, rhbp );

	// load Emac i/o regs from pointers
   EMAC_TLBP_L = ((UINT32) tlbp);
	EMAC_TLBP_H = ((UINT32) tlbp) >> 8;
	EMAC_BP_L   = ((UINT32) bp);
	EMAC_BP_H   = ((UINT32) bp) >> 8;
	EMAC_BP_U   = ((UINT32) bp) >> 16;
	EMAC_RHBP_L = ((UINT32) rhbp);
	EMAC_RHBP_H = ((UINT32) rhbp) >> 8;

	//TOT notes
	//TRP =>  Transmit Read pointer, EMAC controller reads next pkt ot TX from here
	//TWP => Transmit Write pointer, Apliication write next pkt ot TX to here
	//RWP => received write pointer, EMAC controller write next pkt RXed here
	//RRP  => received read pointer, Apliication reads next pkt from here

	
	// setup s/w pointers to ring buffers
	twp = trp = tlbp;
	rwp = rrp = bp;
	
	// write rrp into i/o reg rrp
	EMAC_RRP_L = ((UINT32) rrp);
	EMAC_RRP_H = ((UINT32) rrp) >> 8;

	
	// host owns first buffer
	twp->stat = 0x0000;
	
	// set polling timer for minimum timeout period
	EMAC_PTMR = 1;
	
	// disable EMAC test modes
	EMAC_TEST = 0;
	
	// enable padding, crc generation, full duplex mode
// 	EMAC_CFG1 = PADEN | CRCEN | FULLD;
	
	// set the late collision byte at byte 56
	EMAC_CFG2 = 56;
	
	// set max # collisions = 15
	EMAC_CFG3 = 15;
	
	// Enable the EMAC receiver
	EMAC_CFG4 = RXEN;
	
	// Init the Address Filter
	EMAC_AFR = BROADCAST;

	#ifdef MULTICAST
	EMAC_AFR |= QMULTICAST;
	#endif
	// set the max # bytes per packet to 1536
	EMAC_MAXF_L = 0x00;
	EMAC_MAXF_H = 0x06;

	g_MacSem = RZKCreateSemaphore((RZK_NAME_t*)"g_MacSem",1,RECV_ORDER_PRIORITY);
	if(g_MacSem == NULL)
		printf("\nEmac Semaphore creation error");

   /*
    * Configure the MII management clock and bring the MII out of reset to 
    * initialize the PHY and establish an Ethernet link.  Bring the rest of the
    * EMAC out of reset after the PHY has been initialized.
    */
   EMAC_MIIMGT = CLKDIV20;
   EMAC_RST =  HRTFN | HRRFN | HRTMC | HRRMC;
   status = phyInit();
   EMAC_RST = 0;

	if(  status != EMACDEV_ERR_SUCCESS)
	{
		if(status == EMACDEV_ERR_PEMAC_PHYINIT_FAILED)
		{
				/* Added for initializing the link if cable has been pulled out during initialization */ // Against CR No 4401
		emacConnChkThdHdl = RZKCreateThreadEnhanced((RZK_NAME_t*)"EMACcon",
														(RZK_PTR_t)emacConnThd, 
														( FNP_THREAD_ENTRY* )NULL, 
														512, 
														2, 
														2,
														RZK_THREAD_PREEMPTION|RZK_THREAD_AUTOSTART,
														0 ) ;
			if( emacConnChkThdHdl == NULL )
			{
				return EMACDEV_ERR_RESOURCE_NOT_CREATED ;
			}
		}
		else
		{	
			printf(" EMAC Initialization error\n") ;
			return EMACDEV_ERR_PEMAC_PHYINIT_FAILED ;
		}
	}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//clear interrupt status register
	EMAC_ISTAT = 0xff;	

	// Check access to EMAC shared ram
	// don't overwrite tx status descriptor
	for (i=10; i<18; i++)
		pInternalSramBase[i] = i;

	for (i=10; i<18; i++) 
	{
		if( pInternalSramBase[i] != i )
			return (EMACDEV_ERR_PEMAC_SMEM_FAIL);
	}

	/*
	 * Create the Interrupt Task
	 */

/* Create Interrupt thread */
	emacInterruptThdHdl = RZKCreateThreadEnhanced((RZK_NAME_t*)"EMACIT",
													(RZK_PTR_t)F91EmacTask, 
													( FNP_THREAD_ENTRY* )NULL, 
													EMAC_THD_STACK_SIZE, 
													EMAC_TASK_PRIO, 
													2,
													RZK_THREAD_INTERRUPT|RZK_THREAD_PREEMPTION,
													0 ) ;
		if( emacInterruptThdHdl == NULL )
		{
			return EMACDEV_ERR_RESOURCE_NOT_CREATED ;
		}

	// setup EMAC interrupts
	RZKInstallInterruptHandler( (RZK_FNP_ISR)emacisr, IV_ERX) ;
	RZKInstallInterruptHandler( (RZK_FNP_ISR)emacisr, IV_ETX) ;
	RZKInstallInterruptHandler( (RZK_FNP_ISR)emacisr, IV_ESYS) ;


	// enable the tx and rx interrupts
	EMAC_IEN = TXFSMERRIEN | RXDONEIEN | TXDONEIEN;
//	EMAC_IEN = RXDONEIEN | TXDONEIEN | RXOVRRUNIEN;				// When system interrupt handler added

	return (EMACDEV_ERR_PEMAC_INIT_DONE);
}



/*
 * Internal function to calculate a multicast hash value for the receiver.
 * For this device, a bit mask is used to coarsely filter possible
 * multicast packets.  The bit is calculated by using the CRC algorithm
 * applied to the physical destination address, and then using the six
 * most significant bits as the integer bit index in the eight-byte mask.
 */
INT8 MulticastHash(INT8 *ether_addr)
{
	INT8 bitnum, current_octet;
	INT8 i;
	

	crc = -1;
	for(i=5; i >= 0; i--) {
		current_octet = *ether_addr++;
		for (bitnum = 0; bitnum < 8; bitnum++, current_octet >>= 1)
			crc = (crc << 1) ^ ((crc < 0) ^ (current_octet & 1) ? polynomial : 0);
	}
	return ((UINT32) crc) >> 26;
}



void EthMAddFunc
(
	INT8 *							pAddr
)
{
	INT8 							bitnum;
	INT8 							i;
	INT8							data;
	INT8 							MAR;
	RZK_STATE_t stat ;

//	kprintf( "Add multicast addr %02x:%02x:%02x:%02x:%02x:%02x\n",
//		pAddr[0],
//		pAddr[1],
//		pAddr[2],
//		pAddr[3],
//		pAddr[4],
//		pAddr[5] );
	bitnum = MulticastHash(pAddr);
	stat = RZKDisablePreemption();

	//not sure about TS generating proper code here. Use temporary variables.
	MAR = bitnum/8;
	data = EmacHtbl(MAR);
	data |= (1 << (bitnum%8));
	EmacHtbl(MAR) = data;

	data = 0;
	for(i=0; i < 8; i++)
	{
		data |= EmacHtbl(i);
	}

	//if any bit set, enable multicast
	if(data)
	{
		EMAC_AFR |= QMULTICAST;
	}

	RZKRestorePreemption(stat);
}



/*
 * Remove given address from the list supported in HW.
 */
void EthMDelFunc
(
	INT8 *							pAddr
)
{
	INT8 								bitnum;
	INT8 								i;
	INT8								data;
	INT8 								MAR;
	RZK_STATE_t stat ;

//	kprintf( "Delete multicast addr %02x:%02x:%02x:%02x:%02x:%02x\n",
//		pAddr[0],
//		pAddr[1],
//		pAddr[2],
//		pAddr[3],
//		pAddr[4],
//		pAddr[5] );
	bitnum = MulticastHash(pAddr);
	stat = RZKDisablePreemption();
	
	MAR = bitnum/8;
	data = ~(1 << (bitnum%8));
	data &= EmacHtbl(MAR);
	EmacHtbl(MAR) = data;

	data = 0;
	for(i=0; i < 8; i++)
	{
		data |= EmacHtbl(i);
	}

	//if no bit is set, disable multicast
	if(!data)
	{
		EMAC_AFR &= ~(QMULTICAST);
	}
	RZKRestorePreemption(stat);
}










