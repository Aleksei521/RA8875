/*
 * File       :     bootp.h
 *
 * Description:  This file contains the bootp related defines and API declarations.
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

#ifndef _BOOTP_H_
#define _BOOTP_H_

/* 
 * bootp.h - header definitions for bootp protocol
 * 
 * Programmer:	Scott M Ballew
 * 		Cypress Network Operations Center
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Mon Jun 11 16:32:12 1990
 *
 * Copyright (c) 1990 Cypress Network Operations Center
 *
 */

/*
 * $Log: bootp.h,v $
 * Revision 1.16.2.2  2002/01/29 17:15:12  coskrey
 * This implements a full DHCP client.  It's able to renew its lease, rebind to
 * another DHCP server, and will continue to try to establish a lease if there
 * is no DHCP server available on the network.
 *
 * This also fixes the case where there is no router, which will probably be a
 * common scenario for an embedded device.
 *
 * Revision 1.16.2.1  2002/01/22 14:19:35  coskrey
 * Some DHCP servers respond with 0xffffffff as a lease time (INFINITE).  So treat
 * this as infinite and don't set up dhcp_timer.
 *
 * Also, since sleep() takes an int argument, and the lease times we get back from
 * DHCP are 32 bits, we need to make sure we account for lease times which are
 * bigger than can be held in an int.
 *
 * Revision 1.16  2002/01/04 18:50:40  coskrey
 * This fixes MPTS 1569.  IPWorks will now follow RFC 1531 and renew the DHCP
 * lease before it expires.  If a DHCP lease expires without being renewed, and
 * IPWorks cannot get a new lease, the network interface will be shut down.
 * IPWorks will then attempt to re-establish a DHCP lease every 3 minutes.
 *
 * Revision 1.15  2001/12/06 15:22:52  anderson
 * Make vend unsigned sine on of the values we use it 255
 *
 * Revision 1.14  2001/11/01 20:04:24  coskrey
 * The Windows DHCP service won't give you the gateway address or DNS server
 * name by default; you have to specifically ask for them in the DHCP options.
 * I added all the parameters we could possibly use to the DHCP_REQUEST packet
 * that we send.
 *
 * Revision 1.13  2001/10/23 19:39:08  coskrey
 * Modify bootp_parse to look at the xid in the reply and make sure it matches
 * the xid that we sent out.  This will keep us from getting broadcast replies
 * that are meant for other hosts.
 *
 * Revision 1.12  2001/09/18 23:23:40  nick
 *
 * Fix incorrect format of the DHCPREQUEST packet.
 *
 * The DHCPREQUEST packet should include the server identifier DHCP option,
 * and the "requested IP" DHCP option.  The requested IP is actually the
 * IP offered by the server.
 *
 *                          THIS IS AN UGLY HACK!!!
 *
 * But, as my comments in the code state, to fix this, we would have to
 * drastically re-org the DHCP/BOOTP code, which we cannot and probably
 * will not do.  I tried to make comments wherever I could.  I hope they
 * are found to be explanatory & helpful.
 *
 * I also wrapped some loose debug messages.
 *
 * I've tested this, and it appears to be working.  This fixes MPTS #1497.
 *
 * Nick.
 *
 * Revision 1.11  2001/09/12 19:58:45  dave
 * Align DHCP retry intervals to RFC1531 (minus randomness).
 *
 * Revision 1.10  2001/08/01 19:21:19  dave
 * Add RFC1531 message type option, and final client response message.
 *
 * Revision 1.9  2001/08/01 17:27:54  dave
 * Add byte to BOOTP request length to account for END option.
 *
 * Revision 1.8  2001/07/25 06:02:09  anderson
 * New interface independent interface for bootp()
 *
 * Revision 1.7  2001/07/23 13:22:38  anderson
 * Add copyright statements
 *
 * Revision 1.6  2001/07/13 02:10:10  anderson
 * Correct the size of a bootp packet
 *
 * Revision 1.5  2001/06/05 20:04:20  anderson
 * Seperate bootinfo from bootp
 *
 * Revision 1.4  2001/06/05 13:35:42  anderson
 * Remove the #ifdef WIRELESS stuff
 *
 * Revision 1.3  2001/06/01 17:20:52  anderson
 * Multipl einclusion protection works better if the protecting #define is
 * done at the top of the file, rather than the bottom. In some cases, recursion
 * can occurr if it's not done this way.
 *
 * Revision 1.2  2001/03/07 02:30:32  anderson
 * Add function prototypes and define some DHCP things
 *
 * Revision 1.1  1991/09/29  18:28:31  sdo
 * Initial revision
 *
 * Revision 1.1  90/10/01  21:58:54  sjm
 * Initial revision
 * 
 * Revision 1.6  90/07/10  15:50:51  smb
 * Cleaned up and commented.
 * 
 * Revision 1.5  90/07/10  10:06:24  smb
 * Added external declaration of bpmsg.
 * 
 * Revision 1.4  90/07/03  15:08:40  smb
 * cc11 does not like unsigned chars so removed the unsigned keyword.
 * 
 * Revision 1.3  90/06/27  09:52:49  smb
 * Made definitions of bootp protocol names and ports conditional on DEBUG
 * 
 * Revision 1.2  90/06/20  16:28:49  smb
 * Changed name of magic cookies from MAGIC to RFC1084 and XMAGIC to XINU.
 * 
 * Revision 1.1  90/06/11  16:33:10  smb
 * Initial revision
 * 
 */

/* BOOTP_RECV process info */
#define BOOTP_RECV_PROC bootp_recv

#define BOOTP_RECV_STK  4096
#define BOOTP_RECV_PRIO 100	/* high priority while running */
#define DHCP_TIMER_PRIO 50	/* high priority while running */
#define BOOTP_RECV_NAM	"bootp_recv"
#define BOOTP_RECV_ARGS	1,(INT32)etherdev

#define BOOTPS	"bootps"
#define BOOTPC	"bootpc"
#define NBOOTPS	67
#define NBOOTPC	68

#define SZCHADDR	16    		/* size of client haddr field	*/
#define SZSNAME		64    		/* size of server name field	*/
#define SZFNAME		128    		/* size of file name field	*/
#define SZVENDOR	312    		/* size of vendor spec. field	*/

#define MAX_HOST_NAME_SIZE 64

/* DHCP message types. */
#define DHCPDISCOVER    1
#define DHCPOFFER       2
#define DHCPREQUEST     3
#define DHCPDECLINE     4
#define DHCPACK         5
#define DHCPNAK         6
#define DHCPRELEASE     7
#define DHCPINFORM      8

/* values for op		*/
#define BOOTREQUEST	1
#define BOOTREPLY	2

/* values for htype and hlen	*/
#define ETHERNET	1
#define ELEN		6

/* values for flags */
#define DHCP_RQBCAST	0x8000	/* Request that the server bcast its reply */

struct bootp_msg {
    INT8 op;				/* request or reply 		*/
    INT8 htype;				/* hardware type		*/
    INT8 hlen;				/* hardware address length	*/
    INT8 hops;				/* set to zero			*/
//    uint32_t xid;			/* transaction id		*/
//    uint16_t secs;		/* time client has been trying	*/
//    int16_t flags;			/* DHCP flags			*/
    UINT32 xid;			/* transaction id		*/
    UINT16 secs;		/* time client has been trying	*/
    INT16 flags;			/* DHCP flags			*/

    UINT32 ciaddr;			/* client IP address		*/
    UINT32 yiaddr;			/* your (client) IP address	*/
    UINT32 siaddr;			/* server IP address		*/
    UINT32 giaddr;			/* gateway IP address		*/
    UINT8 chaddr[SZCHADDR];	/* client hardware address	*/
    INT8 sname[SZSNAME];		/* server host name		*/
    INT8 file[SZFNAME];			/* boot file name		*/
    UINT8 vend[SZVENDOR];
};

#define RFC1084		0x63825363UL	/* vendor magic cookie from 1084*/
#define XINU		0x78696e75UL	/* vendor magic cookie "xinu"	*/

struct vendor {
    INT32 v_magic;			/* vendor magic cookie		*/
    INT8 v_data[1];
};

struct sub_vendor {			/* vendor sub-field		*/
    INT8 s_tag;				/* tag octet			*/
    INT8 s_length;			/* length			*/
    INT8 s_data[1];			/* length bytes of data		*/
};

/* tags */
#define PAD			0
#define SUBNETMASK		1
#define TIME_OFFSET		2
#define GATEWAY			3
#define TIME_SERVER		4
#define IEN_NAMESERVER		5
#define DOMAIN_NAMESERVER	6
#define LOG_SERVER		7
#define COOKIE_SERVER		8
#define LPR_SERVER		9
#define IMPRESS_SERVER		10
#define RLP_SERVER		11
#define HOSTNAME		12
#define BOOT_FILE_SIZE		13	/* in 512 octet blocks	*/
#define NETWORK_TIME_SERVERS	42
#define DHCP_REQUESTED_IP	50
#define DHCP_LEASE_TIME		51
#define	DHCP_MSG_TYPE		53
#define	DHCP_SERVER_IDENTIFIER	54
#define DHCP_PARAMETER_LIST	55
#define CLIENT_IDENTIFIER	61

#define BEGIN_RESERVED		128	/* these may define 	*/
#define END_RESERVED		254	/* site-specific items	*/

#define END			255

#define DHCP_OPTIONS_COOKIE     "\143\202\123\143"
#define DHCP_OPTIONS_COOKIE_SIZE	4

/* Minimum DHCP packet plus type and end options. */
#define BOOTP_SIZE (sizeof(struct bootp_msg)-SZVENDOR)

extern struct bootp_msg *bpmsg;

#define	BOOTP_RESEND	2	/* Initial bootp resend interval */
#define	BOOTP_MAXRETRY	5	/* give up after five attempts */

#define DHCP_INFINITE_LEASE	0xffffffffUL	/* Infinite lease time */
/* bootp.c */
extern INT16 bootp(RZK_DEVICE_CB_t *);
extern UINT8	b_use_dhcp;
extern UINT8 Reboot_if_diff_IP;		//Reboot if we get a different IP at renew time.
extern UINT8 ztpDhcpRtrs;


#endif /* _BOOTP_H_ */
