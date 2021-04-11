/*
 * File       :     Ppp_conf.c
 *
 * Description: This file contains routine for PPP initialization.
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

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZThread.h"
#include "ZDevice.h"
#include "ZTimer.h"
#include "EtherMgr.h"
#include "Xc_lib.h"
#include "zhdlc.h"
#include "zppp.h"

/* PPPoE configuration settings */
#ifdef PPPoE // For PPPoE

#include "pppoe.h"


/* HDLC configuration settings */

#else	// For PPP HDLC

/*
 * Chat script needed to connect to a Windows
 * system which is acting as the host using DCC.
 */
chatscript_t DccClient[] = {
	{"CLIENTCLIENT",NULL,0},
	{NULL,"CLIENTSERVER",30},
	};

/*
 * Chat script needed to connect to a Windows
 * system which is acting as the guest using DCC.
 */
chatscript_t DccServer[] = {
	{NULL,"CLIENT",0},
	{"CLIENTSERVER",NULL,0},
	};

/*
 * Chat script needed to accept a connection via a modem.
 * If the Data Flow Control is disabled (ie no RTS/CTS with serparams[1] ) then  
 *        the first chat script should be changed to "ATE&F&K0&S1\r"
 */
chatscript_t DialServer[] = {
	{"ATE&F&K3&S1\r","OK",30},
//	{"ATE&F&K0&S1\r","OK",30},
	{NULL, "RING",0},
	{"ATA\r","CONNECT",60},
	};

/*
 * This table contains the chat script needed to connect with a modem.
 * If the Data Flow Control is disabled (ie no RTS/CTS with serparams[1] )   
 * then the first chat script should be changed to "ATE&F&K0&S1\r"
 */
chatscript_t DialClient[] = {
//	{"ATE&F&K3&S1\r","OK",30},
	{"ATE&F&K0&S1\r","OK",30},
	{"ATDT214\r","CONNECT",30},
	};

/*
 * This table contains HDLC configurations.
 */
HdlcConfig_t g_HdlcConf = {	 
	 "SERIAL1",
	 DccServer,
	 2
};
#endif


/*
 * This table contains the PPP configuration settings.
 */
struct PppConf PPP_CONF = {
	"zilogindia",				/* User ID */
	"password",			/* Password */
	ZTP_PPP_PAP,		/* ZTP_PPP_CHAP,ZTP_PPP_CHAP Authentication protocol*/
	1400,				/* MRU */
	3,					/* ConfigTimer - Time intervals b/w conf requests */
	6,					/* MaxConfigRequest - Max No. of Conf requests */
	PPP_CLIENT,			/* PPP mode - PPP_SERVER or PPP_CLIENT */
};

/*
 * Variable to enable or disable PPP Debug Prints.Enable - TRUE ,Disable - False
 */
UINT8	g_EnablePppDebug = TRUE	;

/*
 * When eZ80 is acting as a PPP server, this variable is used to enable or 
 * disable Auto Reinitialize PPP. Enable - TRUE , Disable - False
 */
UINT8	g_PppServerAutoInitialize = TRUE	;

/*
 * Fill this structure if eZ80 is acting as a server and wants the Peer to use 
 * the configurations below.
 * If eZ80 is acting as a client then fill with 0.
 */
struct PppNetworkConf PppNwConf = {
0,//"192.168.2.11",		/* My IP Address */
0,//"192.168.2.12",		/* Peer IP Address */
0,		/* Peer Primanry DNS IP Address */
0,		/* Peer Secondary DNS IP Address */
0,		/* Peer Primanry NBNS IP Address */
0		/* Peer Secondary NBNS IP Address */
};



// *********************************************************
// PLEASE DO NOT CHANGE ANYTHING BELOW IN PPP  CONFIGURATION
// *********************************************************

#ifdef PPPoE // For PPPoE
PPP_LINK_DEVICE_t	PPP_Link_Device = {
	(PPP_LINK_DEV_INIT)PPPoECreateSession,
	(PPP_LINK_DEV_READ)PPPoERecvSessionData,
	(PPP_LINK_DEV_WRITE)PPPoESendSessionData,
	(PPP_LINK_DEV_CLOSE)PPPoETerminateSession
};
#else	// For PPP HDLC
PPP_LINK_DEVICE_t	PPP_Link_Device = {
	(PPP_LINK_DEV_INIT)HdlcInit,
	(PPP_LINK_DEV_READ)HdlcRead,
	(PPP_LINK_DEV_WRITE)HdlcWrite,
	(PPP_LINK_DEV_CLOSE)HdlcClose
};
#endif