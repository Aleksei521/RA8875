
#ifndef _wattcp_wattcp_h
#define _wattcp_wattcp_h
/*
 * File       :     ZTPtcp.h
 *
 * Description:  This file contains TCP/IP stack related definitions.
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
 
//AJ - for multihomed host
#define MULTIHOMING
/* RK : 02/01/05: Below line is commented as ZTPConfig.h file incFludes ztptcp.h */

#include "Ztypes.h" 
#include "xc_lib.h"

#include "ZTPDefines.h"

extern void* RZKMemcpy(void *, const void *, RZK_LENGTH_t);
/*
 * Are we compiling the kernel?
 *    or an application using the kernel?
 */
#if !defined(__WATTCP_USER__)
#define __WATTCP_KERNEL__
#endif

/*
 * Note that some stuff is not available to user applications.
 *   This is generally detail you shouldn't need to worry about,
 *   and best stay away from to preserve the kernel integrity.
 * Note also that there is a lot of other stuff that should probably
 *   be protected but isn't.
 */

#define WATTCPH
#include "stdio.h"
/* these are visible for select.c routine return values */
#define pack //Ajay-WM, copied from OCB, to nullify structure packing
#define SOCKESTABLISHED 1
#define SOCKDATAREADY   2
#define SOCKCLOSED      4

#if defined(__WATTCP_KERNEL__)

#define IP_TYPE     0x0008

/*
#define DEBUG
*/
/*
#define MoveW( a , b, c ) movmem( a, b,c )
#define busyouta(c) (putch(c))
*/

#ifndef min
#define min(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef max
#define max(a,b) (((a)>(b))?(a):(b))
#endif

#define movmem(s,d,c) RZKMemcpy((void*)d,(void*)s,(RZK_LENGTH_t)c)


#include "elib.h"

#define MAX_GATE_DATA 12
#define MAX_STRING 50   /* most strings are limited */
#endif  /* defined(__WATTCP_KERNEL__) */

#define MAX_NAMESERVERS 10
#define MAX_COOKIES 10

#if defined(__WATTCP_KERNEL__)

#define MAXVJSA     1440 /* 10 s */
#define MAXVJSD     360  /* 10 s */
#define SAFETYTCP  0x538f25a3L
#define SAFETYUDP  0x3e45e154L
#ifndef TRUE
#define TRUE (1)
#define true        TRUE
#endif

#ifndef FALSE
#define FALSE       (0)
#define false       FALSE
#endif


#define EL_INUSE        0x0001
#define EL_DELAY        0x0002
#define EL_TCP          0x0004
#define EL_SERVER       0x0008
#define EL_ASCII        0x0010
#define EL_NEVER        0x0020

/* These are Ethernet protocol numbers but I use them for other things too */
#define UDP_PROTO  0x11
#define TCP_PROTO  0x06
#define ICMP_PROTO 0x01

#endif /* defined(__WATTCP_KERNEL__) */

#define TCP_MODE_BINARY  0       /* default mode */
#define TCP_MODE_ASCII   1
#define UDP_MODE_CHK     0       /* default to having checksums */
#define UDP_MODE_NOCHK   2       /* turn off checksums */
#define TCP_MODE_NAGLE   0       /* Nagle algorithm */
#define TCP_MODE_NONAGLE 4

#define UDP_MODE_CONNECT 8

#if 0
//AJ-CLEANUPs
typedef INT (*procref)();
typedef unsigned long UINT32;     /* 32 bits */
typedef unsigned short word;        /* 16 bits */
typedef unsigned char byte;         /*  8 bits */
#endif 
typedef UINT8 eth_address[6];

#if defined(__WATTCP_KERNEL__)

/* undesirable */
extern UINT32 MsecClock();
#define clock_ValueRough() MsecClock()

//AJ-WM #define checksum( p, len) inchksum( p, len )
#define cksum(p,len) inchksum( p, len ) //Ajay-WM copied here from OCB

#define PD_ETHER 1
#define PD_SLIP  6

extern INT16 g_sock_inactive;      /* in pcbootp.c */
extern UINT16 _pktdevclass;
extern UINT16 _mss;
extern UINT16 _bootptimeout;   /* in pcbootp.c */
extern UINT32 _bootphost;   /* in pcbootp.c */
extern UINT16 _bootpon;
extern void* RZKMemcpy(void *, const void *, RZK_LENGTH_t);
/* The Ethernet header */
typedef struct {
    eth_address     destination;
    eth_address     source;
    UINT16            type;
} eth_Header;

/* The Internet Header: */
/*AJ commented , change the order of hdrlen and ver for bit field's sake
typedef struct {
    unsigned       hdrlen  : 4;
    unsigned       ver     : 4;
    byte       tos;
    word            length;
    word            identification;
    word            frags;
    byte       ttl;
    byte       proto;
    word            checksum;
    UINT32        source;
    UINT32        destination;
} in_Header;
*/

typedef struct {
    UINT8      verHdrlen ; 
    //AJ - No bit fields - unsigned  char     hdrlen  : 4 pack;
    UINT8            tos ;
    UINT16            length ;
    UINT16            identification ;
    UINT16  frags;
    UINT8            ttl ;
    UINT8            proto ;
    UINT16            checksum ;
    UINT32        source ;
    UINT32        destination ;
} in_Header;
//( in_GetHdrlen(ip) << 2 ) 

#if 0
//AJ - for porting on EZ80
#define in_GetVersion(ip) ( (ip)->ver )
#define in_GetHdrlen(ip)  ( (ip)->hdrlen )  /* 32 bit word size */
#define in_GetHdrlenBytes(ip)  ( in_GetHdrlen(ip) << 2 ) /* 8 bit byte size */
#endif 
#define in_GetVersion(ip)    ((ip)->verHdrlen>>4)
#define in_GetHdrlen(ip)     ((ip)->verHdrlen&0x0f)  /* 32 bit word size */
#define in_GetHdrlenBytes(ip)  (in_GetHdrlen(ip) <<2 ) /* 8 bit byte size */

#define in_GetTos(ip)      ( (ip)->tos)
#define in_GetTTL(ip)      ((ip)->ttl)
#define in_GetProtocol(ip) ((ip)->proto )

typedef struct {
    UINT16       srcPort;
    UINT16       dstPort;
    UINT16       length;
    UINT16       checksum;
} udp_Header;

#define UDP_LENGTH ( sizeof( udp_Header ))

typedef struct {
    UINT16            srcPort;
    UINT16            dstPort;
    UINT32        seqnum;
    UINT32        acknum;
    UINT16            flags;
    UINT16            window;
    UINT16            checksum;
    UINT16            urgentPointer;
} tcp_Header;

#define tcp_FlagFIN           0x0001
#define tcp_FlagSYN           0x0002
#define tcp_FlagRST           0x0004
#define tcp_FlagPUSH          0x0008
#define tcp_FlagACK           0x0010
#define tcp_FlagURG           0x0020
#ifdef LEGACY_ZTP_TIMING
   #define tcp_FlagDO         0xF000
#else
   #define tcp_FlagDoAck      0x8000
#endif
#define tcp_GetDataOffset(tp) (intel16((tp)->flags) >> 12)

#endif /* defined(__WATTCP_KERNEL__) */

/* The TCP/UDP Pseudo Header */
typedef struct {
    UINT32    src;
    UINT32    dst;
    UINT8     mbz;
    UINT8     protocol;
    UINT16    length;
    UINT16    checksum;
} tcp_PseudoHeader;


/* A datahandler for tcp or udp sockets */
typedef INT (*dataHandler_t)( VOID *s, UINT8 *data, INT len, tcp_PseudoHeader *pseudohdr, VOID *protohdr );
/* A socket function for delay routines */
typedef INT (*sockfunct_t)( VOID *s );

#if defined(__WATTCP_KERNEL__)
/*
 * TCP states, from tcp manual.
 * Note: close-wait state is bypassed by automatically closing a connection
 *       when a FIN is received.  This is easy to undo.
 */
#define tcp_StateLISTEN    0      /* listening for connection */
#define tcp_StateSYNSENT   1      /* syn sent, active open */
#define tcp_StateSYNREC    2      /* syn received, synack+syn sent. */
#define tcp_StateESTAB     3      /* established */
#define tcp_StateESTCL     4      /* established, but will FIN */
#define tcp_StateFINWT1    5      /* sent FIN */
#define tcp_StateFINWT2    6      /* sent FIN, received FINACK */
#define tcp_StateCLOSWT    7      /* received FIN waiting for close */
#define tcp_StateCLOSING   8      /* sent FIN, received FIN (waiting for FINACK) */
#define tcp_StateLASTACK   9      /* fin received, finack+fin sent */
#define tcp_StateTIMEWT    10     /* dally after sending final FINACK */
#define tcp_StateCLOSEMSL  11
#define tcp_StateCLOSED    12     /* finack received */


#endif


#if defined(__WATTCP_KERNEL__)
/*
 * UDP socket definition
 */
typedef struct _udp_socket 
{
   struct _udp_socket * next;
   UINT16               ip_type;       /* always set to UDP_PROTO */
   INT8               * err_msg;       /* null when all is ok */
   INT8               * usr_name;
   VOID              (* usr_yield )( VOID );
   UINT8                rigid;
   UINT8                stress;
   UINT16               sock_mode;     /* a logical OR of bits */
   UINT32               usertimer;     /* ip_timer_set, ip_timer_timeout */
   dataHandler_t        dataHandler;
   eth_address          hisethaddr;    /* peer's ethernet address */
   UINT32               hisaddr;       /* peer's internet address */
   UINT16               hisport;       /* peer's UDP port */
   UINT32               myaddr;
   UINT16               myport;
   UINT16               locflags;
   INT                  queuelen;
   UINT8              * queue;
   INT                  rdatalen;      /* must be signed */
   UINT16               maxrdatalen;
   UINT8              * rdata;
   UINT8              * rddata;        /* if dataHandler = 0, len == 512 */
   UINT32               safetysig;
   UINT8                err_code;      /* Err code. for blocked threads  -AJ*/
   UINT8                block;   
   #ifdef MULTIHOMING
      UINT8             iface;         /*AJ$ 28may$ for multihoming*/
   #endif
   VOID               * AppThreadRecv; /* to provide blocking support $ AJ*/
} udp_Socket;
#endif /* __WATTCP_KERNEL__ */




/*
 * TCP Retransmission Time-Out (RTO) definitions
 *
 * ZTP uses the RZK system timer (10ms period) for RTO timing (rRZKTicks).
 * The minimum value of the RTO (RTO_MIN_TICKS) is nominally 200ms.
 */
extern unsigned int           tRZKTicks;
#define RTO_TICKS_PER_SEC     100
#define SEND_SOON_TICKS       20
#define RTO_MIN_TICKS         50
#define MAX_RTO_TICKS         (RTO_TICKS_PER_SEC*60*4)

 
/*
 * The rto_mode member of the tcp_Socket structure tracks the current operating
 * mode of the Retransmission Time-Out (RTO) timer as defined below:
 *
 * RTO_MODE_IDLE        RTO Timer is inactive
 * RTO_MODE_NORAML      RTO Timer used to initiate retransmission upon timeout
 * RTO_MODE_SEND_SOON   RTO Timer forced to expire early to initiate deferred transmission
 * RTO_MODE_RETRANSMIT  Active retransmission, RTO increases exponentially until valid ACK
 */
#ifndef LEGACY_ZTP_TIMING
#define RTO_MODE_IDLE         0
#define RTO_MODE_NORMAL       1
#define RTO_MODE_SEND_SOON    2
#define RTO_MODE_RETRANSMIT   3
#else
#define RTO_MODE_NORMAL       0
#define RTO_MODE_SEND_SOON    1
#define RTO_MODE_RETRANSMIT   2
#endif



#if defined(__WATTCP_KERNEL__)
/*
 * TCP Socket definition
 */
typedef struct _tcp_socket 
{
   struct _tcp_socket * next;
   UINT16               ip_type;       /* always set to TCP_PROTO */
   INT8               * err_msg;
   INT8               * usr_name;
   VOID              (* usr_yield )( VOID );
   UINT8                rigid;
   UINT8                stress;
   UINT16               sock_mode;     /* a logical OR of bits */

   UINT32               usertimer;     /* ip_timer_set, ip_timer_timeout */
   dataHandler_t        dataHandler;   /* called with incoming data */
   eth_address          hisethaddr;    /* ethernet address of peer */
   UINT32               hisaddr;       /* internet address of peer */
   UINT16               hisport;       /* tcp ports for this connection */
   UINT32               myaddr;
   UINT16               myport;
   UINT16               locflags;

#ifndef LEGACY_ZTP_TIMING
   UINT                 queuelen;
   UINT8              * queue;

   UINT                 rdatalen;
   UINT16               maxrdatalen;
   UINT8              * rdata;
   UINT8              * rddata   ;     /* received data */
   UINT32               safetysig;
   UINT8                err_code;      /* Err code. for blocked threads  -AJ*/
   UINT8                block;   
   #ifdef MULTIHOMING
      UINT8             iface;         /*AJ$ 28may $ for multihoming*/
   #endif
   VOID               * AppThreadRecv; /* to provide blocking support -AJ*/
   VOID               * AppThreadSend; /* to provide blocking support -AJ*/   
   UINT8                state;         /* connection state */
   UINT32               acknum;
   UINT32               RtoSeq;
   UINT32               seqnum;        /* data ack'd and sequence num */

   UINT8                unhappy;       /* flag, indicates retransmitting segt's */
   UINT                 flags;         /* tcp flags word for last packet sent */

   UINT16               window;        /* other guy's window */
   UINT24               datalen;       /* number of bytes of data to send */
   UINT24               PushLen;       /* number of bytes to be sent with PUSH flag set */
   UINT24               unacked;       /* unacked data */

   UINT8                cwindow;       /* Van Jacobson's algorithm */
   UINT8                wwindow;

   /* Retransmsission Timeout (RTO) structure members */
   UINT16               vj_sa;         /* VJ's alg, standard average */
   UINT16               vj_sd;         /* VJ's alg, standard deviation */
   UINT16               rto;
   UINT8                rto_mode;      /* RTO timer operating mode  */
   UINT24               rto_start;     /* RTT Timestamp prior to packet transmission */
   UINT24               rto_ticks;     /* number of ticks until RTO */

   /* Round Trip Time (RTT) structure members */
   UINT8                rtt_active;    /* non-zero when RTT timer running */
   UINT24               rtt_start;     /* RTT Timestamp prior to packet transmission */

   /* Multi-purpose timer (MPT) */
   ZTP_TIMER            mpt_tmr;

   UINT8                tos;           /* priority */
   UINT16               mss;
   UINT8              * data ;         /* data to send */
 
   ZTP_TIMER            inactive_tmr;

   //#ifdef TCP_KEEPALIVE 
   ZTP_TIMER            KeepAlive_tmr;
   //#endif

#else
   INT                  queuelen;
   UINT8              * queue;

   INT                  rdatalen;      /* must be signed */
   UINT16               maxrdatalen;
   UINT8              * rdata;
   UINT8              * rddata   ;     /* received data */
   UINT32               safetysig;
   UINT8                err_code;      /* Err code. for blocked threads  -AJ*/
   UINT8                block;   
   #ifdef MULTIHOMING
      UINT8             iface;         /*AJ$ 28may $ for multihoming*/
   #endif
   VOID               * AppThreadRecv; /* to provide blocking support -AJ*/
   //#ifdef TCP_KEEPALIVE 
   UINT32               keepAliveTimeout ;
   UINT32               keepAliveExpiryTime;
   //#endif
   VOID               * AppThreadSend; /* to provide blocking support -AJ*/   
   UINT16               state;         /* connection state */
   UINT32               acknum;
   UINT32               seqnum;        /* data ack'd and sequence num */
   INT32                timeout;       /* timeout, in milliseconds */
   UINT8                unhappy;       /* flag, indicates retransmitting segt's */
   UINT8                recent;        /* 1 if recently transmitted */
   UINT16               flags;         /* tcp flags word for last packet sent */

   UINT16               window;        /* other guy's window */
   INT                  datalen;       /* number of bytes of data to send */
                                       /* must be signed */
   INT                  unacked;       /* unacked data */

   UINT8                cwindow;       /* Van Jacobson's algorithm */
   UINT8                wwindow;

   UINT16               vj_sa;         /* VJ's alg, standard average */
   UINT16               vj_sd;         /* VJ's alg, standard deviation */
   UINT32               vj_last;       /* last transmit time */
   UINT16               rto;
   UINT8                rto_mode;      /* count of packets */
   UINT8                tos;           /* priority */
   /* retransmission timeout procedure */
   /* these are in clock ticks */
   UINT32               rtt_lasttran;  /* last transmission time */
   UINT32               rtt_smooth;    /* smoothed round trip time */
   UINT32               rtt_delay;     /* delay for next transmission */
   UINT32               rtt_time;      /* time of next transmission */

   UINT16               mss;
   UINT32               inactive_to;   /* for the inactive flag */
   INT                  sock_delay;

   UINT8              * data ;         /* data to send */
   UINT32               datatimer;     /* EE 99.08.23 note broken connections */
#endif
   UINT32               frag[2];       /* S. Lawson - handle one dropped segment */
   INT16                sock_fd;       /* Satish - corresponging socket[index] */
   UINT8                maxRtrs;       //Ajalgo
} tcp_Socket;
#endif /* __WATTCP_KERNEL__ */

#if defined(__WATTCP_KERNEL__)
/* sock_type used for socket io */
typedef union {
    udp_Socket udp;
    tcp_Socket tcp;
} sock_type;
#else /* __WATTCP_USER__ */
typedef VOID sock_type;
#endif /* __WATTCP_USER__ */

/* similar to UNIX */
/*AJ, for OCB->NCB
typedef struct sockaddr {
    word        s_type;
    word        s_port;
    UINT32    s_ip;
    byte        s_spares[6];    // unused in TCP realm 
} sockaddr;
#define sockaddr_in sockaddr
*/
        /*
         * TCP/IP system variables - do not change these since they
         *      are not necessarily the source variables, instead use
         *      ip_Init function
         */
extern UINT32 my_ip_addr;
extern UINT32 sin_mask;       /* eg.  0xfffffe00L */
extern UINT16 sock_delay;
extern UINT16 sock_data_timeout;  /* EE 99.08.23 */

#if defined(__WATTCP_KERNEL__)
extern eth_address _eth_addr;
extern eth_address _eth_brdcast;
#endif


#if defined(__WATTCP_KERNEL__)
/*
 * ARP definitions
 */
#define arp_TypeEther  0x100      /* ARP type of Ethernet address */

/* arp op codes */
#define ARP_REQUEST 0x0100
#define ARP_REPLY   0x0200

/*
 * Arp header
 */
typedef struct {
    UINT16            hwType;
    UINT16            protType;
    UINT16            hwProtAddrLen;  // hw and prot addr len
    UINT16            opcode;
    eth_address     srcEthAddr;
    UINT32        srcIPAddr;
    eth_address     dstEthAddr;
    UINT32        dstIPAddr;
} arp_Header;

#if !defined(ETH_MSS)   // S. Lawson - allow setting in makefile
#define ETH_MSS 1480  // MSS for Ethernet //Ajay made 1500 for UDP testing
#endif                  // S. Lawson

#ifdef _IP_REASSEMBLY
extern UINT8 *fragment( in_Header * ip );
extern VOID timeout_frags( VOID );
#endif

#endif


/* VG $ CR#5080 $ 02\08\2005 $ API for getting it IP of an interface */
extern UINT32 GetInterfaceIP( INT16 IfNo );

/*
 * Ethernet interface -- pcsed.c
 */
         // S. Lawson
#ifdef MULTIHOMING
UINT8 *_eth_formatpacket( eth_address *eth_dest, UINT16 eth_type, UINT8 iface );
#ifdef LEGACY_ZTP_TIMING
   INT _eth_send( UINT16 len ,UINT8 iface );
#else
   INT _eth_send( HANDLE pPkt, UINT16 len ,UINT8 iface );
#endif
UINT8 *_eth_arrived( UINT16 *type_ptr,UINT8 *iface);
#else
INT  _eth_init( VOID );
UINT8 *_eth_formatpacket( eth_address *eth_dest, UINT16 eth_type);
INT   _eth_send( UINT16 len );
UINT8 *_eth_arrived( UINT16 *type_ptr);
#endif
VOID  _eth_free( VOID *buf );
VOID  _eth_release( VOID );
#if defined(__WATTCP_KERNEL__)
extern VOID *_eth_hardware( UINT8 *p );
#endif


/*
 * ZTP Critical sections (independent of RZK)
 */
extern void ZTP_CriticalBegin( void );
extern void ZTP_CriticalEnd( void );

#define ZTP_CriticalBegin()   \
{                             \
   asm( "LD    a, i" );       \
   asm( "PUSH  af" );         \
   asm( "DI" );               \
}


#define ZTP_CriticalEnd()     \
{                             \
   asm( "POP   hl" );         \
   asm( "BIT   #2, l" );      \
   asm( "JR    Z, $F" );      \
   asm( "EI" );               \
   asm( "$$:" );              \
}


/*
 * timers -- pctcp.c
 */
VOID ip_timer_init( sock_type *s, INT delayseconds );
INT ip_timer_expired( sock_type *s );
UINT32 MsecClock( VOID );


/*
 * sock_init()  -- initialize wattcp libraries -- sock_ini.c
 */
VOID sock_init(VOID);
INT sock_init_noexit(VOID);     // S. Lawson
VOID sock_exit( VOID );   /* normally called via atexit() in sock_init() */


        /*
         * tcp_init/tcp_shutdown -- pctcp.c
         *      - init/kill all tcp and lower services
         *      - only call if you do not use sock_init
         * (NOT RECOMMENDED)
         */
VOID tcp_shutdown(VOID);
INT tcp_init_noexit(VOID);      // S. Lawson
VOID tcp_init(VOID);
VOID tcp_set_ports(UINT16 tcp_base, UINT16 udp_base);   // S. Lawson
VOID tcp_get_ports(UINT16 *tcp_base, UINT16 *udp_base);   // S. Lawson

/*
 * things you probably won't need to know about
 */
   /*
    * sock_debugdump -- sock_dbu.c
    *   - dump some socket control block parameters
    * used for testing the kernal, not recommended
    */
VOID sock_debugdump( sock_type *s );
        /*
         * tcp_config - read a configuration file
         *            - if special path desired, call after sock_init()
         *            - null reads path from executable
         * see sock_init();
         */
INT tcp_config( INT8 *path );
   /* S. Lawson
    * tcp_config_file - sets the configuration filename
    *                 - null silently skips config file processing
    */
VOID tcp_config_file( const INT8 *fname );
   /*
         * tcp_tick - called periodically by user application in sock_wait_...
         *          - returns 1 when our socket closes
         */
//INT tcp_tick( sock_type *s ); 
        /*
         * Retransmitter - called periodically to perform tcp retransmissions
         *          - normally called from tcp_tick, you have to be pretty
         *            low down to use this one
         */
VOID tcp_Retransmitter(VOID);
        /*
         * tcp_set_debug_state - set 1 or reset 0 - depends on what I have done
         */
VOID tcp_set_debug_state( INT x );

/*
 * check for bugs -- pctcp.c
 */
INT tcp_checkfor( sock_type *t );

/*
 * TCP or UDP specific stuff, must be used for open's and listens, but
 * sock stuff is used for everything else -- pctcp.c
 */
INT tcp_open( tcp_Socket *s, UINT16 lport, UINT32 ina, UINT16 port, dataHandler_t datahandler );
INT udp_open( udp_Socket *s, UINT16 lport, UINT32 ina, UINT16 port, dataHandler_t datahandler );
INT tcp_listen( tcp_Socket *s, UINT16 lport, UINT32 ina, UINT16 port, dataHandler_t datahandler, UINT16 timeout );
INT tcp_established( tcp_Socket *s );

/*
 * Clean up a string -- pctcp.c
 */
INT8 *rip( INT8 *s);

/*
 * Name service / name lookup routines -- udp_dom.c
 */
UINT32 resolve( INT8 *name );
UINT32 resolve_fn( INT8 *name, sockfunct_t fn );     // S. Lawson
INT reverse_addr_lookup( UINT32 ipaddr, INT8 *name );
INT reverse_addr_lookup_fn( UINT32 ipaddr, INT8 *name, sockfunct_t fn );  // S. Lawson

/*
 * less general functions
 */
UINT32 intel( UINT32 x );
UINT16 intel16( UINT16 x );
UINT32 MsecClock( VOID );

#if 0
 /*AJ - CR#5329-08-FEB-05 - code cleanup*/
/*
 * Ctrl-break handling -- pc_cbrk.c
 */

VOID tcp_cbrk( INT mode );

VOID outs( CHAR * string );
#endif 

#if defined(__WATTCP_KERNEL__)
/* icmp handler -- pcicmp.c */
#ifdef MULTIHOMING
INT icmp_handler( in_Header *ip ,UINT8 iface);
VOID icmp_Unreach( in_Header *ip,UINT8 iface,UINT8 code);   // S. Lawson /** MK $ CR 6590 $ 30-NOV-2005 */
#else
INT icmp_handler( in_Header *ip );
VOID icmp_Unreach( in_Header *ip, UINT8 code);   // S. Lawson /** MK $ CR 6590 $ 30-NOV-2005 */
#endif
#ifndef LEGACY_ZTP_TIMING
   extern INT _ping( UINT32 Host, UINT16 SeqNum );
   extern INT _send_ping( UINT32 host, UINT16 SeqNum, UINT8 ttl, UINT8 tos, UINT32 *theid );
   extern UINT _chk_ping( UINT32 host, UINT16 *pRxSeq );
#else
   extern INT _ping( UINT32 host , UINT32 countnum );
   extern INT _send_ping( UINT32 host, UINT countnum, UINT8 ttl, UINT8 tos, UINT32 *theid );
   extern UINT32 _chk_ping( UINT32 host , UINT32 *ptr);
#endif
#define _ping( h, c ) _send_ping( h, c, 250, 0, NULL )
#endif

//IGMP_REQ
INT16 igmp_handler(in_Header *,UINT8);
INT8 wlsIGMPAddIP(UINT8,UINT32, UINT8);
INT8 wlsIGMPSrchGrp (UINT32,UINT8);
struct netif * ztpMultiHomes(UINT32 ina,tcp_Socket *s);

#define IGMP_PROTO 0x02
#define CLSDIP 0xE0000000
#define HOST_CLSDIP 0x000000E0
#define IGMP_ENTRY_NOT_FOUND -1
#define IGMP_IPADDR_ERR -2 
#define IGMP_LST_FULL -3
#define IGMP_ADD_IP 1
#define IGMP_DELETE_IP 2
//IGMP 

/*
 * ARP -- pcarp.c
 */

#ifdef MULTIHOMING
#if defined(__WATTCP_KERNEL__)
VOID _arp_add_gateway( INT8 *data, UINT32 ip,UINT8 iface );
INT _arp_handler( arp_Header *in,UINT8 iface );
#endif
VOID _arp_register( UINT32 use, UINT32 instead_of ,UINT8 iface);
VOID _arp_tick( UINT32 ip);      /* kernel only? */
INT _arp_resolve( UINT32 ina, eth_address *ethap, INT nowait ,UINT8);
#else
#if defined(__WATTCP_KERNEL__)
VOID _arp_add_gateway( INT8 *data, UINT32 ip );
INT _arp_handler( arp_Header *in );
#endif
VOID _arp_register( UINT32 use, UINT32 instead_of );
VOID _arp_tick( UINT32 ip );      /* kernel only? */
INT _arp_resolve( UINT32 ina, eth_address *ethap, INT nowait );
#endif


/*
 * Packet -- pcpkt.c
 *
 * These probably shouldn't be visible to user app code.
 */
eth_address *_pkt_eth_init( VOID );
INT pkt_send( INT8 *buffer, INT length );
VOID pkt_buf_wipe( VOID );
VOID pkt_buf_release( INT8 *ptr );
VOID * pkt_received( VOID );
VOID pkt_release( VOID );

#if defined(__WATTCP_KERNEL__)
extern UINT16 debug_on;
#endif


/*
 * pcbsd.c
 */
INT _chk_socket( sock_type *s );
INT8 *inet_ntoa( INT8 *s, UINT32 x );
UINT32 inet_addr( INT8 *s );
INT8 *sockerr( sock_type *s );
//Ajay - for FTP CHAR *sockstate( sock_type *s );
UINT32 gethostid( VOID );
UINT32 sethostid( UINT32 ip );
UINT16 ntohs( UINT16 a );
UINT16 htons( UINT16 a );
UINT32 ntohl( UINT32 x );
UINT32 htonl( UINT32 x );


#if defined(__WATTCP_KERNEL__)
VOID *_tcp_lookup( UINT32 hisip, UINT16 hisport, UINT16 myport );
#ifdef MULTIHOMING
extern VOID _tcp_cancel( in_Header *ip, INT code, INT8 *msg, UINT32 dummyip ,UINT8 iface);
#else
extern VOID _tcp_cancel( in_Header *ip, INT code, INT8 *msg, UINT32 dummyip );
#endif
/** MK $ CR 5083 $ 15-Feb-05 $ ICMP Redirect Routing support */
//VOID _udp_cancel( in_Header *ip, INT code, UINT32 dummyip );

INT _dobootp(VOID);
#endif


/*
 * General socket I/O -- pctcp.c
 */
UINT16 sock_mode( sock_type *, UINT16);        /* see TCP_MODE_... */
VOID sock_abort( sock_type *);
VOID tcp_sendsoon( tcp_Socket *s );
INT sock_fastwrite( sock_type *, UINT8 *, INT );
INT sock_write( sock_type *, UINT8 *, INT );
INT sock_read( sock_type *, UINT8 *, INT );
INT sock_fastread( sock_type *, UINT8 *, INT );
INT sock_gets( sock_type *, UINT8 *, INT );
VOID sock_close( sock_type *s );

UINT8 sock_putc( sock_type  *s, UINT8 c );
INT sock_getc( sock_type  *s );
INT sock_puts( sock_type  *s, UINT8 *dp );

INT sock_yield( tcp_Socket *s, VOID (*fn)( VOID ) );

extern void _tcp_close( tcp_Socket *s ) ;
#ifdef MULTIHOMING
extern void tcp_rst(    in_Header *his_ip, 
               tcp_Header *oldtcpp , 
               UINT8 iface
             ) ;
#else
extern void tcp_rst(    in_Header *his_ip,
               tcp_Header *oldtcpp
               ) ;


#endif
/*
extern INT udp_read( udp_Socket *s, 
               UINT8 *datap, 
               INT maxlen,
               sockaddr_in *pStFrom
               ) ;
*/

/*
 *   s is the pointer to a udp or tcp socket
 */


/*
 * Socket text output/input routines -- sock_prn.c
 */
INT sock_printf( sock_type  *s, INT8 *format, ... );
INT sock_scanf( sock_type  *s, INT8 *format, ... );


/*
 * Misc. socket I/O -- pctcp.c
 */
INT sock_setbuf( sock_type *s, UINT8 *dp, INT len );
INT sock_enqueue( sock_type  *s, UINT8 *dp, INT len );
VOID sock_noflush( sock_type *s );
VOID sock_flush( sock_type  *s );
VOID sock_flushnext( sock_type  *s);
INT sock_dataready( sock_type  *s );
INT sock_established( sock_type *s );
VOID sock_sturdy( sock_type *s, INT level );


/*
 * Debug output -- pcdbug.c
 */
VOID db_write( INT8 *msg );
VOID db_open( VOID );
VOID db_close( VOID );
VOID dbug_printf( INT8 *, ... );
VOID dbug_init( VOID );



/*
 * Socket Select -- select.c
 */
INT sock_sselect( sock_type *s, INT waitstate );



/*
 * recv routines -- pcrecv.c
 */
INT sock_recv_init( sock_type *s, VOID *space, UINT16 len );
INT sock_recv_from( sock_type *s, INT32 *hisip, UINT16 *hisport, INT8 *buffer, INT len, UINT16 flags );
INT sock_recv( sock_type *s, INT8 *buffer, INT len, UINT16 flags );


/*
 * bsd-similar stuff -- pcbuf.c
 */
INT sock_rbsize( sock_type *s );
INT sock_rbused( sock_type *s );
INT sock_rbleft( sock_type *s );
INT sock_tbsize( sock_type *s );
INT sock_tbused( sock_type *s );
INT sock_tbleft( sock_type *s );
INT sock_preread( sock_type *s, UINT8 *dp, INT len );


/*
 * Name conversion stuff -- udp_nds.c
 */
UINT32 aton( INT8 *text );
UINT16 isaddr( INT8 *text );
/*
 * Configuration -- pcconfig.c
 */
INT8 *_inet_atoeth( INT8 *src, UINT8 *eth );
INT tcp_config( INT8 *path );

        /*
         * name domain constants
         */

extern INT8 *def_domain;
extern UINT32 def_nameservers[ MAX_NAMESERVERS ];

extern UINT16 wathndlcbrk;
extern UINT16 watcbroke;

/* user initialization file */
extern VOID (*usr_init)(INT8 *name, INT8 *value);


extern UINT16 _survivebootp;
extern INT _last_cookie;
extern UINT32 _cookie[MAX_COOKIES];

//AJ in udp_dom, extern INT _last_nameserver;
//extern INT8 *_hostname;

/*
 * Elib stuff
 */
INT isstring( INT8 *string, unsigned stringlen );     /* isstring.c */
#define NON_BLOCKING  (0)
#define ACCEPT_BLOCKING (1)
#define READ_BLOCKING (2)
#define WRITE_BLOCKING (4)
#define CONNECT_BLOCKING (8)
#define READ_BLOCKED (16)
#define WRITE_BLOCKED (32)
#define ZTP_ALREADY_BLOCKED (-18)
//Ajay -Added for blocking calls
extern INT8 BlockNow(tcp_Socket * , UINT8 );
extern VOID unBlockNow(tcp_Socket *s, UINT8 err);
#define FRAGHOLDTIME (15)

/* RK $ CR#5092 $ 3rd Feb 2005 $ This API is added newly */
UINT8 ping(UINT32 dst, UINT32 count);
//XTLRM
#define EP_ALEN (6)

extern void largecheck( void *s, 
                   INT size
                );
extern UINT16 findfreeport(    UINT16 oldport, 
                  UINT16 proto_tcp
                   ) ;
extern void _udp_cancel
(
   in_Header          * ip,
   INT                  code,
   UINT32               dummyip
) ;
#define IP_ALEN  (4)
#define IP_MF 0x0020               // more fragments, net UCHAR order
#define MAXINT 20
#define IP_DF       0x0040      /* Don't fragment bit set for FRAG Flags */
#define IP_NF       0x0020      /* Next frag, in intel form */
#define IP_OF       0xFF1F      /* frag off. in intel form */

struct fragmem{
UINT8 used;
UINT8 asmBuf[1];
};

#ifdef SSL_SOCKET
#define SSL_SOCK_TCP   (0x8000)
#endif

#endif /* ndef _wattcp_wattcp_h */
