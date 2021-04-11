/*
* File				:	ZTPInit_Conf.c
*
* Description		:	Initialization routines for the Zilog TCP/IP (ZTP) Software Suite
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
#include "eZ80.h"
#include "ZDevice.h"
#include "ZTPTcp.h"


// extern functions
extern INT16 Init_Serial0_Device( void ) ;
extern INT16 Init_Serial1_Device( void ) ;
extern INT16 Init_RTC_Device( void ) ;
extern INT16 Init_EMAC_Device( void ) ;
extern INT16 Init_TTY_Device( void ) ;
extern INT16 Init_TCPUDP_Device( void ) ;
extern INT16 Init_Wlan_Device( void );
extern INT16 Init_Globals( void );
extern INT16 Init_DataPersistence( void );
extern INT16 CreateZTPAppThread( void ) ;


UINT8 g_rxPkt;

extern VOID SpiderZInit( VOID ) ;
extern INT16 ZTPAppEntry( VOID ) ;
extern UINT8 g_AppEntryTaskPrio;
extern INT16 Init_Wlan_Device( void );
extern UINT16 nifDriverInit( void ) ;
extern INT16 DHCP_Init( void ) ;
extern UINT8  b_use_dhcp ;
extern BOOL	bootp_called;

RZK_DEVICE_CB_t *	                                               SERIAL0;
RZK_DEVICE_CB_t *	                                               SERIAL1;
RZK_DEVICE_CB_t *	                                               TTY;
RZK_DEVICE_CB_t *                                                TCP;
RZK_DEVICE_CB_t *                                                UDP;
RZK_DEVICE_CB_t *                                                EMACDEV;
RZK_DEVICE_CB_t *                                                pRTCDev;


// main function for ZTP system
void main( void )
{
#ifdef RZKFS
   UINT8                Len;


   /*
    * Use the name of the first volume as the initial CWD for all threads created 
    * between calls to RZK_KernelInit and RZK_KernelStart.
    */
   Len = strlen( g_zfs_cfg[0].vol_name );
   if( (Len + 2) <= nRzkMaxCwdPathLen )
   {
      memcpy( fsCwdPath, g_zfs_cfg[0].vol_name, Len );
      strcpy( &fsCwdPath[ Len ], ":/" );
   }
#endif

	RZK_KernelInit() ;			// This function will call the RZKHwInitC 

	Init_Serial0_Device() ;	// Initializes the serial0 port
	Init_Serial1_Device() ;	// Initializes the serial1 port
	Init_RTC_Device() ;		// initializes the RTC device
	Init_TTY_Device() ;		// initializes the TTY device

	Init_DataPersistence() ; // Gets the data stored in FLASH such as IP addr, etc..

#ifdef ZDOT_WLAN
	//Power on, RTL on
	PA_DR	&= (0xFE); 
	PA_DDR	&= (0xFE); 	
	PA_ALT2 &= (0xFE);
	PA_ALT1 &= (0xFE);
#endif

	CreateZTPAppThread() ;	// Creates the ZTP application thread
	RZK_KernelStart() ;			// This function  call will resume the threads 
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

	nifDriverInit() ;
	g_rxPkt=1;
	if( b_use_dhcp == TRUE )
	{
		RZKSuspendThread(RZKGetCurrentThread(),20);
		printf("\r\nQuerying DHCP Server...");
		DHCP_Init();
		RZKSuspendThread(RZKGetCurrentThread(),MAX_INFINITE_SUSPEND);
	}
	SpiderZInit() ;
}
		
