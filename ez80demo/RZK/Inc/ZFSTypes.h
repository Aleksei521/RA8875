/************************************************************************************
 * File       : ZFSTypes.h
 * Description: This file contains the data types for the ZiLOG File System
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

#ifndef _ZFSTYPES_H_
#define _ZFSTYPES_H_

#include "ZTypes.h"


// ZFS definitions
typedef UINT8	ZFS_SEC_TYPE_t ;
typedef INT		ZFS_STATUS_t ;
typedef UINT8	ZFS_CHECKSUM_t ;
typedef void *	ZFS_THD_HANDLE_t ;
typedef void * 	ZFS_HANDLE_t ;
typedef void *	ZFS_SEC_ID_t ;

#define ZFS_TRUE 	( UINT8 ) 0x01
#define ZFS_FALSE	( UINT8 ) 0x00 


#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef IN_OUT
#define IN_OUT
#endif

#ifndef NULL
#define NULL ( ( void * ) 0 )
#endif


///////////////////////////////////////////////////////
// macros that are used to pass values to APIs and
// support macros
///////////////////////////////////////////////////////

// different file types
#define ZFS_DIR_TYPE		((UINT8)(0x04))
#define ZFS_FILE_TYPE		((UINT8)(0x08))

// file related
#define ZFS_FILE_BEGIN		((UINT8)(0x02))
#define ZFS_FILE_CURRENT	((UINT8)(0x00))
#define ZFS_FILE_END		((UINT8)(0x01))

// file modes
#define ZFS_READ			((UINT8)(0x01))
#define ZFS_WRITE			((UINT8)(0x02))
#define ZFS_APPEND			((UINT8)(0x04))
#define ZFS_READ_WRITE		((UINT8)(0x08))

#define ZFS_MODE_ASCII		((UINT8)(0x00))
#define ZFS_MODE_BINARY		((UINT8)(0x01))

///////////////////////////////////////////////////////


///////////////////////////////////////////////////////
// macros that are used to for different volume types
// as present in config table
///////////////////////////////////////////////////////

// different volume types
#define ZFS_RAM_DEV_TYPE			(0x00)
#define ZFS_EXT_FLASH_DEV_TYPE		(0x01)
#define ZFS_INT_FLASH_DEV_TYPE		(0x02)

///////////////////////////////////////////////////////



///////////////////////////////////////////////////////
// Macros that could be converted/removed to support
// variable size of blocks
///////////////////////////////////////////////////////
#define ZFS_SEC_SIZE					(512)
#define ZFS_MAX_FILE_NAME_SIZE			(16)

///////////////////////////////////////////////////////



///////////////////////////////////////////////////////
// error numbers returned by the ZFS APIs
///////////////////////////////////////////////////////

#define ZFSERR_SUCCESS						(-0)
#define ZFSERR_INVALID_HANDLE				(-1)
#define ZFSERR_INVALID_ARGUMENTS			(-2)
#define ZFSERR_NOT_INITIALIZED				(-3)
#define ZFSERR_INVALID_FILEDIR_PATH			(-4)
#define ZFSERR_INVALID_OPERATION			(-5)
#define ZFSERR_DIRECTORY_NOT_EMPTY			(-6)
#define ZFSERR_INVALID_FILE_DIR_NAME		(-7)
#define ZFSERR_FILE_DIR_ALREADY_EXISTS		(-8)
#define ZFSERR_FILE_DIR_COUNT_LIMIT_REACHED	(-9)
#define ZFSERR_DIR_COUNT_LIMIT_REACHED		(-10)
#define ZFSERR_DATAMEDIA_FULL				(-11)
#define ZFSERR_INTERNAL						(-12)
#define ZFSERR_FILE_DIR_DOES_NOT_EXIST		(-13)
#define ZFSERR_FILE_DIR_IN_USE				(-14)
#define ZFSERR_INVALID_OFFSET_RANGE			(-15)
#define ZFSERR_FILE_IS_ALREADY_OPEN			(-16)
#define ZFSERR_MAX_FILE_OPEN_COUNT_REACHED	(-17)
#define ZFSERR_DEVICE						(-18)
#define ZFSERR_INVALID_VOLUME_NAME			(-19)
#define ZFSERR_VOLUME_IS_IN_USE				(-20)
#define ZFSERR_ALREADY_SHUTDOWN				(-21)
#define ZFSERR_FS_BUSY						(-22)
#define ZFSERR_ALREADY_INITIALIZED			(-23)
#define ZFSERR_CWD_PATH_LENGTH_MORE			(-24)
#define ZFSERR_INVALID_VOLUME					(-25)

///////////////////////////////////////////////////////



///////////////////////////////////////////////////////
// Different structure sizes used by File System
///////////////////////////////////////////////////////

#ifdef _IAR_CODE
#define ZFS_BLK_INFO_STR_SIZE		( 31 )
#define ZFS_VOL_INFO_STR_SIZE		( 23 )
#define ZFS_OPEN_REC_STR_SIZE		( 54 )
#define ZFS_DIR_LIST_STR_SIZE		( 40 )
#else
   #ifdef RZK_ZNEO
      #define ZFS_BLK_INFO_STR_SIZE		( 40 )
      #define ZFS_VOL_INFO_STR_SIZE		( 34 )
      #define ZFS_OPEN_REC_STR_SIZE		( 62 )
      #define ZFS_DIR_LIST_STR_SIZE		( 48 )
   #else
      #define ZFS_OPEN_REC_STR_SIZE		( 54 )
      #define ZFS_VOL_INFO_STR_SIZE		( 26 )
      #define ZFS_DIR_LIST_STR_SIZE		( 40 )
      #define ZFS_BLK_INFO_STR_SIZE		( 34 )
   #endif
#endif //_IAR_CODE

///////////////////////////////////////////////////////


#endif //_ZFS_TYPES_H_


