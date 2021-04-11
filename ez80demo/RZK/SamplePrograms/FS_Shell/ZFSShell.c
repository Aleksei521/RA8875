/************************************************************************************
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
 ************************************************************************************/
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZDevice.h"
#include "ZThread.h"
#include "rtc.h"

///////////////////////////////////////
// SHELL RELATED INFORMATION
///////////////////////////////////////

#define SHELL_STACK_SIZE		2048
#define SHELL_PRIORITY1			10
#define SHELL_RR_TICKS			10
extern void Shell_Entry( void ) ;

///////////////////////////////////////
// SHELL RELATED INFORMATION
///////////////////////////////////////

extern void InitUART ( void ) ;

UINT8 g_ShellStack[ SHELL_STACK_SIZE ] ;

INT main( INT argc, void *argv[] )
{
	RZK_THREADHANDLE_t	ShellTask;

	RZK_KernelInit() ;
	InitUART() ;
//	init_malloc() ;Not required as RZK calls this internally

	AddRtc();

	ShellTask = RZKCreateThread( ( UINT8 *) "SHELL",
					(RZK_PTR_t)Shell_Entry,
					NULL,
					( g_ShellStack + SHELL_STACK_SIZE ),
					SHELL_PRIORITY1,
					SHELL_RR_TICKS, 
					RZK_THREAD_AUTOSTART | RZK_THREAD_PREEMPTION,
					0,
					0 ) ; 

	// validate the thread handle
	if( ShellTask == NULL )
	{
		printf("\nUnable to create the thread #1, error description is");
		return 0;
	}

	RZK_KernelStart() ;
	return 0;

}


