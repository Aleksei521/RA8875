/************************************************************************************
 * File       : IntFlash_Driver.c
 * Description: This file contains the Flash driver for use in ZiLOG File System
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
 **********************************************************************************/

#include <stdio.h>
#include <string.h>
#include "IntFlash_InfoPage_Driver.h"
#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZInterrupt.h"

#ifdef _EZ80F91
#include <eZ80F91.h>
#endif

#ifdef _EZ80F92
#include <eZ80F92.h>
#endif

#ifdef _EZ80F93
#include <eZ80F93.h>
#endif

#ifdef _IAR_CODE
#define LOC_ADDR UINT32
#else
#define LOC_ADDR UINT
#endif


#pragma modsect _driver_TEXT



INT IntFlash_Init( void *paddr, UINT num_bytes )
#ifdef _IAR_CODE
@ "DATA_PER_RAM"
#endif
{
   UINTRMASK            IntMask;


   IntMask = RZKDisableInterrupts();

   /*
    * Setup the Flash Frequency Divider Register (FLASH_FDIV).
    * FLASH_FDIV can only be modified while the Flash control registers are
    * unlocked.  Modifying FLASH_FDIV relocks the Flash control registers.
    */
   FLASH_KEY = 0xB6;
   FLASH_KEY = 0x49;

   #ifdef _EZ80F91
      FLASH_FDIV = 0xFF; 
   #endif

   #ifdef _EZ80F92
      FLASH_FDIV = 0x66; 
   #endif

   #ifdef _EZ80F93
      FLASH_FDIV = 0x66; 
   #endif

   /*
    * Enable all of internal Flash to be erased via Flash Protection Register.
    * FLASH_PROT can only be modified while the Flash control registers are
    * unlocked.  Modifying FLASH_PROT relocks the Flash control registers.
    */
   FLASH_KEY  = 0xB6;
   FLASH_KEY  = 0x49;
   FLASH_PROT = 0x00;

   RZKEnableInterrupts( IntMask );

   return SUCCESS;
}



INT IntFlash_Read( void *paddr, void *pbuf, UINT num_bytes )
#ifdef _IAR_CODE
@ "DATA_PER_RAM"
#endif
{
   memcpy( pbuf, paddr, num_bytes );
   return num_bytes;
}



INT IntFlash_Write( UINT8 *paddr, UINT8 *pbuf, UINT num_bytes )
#ifdef _IAR_CODE
@ "DATA_PER_RAM"
#endif
{
   UINT16               i;


   for( i=num_bytes; i; i-- )
   {
      *paddr++ = *pbuf++;
   }

   return( num_bytes );
}



INT IntFlash_Erase( void *paddr, UINT num_bytes )
#ifdef _IAR_CODE
@ "DATA_PER_RAM"
#endif
{
   UINT8                nCurrPage = GETPAGE( (UINT) paddr );
   UINT8                nLastPage = nCurrPage + F9x_PAGES_PER_BLOCK;
   UINTRMASK            IntMask;


   IntMask = RZKDisableInterrupts();
   while( nCurrPage < nLastPage )
   {
      /*
       * Erase the next page in the 32KB protection block
       */
      FLASH_PAGE  = nCurrPage;
      FLASH_PGCTL = FLASH_PGCTL_PG_ERASE_ENABLE;

      /*
       * Wait for program operation to complete
       */
      while( FLASH_PGCTL & FLASH_PGCTL_PG_ERASE_ENABLE );
      nCurrPage++;
   }
   RZKEnableInterrupts( IntMask );

   return SUCCESS;
}



INT IntFlash_Close( void )
#ifdef _IAR_CODE
@ "DATA_PER_RAM"
#endif
{
   // disable INTERNAL FLASH
   FLASH_CTRL = 0x00;
   return SUCCESS;
}
