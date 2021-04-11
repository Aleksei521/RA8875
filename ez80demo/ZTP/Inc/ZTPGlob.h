/*
 * File       : ZTPGlob.h
 *
 * Description: This File contians global variables of TCP/IP stack
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


#ifndef _ZTPGLOB_H_
#define _ZTPGLOB_H_

/* RK : 01/25/05 : _ext_socket is defined in Extsock.h */
#include "ZTPTcp.h"
#include "Extsock.h"

/* RK : 01/25/05 : PKT_BUFF_t is defined in EtherMgr.h */
#include "EtherMgr.h"

#include "ztpdefines.h"

//extern struct fragmem ;

// modified extern definitions
extern UINT8      Reboot_if_diff_IP                        ;
extern UINT8      b_use_dhcp                               ;	
extern INT8 *    eZ80_name                                ;
extern INT8 *    httppath                                 ;
extern UINT8      ztpEnPPPAtBoot                           ;
extern INT8 *    g_ShellPrompt                            ;
extern INT8 *    g_TelnetPrompt                           ;
extern UINT8      ztpDhcpRtrs                              ;
extern UINT8      ztpForwardIP                             ;
extern UINT32      ztpTftpTimeout                           ;
extern UINT8      ztpRarpMaxResend                         ;
extern UINT32      ztpDnsTimeout                            ;
extern UINT8      g_enable_boot_msgs                       ;


/** MK $ CR#5082 $ 04FEB05*/
/** Default user Name and password to login to FTP / Telnet / Shell***/
extern	INT8		*defaultUsrName;
extern 	INT8		*password ;
extern UINT8		g_maxNumUsers ;

extern UINT16      ztpMaxTcpCon                             ;
extern UINT16      ztpTcpRxBufsize                          ;
extern UINT16      ztpTcpTxBufsize                          ;
extern UINT16      ztpMaxUdpCon                             ;
extern UINT8      ztpMaxEthIf                              ;
extern UINT8      ztpMaxSrlIf                              ;
extern UINT8      ztpMaxIf                                 ;
extern UINT8      ztpPrimaryIf                             ;
extern UINT8      ztpPrimaryIfTyp                          ;
extern UINT16      ztpSockMemSize                           ;
extern UINT8      ztpIgmpMaxGrp                            ;
extern UINT8      ztpReasMaxBufs				;
extern UINT16      ztpReasMaxBufSize	 			;
extern UINT8 		ztpTcpMaxRtrs;
extern UINT16 ztpIpReasMaxBufs  ;
extern UINT16 ztpIpReasBufSize  ;

/** MK $ 29-MAY-2006 $ New Feature SNTP Client **/
extern UINT32 		g_ztpsntpTimeout ;

extern INT8		g_ShellLoginReqd ;

extern UINT16 maxNumIGMPGrp ;

//extern UINT8      socketMem[1][1] ;
extern UINT8      socketMem[][SOCKETMEMSIZE] ;
extern INT16                                              Status[];
extern UINT16 	maxNumIntf ;
extern struct fragmem *asmMem;
extern UINT16 fragBufSize ;

// MK $ 04-04-05 $ IAR PORTING 
#include "ExtSock.h"
extern _ext_socket *Socket[]; 
extern struct netif   nif[];

extern INT16     IP_MQ_SIZE                               ;
extern UINT      MAX_IP_RX_BUFF                           ;
extern PKT_BUFF_t                                         ipRxBuff[] ;
extern UINT8      IP_Message_Buff[] ;
extern struct If                                          ifTbl[];

extern struct hg  hgtable[];


#define MAXSOCKETS (ztpMaxTcpCon+ztpMaxUdpCon)

#endif //_ZTPGLOB_H_
