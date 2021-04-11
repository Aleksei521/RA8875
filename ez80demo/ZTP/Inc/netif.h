/*
 * File       :     netif.h
 *
 * Description:  This file contains network interface related definitions.
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
 */
/* netif.h - NIGET */
#ifndef _NETIF_H_
#define _NETIF_H_

#include "ztypes.h"
#include "ZTPtcp.h"
#include "ZDevice.h"
#include "EtherMgr.h"


#define UINT32 UINT32

#define BOOTNAMELENGTH  16 

struct commonServers {
        INT8    timeserver[BOOTNAMELENGTH];
		INT8	NetworkTimeServer[BOOTNAMELENGTH];
        INT8    rfserver[BOOTNAMELENGTH];
        INT8    tftpserver[BOOTNAMELENGTH];
        INT8    dnsserver[BOOTNAMELENGTH];
};



struct If {
	 RZK_DEVICE_CB_t *pNetDev;
	 INT8    ifType;
	 UINT16    mtu;
	 UINT16    speed;
        INT8    myip[BOOTNAMELENGTH];
        INT8    defaultroute[BOOTNAMELENGTH];
        UINT32  subnetmask;
};


#define	NI_MAXHWA 14			/* max size of any hardware	*/
					/*  (physical) net address	*/
struct	hwa	{			/* a hardware address		*/
	UINT16	ha_len;		/* length of this address	*/
	UINT8	ha_addr[NI_MAXHWA]; 	/* the address		*/
};

#define	NI_INQSZ	30		/* interface input queue size	*/
#define	NETNLEN		64		/* length of network name	*/

#define	NI_LOCAL	0		/* index of local interface	*/
#define	NI_PRIMARY	1		/* index of primary interface	*/

#define	NI_MADD		0		/* add multicast (ni_mcast)	*/
#define	NI_MDEL		1		/* delete multicast (ni_mcast)	*/

/* interface states */
#define	NIS_UNUSED	0x00
#define	NIS_UP		0x01
#define	NIS_DOWN	0x02
#define	NIS_TESTING	0x03

/* Definitions of network interface structure (one per interface)	*/

struct	netif {				/* info about one net interface	*/
	//MCB L92	CHAR	ni_name[NETNLEN];	/* domain name of this interface*/
	RZK_DEVICE_CB_t *	pDevCtlBlk;
	UINT8  	ni_state;		/* interface states: NIS_ above	*/
	UINT32	ni_ip;			/* IP address for this interface*/
	UINT32	ni_subnet;		/* subnetwork IP address	*/
	UINT32	ni_mask;			/* IP subnet mask for interface	*/
	UINT32	ni_net;		/* network IP address		*/
	UINT32	ni_brc;			/* IP broadcast address		*/
	UINT32	ni_nbrc;			/* IP net broadcast address	*/
	//Ajay - Added following info (bootRecord Struct moved in here*/
	UINT32	ni_defGtwy;		/* Default gateway for this interface*/
	//IPaddr	ni_dns;			/* Domain name server */
	//IPaddr	ni_ts;			/* Time server */
	//IPaddr     ni_fs;			/* File server */
	UINT16 	ni_mtu;			/* max transfer unit (bytes)	*/
	//End
	UINT16 	ni_hwtype;		/* hardware type (for ARP)	*/
	struct	hwa	ni_hwa;		/* hardware address of interface*/
	struct	hwa	ni_hwb;		/* hardware broadcast address	*/
#ifdef IGMP_REQ	
	UINT32     *arrClsDIP;/* For IGMP supprt */
#endif
	//Ajay SYSCALL	(*ni_mcast)(word,DID,Eaddr,IPaddr);	/* multicast add/delete func.*/
	BOOL	ni_ivalid;		/* is ni_ip valid?		*/
	BOOL	ni_nvalid;	/* is ni_name valid?		*/
	BOOL	ni_svalid;		/* is ni_subnet valid?		*/
	RZK_DEVICE_CB_t	*ni_dev;		/* the Xinu device descriptor	*/
	//Ajay QID	ni_ipinq;		/* IP input queue		*/
	//Ajay QID	ni_outq;		/* (device) output queue	*/
	//Ajay KE_QUEUE ni_ipinq;	/* IP input queue		*/
	//Ajay KE_SEM * InQSem;
	INT16 (*ni_send)(struct netif *,struct ETH_PKT *); /* Function to call to send a packet */
	/* Interface MIB */

	INT8	*	ni_descr;		/* text description of hardware */
	UINT8		ni_mtype;		/* MIB interface type		*/
	//Ajay, need  device CB pointer here.
	/* SNMP specifies that these (the uint32_t values) are 32-bit entities */
	UINT32	ni_speed;		/* bits per second		*/
	INT8		ni_admstate;	/* administrative status (NIS_*)*/
	UINT32	ni_lastchange;	/* last state change (1/100 sec)*/
	UINT32	ni_ioctets;		/* # of octets received		*/
	UINT32	ni_iucast;		/* # of unicast received	*/
	UINT32	ni_inucast;		/* # of non-unicast received	*/
	UINT32	ni_idiscard;		/* # dropped - output queue full*/
	UINT32	ni_ierrors;		/* # input packet errors	*/
	UINT32	ni_iunkproto;	/* # in packets for unk. protos */
	UINT32	ni_ooctets;		/* # of octets sent		*/
	UINT32	ni_oucast;		/* # of unicast sent		*/
	UINT32	ni_onucast;		/* # of non-unicast sent	*/
	UINT32	ni_odiscard;		/* # output packets discarded	*/
	UINT32	ni_oerrors;		/* # output packet errors	*/
	UINT32	ni_oqlen;		/* output queue length		*/
	UINT32	ni_maxreasm;	/* max datagram can reassemble	*/
	INT8		ni_name[NETNLEN];	/* domain name of this interface*/
};

//#define	NIGET(ifn)	((struct ep *)KE_Dequeue(&nif[ifn].ni_ipinq))
//extern word	NIF;	/* # of interfaces */
extern UINT8 ztpMaxIf;
extern struct netif	nif[];
extern struct If ifTbl[];
//extern struct	netinfo	Net;
/* addnif.c */
//xtern struct netif *addnif(DID dev);
extern INT nifinit(VOID);
extern UINT16  nifDriverInit(VOID);
#endif /* _NETIF_H_ */
