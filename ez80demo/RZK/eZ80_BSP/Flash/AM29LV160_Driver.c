/********************************************************************************
 * File       : AM29LV160_Driver.c
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
 *****************************************************************************/

#include <stdio.h>
#include <string.h>
#include "AM29LV160_Driver.h"
#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZInterrupt.h"


#pragma modsect _driver_TEXT



INT AM29LV160_Init( void * paddr, UINT num_bytes )
#ifdef _IAR_CODE
@ "DATA_PER_RAM"
#endif
{
   UINT8              * addr = (UINT8 *)((UINT)paddr & AM29LV160_MASK_BITS);
   UINT8                dev_code, man_code;
   UINTRMASK            IntMask;


   IntMask = RZKDisableInterrupts();

   // the address passed is the base address of the flash.
   // Setup PRODUCT_IDENTIFICATION_ENTRY MODE
   *( addr + 0xAAA ) = FLASH_CODE_AA;
   *( addr + 0x555 ) = FLASH_CODE_55;
   *( addr + 0xAAA ) = ID_IN_CODE;

   // get the manufacturer code and device code
   man_code = *addr;
   dev_code = *(addr + 2);

   // get the device in read mode.
   *addr = CMD_READ_ARRAY;

   RZKEnableInterrupts( IntMask );

   // print the manufacturer code and device code
   printf("\n Manufacturer code : 0x%X", man_code );
   if( dev_code == AMD_ID_29LV160BB )
   {
      printf("\n Device Id: AM29LV160BB ");
   }
   else if(  dev_code  == AMD_ID_29LV160TB )
   {
      printf("\n Device Id: AM29LV160TB ");
   }
   else if(  dev_code  == AMD_ID_S29GL064NTB )
   {
      printf("\n Device Id: S29GL064NTB ");
   }
   else
   {
      printf("\nUnknown device " );
   }

   return SUCCESS;
}



INT AM29LV160_Read( void * paddr, void *pbuf, UINT num_bytes )
#ifdef _IAR_CODE
@ "DATA_PER_RAM"
#endif
{
   // By default the device is in READ ARRAY mode, just copy the
   // contents from the device to the appropriate memory location.
   memcpy( pbuf, paddr, num_bytes );
   return num_bytes;
}



INT AM29LV160_Write( UINT8 * paddr, UINT8 * pbuf, UINT num_bytes )
#ifdef _IAR_CODE
@ "DATA_PER_RAM"
#endif
{
   UINT8              * base_addr = (UINT8 *)((UINT)paddr & AM29LV160_MASK_BITS);
   UINT8                result;
   UINT                 nbytes = 0;
   UINTRMASK            IntMask;


   for( nbytes = 0; nbytes < num_bytes; nbytes++ )
   {
      IntMask = RZKDisableInterrupts();
      // setup the Program command for the device.
      *( base_addr + 0xAAA ) = FLASH_CODE_AA;
      *( base_addr + 0x555 ) = FLASH_CODE_55;
      *( base_addr + 0xAAA ) = CMD_PROGRAM;
      *paddr = *pbuf;

      while(1)
      {
         // loop till when the same data read back that means the byte is written
         // onto the device.
         result = *paddr;
         if( result == *pbuf )
            break;
      }
      RZKEnableInterrupts( IntMask );

      pbuf++;
      paddr++;
   }

   return num_bytes;
}



INT AM29LV160_Erase( UINT8 * paddr, UINT num_bytes )
#ifdef _IAR_CODE
@ "DATA_PER_RAM"
#endif
{
   UINT8              * addr = (UINT8 *)((UINT)paddr & AM29LV160_MASK_BITS);
   UINT8                result;
   UINTRMASK            IntMask;


   IntMask = RZKDisableInterrupts();
   *( addr + 0xAAA ) = FLASH_CODE_AA;
   *( addr + 0x555 ) = FLASH_CODE_55;
   *( addr + 0xAAA ) = CMD_ERASE_SETUP;

   *( addr + 0xAAA ) = FLASH_CODE_AA;
   *( addr + 0x555 ) = FLASH_CODE_55;
   *( addr ) = CMD_ERASE_CONFIRM;

   while(1)
   {
      result = *addr;
      if ( (result) & BIT_ERASE_DONE)
      {
         // the erase is completed.
         break;
      }
   }

   // the device is put in the READ ARRAY mode after the ERASE is complete
   RZKEnableInterrupts( IntMask );

   return SUCCESS;
}



INT AM29LV160_Close( void )
#ifdef _IAR_CODE
@ "DATA_PER_RAM"
#endif
{
   return SUCCESS;
}

