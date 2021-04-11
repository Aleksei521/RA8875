/************************************************************************************
 * File       : MT28F008_Driver.c
 * Description: This file contains the Flash driver for use in ZiLOG File System
 * Author     :  (Reused from Prototype fs driver code)
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
 ******************************************************************************/

#include "MT28F008_Driver.h"
#include <string.h>
#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZInterrupt.h"


#pragma modsect _driver_TEXT



/*Readback Fail Flag: Every line flashed is read back to verify whether 
 * flashing happened properly.  This flag is set when there is a readback
 * fail.
 */
UINT8 gucReadbackFailed = 0 ; //default to no readback fails

INT32 FlashError(UINT8 ucStatus) 
#ifdef _IAR_CODE
@ "DATA_PER_RAM"
#endif
{

   if(ucStatus & MT28F008_ERASE_SUSPENDED) 
      return -MT28F008_ERASE_SUSPENDED;
   
   if(ucStatus & MT28F008_ERASE_ERROR) 
      return -MT28F008_ERASE_ERROR;
   
   if(ucStatus & MT28F008_WRITE_ERROR) 
      return -MT28F008_WRITE_ERROR;
   
   if(ucStatus & MT28F008_NO_VOLTAGE) 
      return -MT28F008_NO_VOLTAGE;

   return SUCCESS;
}



INT MT28F008_Init( UINT8  *ucPtr, UINT num_bytes   )
#ifdef _IAR_CODE
@ "DATA_PER_RAM"
#endif
{
   UINTRMASK            IntMask;


   IntMask = RZKDisableInterrupts();
   *ucPtr=MT28F008_RESET;
   *ucPtr=MT28F008_CLEAR_STATUS_REG;
   *ucPtr=MT28F008_READ_ARRAY;
   RZKEnableInterrupts( IntMask );

   return SUCCESS ;
}



INT MT28F008_Read( void * paddr, void * pbuf, UINT num_bytes )
#ifdef _IAR_CODE
@ "DATA_PER_RAM"
#endif
{
   memcpy( pbuf, paddr, num_bytes );
   return num_bytes ;
}



INT MT28F008_Write( UINT8 * ucPtr, UINT8 * ucSrcAddr, UINT num_bytes )
#ifdef _IAR_CODE
@ "DATA_PER_RAM"
#endif
{
   UINT                 i;
   UINT8                ucStatus;
   UINTRMASK            IntMask;


   IntMask = RZKDisableInterrupts();
   for(i=0; i < num_bytes; i++, ucPtr++) 
   {
      // Write set up       
      *ucPtr=MT28F008_RESET;
      *ucPtr=MT28F008_CLEAR_STATUS_REG;
      *ucPtr=MT28F008_WRITE_SETUP;
      *ucPtr=*ucSrcAddr;
      do
      {
         ucStatus = (UINT8)(MT28F008_STATUS_BITS & *ucPtr);
      } while( ucStatus == MT28F008_BUSY);

      if(ucStatus != MT28F008_READY)
      {   //if failed ..
         //wait(ONE_SECOND);
         *ucPtr=MT28F008_RESET;   // try one more time
         *ucPtr=MT28F008_CLEAR_STATUS_REG;
         *ucPtr=MT28F008_WRITE_SETUP;
         *ucPtr=*ucSrcAddr;
         do 
         {
            ucStatus = (UINT8)(MT28F008_STATUS_BITS & *ucPtr);
         } while( ucStatus == MT28F008_BUSY);
         if( ucStatus != MT28F008_READY) 
         {
            *ucPtr=MT28F008_RESET;
            *ucPtr=MT28F008_CLEAR_STATUS_REG;
            *ucPtr=MT28F008_READ_ARRAY;
            RZKEnableInterrupts( IntMask );

            return FlashError(ucStatus);
         }
      }//end 'if:failed'
      ucSrcAddr++;   
   }
   *ucPtr=MT28F008_RESET;
   *ucPtr=MT28F008_CLEAR_STATUS_REG;
   *ucPtr=MT28F008_READ_ARRAY;
   RZKEnableInterrupts( IntMask );

    //Readback the data from the flash and verify with the
    //   data buffer whether they match

   return num_bytes;
}



INT MT28F008_Erase( UINT8 * ucPtr, UINT num_bytes )
#ifdef _IAR_CODE
@ "DATA_PER_RAM"
#endif
{
   UINT8                ucStatus;
   UINTRMASK            IntMask;


   IntMask = RZKDisableInterrupts();
   *ucPtr=MT28F008_RESET;
   *ucPtr=MT28F008_CLEAR_STATUS_REG;
   *ucPtr=MT28F008_ERASE_SETUP;
   *ucPtr=MT28F008_ERASE_CONFIRM;
   do
   {
      ucStatus = (UINT8)(MT28F008_STATUS_BITS & *ucPtr);
   } while(ucStatus == MT28F008_BUSY);

   if(ucStatus != MT28F008_READY)// if failed ...
   {         
      //wait(ONE_SECOND);
      *ucPtr=MT28F008_RESET;   // try one more time
      *ucPtr=MT28F008_CLEAR_STATUS_REG;
      *ucPtr=MT28F008_ERASE_SETUP;
      *ucPtr=MT28F008_ERASE_CONFIRM;
   
      do
      {
         ucStatus = (UINT8)(MT28F008_STATUS_BITS & *ucPtr);
      } while(ucStatus == MT28F008_BUSY);

      if(ucStatus != MT28F008_READY) 
      {
         *ucPtr=MT28F008_RESET;
         *ucPtr=MT28F008_CLEAR_STATUS_REG;
         *ucPtr=MT28F008_READ_ARRAY;
         RZKEnableInterrupts( IntMask );

         return FlashError(ucStatus);
      }
   }

   *ucPtr=MT28F008_RESET;
   *ucPtr=MT28F008_CLEAR_STATUS_REG;
   *ucPtr=MT28F008_READ_ARRAY;
   RZKEnableInterrupts( IntMask );
   
   return SUCCESS;
}



INT MT28F008_Close( void )
#ifdef _IAR_CODE
@ "DATA_PER_RAM"
#endif
{
   return SUCCESS;
}
