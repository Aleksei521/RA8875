 /*
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
 */
/*
 * File       : emacF91.h
 *
 * Description: Contains macros and structure declarations for the F91 MAC driver
 *
 * Copyright 2004 ZiLOG Inc.  ALL RIGHTS RESERVED.
 *
 * This file contains unpublished confidential and proprietary information
 * of ZiLOG, Inc.
 * NO PART OF THIS WORK MAY BE DUPLICATED, STORED, PUBLISHED OR DISCLOSED 
 * IN ANY FORM WITHOUT THE PRIOR WRITTEN CONSENT OF ZiLOG, INC.
 * This is not a license and no use of any kind of this work is authorized
 * in the absence of a written license granted by ZiLOG, Inc. in ZiLOG's 
 * sole discretion 
 */ 



#ifndef _EMACF91
#define _EMACF91

#include "ZTypes.h"
#include "EtherMgr.h"


#define EMAC_RAM_SIZE						0x2000
#define EMAC_RAM_OFFSET						0xC000

// descriptor buffer sizes
#define BUF256									0x00
#define BUF128									0x01
#define BUF64									0x02
#define BUF32									0x03

// buffer mask
#define BUFMSK256								0xFF00
#define BUFMSK128								0xFF80
#define BUFMSK64								0xFFC0
#define BUFMSK32								0xFFE0

#define ABITS									13

#define TBL_INIT								0x010b00

#define DATA_OS								(sizeof(DESC_TBL_t))
#define CRCLEN									4
#define MINPKTSZ								64

#define MAXDATASIZE         				1500
#define HEADERSIZE          				14
#define CRCSIZE             				4
#define MAXFRAMESIZE        				(MAXDATASIZE + HEADERSIZE + CRCSIZE)
#define MINFRAMESIZE			 				64

#define MIN_NUM_RX_BUFS		 				2

// tx statusH bit definition
#define EMAC_OWNS								0x8000	// 1=mac owns
#define HOST_OWNS								0x0000	// 0=host owns
#define TxOK									0x0000	// pkt transmitted ok
#define TxAbort								0x4000	// pkt aborted
#define TxBPA									0x2000	// back pressure applied
#define TxHuge									0x1000	// pktsize > maxf
#define TxLOOR									0x0800	// length out of range
#define TxLCError								0x0400	// length check error
#define TxCrcError							0x0200	// crc error
#define TxPktDeferred						0x0100	// pkt was deferred

// tx statusL bit definition
#define TxXsdfr								0x0080	// excessive defer
#define TxFifoUnderRun						0x0040	// fifo under run error
#define TxLateCol								0x0020	// late collision
#define TxMaxCol								0x0010	// max # collisions
#define TxNumColMask							0x000f	// # collisions

// rx statusH bit definition
#define RxOK									0x8000	// pkt received ok
#define RxAlignError							0x4000	// checks for an even # of nibbles
#define RxCrcError							0x2000   // checks for CRC# == FCS#
#define RxLongEvent							0x1000	// long event or dropped event
#define RxPCF									0x0800	// pause control frame
#define RxCF									0x0400	// control frame
#define RxMCPkt								0x0200	// multicast pkt
#define RxBCPkt								0x0100	// broadcast pkt

// rx statusL bit definition
#define RxVLAN									0x0080	// vlan frame type
#define RxUOpCode								0x0040	// unsupported opcode
#define RxLOOR									0x0020	// length out of range
#define RxLCError								0x0010	// length check error
#define RxCodeV								0x0008	// receive code violation
#define RxCEvent								0x0004	// carrier event previously seen
#define RxDvEvent								0x0002	// RXDV event previously seen
#define RxOVR									0x0001	// rx fifo overrun


#define PEMAC_NOT_FOUND 					-1
#define PEMAC_PHY_NOT_FOUND 				-2
#define PEMAC_PHYREAD_ERROR 				-3
#define PEMAC_PHYINIT_FAILED 				-4
#define PEMAC_SMEM_FAIL 					-5
#define ILLEGAL_CALLBACKS 					-6
#define PEMAC_INIT_DONE						0

#define OUT_OF_BUFS							4
#define TX_FULLBUF							3
#define TX_WAITING      					2
#define TX_DONE         					1
#define PKTTOOBIG	        					-2

/*
 * Macros for Mode member of F91_EMAC_CONF_S
 */
#define F91_10_HD								0
#define F91_10_FD								1
#define F91_100_HD							2
#define F91_100_FD							3
#define F91_AUTO								4


/* IVT Offsets */
#define EMACRx_IRQ_NUM		0
#define EMACTx_IRQ_NUM		1
#define EMACSYS_IRQ_NUM		2

#define	ep_type	ethPktHeader.ethType

typedef struct	F91_EMAC_CONF
{
	UINT16	txBufSize;	/* total tx buffer size in EMAC RAM */
	UINT16	mode;	/* set Phy operation to either 10 or 100 MBPS*/
	UINT16	bufSize;	/* EMAC_Ram packet buffer size*/	
} F91_EMAC_CONF_t ;


typedef struct DESC_TBL
{
	struct DESC_TBL*	np;	/* next pointer */
	UINT16		pktsz; /* Pointer size */
	UINT16		stat; /* Status */
} DESC_TBL_t;

DDF_STATUS_t EthPktTransmit (ETH_PKT_t * pep );
DDF_STATUS_t EthInitFunc(INT8 * ieeeaddr,void (*)(void));
void TransmitNextPkt (void);
void SendNow(EMAC_FRAME_t* databuff);
void ReceiveEthPkt(EMAC_FRAME_t * databuff);
void EthTransmit(void); 
void EthReceive(void);
void EmacReset( void );
void EmacEnableIrq( void );
void EmacDisableIrq( void );
INT8 MulticastHash(INT8 *ether_addr);
void EthMAddFunc(INT8 * pAddr) ;
void EthMDelFunc(INT8 *	pAddr) ;
void GetEtheraddr(ETH_ADDR *eaddr);
void * AllocPktBuff(void) ;
void FreePktBuff(void *) ;
void PutToQueue(QUEUE_REF_t *refnode, QUEUE_NODE_t *node) ;
QUEUE_NODE_t *GetFromQueue(QUEUE_REF_t *refnode) ;



#endif




