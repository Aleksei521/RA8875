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
 * File			:	cs8900.c
 *
 * Description	:	Driver file for the crystal ethernet controller.
 *					
 * Copyright 2017, ZiLOG Inc.
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

#include <stdio.h>
#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZThread.h"
#include "ZMessageQ.h"
#include "ZInterrupt.h"
#include "ZSemaphore.h"
#include "EtherMgr.h"
#include "cs8900a.h"
#include "_ez80.h"

#define TX_WAITING      2
#define TX_DONE         1
#define PKTTOOBIG	   -2

#ifdef PPPoE
#include "PPPoE_Globals.h"
#define	PPP_PPPoE_EMAC_HEADER_LENGTH	22
#define	EMAC_PPPoE_HEADER_LENGTH		20
#define EMAC_HEADER_LENGTH				14
#define	EMAC_SRC_DSTN_LENGTH			12
#define	PPP_PPPoE_HEADER_LENGTH			8
#define	PPPoE_HEADER_LENGTH				6
#define PPP_HEADER_LENGTH				2
#endif


#define EMAC_POLL_THD_STACK_SIZE 1024
#define CS8900_LINK_OK 0x80

extern void PutToQueue(QUEUE_REF_t *refnode, QUEUE_NODE_t *node) ;
extern QUEUE_NODE_t *GetFromQueue(QUEUE_REF_t *refnode) ;
extern UINT8 EMAC_Task_Stack[] ;
extern UINT8 EMAC_TASK_PRIO ;
extern UINT EMAC_THD_STACK_SIZE ;
extern void emacisr(void) ;
extern RZK_MESSAGEQHANDLE_t rxQueueHandle ; /* receive queue handle */

void CS8900IntTask( void );
void cs8900PollTask(void) ;

RZK_THREADHANDLE_t							emacInterruptThdHdl ;	/* emac interrupt thread handle */
RZK_THREADHANDLE_t   						emacPollThdHdl ;	/* emac poll thread handle */
UINT8 										EMAC_Poll_Task_Stack[EMAC_POLL_THD_STACK_SIZE] ;

IORegExt16									p_emac_base = (IORegExt16) 0x300;	// IO Base address for CS8900A
INT8 										xinu_eth_irq = IV_PD4; 				// Use IV_NULL if no NIC Irq line.

UINT32          							emac_isr_count  = 0;

INT8 										doing_tx = RZK_FALSE;
INT8										tx_q_size = 0;
QUEUE_REF_t									mac_tx_q;

#define CS8900A_INTRQ0						0x00
#define CS8900A_INTRQ1						0x01
#define CS8900A_INTRQ2						0x02
#define CS8900A_INTRQ3						0x03
#define CS8900A_DISABLE_INTRQ				0x04
#define HASH_TABLE_SIZE						4


ETH_DEV_STRUCT_t       						emac[1];
void			  							(*rxnotifyfunc)(void);
EMAC_FRAME_t *								txbuffnext = 0;
ETH_PKT_t *									tx_pep_next;
UINT16                						CRCErrors, 
											RuntErrors, 
											ExcessLengthErrors, 
											RxMissCount,
                                 TxUndrnCount,
											TxColCount;

INT8										bRegistersInitialized = RZK_FALSE;
IORegExt16  								pDataPort0;
IORegExt16									pTxCmd;
IORegExt16									pTxLength;
IORegExt16									pISQ;
IORegExt16									pPktPagePtr;
IORegExt16									pPktPageData0;
IORegExt8	  								pDataPortL0;
IORegExt8   								pDataPortH0;

UINT16             							RxFilter = IndividualA | BroadcastA | RxOKA;
UINT16             							HashTable[ HASH_TABLE_SIZE ] = {0,0,0,0};
INT8        								b_mc_count = 0x00;

RZK_SEMAPHOREHANDLE_t g_MacSem;

#ifdef PPPoE
extern UINT16 intel16( UINT16  );
#endif



/*
 * Function prototypes.
 */
extern void RxFunc(void);

extern void
nic_read
(
	UINT16             			Length,
  	INT8 *							pBuf,
	IORegExt16						p_io_base
);
extern void
nic_discard
(
	UINT16             			Length,
	IORegExt16						p_io_base
);
extern void
nic_write
(
	UINT16             			Length,
  	INT8 *							pBuf,
	IORegExt16						p_io_base
);
void ETHMADDFUNC
(
	INT8 *							pAddr
);
INT8
MulticastHash
(
	INT8 *							ether_addr
);
static DDF_STATUS_t DataPathChk(void);



RZK_SEMAPHOREHANDLE_t g_MacSem;
/*
 * ===========================================================================
 * =
 */
void
setup_registers
(
	IORegExt16						pIOBase
)
{
	if( bRegistersInitialized == RZK_FALSE )
	{
		pDataPort0    = pIOBase + 0x00;
		pTxCmd        = pIOBase + 0x02;
		pTxLength     = pIOBase + 0x03;
		pISQ			  = pIOBase + 0x04;
		pPktPagePtr   = pIOBase + 0x05;
		pPktPageData0 = pIOBase + 0x06;

		pDataPortL0   = (IORegExt8)pIOBase + 0x00;
		pDataPortH0   = (IORegExt8)pIOBase + 0x01;

  		bRegistersInitialized = RZK_TRUE;
	}
}

void
EmacEnableIrq
(
	void
)
{
	setup_registers( p_emac_base );
	PKTPAGEPTR    = BUSCTL;
	PKTPAGEDATA0  = ADDR_BUSCTL | ENABLE_IRQ;
}


void
EmacDisableIrq
(
	void
)
{
	setup_registers( p_emac_base );
	PKTPAGEPTR    = BUSCTL;
	PKTPAGEDATA0  = ADDR_BUSCTL;
}



void 
EmacReset
(
	void
)
{
	volatile INT8					bData;

//kprintf("R+");
	/*
	 * Setup internal register used to access the CS8900A in IO mode. 
	 */
	setup_registers( p_emac_base );

	/*
	 * Read a byte from an odd memory location to make sure CS8900 internal 8bit
	 * state machine is synchronized to the driver.
	 */
	bData = DATAPORTH0;
	
	/*
    * Issue a software reset to CS8900S.  This will put the device into a known
 	 * initial state.
	 */	
	PKTPAGEPTR   = SELFCTL;
	PKTPAGEDATA0 = ADDR_SELFCTL | RESETBIT;
	PKTPAGEPTR   = SELFST;
	while( !(PKTPAGEDATA0 & INITDONE) )//changed
	{
		/*
	 	 * Wait till CS8900A init is complete 
		 */
	}

	/*
	 * Note that the Reset will Clear all interrupt enables and leave the CS8900A
	 * interrupt request lines in high impedance.  This may cause a voltage to 
	 * appear on the GPIO interrupt pin and generate spurious interrupts.
	 * Therefore, we always enable the Interrupt line of the CS8900A immediately
	 * after the reset is finished.  This will ensure that the IRQ line is driven
	 * low and since no interrupt enable bits have been set, no interrupts will
	 * be generated.
	 */

	/*
	 * Enable IRQ0 (active high)
	 */
	PKTPAGEPTR    = IRQNUMBER;
	PKTPAGEDATA0  = CS8900A_INTRQ0;
	PKTPAGEPTR    = BUSCTL;
	PKTPAGEDATA0  = ADDR_BUSCTL; // | ENABLE_IRQ;
//kprintf("R-");
}




/*----------------------------------------------------------------------------*/
DDF_STATUS_t
EthInitFunc
(
	INT8 *							ieeeaddr, 
	void							(*rxnotify)(void)
)
/*
 * ieeeaddr is pointer to a char array containing 6 bytes of MAC ID which is
 * this MAC's ethernet ID. rxnotify is the address of a function which this
 * driver will call on reception of a packet. rxnotify is the address of a
 * function which must be called every time a receive frame is available in HW.
 * 
 * Return value 0 - successful initialization
 * 
 */
{
	extern INT8 b_poll_emac;
	UINT16 i,j;
	CRCErrors = RuntErrors = ExcessLengthErrors = RxMissCount = TxUndrnCount = TxColCount = 0;

	g_MacSem = RZKCreateSemaphore((RZK_NAME_t*)"g_MacSem",1,RECV_ORDER_PRIORITY);
	if(g_MacSem == NULL)
		printf("\nEmac Semaphore creation error");
	EmacReset();

	/* Check if we can read the CS8900A ID */	
	PKTPAGEPTR = PRODUCTCODE;
	if( PKTPAGEDATA0 != CS8900ID )
	{
		printf("\r\nCS8900 not found ");
		return (CS8900_NOTFOUND);
	};

	/* Databus path checkout */
	if ((i = DataPathChk()) != 0) 
	{
		printf("\r\nDatapath problem detected\n");
		return (CS8900_DIAGFAIL1);
	}
	/* Setup our 6 byte (48 bit) IEEE MAC address */
	for (i = 0; i < 3; i++) 
	{
//		PKTPAGEPTR   = (unsigned short)IEEEADDR + i * 2;
		j = IEEEADDR + i * 2;
		PKTPAGEPTR   = j;
		PKTPAGEDATA0 = *(UINT16 *)ieeeaddr;
		ieeeaddr += 2;	
	}


/* Create Interrupt thread */
	emacInterruptThdHdl = RZKCreateThreadEnhanced((RZK_NAME_t*)"EMACIT",
													(RZK_PTR_t)CS8900IntTask,
													NULL, 
													EMAC_THD_STACK_SIZE, 
													EMAC_TASK_PRIO, 
													2,
													RZK_THREAD_INTERRUPT|RZK_THREAD_PREEMPTION,
													0 ) ;
		if( emacInterruptThdHdl == NULL )
		{
			return EMACDEV_ERR_RESOURCE_NOT_CREATED ;
		}

/* Create Interrupt thread */
	emacPollThdHdl = RZKCreateThreadEnhanced((RZK_NAME_t*)"EMACpoll",
													(RZK_PTR_t)cs8900PollTask,
													NULL, 
													EMAC_POLL_THD_STACK_SIZE, 
													EMAC_TASK_PRIO, 
													2,
													RZK_THREAD_PREEMPTION|RZK_THREAD_AUTOSTART,
													0 ) ;
		if( emacPollThdHdl == NULL )
		{
			return EMACDEV_ERR_RESOURCE_NOT_CREATED ;
		}

		

/* Install interrupt Handler */
#ifndef _EZ80190
	RZKInstallInterruptHandler( (RZK_FNP_ISR)emacisr, 0x58) ;
#else
	RZKInstallInterruptHandler( (RZK_FNP_ISR)emacisr, 0x4E) ;
#endif

	PKTPAGEPTR    = BUSCTL;
	PKTPAGEDATA0  = ADDR_BUSCTL | ENABLE_IRQ;

	/*
	 * Save the interrupt callback addresses
	 */
	rxnotifyfunc = rxnotify;	
	if( rxnotifyfunc == 0 )
	{
		return (ILLEGAL_CALLBACKS);
	}

	/*
	 * Start the mac poll thread if requested by user.
	 */

	PKTPAGEPTR   = LINECTL;
	PKTPAGEDATA0 = SerTxON | SerRxON | ADDR_LINECTL;
	SetRxFilter(RxFilter, (INT8 *) &HashTable[0]);

	PKTPAGEPTR   = TXCFG;
	PKTPAGEDATA0 = ADDR_TXCFG | ENABLE_ALLTXCFG;	/* Enable Tx completion INTR */
	PKTPAGEPTR   = BUFCFG;
	PKTPAGEDATA0 = ADDR_BUFCFG | ENABLE_RDY4TXINT | ENABLE_RX_MISS_INT | ENABLE_TX_UNDRN_INT;
	PKTPAGEPTR   = RXCFG;
	PKTPAGEDATA0 = ADDR_RXCFG | BIT_RXOK;//RXALLEVENTS;	/* Enable RxOK and Err bits */


	return (EMACDEV_ERR_PEMAC_INIT_DONE);}


/*----------------------------------------------------------------------------*/


static DDF_STATUS_t
DataPathChk(void)
{
	PKTPAGEPTR   = IEEEADDR;
	PKTPAGEDATA0 = 0x5555;
	if( PKTPAGEDATA0 != 0x5555)
		return (0x5555 ^ PKTPAGEDATA0);

	PKTPAGEPTR   = (IEEEADDR + 2);
	PKTPAGEDATA0 = 0xAAAA;
	if( PKTPAGEDATA0 != 0xAAAA)
		return (0xAAAA ^ PKTPAGEDATA0);

	return (0);}

	
/*----------------------------------------------------------------------------*/


static void 
SetRxFilter(UINT16 rxflags, INT8 *cHashbits)
{
	UINT16          *Hashbits = (UINT16 *) cHashbits;
	UINT16           i;
	UINT16				temp;

	PKTPAGEPTR    = LINECTL;
	PKTPAGEDATA0 &= ~SerRxON;	/* Disable serial recv */	
	if (Hashbits != 0) 
	{	
		/* Not NULL if a valid hash index field is given */		
		for (i = 0; i < HASH_TABLE_SIZE; i++) 
		{
//			PKTPAGEPTR   = LAFILTER + (i<<1);
			temp = i<<1;
			temp += LAFILTER;
			PKTPAGEPTR   = temp;
 			PKTPAGEDATA0 = Hashbits[i];
		}	
	}	
	/* mask user flag word for valid bits and update RXCTL */	
	PKTPAGEPTR   = RXCTL;
	PKTPAGEDATA0 = rxflags | ADDR_RXCTL;

	PKTPAGEPTR    = LINECTL;
	PKTPAGEDATA0 |= SerRxON;
}



UINT16
TransmitNextPkt
(
	void
)
/*
 * Assumes interrupts or off.
 */
{
	UINT16  	 			BufAvail = RZK_FALSE;
	UINTRMASK intmask ;

//	kprintf( "Do Next Tx @ %x\n", SysUpTime );
	if( doing_tx == RZK_FALSE )
	{
/* Deque from the queue */
		tx_pep_next = (ETH_PKT_t *)	GetFromQueue( &mac_tx_q );
		if( tx_pep_next )
		{
			intmask = RZKDisableInterrupts() ;
			doing_tx = RZK_TRUE;
//			tx_q_size--;

			txbuffnext  = tx_pep_next;
			TXCMD      = 0x00C0;
			TXLENGTH   = txbuffnext->ethPktLen;
	   		PKTPAGEPTR = BUSST;
			BufAvail   = PKTPAGEDATA0 & RDY4TXNOW;
			RZKEnableInterrupts(intmask) ;
		}
	}
	return( BufAvail );
}


//UINT16 max_queued = 0;

DDF_STATUS_t
EthPktTransmit
(
	ETH_PKT_t *						pep
)
{	
	DDF_STATUS_t	status;
	RZK_STATE_t					CritFlag;	
	UINT16 frmType = pep->ethPktHeader.ethType ;

	if( pep->ethPktLen <= MAXFRAMESIZE ) 
	{

		status = EMACDEV_ERR_TX_WAITING;    // CR6168 - MSS

/* Queue the packet to the Tx Queue */
//		CritFlag = RZKDisablePreemption() ;
		RZKAcquireSemaphore(g_MacSem,MAX_INFINITE_SUSPEND);

		PutToQueue( &mac_tx_q, &pep->link ); //Moved after disable preemption

		if( TransmitNextPkt() )
		{
			#ifdef PB_DEBUG
				OrPortB( 0x10 );
			#endif
			pep = tx_pep_next;
			RZKReleaseSemaphore(g_MacSem);
//			RZKRestorePreemption(CritFlag);
#ifdef PPPoE
			if( ( frmType == g_PPPoE_EthTypeDiscovery ) ||
				( frmType == g_PPPoE_EthTypeSession )
		      )
			{

				nic_write( 14, (INT8*)&txbuffnext->ethPktHeader, p_emac_base );

				g_PPPoETxHeader.payloadLength = intel16 ( txbuffnext->ethPktLen - EMAC_PPPoE_HEADER_LENGTH );

				nic_write( 6, (INT8*)&g_PPPoETxHeader, p_emac_base );

				if( frmType == g_PPPoE_EthTypeSession )
				{
					nic_write( 	2, 
								(INT8*)txbuffnext, 
								p_emac_base
							  );
					nic_write( txbuffnext->ethPktLen-22, (INT8*)&txbuffnext->ethPktData, p_emac_base );
				}
				else
				nic_write( txbuffnext->ethPktLen-20, (INT8*)&txbuffnext->ethPktData, p_emac_base );	
				status = pep->ethPktLen ;   // CR6168 - MSS
			}
			else	
			{
				nic_write( txbuffnext->ethPktLen, (INT8*)&txbuffnext->ethPktHeader, p_emac_base );
				status = pep->ethPktLen ;   // CR6168 - MSS
			}
#else
			{
				nic_write( txbuffnext->ethPktLen, (INT8*)&txbuffnext->ethPktHeader, p_emac_base );
				status = pep->ethPktLen ;   // CR6168 - MSS
			}
#endif            
		}
		else
		{
			RZKReleaseSemaphore(g_MacSem);
//			RZKRestorePreemption(CritFlag);
		}

	}
	else
	{
		status = PKTTOOBIG;
	}

	return( status );
}

/*----------------------------------------------------------------------------*/
UINT8 no_emac_rx_buffer;

void 
ReceiveEthPkt
(
	EMAC_FRAME_t * 					databuff
)
{	
   UINT16 temp;
	UINT16 recvdFrmType ;
	no_emac_rx_buffer = 0;
	/*
	 * Rx Data Preceeded by Status (2 bytes) and Length (2 bytes)
	 * Note:  The Status bytes must be read HiLo before reading the 
	 * rest of the data (LoHi) since the CS8900 stores word data in the
	 * opposite endian of the network.
	 *
	 * Even though documentation from Cirrus suggests reading the 
	 * packet length Hi first then Lo, doing this causes truncation of Rx data.
	 * Therefore we go against the spec and read data LoHi.
	 */
	temp   = DATAPORTH0;
	temp	= (temp << 8) + DATAPORTL0;
	databuff->ethPktLen  = DATAPORT0;
	if( databuff->ethPktLen )
	{
#ifdef PPPoE
	nic_read( 14, (INT8*)&databuff->ethPktHeader, p_emac_base );
	recvdFrmType = databuff->ethPktHeader.ethType;
	if( ( recvdFrmType == g_PPPoE_EthTypeDiscovery )||
		( recvdFrmType == g_PPPoE_EthTypeSession )
		)
	{
		databuff->PPPoEField = recvdFrmType ;
		nic_read( 6, (INT8*)&g_PPPoERxHeader, p_emac_base );
		if( recvdFrmType == g_PPPoE_EthTypeSession )
		{
			nic_read( 2, (INT8*)&databuff->ethPktHeader.ethType, p_emac_base );
			nic_read( databuff->ethPktLen, (INT8*)&databuff->ethPktData[0], p_emac_base );
		}
		else
			nic_read( databuff->ethPktLen, (INT8*)&databuff->ethPktData[0], p_emac_base );
		//databuff->length -= 6;
	}
	else
	{
		nic_read( databuff->ethPktLen, (INT8*)&databuff->ethPktData[0], p_emac_base );
		databuff->PPPoEField = databuff->ethPktHeader.ethType;
	}
#else
	nic_read( databuff->ethPktLen, (INT8*)&databuff->ethPktHeader, p_emac_base );
	databuff->PPPoEField = databuff->ethPktHeader.ethType;
#endif
	}
	else
	{
		printf( "Nothing to read!!!\n" );
	}


#ifdef PERF_TRACE
		kprintf( " M%x ", databuff->Length );
#endif

#if 0
	if( (databuff->flags & 0x304) == 0x304 )
	{
		INT8 *						ptr;
		UINT16							i;

		printf( "Rx Status Flags %x\n", databuff->flags );
		printf( "Rx Length %x\n", databuff->length );
		printf( "Hash = %x\n", MulticastHash( (INT8*)&databuff->dstAddr[0]) );
		printf( "Rx Data: \n");
		ptr = (INT8 *) &databuff->dstAddr[0];	
		for( i=0; i< databuff->length; i++ ) 
		{
			printf( " %x", ptr[i] & 0xFF );
		}	
		printf( "\n" );
	}
#endif
}



/*
 * Internal function to calculate a multicast hash value for the receiver.
 * For this device, a bit mask is used to coarsely filter possible
 * multicast packets.  The bit is calculated by using the CRC algorithm
 * applied to the physical destination address, and then using the six
 * most significant bits as the integer bit index in the eight-byte mask.
 */
static UINT32 crc_poly = 0x04C11DB7;
INT8	
MulticastHash
(
	INT8 * 							addr
)
{
	INT8								HashIndex;
	INT8								AddrByte;
	UINT16								Byte;
	UINT16								Bit;
	INT32 							CRC;

	/*
	 * Compute CRC32 on the 6 byte DA
	 */
	CRC = 0xFFFFFFFF;
	for( Byte=0; Byte<6; Byte++ )
	{
		AddrByte = *addr++;
		for( Bit=0; Bit<8; Bit++, AddrByte>>=1 )
		{
			CRC = (CRC << 1) ^ ( ((CRC < 0) ^ (AddrByte & 1)) ? crc_poly : 0 );
		}
	}

	/*
	 * Hash index is the bottom 6 bits of the CRC in reverse bit order.
	 */
	HashIndex = 0;
	for( Bit=0, HashIndex=0; Bit<6; Bit++ )
	{
		HashIndex = (HashIndex << 1) + ((CRC >> Bit) & 1);
	}

	return( HashIndex );
}



/*
 * Update the hash filter in HW to add another MC address.
 */
void EthMAddFunc
(
	INT8 *							pAddr
)
{
	UINT16             			data;
	UINT16             			bitnum;
	UINT16             			addr;
	UINT16             			saved_PktPage;
	RZK_STATE_t							CritFlag;

/*
kprintf( "Add %02x:%02x:%02x:%02x:%02x:%02x to MC Filter table\n",
		pAddr[0],
		pAddr[1],
		pAddr[2],
		pAddr[3],
		pAddr[4],
		pAddr[5] );
*/

	bitnum = MulticastHash(pAddr);
//	kprintf( "CRC mapping is %x\n", bitnum );

	CritFlag = RZKDisablePreemption();
	saved_PktPage = PKTPAGEPTR;
 	PKTPAGEPTR    = LINECTL;
	PKTPAGEDATA0 &= ~SerRxON;	


	addr   = (LAFILTER + ((bitnum/16)*2));
	bitnum = ( 1 << (bitnum%16) );
	PKTPAGEPTR = addr;
	data       = PKTPAGEDATA0;
//	kprintf( "Read %x from port %x\n", data, addr);
	if( (data & bitnum) == 0 )
	{
		data |= bitnum;
		b_mc_count++;
//		kprintf( "Writing %x to %x\n", data, addr );
	   PKTPAGEDATA0 = data;

//		kprintf( "MC count now %x\n", b_mc_count);

		if( b_mc_count )
		{
			/*
			 * Enable Rx Multicast frames
			 */
			RxFilter 	|= MulticastA;
			PKTPAGEPTR   = RXCTL;
			PKTPAGEDATA0 = (RxFilter | ADDR_RXCTL);
		}
	}

	PKTPAGEPTR    = LINECTL;
	PKTPAGEDATA0 |= SerRxON;
	PKTPAGEPTR 	  = saved_PktPage;

	RZKRestorePreemption(CritFlag);
}



/*
 * Remove given address from the list supported in HW.
 */
void EthMDelFunc
(
	INT8 *							pAddr
)
{
	UINT16             			data;
	UINT16             			bitnum;
	UINT16             			addr;
	UINT16             			saved_PktPage;
	RZK_STATE_t							CritFlag;

/*
kprintf( "Remove %02x:%02x:%02x:%02x:%02x:%02x to MC Filter table\n",
		pAddr[0],
		pAddr[1],
		pAddr[2],
		pAddr[3],
		pAddr[4],
		pAddr[5] );
*/

	bitnum = MulticastHash(pAddr);
//	kprintf( "CRC mapping is %x\n", bitnum );

	CritFlag = RZKDisablePreemption();
	saved_PktPage = PKTPAGEPTR;
 	PKTPAGEPTR    = LINECTL;
	PKTPAGEDATA0 &= ~SerRxON;	


	addr   = (LAFILTER + ((bitnum/16)*2));
	bitnum = ( 1 << (bitnum%16) );
	PKTPAGEPTR = addr;
	data       = PKTPAGEDATA0;
//	kprintf( "Read %x from port %x\n", data, addr);
	if( data & bitnum )
	{
		data &= ~bitnum;
		b_mc_count--;
//		kprintf( "Writing %x to %x\n", data, addr );
	   PKTPAGEDATA0 = data;

//		kprintf( "MC count now %x\n", b_mc_count);

		if( b_mc_count == 0 )
		{
			/*
			 * Disable Rx Multicast frames
			 */
			RxFilter 	&= ~MulticastA;
			PKTPAGEPTR   = RXCTL;
			PKTPAGEDATA0 = (RxFilter | ADDR_RXCTL);
		}
	}
	PKTPAGEPTR    = LINECTL;
	PKTPAGEDATA0 |= SerRxON;
	PKTPAGEPTR 	  = saved_PktPage;

	RZKRestorePreemption(CritFlag);
}




UINT16 emac_save_count = 0;
void cs8900PollTask(void)
{
	UINT32 my_emac_isr_count = 0;
	UINTRMASK intmask ;


	while( 1 )
	{
		RZKSuspendThread( RZKGetCurrentThread(), 100 ) ;    //CR6167 - MSS

		if( my_emac_isr_count == emac_isr_count )
		{
			/*
			 * CS8900A 8-bit interrupt patch:
			 * Simulate a call into the MAC ISR to re-enable interrupts.
			 */
//			emac_save_count++;

			intmask = RZKDisableInterrupts();
			emacisr();
			RZKEnableInterrupts(intmask);
		}
		my_emac_isr_count = emac_isr_count;

	}
}


extern UINT32 SysUpTime;
void CS8900IntTask( void )
{
	UINT16                      i;
	ETH_PKT_t *						pep;
	RZK_STATE_t								CritFlag;
//	RZK_STATUS_t status ;           // Commented during IAR Porting
	UINTRMASK intmask1, intmask2 ;
	
	intmask1 = RZKDisableInterrupts();
	while( 1 )
	{
		i = ISQ;

		while( i )
		{
			RZKEnableInterrupts(intmask1); //changed for debugging
			emac_isr_count++;
#ifdef PERF_TRACE
			kprintf( "ISQ=%x @ %x", i, SysUpTime );
#endif

			switch (i & 0x3F) 	/* lower 6 bits give reg number causing INTR */
			{
				case 0x04:	/* RXEVENT cause ID */
				{
#if 0
					if ((i & BIT_CRCError) && (CRCErrors != 0xFFFF))
						CRCErrors++;
					if ((i & BIT_Runt) && (RuntErrors != 0xFFFF))
						RuntErrors++;
					if ((i & BIT_Extradata) && (ExcessLengthErrors != 0xFFFF))
						ExcessLengthErrors++;
#endif
					if( i & BIT_RXOK )
					{

						RZKAcquireSemaphore(g_MacSem,MAX_INFINITE_SUSPEND);
//						CritFlag = RZKDisablePreemption();
//						b_rx_count++;

						if (rxnotifyfunc != 0)
						{
							no_emac_rx_buffer = 1;
							rxnotifyfunc();
							/*
							 * Check if the ReceivePkt routine was called.  
							 * If it was, no_emac_rx_buffer will be reset to 0.
							 * If it wasn't then the system is out of receive buffers.
							 */
							if(no_emac_rx_buffer)
							{
								volatile INT8	temp;
								volatile INT8 	temp2;
								UINT16	         Len;

								temp  = DATAPORTH0;
								temp2 = DATAPORTL0;
								Len  = DATAPORT0;
								// kprintf( "discarding packet, len is %u\n", Len );
								if( Len )
								{
									nic_discard( Len, p_emac_base );
								}
//								b_rx_count--;
							}
							
						}

//						RZKRestorePreemption(CritFlag);	
						RZKReleaseSemaphore(g_MacSem);
					}

					break;
				}
            case 0x08:	/* TXEVENT cause ID */
            {
_NextTx_:
               RZKAcquireSemaphore( g_MacSem, MAX_INFINITE_SUSPEND );
               doing_tx = RZK_FALSE;
               tx_pep_next->MacOwns = FALSE;
               if( TransmitNextPkt() )
               {
                  pep = tx_pep_next;
                  nic_write( txbuffnext->ethPktLen, (INT8*)&txbuffnext->ethPktHeader, p_emac_base );
               }
               RZKReleaseSemaphore(g_MacSem);
               break;
            }
				case 0x0C:	/* BUF EVENT casue ID, space now available for earlier TxCMD request */
				{
                    //printf( "Buf Event " );
					if (i & RDY4TX) 
					{
						#ifdef PB_DEBUG
							OrPortB( 0x04 );
						#endif		
						pep = tx_pep_next;
						nic_write( txbuffnext->ethPktLen, (INT8*)&txbuffnext->ethPktHeader, p_emac_base );
						#ifdef PB_DEBUG	
							AndPortB( 0xFB );
						#endif		
					};			
					if( i & TX_UNDRN )
					{
                  printf( "TxUndrn %x\n", ++TxUndrnCount );
                  goto _NextTx_;
					}
					if( i & RX_MISS )
					{
                  RxMissCount++;
                  //printf( "RxMiss %x\n", RxMissCount );
					}

					break;
				}
				case 0x10:	/* RxMiss count overflow */
				{
					if (RxMissCount != 0xFFFF)
						RxMissCount++;	/* Saturating counter */

					break;
				}
            case 0x12:  /* TxCOL count overflow */
            {
               printf( "TxCol %x\n", TxColCount );
               if (TxColCount != 0xFFFF )
               {
                  TxColCount++;
               }
               break;
            }
				default:
				{
					#if DEBUG
					printf("Unexpected CS8900 Interrupt %x\n", i );
					#endif
					break;
				}
			}

			/*
			 * Look for another interrupt
			 */
			intmask1 = RZKDisableInterrupts(); //changed for debugging
			i = ISQ;
		}

		RZKEnableInterrupts(intmask1);

		intmask2 = RZKDisableInterrupts();

		/*
		 * Re-enable CS8900 interrupt generation
		 */
		PKTPAGEPTR    = BUSCTL;
		PKTPAGEDATA0  = ADDR_BUSCTL | ENABLE_IRQ;

		RZKSuspendInterruptThread() ;
		RZKEnableInterrupts(intmask2) ;
	}

}



UINT8
IsEthernetConnected( void )
{
   UINT16               Save, Data;
	UINT8                Status = RZK_FALSE;
	UINT8                CritFlag;


   /*
    * Disable interrupts to prevent the CS8900 emacisr.asm routine from modifying
    * PKTPAGEPTR before PKTPAGEDATA0 is read.
    */
   CritFlag   = RZKDisableInterrupts();
   Save 		  = PKTPAGEPTR;
   PKTPAGEPTR = LINEST;
   Data       = PKTPAGEDATA0;
   PKTPAGEPTR = Save;
   RZKEnableInterrupts( CritFlag );

	/*
	 * Read the Line Status register to determine if the Link is OK.
	 */
	if( Data & CS8900_LINK_OK )
	{
		Status = RZK_TRUE;
	}
   else
   {
      printf( "<CS8900 Not connected - %x>\r\n", Data );
   }

	return( Status );
}
