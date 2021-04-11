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
 * File       : EtherMgr.h
 *
 * Description: Contains macros and structure declarations related to EMAC driver
 *
 * Copyright 2017 ZiLOG Inc.  ALL RIGHTS RESERVED.
 *
 * This file contains unpublished confidential and proprietary information
 * of ZiLOG, Inc.
 * NO PART OF THIS WORK MAY BE DUPLICATED, STORED, PUBLISHED OR DISCLOSED 
 * IN ANY FORM WITHOUT THE PRIOR WRITTEN CONSENT OF ZiLOG, INC.
 * This is not a license and no use of any kind of this work is authorized
 * in the absence of a written license granted by ZiLOG, Inc. in ZiLOG's 
 * sole discretion 
 */



#ifndef _ETHERMGR
#define _ETHERMGR

//#define WLAN 1

#include "ZDevice.h"
#include "common.h"



#define	ETHPKT_MINLEN	60	   /* minimum packet length		*/
#ifdef ZDOTS_WLAN
#define	ETHPKT_DLEN		1508	/* length of data field ep		*/
#else
#define	ETHPKT_DLEN		1500	/* length of data field ep		*/
#endif
#define	ETHPKT_HLEN		(sizeof(ETH_PKT_t) - ETHPKT_DLEN)	/* size of (extended) Ether header	*/
#define	ETHPKT_ALEN		6	   /* number of octets in physical address	*/
#define	ETHPKT_CRC		4	   /* ether CRC (trailer)			*/
#define	ETHPKT_MAXLEN	ETHPKT_HLEN+ETHPKT_DLEN
#define	ETHPKT_RETRY	   3	            /* number of times to retry xmit errors	*/
#define	ETHPKT_BRC	   (UINT8 *)"\377\377\377\377\377\377"/* Ethernet broadcast address	*/
#define   ETHPKT_RTO      300	/* time out in seconds for reads	*/

#define 	ETHPKT_NUMRCV    16		/* number LANCE recv buffers (power 2)	*/
#define 	ETHPKT_NUMRCVL2  4		/* log2 of the number of buffers	*/

#define	EPT_LOOP	   0x0060		/* type: Loopback		*/
#define	EPT_ECHO	   0x0200		/* type: Echo			*/
#define	EPT_PUP		0x0400		/* type: Xerox PUP		*/
#define	EPT_IP		0x0800		/* type: Internet Protocol	*/
#define	EPT_ARP		0x0806		/* type: ARP			*/
#define	EPT_RARP	   0x8035		/* type: Reverse ARP		*/


/* ethernet function codes */

#define	EPC_PROMON	   1		/* turn on promiscuous mode	*/
#define	EPC_PROMOFF	   2		/* turn off promiscuous mode	*/
#define	EPC_MADD	      3		/* add multicast address	*/
#define	EPC_MDEL	      4		/* delete multicast address	*/
#define EPV_IRQ_ENABLE  5
#define EPV_IRQ_DISABLE 6
#define EPV_RESET       7


#define MAX_DATA_SIZE 1500

#define HEADER_SIZE 17+18+4   //4 // mk , 4 added for ICV LEN, in wlan security 


/* Packet Byte Order Constants (set means host order) */

#define	EPO_NET		1	/* network layer		*/
#define	EPO_IP		2	/* level 1 protocols...		*/
#define	EPO_ARP		2
#define	EPO_RARP	   2
#define	EPO_ICMP	   4	/* level 2 protocols		*/
#define	EPO_IGMP	   4
#define	EPO_TCP		4
#define	EPO_UDP		4
#define	EPO_OSPF	   4
#define	EPO_DNS		5	/* application protocols	*/


extern UINT16 ez80_hs2net( UINT16 value );
#define hs2net(x) ez80_hs2net( (x) )
#define	net2hs(x) hs2net(x)


typedef INT8 ETH_ADDR[ETHPKT_ALEN] ;


typedef struct	ETH_HEADER {			/* ethernet header			*/
	ETH_ADDR	ethDst;	/* destination host address		*/
	ETH_ADDR	ethSrc;		/* source host address			*/
UINT16	ethType;    /* Ethernet packet type (see below)	*/
} ETH_HEADER_t;


/* Ethernet Packet Structure */
typedef struct	ETH_PKT {	/* complete structure of Ethernet packet*/
	QUEUE_NODE_t	link;		/* pointers to link the packet to the queue */
	UINT32	   	ethPktNextHop ; /* niput() uses this */
	UINT8 		ifNum ;			/* Interface Number */
	UINT16		PPPoEField;
	UINT16	      ethPktOrder ;  /* byte order mask (for debugging)	*/
	UINT16	      ethPktLen ; /* length of the packet		*/
#ifdef LEGACY_ZTP_TIMING
	UINT8		wlanHdr[22];   // WLAN HEADER
#else
   UINT8          MacOwns; /* non-zero during packet trasmission */
   UINT8          NotUsed[21];
#endif
	ETH_HEADER_t   	ethPktHeader ; /* the ethernet header	*/
	UINT8  	      ethPktData[ETHPKT_DLEN]; /* data in the packet 	*/
} ETH_PKT_t;
#define ETH_DATA_OFFSET (HEADER_SIZE+sizeof(ETH_HEADER_t))


typedef struct  ETH_DEV_STRUCT {

RZK_DEVICE_CB_t *pDev ;
ETH_ADDR			edcPAddr;	/* hardware address		*/
ETH_ADDR			edcBCast;	/* broadcast address */
INT8 *			edcDescr;
/* Not yet supported */
//struct netif *		edcPNif;
} ETH_DEV_STRUCT_t ;


#define EMAC_FRAME_t ETH_PKT_t


/* EMAC status values */
typedef enum {
	EMACDEV_ERR_SUCCESS = 0,
	EMACDEV_ERR_OVERRUN = -20,
	EMACDEV_ERR_INVALID_OPERATION,
	EMACDEV_ERR_INVALID_ARGS,
	EMACDEV_ERR_TEMP_BUSY,
	EMACDEV_ERR_PKTTOOBIG,
	EMACDEV_ERR_TX_WAITING,
	EMACDEV_ERR_PEMAC_INIT_DONE,
	EMACDEV_ERR_PEMAC_NOT_FOUND,
	EMACDEV_ERR_PEMAC_PHYREAD_ERROR,
	EMACDEV_ERR_PEMAC_PHY_NOT_FOUND,
	EMACDEV_ERR_PEMAC_PHYINIT_FAILED,
	EMACDEV_ERR_PEMAC_SMEM_FAIL,
	EMACDEV_ERR_KERNEL_ERROR,
	EMACDEV_ERR_RESOURCE_NOT_CREATED,
	EMACDEV_ERR_KERNEL_TIMEOUT
} EMACDEV_ERR_NUMS ;


/* Rx Buffer management */
#define Rx_BUFF_ALLOCATED 	0x01
#define Rx_BUFF_FREE		0x00


typedef struct PKT_BUFF{
	UINT8 status ; /* Status of the buffer allocated /free */
	EMAC_FRAME_t pktBuff ;	/* Buffer size equal to the Ethernet packet */
} PKT_BUFF_t ; 



/* Function declarations */

	DDF_STATUS_t AddEmac(void);

	DDF_STATUS_t EmacOpen(RZK_DEVICE_CB_t *pDev,RZK_DEV_NAME_t *devName, RZK_DEV_MODE_t * mode);
	DDF_STATUS_t EmacClose(RZK_DEVICE_CB_t *pDev);
	DDF_STATUS_t EmacWrite(RZK_DEVICE_CB_t *pDev, ETH_PKT_t *pep, RZK_DEV_BYTES_t len);
	DDF_STATUS_t EmacRead(RZK_DEVICE_CB_t *pDev, ETH_PKT_t **pep, RZK_DEV_BYTES_t len);
	DDF_STATUS_t EmacControl(RZK_DEVICE_CB_t *pDev, RZK_DEV_BYTES_t func, INT8 *arg1, INT8 *arg2);
    UINT8        IsEthernetConnected( void ) ;



#endif







