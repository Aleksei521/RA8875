/*
* File				:	XTLInit.c
*
* Description		:	This file contains initalization of XTL for ZTP
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
#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZThread.h"
#include "EtherMgr.h"
#include "Rtc.h"
#include "ZDevice.h"
#include "ZTPTcp.h"


// extern functions
extern INT16 Init_Serial0_Device( void ) ;
extern INT16 Init_Serial1_Device( void ) ;
extern INT16 Init_RTC_Device( void ) ;
extern INT16 Init_EMAC_Device( void ) ;
extern INT16 Init_TTY_Device( void ) ;
extern INT16 Init_TCPUDP_Device( void ) ;
extern UINT16 nifDriverInit( void ) ;
extern INT16 DHCP_Init( void ) ;
extern INT16 CreateZTPAppThread( void ) ;
extern INT16 Init_Globals( void );
extern INT16 Init_DataPersistence( void );
extern void SpiderZInit( VOID ) ;

UINT8 g_rxPkt;
// extern variables
extern UINT8  b_use_dhcp ;
extern BOOL	bootp_called;


// **************************
// CHANGE BELOW VALUES IF YOU KNOW WHAT YOU ARE DOING
// BELOW VALUES PRESENT IN STRUCTURE/MACRO/VARIABLES CAN BE
// CHANGED ACCORDING TO THE SYSTEM NEEDS. PLEASE REFER TO THE
// REFERENCE MANUAL FOR STRUCTURE/MACRO/VARIABLE DEFINITIONS
// **************************
#define SYS_TASK_STK_SIZE               (2048)
#define SYS_TASK_STK_PRIO               (16)	

UINT16      xinu_min_stack                           = 1024;


// **************************
// PLEASE DO NOT CHANGE ANYTHING BELOW IN ZTP CORE CONFIGURATION
// **************************
RZK_THREADHANDLE_t                                 g_hSysTask ;
UINT      g_SysTaskStkSize                         = SYS_TASK_STK_SIZE ;

UINT      g_SysTaskPrio                            = SYS_TASK_STK_PRIO ;
UINT8     g_pSysTaskStk[ SYS_TASK_STK_SIZE ] ;



// main function for ZTP system
void main( INT argc, void *argv[] )
{
	RZK_KernelInit() ;			// This function will call the RZKHwInitC 
								// function, but will not resume the idle thread.
	Init_Serial0_Device();
	CreateZTPAppThread() ;	// Creates the ZTP application thread
	RZK_KernelStart() ;			// This function will call resume the threads 
								// created just before. Calling of these functions 
								// is a must and should be at the end of the main.
}


void networkInit(void)
{

#ifdef ZDOT_WLAN
	if (Init_Wlan_Device() < 0 )
		return;
#else
	Init_EMAC_Device() ;		// initializes the EMAC device	
#endif
	
	bootp_called = 2;

	nifDriverInit() ;		// initializes the interfaces specified in the table	
	g_rxPkt=1;
	// check whether the DHCP should be used to get the IP address for the system
	if( b_use_dhcp == TRUE )
	{
		RZKSuspendThread(RZKGetCurrentThread(),20);
		printf("\r\nQuerying DHCP Server...");
		DHCP_Init();
		RZKSuspendThread(RZKGetCurrentThread(),MAX_INFINITE_SUSPEND);
	}
	SpiderZInit() ;
}
	