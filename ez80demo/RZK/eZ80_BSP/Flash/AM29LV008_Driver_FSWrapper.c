/************************************************************************************
 * File       : AM29LV008_Driver_FSWrapper.c
 * Description: This file contains the Flash driver for use in ZiLOG File System
 * Author     :  
 * Created on : 04-OCT-2004
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

/* Revision History
 * CR #6089:05/30/2005
 * Semaphore is not being deleted when the driver is closed after 
 * 20 successive driver open-close calls are made.
 */

#include <stdio.h>
#include <string.h>
#include "AM29LV008_Driver.h"
#include "ZSysgen.h"
#include "ZThread.h"
#include "ZSemaphore.h"

// External Flash driver wrapper that will call the actual flash driver routines
RZK_SEMAPHOREHANDLE_t   g_amd_flash_sem_Handle;
UINT8                   g_amd_flash_initialized;



INT FS_AM29LV008_Init( void * paddr, UINT num_bytes )
{
   RZK_STATE_t          preempt_status;
   INT                  status = -1;


   preempt_status = RZKDisablePreemption();
   if( g_amd_flash_initialized != RZK_TRUE )
   {
      g_amd_flash_sem_Handle = RZKCreateSemaphore( ( RZK_NAME_t * ) "AMDFLASH", 1, RECV_ORDER_PRIORITY );
      if( g_amd_flash_sem_Handle )
      {
         g_amd_flash_initialized = RZK_TRUE;
         status = AM29LV008_Init( paddr, num_bytes );
      }
   }
   RZKRestorePreemption( preempt_status );
   return( status );
}



INT FS_AM29LV008_Read( void * paddr, void * pbuf, UINT num_bytes )
{
   INT                  status;
   INT                  num_bytes_read;


   if( g_amd_flash_initialized == RZK_FALSE )
      return -1;
   
   status = RZKAcquireSemaphore( g_amd_flash_sem_Handle, MAX_INFINITE_SUSPEND );
   if( status != RZKERR_SUCCESS )
      return -1;

   num_bytes_read = AM29LV008_Read( paddr, pbuf, num_bytes );

   RZKReleaseSemaphore( g_amd_flash_sem_Handle );

   return num_bytes_read;
}



INT FS_AM29LV008_Write( void * paddr, void * pbuf, UINT num_bytes )
{
   INT                  status;
   INT                  num_bytes_written;


   if( g_amd_flash_initialized == RZK_FALSE )
      return -1;
   
   status = RZKAcquireSemaphore( g_amd_flash_sem_Handle, MAX_INFINITE_SUSPEND );
   if( status != RZKERR_SUCCESS )
      return -1;

   num_bytes_written = AM29LV008_Write( paddr, pbuf, num_bytes );

   RZKReleaseSemaphore( g_amd_flash_sem_Handle );

   return num_bytes_written;
}



INT FS_AM29LV008_Erase( void * paddr, UINT num_bytes )
{
   INT                  status;


   if( g_amd_flash_initialized == RZK_FALSE )
      return -1;
   
   status = RZKAcquireSemaphore( g_amd_flash_sem_Handle, MAX_INFINITE_SUSPEND );
   if( status != RZKERR_SUCCESS )
      return -1;

   status = AM29LV008_Erase( paddr, num_bytes );

   RZKReleaseSemaphore( g_amd_flash_sem_Handle );

   return status;
}



INT FS_AM29LV008_Close( void )
{
   RZK_STATE_t          preempt_status;
   INT                  status;


   if( g_amd_flash_initialized == RZK_FALSE )
      return -1;
   
   preempt_status = RZKDisablePreemption();
   
   g_amd_flash_initialized = RZK_FALSE;

   status = AM29LV008_Close();

   // CR#6089
   // Delete the semaphore
   RZKDeleteSemaphore( g_amd_flash_sem_Handle );

   RZKRestorePreemption( preempt_status );

   return status;
}
