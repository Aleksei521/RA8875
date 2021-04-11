/*
 * File       : ZTPConfig.c
 *
 * Description: This File contains TCP/IP stack configuration definitions
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

// ********************************************************
/** ZTP CORE Includes	**/
#include "netif.h"
#include "ztptcp.h"
#include "ZTPDefines.h"

/**	IGMP Includes **/
#include "igmp.h"


/** IP Message Queue and the Message sizes */
#define IP_MQ_SIZEH			            12
#define IP_MESSAGE_SIZEH	            3


// **************************
// The below macro values can be changed according to the
// system needs. Please refer to the Reference Manual for
// macro descriptions
// **************************

// ZTP related macros
// Number of emac recevie buffers
#define MAX_IP_RX_BUFFH            	8

/** TCP RX/TX Buffer Sizes **/
#define MAX_RX_BUFSIZE              2048
#define MAX_TX_BUFSIZE              2048

/** Max Number of TCP/UDP Connections **/
#define MAX_TCP_CONNECTIONS        	10
#define MAX_UDP_CONNECTIONS        	2

/** Max Number of Interfaces **/
#define MAX_NO_ETH_IF               1
#define MAX_NO_SERIAL_IF 			0

/** Default Interface	**/
#define DEFAULT_IF                  ETH
#define DEFAULT_IF_TYPE            	ETH 

/** Max number of user names allowed to login to shell/telnet/FTP **/
#define MAX_NUM_USERS               3

/** IP Reassembly Buffers and Buffer Sizes **/
#define REAS_MAXBUFS                2
#define REAS_MAXBUF_SIZE            4507
#define REAS_FRGHDR_SIZE			21


/** Max number of TTY devices **/
#define NUMBER_OF_TTY_DEVICES       (1)

/** Max Number of IGMP Groups **/
#define IGMP_MAX_NO_GRP             10

// ******************************************
// PLEASE DO NOT CHANGE BELOW MACRO VALUES.
// ******************************************
#define MAX_TCP						MAX_TCP_CONNECTIONS
#define MAX_UDP               		MAX_UDP_CONNECTIONS
#define MAXSOCKS             		(MAX_TCP+MAX_UDP)
#define MAX_NO_IF            		(MAX_NO_ETH_IF+MAX_NO_SERIAL_IF)
// ****************************************************
// END OF ZTP CORE CONFIGURATION

/** MK $ 04-04-05 $ removed const as the below variable is not defined as const */
extern RZK_DEVICE_CB_t		usrDevBlk[];
extern RZK_DEVICE_CB_t 		pppdev;

// *******************************************************************
// CHANGE BELOW VALUES IF YOU KNOW WHAT YOU ARE DOING
// BELOW VALUES PRESENT IN STRUCTURE/MACRO/VARIABLES CAN BE
// CHANGED ACCORDING TO THE SYSTEM NEEDS. PLEASE REFER TO THE
// REFERENCE MANUAL FOR STRUCTURE/MACRO/VARIABLE DEFINITIONS
// *******************************************************************

struct commonServers csTbl={
 	"172.16.6.38",		/* Default Timer Server */
	"",					/* Default Network Timer Server( NTP ) */
 	"",					/* Default rfs server */
 	"",					/* Default File Server - Not currently Used */
 	"172.16.6.194"		/* Default Name Server */
};	

struct If ifTbl[MAX_NO_IF]= {

/*interface 0 -> Ethernet Configuration*/
{
	&usrDevBlk[0],		/* Control block for this device*/
	ETH,				/* interface type */
	ETH_MTU,			/* MTU */
	ETH_100,			/* Speed */ /* may have values such as ETH_100, ETH_10, ETH_AUTOSENSE */
	"172.16.6.186",		/* Default IP address */
	"172.16.6.1",		/* Default Gateway */
	0xffffff00UL		/* Default Subnet Mask */
}
};



UINT8      	Reboot_if_diff_IP		= 	FALSE ;
UINT8      	b_use_dhcp		       	= 	TRUE ;	
INT8*		eZ80_name 		       	= 	"eZ80Acclaim" ; // maximum 16 bytes
INT8*		httppath				=	"/" ;
UINT8      	g_DefaultSearchFS		= 	FALSE; //FALSE = Http searches static array first & then file system
UINT8      	ztpEnPPPAtBoot          = 	FALSE;
INT8*  		g_ShellPrompt          	= 	"ZTP "; 		//Max of 16 characters
INT8*  		g_TelnetPrompt         	= 	"eZ80 Telnet "; //Max of 16 characters
UINT8      	ztpDhcpRtrs         	= 	3;
UINT8	 	ztpTcpMaxRtrs			= 	8;
UINT8      	ztpForwardIP           	= 	1; // 1 to enable IP forwarding, 0 to disable IP forwarding
UINT32      ztpTftpTimeout         	= 	5;		
UINT8      	ztpRarpMaxResend     	= 	3;
UINT32      ztpDnsTimeout  			= 	5;
INT16 		g_sock_inactive			= 	0; //Timeout a tcp socket if not active for more than specified time, set 0 to disable 
INT16 		g_tcpConnect_timeout	=	75; // Timeout in seconds

/** MK $ 29-MAY-2006 $ New Feature SNTP Client **/
UINT32 		g_ztpsntpTimeout 		=  10;  // Holds the SNTP time out value

// DEV_SERIAL0 for serial 0, DEV_SERIAL1 for serial 1 port
UINT8      	g_console_dev_to_use	= 	DEV_SERIAL0 ;

/* MK $ CR#5082 $ 04FEB05  */
/** Default user Name and password to login to FTP / Telnet / Shell***/
INT8		*defaultUsrName			= 	"anonymous" ;  	//Max of 25 characters
INT8		*password				= 	"anonymous" ;	//Max of 25 characters

/* RK $ CR #6274 : 9 September 2005 $ */
INT8		g_ShellLoginReqd 		=	TRUE;


// **************************
// PLEASE DO NOT CHANGE ANYTHING BELOW IN ZTP CORE CONFIGURATION
// **************************
UINT8       ztpMaxEthIf  	       	= 	MAX_NO_ETH_IF; 
UINT8       ztpMaxIf  				= 	MAX_NO_ETH_IF+MAX_NO_SERIAL_IF;
UINT8       ztpPrimaryIf            = 	DEFAULT_IF;
UINT8		ztpIgmpMaxGrp           = 	IGMP_MAX_NO_GRP;
UINT8      	ztpReasMaxBufs			=	REAS_MAXBUFS;
UINT16 		ztpIpReasMaxBufs 		= 	REAS_MAXBUFS ;
UINT16 		ztpIpReasBufSize  		= 	REAS_MAXBUF_SIZE ;
UINT16 		maxNumIGMPGrp 			= 	IGMP_MAX_NO_GRP ;
UINT16 		maxNumIntf 				= 	MAX_NO_IF ;
UINT16		ztpMaxTcpCon            = 	MAX_TCP_CONNECTIONS;
UINT16		ztpTcpRxBufsize         = 	MAX_RX_BUFSIZE;
UINT16      ztpTcpTxBufsize         = 	MAX_TX_BUFSIZE;
UINT16      ztpMaxUdpCon            = 	MAX_UDP_CONNECTIONS;
UINT16      ztpSockMemSize			=	SOCKETMEMSIZE;
UINT16      ztpReasMaxBufSize	 	=	REAS_MAXBUF_SIZE;

/** Socket Memory **/
UINT8      	socketMem[MAXSOCKS][SOCKETMEMSIZE];
INT16 		Status[MAXSOCKS];
void*		Socket[MAXSOCKS];
struct netif nif[MAX_NO_IF];

/* MK $ CR#5131 $ 28JAN05  */ 
UINT8		g_maxNumUsers 			=  MAX_NUM_USERS ;

/* AJ $ CR#5084 $ 14FEB05  */ 
#ifdef _IP_REASSEMBLY
UINT8 		fraglist [REAS_MAXBUFS][REAS_FRGHDR_SIZE];
UINT16 		fragBufSize 			= 	sizeof(struct fragmem)+REAS_MAXBUF_SIZE-1 ;
struct 		fragmem *asmMem;
#endif

/** **/
#ifndef RZKFS
INT8       g_szmaxUsrs[MAX_NUM_USERS][FTP_USR_DATA_SIZE]; 
#endif

/** IP Message queue **/
UINT      	MAX_IP_RX_BUFF         	= 	MAX_IP_RX_BUFFH ;
UINT8      	IP_Message_Buff[(IP_MESSAGE_SIZEH*IP_MQ_SIZEH)+(IP_MQ_SIZEH*sizeof(RZK_PTR_t))] ;
INT16     	IP_MQ_SIZE   		    = 	IP_MQ_SIZEH ;
PKT_BUFF_t	ipRxBuff[MAX_IP_RX_BUFFH] ;


/** IGMP Configuration**/
UINT32		IPAddrArray[IGMP_MAX_NO_GRP];
struct 		hg 	hgtable[IGMP_MAX_NO_GRP];

/** Protocol Thread Priorities - 
Please do not change until you are sure about what you are doing. **/
UINT8 		g_TELNETDPri			=	10;
UINT8 		g_TELNETDShellPri		=	15;
UINT8 		g_FTPDPri				=	10;
UINT8 		g_HTTPDPri				=	10;
UINT8 		g_PPPDPri				=	7;
UINT8 		g_SYSIPDPri				=	28;
UINT8 		g_BOOTPDPri				=	8;
UINT8 		g_BOOTPDTimPri			=	10;
UINT8 		g_SHELLDPri				=	15;
UINT8 		g_SNMPDPri				=	20; //RK : 31st Jan 2006 : Changed the priority from 25 to 20 during IAR testing
UINT8 		g_AppEntryTaskPrio		=	16;

/*
 * HTTP Parameters
 */
#include "HTTP.h"
UINT16                  HttpdStackSize   = HTTPSTK;
UINT16                  HttpReqStackSize = HTTPRQSTK;
UINT8                   PersistentHttp   = TRUE;
