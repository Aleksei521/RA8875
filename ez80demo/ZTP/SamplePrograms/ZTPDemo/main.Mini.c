/*
 * Copyright 2003, ZiLOG Inc.
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
#include "http.h"
#include "igmp.h"

extern RZK_DEVICE_CB_t* CONSOLE;
extern RZK_DEVICE_CB_t* SERIAL0;
extern RZK_DEVICE_CB_t* TTY;
extern Webpage website[];
extern void networkInit(void);
extern void nifDisplay(RZK_DEVICE_CB_t *dev);
INT16 OpenSerialPort( RZK_DEVICE_CB_t 	**TTYDevID );

INT16 ZTPAppEntry(void)
{
	RZK_DEVICE_CB_t* 	TTYDevID;
	struct cmdent*		mycmds;

	networkInit();

	nifDisplay(CONSOLE);

	/*This is for initializing the HTTP server without authentication. Use the 
	  Mini_Website.lib Website Library for this(Modify at Project->Settings->Linker->
	  General->Object/library modules )*/
	http_init(http_defmethods,httpdefheaders,website,80);

	/*This is for initializing the HTTP server with MD5 Digest authentication 
	  Only one HTTP server should be initialised. Use the AuthMini_Website.lib Website Library 
	  for this(Modify at Project->Settings->Linker->General->Object/library modules )*/
//	httpDigestAuth_init(httpDigestAuth_defmethods,httpdefheaders,website,80);

	/*This is for initializing the HTTP server with Basic authentication 
	  Only one HTTP server should be initialised.Use the Mini_Website.lib Website Library 
	  for this(Modify at Project->Settings->Linker->General->Object/library modules )*/
//	httpBasicAuth_init(http_defmethods,httpdefheaders,website,80);

	/*
	 * To start up a shell on the serial port, call shell_init(), and pass it the
	 * device to use.
	 */
	 
	/*
	 * To start up a shell on the serial port, call shell_init(), and pass it the
	 * device to use.
	 */
	if( OpenSerialPort(&TTYDevID) == SYSERR )
		return SYSERR;

	hginit(0);
	shell_init(TTYDevID);
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
	return OK ;
	
}