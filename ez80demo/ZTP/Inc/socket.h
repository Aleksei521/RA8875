#ifndef _SOCKET_H
/*
 * File       :     socket.h
 *
 * Description:  This file contains socket related definitions.
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
#define _SOCKET_H
#include "stdarg.h"
#include "sockaddr.h"

#ifndef WATTCPF
#include "ZTPtcp.h"
#endif

/* Local IP wildcard address */
#define INADDR_ANY      0x0L

/* IP protocol numbers */
/* now in internet.h */

/* TCP port numbers */
#define IPPORT_ECHO     	7       /* Echo data port */
#define IPPORT_DISCARD  9       /* Discard data port */
#define IPPORT_FTPD     	20      /* FTP Data port */
#define IPPORT_FTP      	21      /* FTP Control port */
#define IPPORT_TELNET   	23      /* Telnet port */
#define IPPORT_SMTP     	25      /* Mail port */
#define IPPORT_FINGER   	79      /* Finger port */
#define IPPORT_TTYLINK 	87      /* Chat port */
#define IPPORT_POP      	109     /* pop2 port */
#define IPPORT_NNTP     	119     /* Netnews port */

/* UDP port numbers */
#define IPPORT_DOMAIN   53
#define IPPORT_BOOTPS   	67
#define IPPORT_BOOTPC   	68
#define IPPORT_RIP      	520
#define IPPORT_REMOTE   1234    /* Pulled out of the air */

#define AF_UNSPEC       	0
#define AF_INET         	1
#define AF_AX25         	2
#define AF_NETROM       	3
#define AF_LOCAL        	4

#ifdef	SSL_SOCKET
#define SOCK_SSL     		4
#endif

#define SOCK_STREAM     	0
#define SOCK_DGRAM      	1
#define SOCK_RAW        	2
#define SOCK_SEQPACKET 3

#define DONT_ROUTE  0
#define OOB			1

/* Socket flag values - controls newline mapping */
#define SOCK_BINARY     	0       /* socket in raw (binary) mode */
#define SOCK_ASCII      	1       /* socket in cooked (newline mapping) mode */
#define SOCK_QUERY      	2       /* Return setting without change */

#define ZTP_SOCK_OK			(INT16)0
#define ZTP_SOCK_ERR		(INT16)-1
#define EAFNOSUPPORT    		(INT16)-2
#define EOPNOTSUPP       		(INT16)-3
#define EFAULT            		(INT16)-4
#define EISCONN         		(INT16)-5
#define ECONNREFUSED    		(INT16)-6
#define EPROTONOSUPPORT 	(INT16)-7
#define ENOBUFS         		(INT16)-8
#define EINVAL  				(INT16)-9
#define EBADF  				(INT16)-10
#define ENOCON				(INT16)-11
#define EMFILE				(INT16)-12
#define EINVALBKLOG			(INT16)-13
#define EPIPE				(INT16)-14
#define EFETNOSUPPORT 		(INT16)-15
#define EDEADSOCK			(INT16)-16
#define EIOBINPRGRSS		(INT16)-17
#define EGRACECLOSE			(INT16) 0
//End

#define FIONBIO 			1 
#define FIONREAD 		2
#define SIOCATMARK 		3
#define FCNCLBIO			4
#define FUDPCKSUM		5
#define UDPTIMEOUT		6
#define FDISNAGLE		7
#define FENANAGLE		8
#define FIONWRITE 		9
//#ifdef	SSL_SOCKET
#define FFLUSH		      12
//#endif

/** KM $ 16-JUN-2005 **/
#define UDPCONNECT		10
#define UDPDISCONNECT	11
//End

//#ifdef TCP_KEEPALIVE
#define TCPKEEPALIVE_ON		13
#define TCPKEEPALIVE_OFF	14
//#endif

extern INT tcp_tick(sock_type *s);
VOID SocketInit();
VOID SocketClose();
INT16 accept (INT16 s,struct sockaddr *peername,INT16 *peernamelen);
INT16 bind (INT16 s,struct sockaddr *name,INT16 namelen);
INT16 close_s (INT16 s);
INT16 connect (INT16 s,struct sockaddr *peername,INT16 peernamelen);
INT16 listen (INT16 s,INT16 backlog);
INT16 socket (INT16 af,INT16 type,INT16 protocol);
INT16 recvfrom (INT16 s,INT8 *buf,INT16 len,INT16 flags,struct sockaddr *from,INT16 *fromlen);
INT16 sendto (INT16 s,INT8 *buf,INT16 len,INT16 flags,struct sockaddr *to,INT16 tolen);
INT16 send (INT16 s, INT8 *buf, INT16 nbyte, INT16 flags);
INT16 recv (INT16 s, INT8 *buf, INT16 nbyte, INT16 flags);
INT getpeername (INT16 ,struct sockaddr *, INT *);
INT getsockname(INT16 s,struct sockaddr *sock_addr,INT *addr_size);
INT16 ioctlsocket (INT16 ,INT,UINT32*);

// MK $ 04-04-05 $ IAR PORTING
/*
extern _ext_socket *Socket[]; */
#define sFree  0
#define sDev   1
#define sAlloc 2
#define sBind  3
#define sListn 4
#define sUsed  5

#endif  /* _SOCKET_H */
