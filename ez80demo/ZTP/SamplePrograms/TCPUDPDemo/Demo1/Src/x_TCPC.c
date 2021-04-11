/*
 * File       :     x_TCPC.c
 *
 * Description:  This file contains the implementation of TCP client application.
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
#define TCPCLI "TcpClient"
/* Extern function declaration */
extern RZK_DEVICE_CB_t *CONSOLE ;

INT16 x_TCLIENT(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, INT nargs, INT8 *args[])
{


	INT8 *Buf,Buf1[10];
	INT16 port;
	RZK_THREADHANDLE_t th1;
	xc_fprintf(CONSOLE,"\n####### TCP CLIENT DEMO  #######");
	Buf = (INT8 *)malloc( (COUNT_t)20);
	if (Buf < 0 )
	{
		return 0;
	}
	xc_fprintf(CONSOLE,"\nEnter the server's IP address:");
	GetString(stdin, Buf, 20);
	xc_fprintf(CONSOLE,"\nEnter the server's port number:");
	GetString(stdin, Buf1, sizeof(Buf1));
	port = atoi(Buf1);
	th1 = RZKCreateThreadEnhanced((UINT8 *)TCPCLI,
										(void*)Tcpclient,		
										NULL,
										2020,
										18,
										0,
										RZK_THREAD_AUTOSTART|RZK_THREAD_PREEMPTION,
										2,
										Buf,
										port);

    if(th1 == NULL)
	{
	 	xc_fprintf(CONSOLE,"\nThread could not be created");
		return 0;
	}
	
	return 0;
}

























