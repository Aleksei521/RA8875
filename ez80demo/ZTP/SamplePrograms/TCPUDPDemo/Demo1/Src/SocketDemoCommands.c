/*
 * File       :     SocketDemoCommands.c
 *
 * Description:  This file contains the implementation of adding commands to the shell.
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

#include "shell.h"
#include "x_SocketDemo.h"

void AddSocketDemoCommands( void )
{


	struct cmdent	*mycmds;
	INT i=0;
	mycmds = (struct cmdent *) malloc( sizeof(struct cmdent) * 4);

	mycmds[i].cmdnam = "bsdtcpclient";
	mycmds[i].cbuiltin = TRUE;
	mycmds[i].cproc = (SHELL_CMD)x_TCLIENT;
	mycmds[i].helpDesc = "Tcp block client Test\n";
	mycmds[i++].cnext=(struct cmdent *)NULL;


	mycmds[i].cmdnam = "bsdtcpserver";
	mycmds[i].cbuiltin = TRUE;
	mycmds[i].cproc = (SHELL_CMD)x_TSERVER;
	mycmds[i].helpDesc = "Tcp  block server Test\n";
	mycmds[i++].cnext=(struct cmdent *)NULL;
	

	mycmds[i].cmdnam = "bsdudpclient";
	mycmds[i].cbuiltin = TRUE;
	mycmds[i].cproc = (SHELL_CMD)x_UCLIENT;
	mycmds[i].helpDesc = "UDP client  Test\n";
	mycmds[i++].cnext=(struct cmdent *)NULL;
	
	mycmds[i].cmdnam = "bsdudpserver";
	mycmds[i].cbuiltin = TRUE;
	mycmds[i].cproc = (SHELL_CMD)x_USERVER;
	mycmds[i].helpDesc = "UDP  server Test\n";
	mycmds[i++].cnext=(struct cmdent *)NULL;

	
	shell_add_commands(mycmds, i);


}
