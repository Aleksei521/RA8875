/************************************************************************************************
** File:        DataPer_Conf.c
** Description: Data persistence configuration file.
**
** Copyright 2017 Zilog Inc. ALL RIGHTS RESERVED.
*
*************************************************************************************************
* The source code in this file was written by an authorized Zilog employee or a licensed 
* consultant. The source code has been verified to the fullest extent possible. 
*
* Permission to use this code is granted on a royalty-free basis. However, users are cautioned to
* authenticate the code contained herein. 
* 
* ZILOG DOES NOT GUARANTEE THE VERACITY OF THIS SOFTWARE; ANY SOFTWARE CONTAINED HEREIN IS
* PROVIDED "AS IS." NO WARRANTIES ARE GIVEN, WHETHER EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
* IMPLIED WARRANTIES OF FITNESS FOR PARTICULAR PURPOSE OR MERCHANTABILITY. IN NO EVENT WILL ZILOG
* BE LIABLE FOR ANY SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES OR ANY LIABILITY IN TORT,
* NEGLIGENCE, OR OTHER LIABILITY INCURRED AS A RESULT OF THE USE OF THE SOFTWARE, EVEN IF ZILOG 
* HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. ZILOG ALSO DOES NOT WARRANT THAT THE USE 
* OF THE SOFTWARE, OR OF ANY INFORMATION CONTAINED THEREIN WILL NOT INFRINGE ANY PATENT, 
* COPYRIGHT, OR TRADEMARK OF ANY THIRD PERSON OR ENTITY.

* THE SOFTWARE IS NOT FAULT-TOLERANT AND IS NOT DESIGNED, MANUFACTURED OR INTENDED FOR USE IN 
* CONJUNCTION WITH ON-LINE CONTROL EQUIPMENT, IN HAZARDOUS ENVIRONMENTS, IN APPLICATIONS 
* REQUIRING FAIL-SAFE PERFORMANCE, OR WHERE THE FAILURE OF THE SOFTWARE COULD LEAD DIRECTLY TO 
* DEATH, PERSONAL INJURY OR SEVERE PHYSICAL OR ENVIRONMENTAL DAMAGE (ALL OF THE FOREGOING, 
* "HIGH RISK ACTIVITIES"). ZILOG SPECIFICALLY DISCLAIMS ANY EXPRESS OR IMPLIED WARRANTY TO HIGH 
* RISK ACTIVITIES.
*
************************************************************************************************/

#include "DataPersistence.h"
#include "eZ80DevKit.h"


#ifdef _EZ80
   /*
    * The eZ80L92 and eZ80190 microprocessors do not contain any internal Flash.
    * Therefore external Flash must be used for storing non-volatile parameters.
    * When external Flash is used for data persistence this module uses the last
    * DATA_PER_BLOCK_SIZE bytes of Flash for non-volatile parameters.
    */
   #define DATA_PER_EXT_FLASH
#else
   /*
    * It is recommended to always use the eZ80F9x information page for storing 
    * persistent parameters.  Alternatively, internal or external Flash can be 
    * used for storing parameters by commenting out the DATA_PER_INFO_PAGE
    * definition and uncommenting the DATA_PER_INT_FLASH or DATA_PER_INT_FLASH
    * definition.  Only one of the following definitions should be uncommented.
    * When internal or external Flash is used for data persistence this module 
    * uses the last DATA_PER_BLOCK_SIZE bytes of Flash for non-volatile storage.
    */
   #define DATA_PER_INFO_PAGE
   //#define DATA_PER_INT_FLASH
   //#define DATA_PER_EXT_FLASH
#endif



/*
 * eZ80F9x information page configuration
 */
#ifdef DATA_PER_INFO_PAGE
   INT8
   DataPerInit_IP
   (
      void
   )
   {
      return( IFL_Init(0) );
   }

   DATA_PER_CFG            DataPerCfg = 
   {
      MK_PTR_OFS_SEG( 0x0000, FLASH_ADDR_U_INIT_PARAM ),
      DataPerInit_IP,
      (FP_ZFL_ERASE)IFL_EraseInfoPage,
      IFL_ProgramInfoPage,
      IFL_ReadInfoPage
   };
#endif


/*
 * eZ80F9x internal Flash configuration
 */
#ifdef DATA_PER_INT_FLASH
   INT8
   DataPerInit_IntFlash
   (
      void
   )
   {
      return( IFL_Init(0) );
   }

   DATA_PER_CFG            DataPerCfg = 
   {
      MK_PTR_OFS_SEG( (IFL_FLASH_SIZE_BYTES - IFL_PAGE_SIZE), FLASH_ADDR_U_INIT_PARAM ),
      DataPerInit_IntFlash,
      IFL_PageErase,
      IFL_Program,
      IFL_Read
   };
#endif


/*
 * External Flash configuration (only option for non eZ80F9x devices)
 */
#ifdef DATA_PER_EXT_FLASH
   /*
    * The Zilog Flash Library (ZFL) provides a lightweight API for manipulating
    * external Flash devices (XFLD API) that should be used when the command 
    * set the external Flash device implements is known.  In this instance, the 
    * default FlashLib.h header file defines a set of XFLD macros (XFLD_Erase,
    * XFLD_Program, XFLD_Query, and XFLD_Read) based on the value of the DEV_KIT
    * symbol.  When running on an unknown device kit, ZFS uses the full-featured
    * (larger code size) external Flash (XFL) API of the Flash library.
    */
   INT8
   DataPerInit_ExtFlash
   (
      void
   )
   {
      #ifdef XFLD_API_VALID
         /*
          * The XFLD API does not require initialization.
          */
         return( ZFL_ERR_SUCCESS  );
      #else
         #if( (DEV_KIT==eZ80F91_99C0942) || (DEV_KIT==eZ80F91_99C1322) ||\
              (DEV_KIT==eZ80F91_99C1380) || (DEV_KIT==eZ80F91_ZGATE)   ||\
              (DEV_KIT==eZ80L92_99C1400) )
            /*
             * No need to include support for non-CFI Flash devices when the 
             * target application board includes a CFI-compliant Flash device.
             */
            return( XFL_Init(0, NULLPTR) );
         #else
            /*
             * The target application board does not contain a CFI-compliant
             * Flash.  Use statically defined geometry information.
             */
            return( XFL_Init(0, NonCfiDevices) );
         #endif
      #endif
   }

   DATA_PER_CFG            DataPerCfg = 
   {
      #if DEV_KIT == eZ80L92_99C0857
         MK_PTR_OFS_SEG( 0x004000, CS0_LBR_INIT_PARAM ),
      #else
         MK_PTR_OFS_SEG( (UINT16)-sizeof(DATA_PER_t), CS0_UBR_INIT_PARAM +1 ),
      #endif
      DataPerInit_ExtFlash,
      #ifdef XFLD_API_VALID
         XFLD_Erase,
         XFLD_Program,
         XFLD_Read
      #else
         XFL_BlockErase,
         XFL_Program,
         XFL_Read
      #endif
   };
#endif
