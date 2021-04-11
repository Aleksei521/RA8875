/************************************************************************************************
** File:        DataPersistence.h
** Description: Data persistence header file.
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

#ifndef _DATA_PERSISTENCE_H_
#define _DATA_PERSISTENCE_H_

#include "FlashLib.h"
#include "PTR_OFS_SEG.h"


/*
 * The data persistence module is used to store configuration parameters in non-
 * volatile storage independently of the application program.  This allows the 
 * user to modify parameters at run-time without having to reprogram Flash with
 * the entire program image.
 *
 * The data persistence module can be configured to use internal or external
 * Flash to store parameters.  The eZ80F9x Flash information page is an ideal
 * location for the persistent data to be stored and is used by default on all
 * eZ80F9x devices.  To ensure compatibility with all members of the eZ80F9x 
 * series, the default size of the non-volatile (persistent) block is 256 bytes
 * (matching the size of the DATA_PER_t structure).  If internal or external 
 * Flash is used for data persistence, then the region used must not conflict 
 * with the program's 'ROM address space' or the area of Flash used by ZFS.
 */

typedef struct          DATA_PER_s
{
   /*
    * ZTP reserved area of data persistence for storing static IP parameters.
    * (These parameters are only used if c_IsDhcpEnabled is 0).
    */
   UINT8                c_EmacAddr[ 6 ];
   UINT8                c_IsDhcpEnabled;
   UINT32               ul_IPAddr;
   UINT32               ul_NetMask;
   UINT32               ul_GateWay;
#if 0
   /*
    * The following structure members are only used with the WLAN driver.
    * Applications that do not use the WLAN driver can use these fields for
    * storing their own purposes.
    */
   UINT8                c_ssid[ NDIS_802_11_LENGTH_SSID +1 ];
   UINT8                c_wepKey[ WEP_KEYS ][ WEP_KEY_LEN ];   
   UINT8                c_wepKeyLen;
   UINT8                WepKeyIndex;
   UINT8                c_PSK[ PMK_SIZE_BYTES ];
   /*
    * The userdata area is not used by ZTP.  Applications are free to use this 
    * area for storing parameters.
    */
   UINT8                c_userdata[ 118 ];
#else
   UINT8                c_userdata[ 237 ];
#endif
} DATA_PER_t, *PDATA_PER_t;

#define DATA_PER_BLOCK_SIZE   sizeof(DATA_PER_t)



/*
 * When using eZ80F9x internal Flash or the information page for data persistence
 * the value of the Flash upper address linker symbol (set in project settings)
 * determines where parameters are stored.
 */
extern UINT8 _FLASH_ADDR_U_INIT_PARAM;
#define FLASH_ADDR_U_INIT_PARAM        ((UINT8)&_FLASH_ADDR_U_INIT_PARAM)

/*
 * When using external Flash for data persistence, the value of the chip select 0
 * upper bound linker symbol (set in project settings) is used to determine where
 * parameters are stored.
 */
extern UINT8 _CS0_UBR_INIT_PARAM;
#define CS0_UBR_INIT_PARAM             ((UINT8)&_CS0_UBR_INIT_PARAM)



/*
 * Data persistence function pointers reference routines to manipulate external
 * or internal Flash (or the information page).
 */
typedef INT8 (* FP_ZFL_INIT)(    void );
typedef INT8 (* FP_ZFL_ERASE)(   HANDLE hAddr );
typedef INT8 (* FP_ZFL_PROGRAM)( HANDLE hDst, HANDLE hSrc, UINT24 Len );
typedef INT8 (* FP_ZFL_READ)(    HANDLE hDst, HANDLE hSrc, UINT24 Len );

/*
 * Data persistence configuration structure.  The hBaseAddr parameter is the 
 * memory location used to access persistent data storage.  The size of the data
 * persistent data block is DATA_PER_BLOCK_SIZE, but when stored in internal or
 * external Flash, erasing the data persistence parameters will erase the entire
 * block of LFash which could be several KB long.  Therefore, it is recommended 
 * to always use the eZ80F9x information page for data persistence.
 */
typedef struct          DATA_PER_CFG_s
{
   PTR_OFS_SEG          hBaseAddr;
   FP_ZFL_INIT          fpInit;
   FP_ZFL_ERASE         fpErase;
   FP_ZFL_PROGRAM       fpProgram;
   FP_ZFL_READ          fpRead;
} DATA_PER_CFG;


/*
 * Global Variables
 */
extern DATA_PER_CFG     DataPerCfg;


/*
 * Function prototypes
 */
extern INT16 Init_DataPersistence( void );
extern INT8  GetDataPersistence( PDATA_PER_t p_data_per );
extern INT8  SetDataPersistence( PDATA_PER_t p_data_per );


#endif
