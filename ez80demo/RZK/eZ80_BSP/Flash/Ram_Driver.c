/************************************************************************************
 * File       : RamDrv.c
 * Description: This file contains the RAM driver for use in ZiLOG File System
 * Author     : 
 * Created on : 30-APR-2003
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
 ************************************************************************************/

#include "Ram_Driver.h"
#include "ZSysgen.h"
#include "ZThread.h"
#include <string.h>>


UINT8                   g_ram_driver_initialized;



INT RamDrv_Init( void * paddr, UINT num_bytes )
{
   RZK_STATE_t          preempt_status;
   INT                  status = -1;


   preempt_status = RZKDisablePreemption();
   if( g_ram_driver_initialized != RZK_TRUE )
   {
      g_ram_driver_initialized = RZK_TRUE;
      status = SUCCESS;
   }
   RZKRestorePreemption( preempt_status );
   return( status );
}



INT RamDrv_Read( void * paddr, void * pbuf, UINT num_bytes )
{
   if( g_ram_driver_initialized == RZK_FALSE )
   {
      return -1;
   }
   memcpy( pbuf, paddr, num_bytes );
   return( num_bytes );
}



INT RamDrv_Write( void * paddr, void * pbuf, UINT num_bytes )
{
   if( g_ram_driver_initialized == RZK_FALSE )
   {
      return -1;
   }
   memcpy( paddr, pbuf, num_bytes );
   return( num_bytes );
}



INT RamDrv_Erase( void * paddr, UINT num_bytes )
{
   if( g_ram_driver_initialized == RZK_FALSE )
   {
      return -1;
   }
   memset( paddr, 0xFF, num_bytes );
   return( num_bytes );
}



INT RamDrv_Close( void )
{
   RZK_STATE_t          preempt_status;


   if( g_ram_driver_initialized == RZK_FALSE )
      return -1;
   
   preempt_status = RZKDisablePreemption();

   g_ram_driver_initialized = RZK_FALSE;

   RZKRestorePreemption( preempt_status );

   return SUCCESS;
}
