/*
 * File       :     bootinfo.h
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

#ifndef _BOOTINFO_H
#define _BOOTINFO_H
//#define BOOTNAMELENGTH  32
struct BootInfo {
        UINT32    myip;
        UINT32    defaultroute;
        UINT32    timeserver;
		UINT32	 NetworkTimeServer;
        UINT32    rfserver;
        UINT32    tftpserver;
        UINT32    dnsserver;
        INT8    bootfile[64];
        UINT32  subnetmask;
};
#define IPADDR1  	Bootrecord.myip
#define DEFRTR   	Bootrecord.defaultroute
#define TSERVER  	Bootrecord.timeserver
#define RSERVER  	Bootrecord.rfserver
#define NSERVER  	Bootrecord.dnsserver
/* Default values for network ports */
#define TIME_PORT       37
#define NAME_PORT       53
#define RFS_PORT        2001
extern struct BootInfo Bootrecord;
#endif

