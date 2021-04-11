/*
 * Copyright 2016, ZiLOG Inc.
 * All Rights Reserved
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of ZiLOG Inc., and might
 * contain proprietary, confidential and trade secret information of
 * ZiLOG, our partners and parties from which this code has been licensed.
 * 
 * The contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of ZiLOG Inc.
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

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZThread.h"
#include "shell.h"
#include "Xc_lib.h"
#include "rtc.h"
#include "zfstypes.h"
#include "zfsapi.h"

#include "SocketDemoCommands.h"

extern RZK_DEVICE_CB_t * CONSOLE;
extern RZK_DEVICE_CB_t * SERIAL0;
extern RZK_DEVICE_CB_t * TTY;
extern UINT XTL_MAX_DCB;
extern RZK_DEVICE_CB_t usrDevBlk[];
extern void nifDisplay(RZK_DEVICE_CB_t *dev);
extern void networkInit(void);
INT16 OpenSerialPort( RZK_DEVICE_CB_t 	**TTYDevID ) ;
void Initialize_FileSystem() ;

INT16 ZTPAppEntry(void)
{
	RZK_DEVICE_CB_t *	TTYDevID;

	networkInit();

	nifDisplay(CONSOLE);
	 /** File System initialization */
	Initialize_FileSystem();		
	/*
	 * To start up a shell on the serial port, call shell_init(), and pass it the
	 * device to use.
	 */
	if( OpenSerialPort(&TTYDevID) == SYSERR )
		return SYSERR;

	

	shell_init(TTYDevID);
	AddSocketDemoCommands();
	
	return(OK);
}


INT16 OpenSerialPort( RZK_DEVICE_CB_t 	**TTYDevID )
{
	struct devCap 		*devSerial;
	devSerial = (struct devCap *) malloc (sizeof (struct devCap));
	devSerial->devHdl = (VOID *)CONSOLE ; 
	devSerial->devType = 0;
	if( (*TTYDevID = RZKDevOpen("TTYM",(RZK_DEV_MODE_t*)devSerial )) == (RZK_DEVICE_CB_t *)NULL ) 
	{
#ifdef DEBUG
	   	printf("Can't open tty for SERIAL0\n");
#endif
		return SYSERR;
	}
	return OK;
	
}

void Initialize_FileSystem()
{
	ZFS_STATUS_t status ;
	ZFS_STATUS_t vol_cnt ;
	PZFS_VOL_PARAMS_t pvol_params, ptmp_vol_params ;
	printf("\nInitializing File System, Please Wait..." ) ;
	vol_cnt = ZFSGetVolumeCount() ;
	if( vol_cnt )
	{
		pvol_params = malloc( sizeof( ZFS_VOL_PARAMS_t ) * vol_cnt ) ;
		if( pvol_params != NULL )
		{
			vol_cnt = ZFSInit( pvol_params ) ;
			if( vol_cnt == ZFSERR_SUCCESS )
				printf("Done") ;
			else
			{
				UINT8 cnt = 0 ;
				printf("Failed" ) ;
				// now format all the volumes
				ptmp_vol_params = pvol_params ;
				for( cnt = 0 ; cnt < vol_cnt ; cnt++ )
				{
					printf("\nFormatting the Invalid Volume : %s", &ptmp_vol_params->vol_name[0] ) ;
					status = ZFSFormat( ( INT8*) &ptmp_vol_params->vol_name[0] ) ;
					if( status != ZFSERR_SUCCESS )
						printf("FAILED");
					else
						printf("SUCCESS");
					ptmp_vol_params++;
				}
			}
			free( pvol_params ) ;
		}
		else
			printf("\nCouldn't Allocate memory to get volume parameters");
	}
	else
		printf("\nNo File System volumes are found in the system");
}


