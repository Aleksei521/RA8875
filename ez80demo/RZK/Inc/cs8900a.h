 /*******************************************************************************************
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
 *******************************************************************************************/
/*******************************************************************************************
 * File			:	cs8900.h
 *
 * Description	:	Driver header file for the crystal ethernet controller.
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
 *******************************************************************************************/

#ifndef _CS8900A_H_
#define _CS8900A_H_

#include "ZTypes.h"
#include "EtherMgr.h"

//------------------------------------------------------------------------------
#define IOBASE              0x300
#define CS8900ID            0x630E  
#define MAXDATASIZE         1500
#define HEADERSIZE          14
#define CRCSIZE             4
#define MAXFRAMESIZE        (MAXDATASIZE + HEADERSIZE + CRCSIZE)




//------------------------------------------------------------------------------
#define RELOCATABLE_MAC
#ifndef RELOCATABLE_MAC
	#define DATAPORT0          (*(IORegExt16)(IOBASE + 0x0))		
	#define DATAPORT1          (*(IORegExt16)(IOBASE + 0x2))
	#define TXCMD              (*(IORegExt16)(IOBASE + 0x4))
	#define TXLENGTH           (*(IORegExt16)(IOBASE + 0x6))
	#define ISQ                (*(IORegExt16)(IOBASE + 0x8))
	#define PKTPAGEPTR         (*(IORegExt16)(IOBASE + 0xA))
	#define PKTPAGEDATA0       (*(IORegExt16)(IOBASE + 0xC))
	#define PKTPAGEDATA1       (*(IORegExt16)(IOBASE + 0xE))
	#define DATAPORTL0         (*(IORegExt8)(IOBASE + 0x0))		
	#define DATAPORTH0         (*(IORegExt8)(IOBASE + 0x1))		
#else									
	#define DATAPORT0          *pDataPort0
	#define TXCMD              *pTxCmd
	#define TXLENGTH           *pTxLength
	#define ISQ                *pISQ
	#define PKTPAGEPTR         *pPktPagePtr
	#define PKTPAGEDATA0       *pPktPageData0
	#define DATAPORTL0         *pDataPortL0
	#define DATAPORTH0         *pDataPortH0
#endif

//------------------------------------------------------------------------------
//Definition of on-chip registers of CS8900A (not all registers def here)

//BUS INTERFACE REGISTERS OF CS8900A

#define PRODUCTCODE         0x000
#define IOBASEADDR          0x020
#define IRQNUMBER           0x022

//STATUS AND CONTROL REGISTERS OF CS8900A

#define ISQREG              0x120
#define RXCFG               0x102
#define RXEVENT             0x124
#define RXCTL               0x104
#define TXCFG               0x106
#define TXEVENT             0x128
#define TXCMDREAD           0x108
#define BUFCFG              0x10A
#define BUFEVENT            0x12C
#define RXMISS              0x130
#define TXCOL               0x132
#define LINECTL             0x112
#define LINEST              0x134
#define SELFCTL             0x114
#define SELFST              0x136
#define BUSCTL              0x116
#define BUSST               0x138
#define TESTCTL             0x118
#define TXCMDWRITE          0x144

//STATUS AND CONTROL REG INTERNAL ADDRESS IDENTIFIER
#define ADDR_ISQREG              0x00
#define ADDR_RXCFG               0x03
#define ADDR_RXEVENT             0x04
#define ADDR_RXCTL               0x05
#define ADDR_TXCFG               0x07
#define ADDR_TXEVENT             0x08
#define ADDR_TXCMDREAD           0x09
#define ADDR_BUFCFG              0x0b
#define ADDR_BUFEVENT            0x0c
#define ADDR_RXMISS              0x10
#define ADDR_TXCOL               0x12
#define ADDR_LINECTL             0x13
#define ADDR_LINEST              0x14
#define ADDR_SELFCTL             0x15
#define ADDR_SELFST              0x16
#define ADDR_BUSCTL              0x17
#define ADDR_BUSST               0x18
#define ADDR_TESTCTL             0x19
#define ADDR_TXCMDWRITE          0x09



//ADDRESS FILTER REGISTERS OF CS8900A

#define LAFILTER            0x150
#define IEEEADDR            0x158

//FRAME LOCATION REGISTERS OF CS8900A

#define RXST                0x400
#define RXLENGTH            0x402
#define RXFRMLOCATION       0x404
#define TXFRMLOCATION       0xA00

//BIT DEFINITIONS
#define RESETBIT            0x0040
#define INITDONE            0x0080

//TX CFG Bits
#define ENABLE_TXOKINT      0x0100
#define	ENABLE_ALLTXCFG		0x8FC0

//BufCFG Bits
#define ENABLE_RX_MISS_INT    0x400
#define ENABLE_TX_UNDRN_INT   0x200
#define ENABLE_RDY4TXINT    0x0100

//BufEvent bits
#define RX_MISS               0x400
#define TX_UNDRN              0x200
#define RDY4TX              0x0100

//BUS CTRL Reg Bits
#define ENABLE_IRQ          0x8000

//BusST register bits
#define RDY4TXNOW           0x0100

//LINE CTL Bits
#define SerRxON             0x0040
#define SerTxON             0x0080

//CS8900 IRQ control macros
#define EMACINT_ON           PKTPAGEREG(BUSCTL) = ADDR_BUSCTL | ENABLE_IRQ;
#define EMACINT_OFF          \
 PKTPAGEREG(BUSCTL) = ((PKTPAGEREG(BUSCTL) & ~0x3F) | ADDR_BUSCTL) & ~ENABLE_IRQ;


//------------------------------------------------------------------------------
//Register bit definitions

/*
 * RxEvent bits
 */
#define	BIT_RXOK	        	 0x0100
#define BIT_CRCError        0x1000
#define BIT_Runt            0x2000
#define BIT_Extradata       0x4000
#define RXALLEVENTS         (BIT_RXOK | BIT_CRCError | BIT_Runt | BIT_Extradata)

//TX EVENT BITS
#define BIT_TXOK            0x0100


//Rx CTL bits
#define IAHashA             0x0040
#define PromiscuousA        0x0080
#define RxOKA               0x0100
#define MulticastA          0x0200
#define IndividualA         0x0400
#define BroadcastA          0x0800
#define CRCerrorA           0x1000
#define RuntA               0x2000
#define ExtradataA          0x4000
#define RXALLFILTER         (IAHashA |PromiscuousA |RxOKA|MulticastA|IndividualA \
                            | BroadcastA | CRCerrorA | RuntA | ExtradataA)

//TX CMD option Bits
#define DISABLE_PADDING     0x4000
#define DISABLE_CRC         0x2000
#define SINGLE_COLLISION    0x0200
#define FORCE_SEND          0x0100

#define TXALLCMDBITS        (DISABLE_PADDING | DISABLE_CRC | SINGLE_COLLISION | FORCE_SEND)
#define TXCMDWORD           0x00C9

//TEST CTL Register bits
#define DisableLT           0x0080
#define FDX                 0x4000
#define DisableBackoff      0x0800
#define AUILoop             0x0400
#define ENDECLoop           0x0200
#define ExtLoop10BaseT      (DisableLT | FDX | DisableBackoff)
#define NormalOperation     0x0000
#define ALLTESTBITS         (ExtLoop10BaseT | AUILoop | ENDECLoop) //used as clearing mask

//Return codes
#define	CS8900_INITDONE	0

//Rx State control
#define RECEIVE_ON      1
#define RECEIVE_OFF     0

//Intr Ctrl codes
#define ENTERING        0
#define LEAVING         1

//ERROR CODES
#define CS8900_NOTFOUND     -1
#define ILLEGAL_CALLBACKS   -3
#define CS8900_DIAGFAIL1    -4


#define TX_FULLBUF	   3
#define TX_WAITING      2
#define TX_DONE         1
#define PKTTOOBIG	   -2

//------------------------------------------------------------------------------
//Function Prototypes.
extern void SetRxFilter(UINT16 rxflags, INT8 *Hashbits);

DDF_STATUS_t EthPktTransmit (ETH_PKT_t * );
DDF_STATUS_t EthInitFunc(INT8 * ieeeaddr,void (*)(void));
UINT16 TransmitNextPkt (void);
void SendNow(EMAC_FRAME_t* );
void ReceiveEthPkt(EMAC_FRAME_t * );
void EthTransmit(void); 
void EthReceive(void);
void EmacReset( void );
void EmacEnableIrq( void );
void EmacDisableIrq( void );
INT8 MulticastHash(INT8 *);
void EthMAddFunc(INT8 * ) ;
void EthMDelFunc(INT8 *	) ;
void GetEtheraddr(ETH_ADDR *);
void PutToQueue(QUEUE_REF_t *, QUEUE_NODE_t *) ;
QUEUE_NODE_t *GetFromQueue(QUEUE_REF_t *) ;



#endif
