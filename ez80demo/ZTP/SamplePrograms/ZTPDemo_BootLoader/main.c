/*
 * File       :     main.c
 *
 * Description: This file contains the Entry point function.
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

/** Standard Includes **/
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

/** RZK Includes */
#include "eZ80DevKit.h"
#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZThread.h"

/** ZTP Includes */
#include "ZTimer.h"
#include "shell.h"
#include "Xc_lib.h"
#include "http.h"
#include "date.h"
#include "igmp.h"
#include "snmpv1.h"
#include "telnet_api.h"
#include "Telnet_Shell.h"
#include "rtc.h"


/** File System Includes*/
#include "FS_Shell.h"
#include "zfstypes.h"
#include "zfsapi.h"
#include "cfileapi.h"

/** EXtern Functions */
extern void DisplayTime();
extern void ftpdinit(void);
extern void nifDisplay(RZK_DEVICE_CB_t *dev);
extern void networkInit(void);
/** Extern Variables */
extern RZK_DEVICE_CB_t * CONSOLE;
extern Webpage website[];

/* Forward referencing the functions which are defined after ZTPAppEntry*/
void Initialize_FileSystem() ;
INT16 OpenSerialPort( RZK_DEVICE_CB_t 	**TTYDevID );
void snTrapNotifyCallback( INT8 trapType );
extern UINT  RZK_DEVTICKS_PERSYSTICK;


extern void ztpAddPPPShellCmds( void );
extern INT16 ztpSnmpV2Init(ZTPSNMP_TRAP_NOTIFY snTrapNotifyFunc);

/*****************************************************************************
 * Function Name			:	ZTPAppEntry()
 * 
 * Return Values			:	Success -- OK
 *								Failure   -- SYSERR
 *
 * Formal Parameters		:	None
 *				
 * Global variables used   	: 	CONSOLE
 *								website
 *
 * Description				:	This is the entry point function for the 
 *								application.
 *								All the protocol Servers like FTP, Telnet 
 *								etc are initialized in this function.
 *								If any protocol is not required comment the 
 *								corresponding function.For e.g if Telnet Server
 *								is not required, telnetdinit()can be commented
 *								to get the reduced footprint.
 *								All the protocol clients like FTP, Telnet can  
 *								be executed in the form of shell commands apart
 *								from executing programmatically with the  
 *								appropriate API.These commands are present in  
 *								shell_conf.c.If any of the commands are not 
 *								required please comment the corresponding 
 *								command in shell_conf.c
 ****************************************************************************/
INT16 ZTPAppEntry(void)
{
	RZK_DEVICE_CB_t 	*TTYDevID;
	

	networkInit();
	/** This function displays the IP Address and the related 
	      information of all the interfaces */
	nifDisplay(CONSOLE);

	// File System initialization
	 Initialize_FileSystem();	
 
	 /** Protocol Initializations */

	/*This is for initializing the HTTP server without authentication. Use the 
	  Acclaim_Website.lib Website Library for this(Modify at Project->Settings->Linker->
	  General->Object/library modules )*/
	 http_init(http_defmethods,httpdefheaders,website,80);

	/*This is for initializing the HTTP server with MD5 Digest authentication 
	  Only one HTTP server should be initialised. Use the AuthAcclaim_Website.lib Website Library 
	  for this(Modify at Project->Settings->Linker->General->Object/library modules )*/
	//httpDigestAuth_init(httpDigestAuth_defmethods,httpdefheaders,website,80);

	/*This is for initializing the HTTP server with Basic authentication 
	  Only one HTTP server should be initialised.Use the Acclaim_Website.lib Website Library 
	  for this(Modify at Project->Settings->Linker->General->Object/library modules )*/
	//httpBasicAuth_init(http_defmethods,httpdefheaders,website,80);

	/** FTP Server Initialization */
	ftpdinit();

	/** Time Request */
	time_rqest();

	/** Telnet Server Initialization **/
	telnet_init();

	/** IGMP Initialization */
	hginit(0);

	/** SNMP V1 Initialization, if V1 has to be used, 
		please comment ztpSnmpV2Init() */
	ztpSnmpV1Init(snTrapNotifyCallback);

	/** SNMP V2 Initialization 
	If V2 alone has to be used, please comment ztpSnmpV1Init() **/
	//ztpSnmpV2Init(snTrapNotifyCallback);
	
	/*
	 * To start up a shell on the serial port, call shell_init(), and pass it the
	 * device to use.
	 */
	if( OpenSerialPort(&TTYDevID) == SYSERR )
		return SYSERR;

	/** Displays default time */
	DisplayTime();

	/** Initialize Shell */
	shell_init(TTYDevID);
 	ztpAddPPPShellCmds();
	return(OK);
}
/*****************************************************************************
 * Function Name			:	snTrapNotifyCallback()
 * 
 * Return Values			:	None
 *
 * Formal Parameters		:	trapType
 *				
 * Global variables used    :   None
 *							
 *
 * Description				:	This call back function gets called by SNMP
 *								whenever there is a trap generated.Application
 *								has to register this callback during
 *								SNMP Initialization
 ****************************************************************************/
void snTrapNotifyCallback( INT8 trapType )
{
	xc_fprintf(CONSOLE,"\nA Trap is generated\n" );	
}

/*****************************************************************************
 * Function Name			:	OpenSerialPort()
 * 
 * Return Values			:	Success 	-- OK
 *								Failure   	-- SYSERR
 *
 * Formal Parameters		:	TTY Device Id
 *				
 * Global variables used    :   	CONSOLE
 *							
 *
 * Description				:	This function is used to open a 
 *								Serial Port on a particular TTY Device
 ****************************************************************************/
INT16 OpenSerialPort( RZK_DEVICE_CB_t 	**TTYDevID )
{
	struct devCap 		*devSerial;
	devSerial = (struct devCap *) malloc (sizeof (struct devCap));
	devSerial->devHdl = (VOID *)CONSOLE ; 
	devSerial->devType = 0;
	if( (*TTYDevID = RZKDevOpen("TTYM",
								 (RZK_DEV_MODE_t*)devSerial )
								 ) == (RZK_DEVICE_CB_t *)NULL ) 
	{
#ifdef DEBUG
	   	printf("Can't open tty for SERIAL0\n");
#endif
		return SYSERR;
	}
	return OK ;
	
}

/*****************************************************************************
 * Function Name			:	Initialize_FileSystem()
 * 
 * Return Values			:	None
 *
 * Formal Parameters		:	None
 *				
 * Global variables used    :   CONSOLE
 *							
 *
 * Description				:	This function initializes the File Sytem and 
 *								adds the File System commands to the shell
 ****************************************************************************/
void Initialize_FileSystem()
{
	
	ZFS_STATUS_t status ;
	ZFS_STATUS_t vol_cnt ;
	PZFS_VOL_PARAMS_t pvol_params, ptmp_vol_params ;
	xc_fprintf(CONSOLE,"\nInitializing File System...") ;
	vol_cnt = ZFSGetVolumeCount() ;
	if( vol_cnt )
	{
		pvol_params = malloc( sizeof( ZFS_VOL_PARAMS_t ) * vol_cnt ) ;
		if( pvol_params != NULL )
		{
			vol_cnt = ZFSInit( pvol_params ) ;
			if( vol_cnt == ZFSERR_SUCCESS )
				xc_fprintf(CONSOLE,"Done") ;
			else
			{
				UINT8 cnt = 0 ;
				xc_fprintf(CONSOLE,"Failed" ) ;
				// now format all the volumes
				ptmp_vol_params = pvol_params ;
				for( cnt = 0 ; cnt < vol_cnt ; cnt++ )
				{
					xc_fprintf(CONSOLE,"\nFormatting Invalid Volume : %s", &ptmp_vol_params->vol_name[0] ) ;
					status = ZFSFormat( ( INT8*) &ptmp_vol_params->vol_name[0] ) ;
					if( status != ZFSERR_SUCCESS )
						xc_fprintf(CONSOLE,"Failed");
					else
						xc_fprintf(CONSOLE,"Done");
					ptmp_vol_params++;
				}
			}
			free( pvol_params ) ;
		}
		else
			xc_fprintf(CONSOLE,"\nNo memory for vol. params");
	}
	else
		xc_fprintf(CONSOLE,"\nNo volumes");

	/** Add File system commands to the shell */
	Shell_Add_FS_Commands();
}
