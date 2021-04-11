/*
* File:        ZTPInit_Conf_mini.c
*
* Description: Initialization routines for the Zilog TCP/IP (ZTP) Software Suite
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
#include "ZDevice.h"
#include "ZTPTcp.h"
#include "ZFSCfg.h"
#include "externvar.h"


// external functions
extern VOID SpiderZInit( VOID );
extern INT16 Init_Serial0_Device( void );
extern INT16 Init_Serial1_Device( void );
extern INT16 Init_RTC_Device( void );
extern INT16 Init_EMAC_Device( void );
extern INT16 Init_TTY_Device( void );
extern INT16 nifDriverInit( void );
extern INT16 DHCP_Init( void );
extern INT16 CreateZTPAppThread( void );
extern INT16 Init_DataPersistence( void );


// external variables
extern UINT8                  b_use_dhcp;
extern BOOL                   bootp_called;
extern INT8                   fsCwdPath[];
extern ZFS_CONFIG_t           g_zfs_cfg[];


UINT8                         g_rxPkt;



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

   RZK_KernelInit();       // This function will call the RZKHwInitC 
   Init_Serial0_Device();  // Initializes the serial0 port
   Init_DataPersistence(); // Gets the data stored in FLASH such as IP addr, etc..
#ifndef SSL_F91_MINI
   Init_RTC_Device();      // initializes the RTC device
#endif
   Init_TTY_Device();      // initializes the TTY device
   Init_EMAC_Device();
   nifDriverInit();

   CreateZTPAppThread();   // Creates the ZTP application thread
   RZK_KernelStart();      // This function  call will resume the threads 
                           // created just before. Calling of these functions 
                           // is a must and should be at the end of the main.
}



void networkInit(void)
{
   bootp_called = 2;
   g_rxPkt=1;
   if( b_use_dhcp == TRUE )
   {
      RZKSuspendThread(RZKGetCurrentThread(),20);
      printf("\r\nQuerying DHCP Server...");
      DHCP_Init();
      RZKSuspendThread(RZKGetCurrentThread(),MAX_INFINITE_SUSPEND);
   }
   SpiderZInit();
}
