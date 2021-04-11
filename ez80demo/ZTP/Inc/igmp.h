/*
 * File       :     igmp.h
 *
 * Description:  This file contains igmp defines and API declarations
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

#ifndef _IGMP_H_
#define _IGMP_H_
/* igmp.h - IG_VER, IG_TYP */
#include "ZTPDefines.h"
#include "ZTPTCP.h"

extern UINT16	HG_TSIZE;		/* host group table size	*/

#define	IG_VERSION	 1		/* RFC 1112 version number	*/
#define	IG_HLEN		 8		/* IGMP header length		*/
#define	IGT_HQUERY	 1		/* host membership query	*/
#define	IGT_HREPORT	 2		/* host membership report	*/

struct igmp {
	UINT8	   ig_vertyp;	/* version and type field	*/
	UINT8	   ig_unused;	/* not used by IGMP		*/
	UINT16	   ig_cksum;	/* compl. of 1's compl. sum	*/
	UINT32   ig_gaddr;	/* host group IP address	*/
};

#define	IG_VER(pig)	(((pig)->ig_vertyp>>4) & 0xf)
#define	IG_TYP(pig)	((pig)->ig_vertyp & 0xf)

#define	IG_NSEND	2		/* # IGMP join messages to send	*/
#define	IG_DELAY	5		/* delay for resends (1/10 secs)*/

/* Host Group Membership States */

#define	HGS_FREE	0	/* unallocated host group table entry	*/
#define	HGS_DELAYING	1	/* delay timer running for this group	*/
#define	HGS_IDLE	2	/* in the group but no report pending	*/
#define	HGS_STATIC	3	/* for 224.0.0.1; no state changes	*/

struct hg {
	UINT8	   hg_state;	/* HGS_* above			*/
	UINT8	   hg_ifnum;	/* interface index for group	*/
	UINT32	hg_ipa;	   /* IP multicast address		*/
	UINT16	   hg_refs;	   /* reference count		*/
	BOOL		hg_ttl;	   /* max IP ttl for this group	*/
};

/* Host Group Update Process Info. */

#define	IGUSTK		512		/* stack size for update proc.	*/
#define	IGUPRI		 50		/* update process priority	*/
#define	IGUNAM		"igmp_update"	/* name of update  process	*/
#define	IGUARGC		0		/* count of args to hgupdate	*/

struct hginfo {
	BOOL	hi_valid;	/* TRUE if hginit() has been called	*/
	RZK_SEMAPHOREHANDLE_t	hi_mutex;
//	SID	hi_mutex;	/* table mutual exclusion		*/
//	MPID	hi_uport;	/* listen port for delay timer expires	*/
	RZK_TIMERHANDLE_t hi_tmrhdl;
};

extern struct hginfo	HostGroup;
extern BOOL IGMPTmr;

extern UINT32 ig_allhosts;	/* "all hosts" group address (224.0.0.1)*/
extern UINT32 ig_allDmask;	/* net mask to match all class D addrs.	*/
/* hgadd.c */
extern INT16 hgadd(UINT8 ifnum, UINT32 ipa, BOOL islocal);
/* hgarpadd.c */
extern INT16 hgmacadd(UINT8 ifnum, UINT32 ipa);
/* hgarpdel.c */
extern INT16 hgmacdel(UINT8 ifnum, UINT32 ipa);
/* hginit.c */
extern INT16 hginit(UINT8 iface);
/* hgjoin.c */
extern INT16 hgjoin(UINT8 ifnum, UINT32 ipa, BOOL islocal);
/* hgleave.c */
extern INT16 hgleave(UINT8 ifnum, UINT32 ipa);
/* hglookup.c */
extern struct hg *hglookup(UINT8 ifnum, UINT32 ipa);
extern INT16 hggetttl(UINT8 ifnum, UINT32 ipa);
/* hgprint.c */
extern INT16 hgprint(UINT8 ifnum,RZK_DEVICE_CB_t * fd);
/* hgrand.c */
extern UINT16 hgrand(UINT8 iface);
/* igmp.c */
extern INT16 igmp(UINT8 typ, UINT8 ifnum, UINT32 hga);
/* igmp_in.c */
//extern SYSCALL igmp_handler(  in_Header *ip );   
/* igmp_settimers.c */
extern INT16 igmp_settimers(UINT8 ifnum,UINT32 hga);
/* igmp_update.c */
extern INT16 igmp_update(void);

extern INT16 hgip2hwaddr(  eth_address	hwa,UINT32	ipa );

#endif /* _IGMP_H_ */
