/************************************************************************************************
** File:        ZFS_Conf.c
** Description: Zilog Flash File System (ZFS) configuration file.
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

#include "ZFSTypes.h"
#include "ZFStruct.h"
#include "ZFSCfg.h"
#include "eZ80DevKit.h"


#ifdef RZKFS

/*
 * Uncomment one or more of the ZFS_USES_xxx definitions depending on which
 * memory devices are used for ZFS volume storage.  Internal Flash can only 
 * be used when building for an eZ80F9x device.  By default, ZFS volumes are
 * stored stored in external Flash (ZFS_USES_XFL).
 */
#define ZFS_USES_XFL
//#define ZFS_USES_IFL
//#define ZFS_USES_RAM


#ifdef ZFS_USES_XFL
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
   ZFS_XFL_Init
   ( 
      void
   )
   {
      #ifdef XFLD_API_VALID
      {
         /*
          * The XFLD API does not require initialization.
          */
         return( ZFL_ERR_SUCCESS );
      }
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

   /*
    * Define the characteristics of ZFS volumes located in external Flash here.
    *
    * Each volume must contain erase blocks of identical size. The size of each
    * erase block is determined by the geometry of the external Flash.  Select
    * a region in external Flash that has an erase block size at least 2x the
    * size of ZFS_SEC_SIZE.  Each volume should contain at least 3 erase blocks.
    * Do not include the memory range used by ZFS volumes located in Flash in 
    * the ROM address space of the project.
    */

   #if( (DEV_KIT==eZ80F91_99C1322) || (DEV_KIT==eZ80F91_99C1380) || (DEV_KIT==eZ80F91_ZGATE) || (DEV_KIT==eZ80L92_99C1400) )
      // External Flash Volume 1 is located at CS0_LBR + 0x200000 and contains 16
      // erase blocks with each block containing 0x010000 bytes.
      //                         Spansion S29GL064N
      #define XFL1_NUM_BLOCKS    16
      #define XFL1_BLOCK_SIZE    0x010000
      #define XFL1_BASE_ADDR     MK_PTR_OFS_SEG( 0x200000, CS0_LBR_INIT_PARAM)
   #endif

   #if( DEV_KIT==eZ80F91_99C0879 )
      /*
       * If the default data persistence storage area (eZ80F9x information page)
       * is moved to the end of external Flash, then reduce the size of the ZFS
       * volume by 1 erase block (see DataPer_Conf.c).
       */
      #ifdef REV1_DEV_KIT        // Micron MT28F008B3 Flash
         #define XFL1_NUM_BLOCKS 4
         #define XFL1_BLOCK_SIZE 0x020000
      #else                      // AMD AM29LV008BB Flash
         #define XFL1_NUM_BLOCKS 8
         #define XFL1_BLOCK_SIZE 0x010000
      #endif
      #define XFL1_BASE_ADDR     MK_PTR_OFS_SEG( 0x080000, CS0_LBR_INIT_PARAM)
   #endif

   #if( (DEV_KIT==eZ80F92_99C0873) || (DEV_KIT==eZ80F93_99C0873) )
      /*
       * If the default data persistence storage area (eZ80F9x information page)
       * is moved to the end of external Flash, then reduce the size of the ZFS
       * volume by 1 erase block (see DataPer_Conf.c).
       *
       * External Flash Volume 1 is located in the last 3 erase blocks
       *                         Micron MT28F008B3 Flash
       */
      #define XFL1_NUM_BLOCKS    3
      #define XFL1_BLOCK_SIZE    0x020000
      #define XFL1_BASE_ADDR     MK_PTR_OFS_SEG( -(XFL1_BLOCK_SIZE * XFL1_NUM_BLOCKS), \
                                                (CS0_UBR_INIT_PARAM +1) )
   #endif
   #if( DEV_KIT==eZ80L92_99C0857 )
      #ifdef REV1_DEV_KIT        // Micron MT28F008B3 Flash
         #define XFL1_NUM_BLOCKS 3
         #define XFL1_BLOCK_SIZE 0x020000
      #else                      // AMD AM29LV008BB Flash
         #define XFL1_NUM_BLOCKS 6
         #define XFL1_BLOCK_SIZE 0x010000
      #endif
      // External Flash Volume 1 is located in the last 384KB of CS0 Flash
      #define XFL1_BASE_ADDR     MK_PTR_OFS_SEG( -(XFL1_BLOCK_SIZE * XFL1_NUM_BLOCKS), \
                                                (CS0_UBR_INIT_PARAM +1) )
   #endif
   #define XFL1_NUM_SEC          ((XFL1_BLOCK_SIZE / ZFS_SEC_SIZE) * XFL1_NUM_BLOCKS)
#else
   // No ZFS volumes are located in external Flash
   #define XFL1_NUM_BLOCKS       0
   #define XFL1_BLOCK_SIZE       0
   #define XFL1_NUM_SEC          0
#endif



#if defined(ZFS_USES_IFL) && !defined(_EZ80)
   /*
    * Use the Zilog Flash library's internal Flash interface to manipulate 
    * internal Flash (only available on eZ80F9x microprocessors).
    */
   INT8
   ZFS_IFL_Init
   ( 
      void
   )
   {
      return( IFL_Init(0) );
   }

   /*
    * Define the characteristics of ZFS volumes located in internal Flash here.
    *
    * Internal Flash is only available on eZ80F9x microprocessors.
    *
    * Internal Flash is composed of identically sized pages (erase blocks) that
    * are either 4x ZFS_SEC_SIZE (eZ80F91) or 2x ZFS_SEC_SIZE (eZ80F92/93). Each
    * volume of internal Flash should contain at least 4 erase blocks.  Do not 
    * include the memory range used by ZFS volumes located in Flash in the ROM 
    * address space of the project.
    */

   // Internal Flash Volume 1 is located at the end of internal Flash and contains
   // contains 32 erase blocks with each block containing IFL_PAGE_SIZE bytes.
   #define IFL1_NUM_BLOCKS       32
   #define IFL1_BLOCK_SIZE       IFL_PAGE_SIZE
   #define IFL1_NUM_SEC          ((IFL1_BLOCK_SIZE / ZFS_SEC_SIZE) * IFL1_NUM_BLOCKS)
   #define IFL1_BASE_ADDR        MK_PTR_OFS_SEG( IFL_FLASH_SIZE_BYTES - (IFL1_NUM_BLOCKS * IFL1_BLOCK_SIZE), FLASH_ADDR_U_INIT_PARAM)
#else
   // No ZFS volumes are located in internal Flash
   #define IFL1_NUM_BLOCKS       0
   #define IFL1_BLOCK_SIZE       0
   #define IFL1_NUM_SEC          0
#endif



#ifdef ZFS_USES_RAM
   /*
    * The following routines implement the low-level 'Flash-driver' algorithms 
    * that can be used with RAM-based ZFS volumes. Since RAM volumes loose their
    * contents when power is lost, RAM volumes are typically only used for testing
    * purposes.
    */
   INT8
   ZFS_RAM_Init
   ( 
      void
   )
   {
      return( ZFL_ERR_SUCCESS );
   }

   INT8
   ZFS_RAM_Erase
   ( 
      HANDLE            hAddr
   )
   {
      extern UINT       g_max_volumes;
      ZFS_CONFIG_t    * pCfg = g_zfs_cfg;
      UINT8             i;


      for( i=0; i<g_max_volumes; i++, pCfg++ )
      {
         if( (hAddr >= pCfg->vol_addr.Ptr) && (hAddr < ((UINT8*)pCfg->vol_addr.Ptr + (pCfg->vol_blk_size * pCfg->vol_blks))) )
         {
            memset( (HANDLE)((UINT)hAddr & ~(pCfg->vol_blk_size -1)), 0xFF, pCfg->vol_blk_size );
            return( ZFL_ERR_SUCCESS );
         }
      }
      return( ZFL_ERR_ADDRESS );
   }

   INT8
   ZFS_RAM_ReadWrite
   ( 
      HANDLE            hDst,
      HANDLE            hSrc,
      UINT24            Len
   )
   {
      memcpy( hDst, hSrc, Len );
      return( ZFL_ERR_SUCCESS );
   }

   /*
    * Define the characteristics of ZFS volumes located in RAM here.
    *
    * Each volume must contain erase blocks of identical size that are an exact
    * multiple of ZFS_SEC_SIZE.  For proper operation of ZFS, use a block size
    * of at least 2x ZFS_SECSIZE.  File system efficiency can be increased by 
    * using a larger block size.  Each volume should contain at least 4 erase
    * blocks.  Do not include the memory range used by ZFS volumes located in 
    * RAM Flash in the ROM or RAM address space of the project.
    */

   // RAM Volume 1 is located in the first 4KB of eZ80F9x internal RAM and 
   // contains 8 erase blocks with each block containing ZFS_SEC_SIZE bytes.
   // This example is only intended for use with eZ80F9x devices.  It will be
   // necessary to modify the project settings to relocate the RAM volume.
   #define RAM1_NUM_BLOCKS       4
   #define RAM1_BLOCK_SIZE       1024
   #define RAM1_NUM_SEC          ((RAM1_BLOCK_SIZE / ZFS_SEC_SIZE) * RAM1_NUM_BLOCKS)
   #define RAM1_BASE_ADDR        MK_PTR_OFS_SEG( 0x00F000, RAM_ADDR_U_INIT_PARAM)
#else
   // No ZFS volumes are located in internal RAM
   #define RAM1_NUM_BLOCKS       0
   #define RAM1_BLOCK_SIZE       0
   #define RAM1_NUM_SEC          0
#endif



/*
 * ZFS Volume Configuration table
 *
 * Each bracketed entry corresponds to one ZFS volume.
 * The default configuration has a single volume located in external Flash.
 */
ZFS_CONFIG_t            g_zfs_cfg[] =
{
   #ifdef ZFS_USES_IFL
   {
      "INTF",                    // vol name
      ZFS_INT_FLASH_DEV_TYPE,    // vol type
      IFL1_BASE_ADDR,            // vol_addr
      IFL1_BLOCK_SIZE,           // vol_blk_size
      IFL1_NUM_BLOCKS,           // vol_blks

      ZFS_IFL_Init,
      IFL_PageErase,
      IFL_Program,
      IFL_Read
   },
   #endif

   #ifdef ZFS_USES_XFL
   {
      "EXTF",                    // vol name
      ZFS_EXT_FLASH_DEV_TYPE,    // vol type
      XFL1_BASE_ADDR,            // vol_addr
      XFL1_BLOCK_SIZE,           // vol_blk_size
      XFL1_NUM_BLOCKS,           // vol_blks

      #ifdef XFLD_API_VALID
         ZFS_XFL_Init,
         XFLD_Erase,
         XFLD_Program,
         XFLD_Read
      #else
         ZFS_XFL_Init,
         XFL_BlockErase,
         XFL_Program,
         XFL_Read
      #endif
   },
   #endif

   #ifdef ZFS_USES_RAM
   {
      "RAM",                     // vol name
      ZFS_RAM_DEV_TYPE,          // vol type
      RAM1_BASE_ADDR,            // vol_addr
      RAM1_BLOCK_SIZE,           // vol_blk_size
      RAM1_NUM_BLOCKS,           // vol_blks

      ZFS_RAM_Init,
      ZFS_RAM_Erase,
      ZFS_RAM_ReadWrite,
      ZFS_RAM_ReadWrite
   }
   #endif
};


/*******************************************************************************
 *             USE CAUTION WHEN MODIFYING THE FOLLOWING VALUES                 *
 *******************************************************************************

 Modifying the values below can cause the file system to fail to operate as 
 expected, or to fail to operate at all.


 ZFS_TOTAL_NUM_BLOCKS
 --------------------
 Total number of erase blocks in all ZFS volumes (Flash and RAM).

 ZFS_TOTAL_NUM_SECTORS
 ---------------------
 Total number of sectors present in all erase blocks in all volumes.  Each erase 
 block must be an integer multiple of ZFS_SEC_SIZE such that all erase blocks are
 at least twice the size of ZFS_SEC_SIZE.  This applies to Flash and RAM volumes.

 ZFS_TOTAL_NUM_VOLUMES
 ---------------------
 Total number of volumes present in the file system

 ZFS_MAX_FILE_OPEN_COUNT
 -----------------------
 Maximum number of files that can be open at the same time across all volumes. 
 This is a file system maximum and not a per-volume maximum.

 ZFS_MAX_DIRS_SUPPORTED
 ----------------------
 Maximum number of directories that can exist within the file system across all 
 volumes.  The minimum value must be at least ZFS_TOTAL_NUM_VOLUMES to allow 
 access the root directory of each volume.

 ERASE_FLASH
 -----------
 Global flag indicating whether Flash (external or internal) volumes are erased
 (i.e. reformatted) during initialization of the file system.  RAM volumes are
 always reformatted formatted every time the file system is initialized 
 regardless of the value of ERASE_FLASH
 0 - Persistent storage (Flash volumes not erased during initialization)
 1 - Format each volume during file system initialization

 */
#define ZFS_TOTAL_NUM_BLOCKS     ( XFL1_NUM_BLOCKS + IFL1_NUM_BLOCKS + RAM1_NUM_BLOCKS )
#define ZFS_TOTAL_NUM_SECTORS    ( XFL1_NUM_SEC    + IFL1_NUM_SEC    + RAM1_NUM_SEC )
#define ZFS_TOTAL_NUM_VOLUMES    (sizeof(g_zfs_cfg) / sizeof(ZFS_CONFIG_t))
#define ZFS_MAX_FILE_OPEN_COUNT  ( 20 )
#define ZFS_MAX_DIRS_SUPPORTED   ( 50 )
#define ERASE_FLASH              ( 0 )


// ********************************************
// DO NOT CHANGE ANYTHING BELOW IN THIS SECTION
// ********************************************
ZFS_OPEN_REC_CB_t       g_zfs_or[ ZFS_MAX_FILE_OPEN_COUNT ];
ZFS_VOL_INFO_CB_t       g_zfs_vol_info[ ZFS_TOTAL_NUM_VOLUMES ];
ZFS_DIR_LIST_CB_t       g_dir_list[ ZFS_MAX_DIRS_SUPPORTED ];
ZFS_BLK_INFO_CB_t       g_blk_info[ ZFS_TOTAL_NUM_BLOCKS ];
UINT8 *                 g_zfs_sat[ ZFS_TOTAL_NUM_SECTORS ];
UINT                    g_max_threads;
UINT                    g_max_cwd_len;
UINT                    g_max_or_entries        = ZFS_MAX_FILE_OPEN_COUNT;
UINT                    g_max_dirs_supported    = ZFS_MAX_DIRS_SUPPORTED;
UINT                    g_max_volumes           = ZFS_TOTAL_NUM_VOLUMES;
UINT                    g_max_flash_sectors     = ZFS_TOTAL_NUM_SECTORS;
UINT                    g_max_flash_blks        = ZFS_TOTAL_NUM_BLOCKS;
UINT8                   g_byEraseFlash          = ERASE_FLASH;

#endif // RZKFS
