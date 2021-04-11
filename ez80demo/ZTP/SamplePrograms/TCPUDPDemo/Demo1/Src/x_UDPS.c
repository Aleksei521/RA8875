/*
 * File       :     x_UDPS.c
 *
 * Description:  This file contains the implementation of UDP server application.
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

/*Standard includes*/
#include <stdio.h>

/* User includes */
#include "ztptcp.h"
#include "sockaddr.h"
#include "socket.h"
#include "ZTypes.h"
#include "ZSysgen.h"
#include "ZThread.h"
#include "SocketDemo.h"
#include "ZRegion.h"
#include "shell.h"
#define UDPSERV "UdpSer"

/* Extern function declaration */

extern RZK_DEVICE_CB_t *CONSOLE ;
extern INT16 GetString(RZK_DEVICE_CB_t * dev,INT8 	*buf, UINT16	bufsz);


INT16 x_USERVER(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, INT nargs, INT8 *args[])
{
	
	
	
	INT8 Buf[10];
	INT16 port; 
	RZK_THREADHANDLE_t th3;
	xc_fprintf(CONSOLE,"\n####### 	UDP SERVER DEMO  #######");
	xc_fprintf(CONSOLE,"\nEnter the server's port number to listen:");
	GetString(stdin, Buf, sizeof(Buf));
	port = atoi(Buf);
	th3 = RZKCreateThreadEnhanced((UINT8 *)UDPSERV,
										(void*)Udpserver,		
										NULL,
										2020,
										17,
										0,
										RZK_THREAD_AUTOSTART|RZK_THREAD_PREEMPTION,
										1,
										port);

    if(th3 == NULL)
	{
	 	xc_fprintf(CONSOLE,"\nThread could not be created");
		return 0;
	}
	return 0;

}



