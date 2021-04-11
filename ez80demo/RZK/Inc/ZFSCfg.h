/************************************************************************************************
** File:        ZFSCfg.h
** Description: Zilog File System (ZFS) configuration header file.
**
** Copyright 2016 Zilog Inc. ALL RIGHTS RESERVED.
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

#ifndef _ZFSCFG_H_
#define _ZFSCFG_H_

#include "FlashLib.h"
#include "PTR_OFS_SEG.h"


//#define ZFS_LEGACY_RAM

// maximum volume name length
#define ZFS_MAX_VOL_NAME_LEN           ( 16 )

/*
 * Zilog File System (ZFS) function pointers reference routines to manipulate 
 * external or internal Flash (only available on eZ80F9x devices)
 */
typedef INT8 (* FP_ZFL_INIT)(    void );
typedef INT8 (* FP_ZFL_ERASE)(   HANDLE hAddr );
typedef INT8 (* FP_ZFL_PROGRAM)( HANDLE hDst, HANDLE hSrc, UINT24 Len );
typedef INT8 (* FP_ZFL_READ)(    HANDLE hDst, HANDLE hSrc, UINT24 Len );

/*
 * Zilog File System (ZFS) configuration structure.  ZFS provides routines to
 * create/ read/ write files stored within a 'volume' of storage (typically 
 * Flash).  The ZFS_CONFIG_t structure defines the location of the volume in 
 * memory and the set of low-level Zilog Flash Library (ZFL) routines (or ZFS
 * RAM routines) used to manipulate data within the volume.
 *
 * When the ZFS volume is located in external Flash, the application developer
 * must ensure the volume start address (vol_addr) corresponds to the first byte
 * of Flash in an erase block.  ZDS requires that all erase block in the volume 
 * to have the same size.
 */
typedef struct          ZFS_CONFIG_s
{
   char                 vol_name[ ZFS_MAX_VOL_NAME_LEN + 1];
   UINT8                vol_type;      // Must be one of: ZFS_VOL_RAM, ZFS_VOL_INTFLASH, ZFS_VOL_EXTFLASH
   PTR_OFS_SEG          vol_addr;      // Starting address of the volume
   UINT                 vol_blk_size;  // Size of each erase block in the volume
   UINT                 vol_blks;      // Number of blocks in the volume (use 1 for RAM)

   FP_ZFL_INIT          fpInit;
   FP_ZFL_ERASE         fpErase;
   FP_ZFL_PROGRAM       fpProgram;
   FP_ZFL_READ          fpRead;
} ZFS_CONFIG_t, *PZFS_CONFIG_t;


/*
 * Global Variables
 */
extern ZFS_CONFIG_t     g_zfs_cfg[ ];


/*
 * Function prototypes
 */

#endif // _ZFSCFG_H_
