/*
 * File       : ftp_client_demo.c
 *
 * Description: Showcases the featurs of FTP Client. 
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



/** STANDARD INCLUDE FILES **/
#include <stdio.h>
#include <string.h>

#include "ztypes.h"
/** RZK Include Files **/
#include "ZSysgen.h"
#include "ZThread.h"

/** COMMON INCLUDE FILES */
#include "shell.h"
#include "tty.h"
#include "xc_lib.h"

/** File System Include Files */
#include "zfsapi.h"
#include "cfileapi.h"


/** FTP Server's TCP Port */
#define FTP_PORT 21


/** Source File and Destination Files */
#define DEMOFILE	"ftpDemo.txt"
#define DEMORXFILE	"ftpDemo_Rx.txt"

/* externs */
extern void *getmem(UINT32 nbytes);


/** FTP APIs **/
extern INT ftp_connect(INT8 * server_name, INT server_port, RZK_DEVICE_CB_t *stdout);
extern INT16 do_a_ftp_command( 	RZK_DEVICE_CB_t * 	device, 
									UINT16	nargs,
									INT8		*args[]
							    );
extern INT do_programatic_login(
									RZK_DEVICE_CB_t * 		stdin, 
									RZK_DEVICE_CB_t * 		stdout, 
									INT8 	*username, 
									INT8 	*passwd
							   ) ;

extern void greeting(RZK_DEVICE_CB_t * stdout);

extern INT16
getStr
(
	RZK_DEVICE_CB_t * dev,
	INT8 	*buf,
	UINT16	bufsz
) ;

/** Extern Variables */
extern RZK_DEVICE_CB_t *	CONSOLE;
extern INT16 open_done ;
//extern INT control_conn ;
extern INT getch(void);
extern RZK_DEVICE_CB_t * TTYDevID;

/** GLobal Data */
INT8 * dircmd[]={"dir"};
INT8 * hashcmd[]={"hash"};
INT8 * getcmd[]={"get", DEMOFILE,DEMORXFILE};
INT8 * putcmd[]={"put", DEMOFILE};
INT8 * byecmd[]={"bye"};

/** File Contents */
const INT8 g_ftp_client_buf [] = \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " ;


/** Function to showcase the features of FTP Client */
void ftp_cli_demo()
{
	FILE    *ftpDemoFileHandle ;
	DDF_STATUS_t iflags ;
	INT8 buffer[50];
	INT8 username[50];
	INT8 password[50];
	INT len ;
	INT sktId ;
	RZK_DEVICE_CB_t * stdin;
	RZK_DEVICE_CB_t * stdout;
	UINT8	bye = 0;
	stdin=stdout=CONSOLE;

	/** Creating a file and writing in to that file */
	ftpDemoFileHandle = fopen( DEMOFILE , "w") ;
	if( ftpDemoFileHandle )
	{
		if( fwrite( g_ftp_client_buf, 1, 512, ftpDemoFileHandle ) <= 0 )
			printf("\nError writing to the sample file");
	}
	fclose( ftpDemoFileHandle );
	printf("\nStarting FTP Clinet programatic invocation demo\n");

	/** Get the input from the User */
	printf("\nEnter the Destination IP Address ");
	getStr(TTYDevID, buffer, 50 );

	printf("\nUser name ");
	
	getStr(TTYDevID, username, 50 );

	printf("\nPassword ");
	if((iflags = RZKDevIOCTL(TTYDevID, TTC_GIF, (INT8 *)0, (INT8 *)0)) == (UINT16)SYSERR)
	{
		RZKDevClose(TTYDevID);
		return;
	}

	if(RZKDevIOCTL(TTYDevID, TTC_SIF, (INT8 *)(iflags | TIF_NOECHO), (INT8 *)0) == SYSERR) 
	{
		RZKDevClose(TTYDevID);
		return;
	}
	getStr(TTYDevID, password, 50 );
	//echo on
	if(RZKDevIOCTL(TTYDevID, TTC_SIF, (INT8 *)(iflags & ~TIF_NOECHO), (INT8 *)0) == (INT16)SYSERR)
	{
		RZKDevClose(TTYDevID);
		return ;
	}

	/** Connect to FTP Server */
	if( ( ftp_connect( buffer ,FTP_PORT , stdin)) >= 0)
	{
		/** Send login name and Password to the Server */
		if(do_programatic_login(stdin, stdin,username, password) != 1)
			return ;
		printf("Connected........\n");

		printf("\n** Press any key to send a Sample File to the Server **\n");
		getch();

		/** Send a Sample File to the Server */
		printf("\nSending a sample file to the FTP Server........\n");
		do_a_ftp_command(stdin, 2, putcmd);

		/** Sending a dir command to check the file */
		printf("\n** Press any key to send a \"dir\" command **\n");
		getch();
		do_a_ftp_command(stdin, 1, dircmd);

		/** Getting back the same file from the Server */
		printf("\n** Press any key to get the file from the FTP Server **\n");
		getch();
		printf("\nGetting a file from the Destination.....\n");
		do_a_ftp_command(stdin, 3, getcmd);

		/** Closing the connection from the Server*/
		printf("\n** Press any key to close the connection **\n");
		getch();
		printf("\nClosing the connection......\n");
		do_a_ftp_command(stdin, 1, byecmd);
	}	
	else
		return;
}

void x_ftpDemo()
{
	  ftp_cli_demo();
}

/** FTP Client Demo */
void add_ftp_cli_demo_command()
{
	struct cmdent	*mycmds;
	mycmds = (struct cmdent *) malloc( sizeof(struct cmdent) );

	/* Set up ftpdemo command */
	mycmds->cmdnam = "ftp_demo";
	mycmds->cbuiltin = TRUE;
	mycmds->cproc = (SHELL_CMD)x_ftpDemo;
	mycmds->cnext=(struct cmdent *)NULL;
	mycmds->helpDesc="Demonstrats features of FTP Client\n";

	/* Add FTP Client demo command */
	shell_add_commands(mycmds, 1);
}

INT16
getStr
(
	RZK_DEVICE_CB_t * dev,
	INT8 	*buf,
	UINT16	bufsz
)
{
	INT16	   len;
	UINT16     idx;

	idx=0;
	memset(buf, 0, bufsz);
	
	while(!xc_index(buf, '\n')) 
	{
		//len = read( dev, buf, bufsz-idx );
		len = RZKDevRead(dev, buf, bufsz-idx);
	   // if( (len = read(dev, &buf[idx], bufsz-idx)) <= SYSERR )
			//return -1;

		idx+=len;
	    if(idx == bufsz)
		return OK;
	}

	/* Remove trailing \n (and possibly \r before it) */
	if(*(xc_index(buf, '\n') - 1) == '\r')
		*(xc_index(buf, '\n') - 1) = '\0';
	else
		*(xc_index(buf, '\n')) = '\0';

	return OK;
}
