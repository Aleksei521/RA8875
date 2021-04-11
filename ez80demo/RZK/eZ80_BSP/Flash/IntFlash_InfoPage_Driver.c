/************************************************************************************
 * File       : IntFlash_InfoPage_Driver.c
 * Description: This file contains the Flash driver for use in data persistence 
 *              where the values are stored in information page
 * Created on : 03-FEB-2005
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



INT IntFlash_IP_Init(  void * paddr, UINT num_bytes   )
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
    * Contrary to the F9x product spec, it appears that BIT0 in the  FLASH_PROT
    * register must be 0 to enable program/ erase operations on the InfoPage.
    */
   FLASH_KEY   = 0xB6;
   FLASH_KEY   = 0x49;
   FLASH_PROT &= 0xFE;

   RZKEnableInterrupts( IntMask );

   return SUCCESS;
}



INT IntFlash_IP_Read( UINT8 *paddr, UINT8 *pbuf, UINT num_bytes )
#ifdef _IAR_CODE
@ "DATA_PER_RAM"
#endif
{
   UINT16               Cnt = num_bytes;
   UINTRMASK            IntMask;


   IntMask = RZKDisableInterrupts();

   /*
    * Configure Flash Page. Row Col to access specified location (paddr) within
    * the info page.
    */
   FLASH_PAGE = FLASH_PAGE_INFO_ACCESS;
   if( (UINT16)paddr & F9x_ROW_SIZE_BYTES )
   {
      FLASH_ROW = 1;
   }
   else
   {
      FLASH_ROW = 0;
   }
   FLASH_COL  = GETCOL( (UINT16)paddr );

   /*
    * Copy Cnt consecutive bytes from the info page to pbuf.
    * Note that each access to the FLASH_DATA register automatically increments
    * the {Page,Row,Col} registers.
    */
   while( Cnt )
   {
      *pbuf = FLASH_DATA;
      pbuf = pbuf + 1;
      Cnt--;
   }

   RZKEnableInterrupts( IntMask );

   return( num_bytes );
}



INT IntFlash_IP_Write( UINT8 *paddr, UINT8 *pbuf, UINT num_bytes )
#ifdef _IAR_CODE
@ "DATA_PER_RAM"
#endif
{
   UINT16               Cnt = num_bytes;
   UINTRMASK            IntMask;


   IntMask = RZKDisableInterrupts();

   /*
    * Configure Flash Page. Row Col to access specified location (paddr) within
    * the info page.
    */
   FLASH_PAGE = FLASH_PAGE_INFO_ACCESS;
   if( (UINT16)paddr & F9x_ROW_SIZE_BYTES )
   {
      FLASH_ROW = 1;
   }
   else
   {
      FLASH_ROW = 0;
   }
   FLASH_COL  = GETCOL( (UINT16)paddr );

   /*
    * Copy Cnt consecutive bytes from pbuf to the info page.
    * Note that each access to the FLASH_DATA register automatically increments
    * the {Page,Row,Col} registers.
    */
   while( Cnt )
   {
      FLASH_DATA = *pbuf;
      pbuf = pbuf + 1;
      Cnt--;
   }

   RZKEnableInterrupts( IntMask );

   return( num_bytes );
}



INT IntFlash_IP_Erase( void *paddr, UINT num_bytes )
#ifdef _IAR_CODE
@ "DATA_PER_RAM"
#endif
{
   /*
    * The Info Page on all F9x devices is less than 1 page in length (512 bytes
    * on F91 and 256 bytes on F92/ F93).  Consequently this routine erases the
    * entire Info Page area regardless of the values of paddr and num_bytes,
    */
   UINTRMASK            IntMask;


   IntMask = RZKDisableInterrupts();

   FLASH_PAGE  = FLASH_PAGE_INFO_ACCESS;
   FLASH_ROW   = 0;
   FLASH_PGCTL = FLASH_PGCTL_PG_ERASE_ENABLE;

   /*
    * Wait for program operation to complete
    */
   while( FLASH_PGCTL & FLASH_PGCTL_PG_ERASE_ENABLE );

   RZKEnableInterrupts( IntMask );

   return SUCCESS;
}



INT IntFlash_IP_Close( void )
#ifdef _IAR_CODE
@ "DATA_PER_RAM"
#endif
{
   return SUCCESS;
}
