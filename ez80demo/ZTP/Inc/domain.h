/*
 * File       :     domain.h
 *
 * Description:  This file contains the DNS Client side defines and API declarations.
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

#ifndef _DOMAIN_H_
#define _DOMAIN_H_
/* domain.h - dn_cat */

/* Definitions of bits in the operation and parameter field */

#define	DN_QR		0100000		/* Query (0) or request (1) bit	*/
#define	DN_OPCDE	0074000		/* operation  code for query:	*/
					/*  0 => standard query		*/
					/*  1 => inverse query,		*/
					/*  2 => completion query,	*/
					/*  3 => completion (1 answer),	*/
					/*  4-15 => reserved.		*/
#define	DN_AA		0002000		/* 1 if authoritative answer	*/
#define	DN_TR		0001000		/* 1 if message truncated	*/
#define	DN_RD		0000400		/* 1 if recursion desired	*/
#define	DN_RA		0000200		/* 1 if recursion available	*/
#define	DN_RESVD	0000160		/* reserved			*/
// MCB L92 #define	DN_RESP		0000017		/* response code:		*/
#define	DN_RESP	0x0F
					/*  0 => no errors in query	*/
					/*  1 => format error in query	*/
					/*  2 => server failure		*/
					/*  3 => name does not exist	*/
 
struct	dn_mesg	{			/* domain system message format	*/
	UINT16	dn_id;			/* message id			*/
	UINT16	dn_opparm;		/* operation and parmameter bits*/
	UINT16	dn_qcount;		/* # entries in question seciton*/
	UINT16	dn_acount;		/* # RRs in answer section	*/
	UINT16	dn_ncount;		/* # RRs in nameserver section	*/
	UINT16	dn_dcount;		/* # RRs in additional section	*/
	INT8	dn_qaaa[1];		/* start of rest of the message	*/
	/* remaining fields of the domain name message are of variable	*/
	/* length, and consist of (1) a question section, (2) an answer	*/
	/* section, (3) an authority section (which says where to find	*/
	/* answers when they cannot be supplied), and (4) an addition	*/
	/* information section.  Entries in these are Resource Records.	*/
};

#define MAX_DNS_LABEL_SIZE		63
#define MAX_DNS_NAME_SIZE		255
#define DNS_SERVER_PORT			53

extern UINT32 ztpDnsTimeout;


typedef struct						DNS_LABEL_S
{
	UINT8								len;
	UINT8								label[ MAX_DNS_LABEL_SIZE ];
} DNS_LABEL_S;

typedef union 						DOMAIN_NAME_U
{
		UINT16							ptr;
		DNS_LABEL_S					name;
		UINT8			 				data[MAX_DNS_NAME_SIZE];
} DOMAIN_NAME_U;

/*
 * Resource records have the following format:
 *		Name (variable)		// Not included in RR Header structure
 *		Type
 * 	Class
 *		TTL
 * 	RD Lemgth
 *		Record Data				// Not included in RR Header structure
 * The RR_MINUS_NAME structure contains
 */
typedef struct						RR_HEADER_S
{
	UINT16								Type;
	UINT16								Class;
	INT32								TTL;
	UINT16								RDLength;
} RR_HEADER_S;


struct	dn_qsuf	{			/* question section name suffix	*/
	UINT16	dn_type;		/* type of this name		*/
	UINT16	dn_clas;		/* class of this name		*/
};

#define	DN_MLEN		128		/* message length (small query)	*/
#define	dn_cat(t,f)	{(*t) =(INT8)strlen(f);t++;strcpy(t,f);t+=strlen(f);}

/* Query type codes */

#define	DN_QTHA		1		/* Host address			*/
#define	DN_QTNS		2		/* Authoratative name server	*/
#define	DN_QTMD		3		/* Mail destination (obsolete)	*/
#define	DN_QTMF		4		/* Mail forwarder (obsolete)	*/
#define	DN_QTCN		5		/* Canonical name for an alias	*/
#define	DN_QTSZ		6		/* Start of zone of authority	*/
#define	DN_QTMB		7		/* Mailbox domain name		*/
#define	DN_QTMG		8		/* Mail group member		*/
#define	DN_QTMR		9		/* Mail rename domain name	*/
#define	DN_QTNL		10		/* Null resource record		*/
#define	DN_QTWK		11		/* Well-known service descriptor*/
#define	DN_QTPR		12		/* Domain name pointer		*/
#define	DN_QTHI		13		/* Host information		*/
#define	DN_QTMI		14		/* Mailbox or mail list info.	*/
#define	DN_QTMX		15		/* Mail, replaces MD & MF	*/

/* Query class codes */

#define	DN_QCIN		1		/* The DARPA Internet		*/
#define	DN_QCCS		2		/* CSNET (now obsolete)		*/
#define	DN_QCHA		3		/* Chaos network		*/

#define	DN_CMPRS	0300		/* Compressed format is pointer	*/
#define	DN_CPTR		037777		/* Compressed format bits of ptr*/
#define	DN_RLEN		10		/* resource record heading len.	*/

extern UINT32 name2ip(INT8 *nam);
extern UINT32 gname(INT8 *nam);
extern UINT32 resolve(INT8 *nam);
extern INT8 *ip2dot(INT8 *pdot, UINT32 ip);
extern INT8 *ip2name(UINT32 ip, INT8 *nam);

#endif
