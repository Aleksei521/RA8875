/************************************************************************************************
** File:        main.c
** Description: ZTP application entry point of the ZGATE Demo.
**
** Copyright 2012 Zilog Inc. ALL RIGHTS RESERVED.
*
*************************************************************************************************
* The source code in this file was written by an authorized Zilog employee or a licensed 
* consultant. The source code has been verified to the fullest extent possible. 
*
* Permission to use this code is granted on a royalty-free basis. However, users are cautioned to
* authenticate the code contained herein. 
* 
* ZILOG DOES NOT GUARANTEE THE VERACITY OF THIS SOFTWARE; ANY SOFTWARE CONTAINED HEREIN IS
* PROVIDED "AS IS." NO WARRANTIES ARE GIVEN, WHETHER EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
* IMPLIED WARRANTIES OF FITNESS FOR PARTICULAR PURPOSE OR MERCHANTABILITY. IN NO EVENT WILL ZILOG
* BE LIABLE FOR ANY SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES OR ANY LIABILITY IN TORT,
* NEGLIGENCE, OR OTHER LIABILITY INCURRED AS A RESULT OF THE USE OF THE SOFTWARE, EVEN IF ZILOG 
* HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. ZILOG ALSO DOES NOT WARRANT THAT THE USE 
* OF THE SOFTWARE, OR OF ANY INFORMATION CONTAINED THEREIN WILL NOT INFRINGE ANY PATENT, 
* COPYRIGHT, OR TRADEMARK OF ANY THIRD PERSON OR ENTITY.

* THE SOFTWARE IS NOT FAULT-TOLERANT AND IS NOT DESIGNED, MANUFACTURED OR INTENDED FOR USE IN 
* CONJUNCTION WITH ON-LINE CONTROL EQUIPMENT, IN HAZARDOUS ENVIRONMENTS, IN APPLICATIONS 
* REQUIRING FAIL-SAFE PERFORMANCE, OR WHERE THE FAILURE OF THE SOFTWARE COULD LEAD DIRECTLY TO 
* DEATH, PERSONAL INJURY OR SEVERE PHYSICAL OR ENVIRONMENTAL DAMAGE (ALL OF THE FOREGOING, 
* "HIGH RISK ACTIVITIES"). ZILOG SPECIFICALLY DISCLAIMS ANY EXPRESS OR IMPLIED WARRANTY TO HIGH 
* RISK ACTIVITIES.
*
************************************************************************************************/

/** Standard Includes **/
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

/** RZK Includes */
#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZThread.h"
#include "rtc.h"

/** File System Includes*/
#include "FS_Shell.h"
#include "zfsapi.h"

/** ZTP Includes */
#include "date.h"
#include "igmp.h"
#include "snmpv1.h"
#include "telnet_api.h"

#include "ZGATE.h"


/** Extern Functions */
extern void DisplayTime();
extern void ftpdinit(void);
extern void nifDisplay(RZK_DEVICE_CB_t *dev);
extern void networkInit(void);
/** Extern Variables */
extern RZK_DEVICE_CB_t * CONSOLE;


/* Forward referencing the functions which are defined after ZTPAppEntry*/
void Initialize_FileSystem() ;
INT16 OpenSerialPort( RZK_DEVICE_CB_t 	**TTYDevID );
void snTrapNotifyCallback( INT8 trapType );
extern UINT  RZK_DEVTICKS_PERSYSTICK;



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

   #ifdef ZGATE_BUILD
      /*
       * Initialize the ZGATE Firewall
       */
      ZGATE_initialize();
      ZGATE_AddShellCmds();
      ZGATE_WebInit();
   #endif

	/** Initialize Shell */
	shell_init(TTYDevID);
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
