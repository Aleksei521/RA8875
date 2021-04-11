/************************************************************************************
 * File       : zfstruct.h
 * Description: This file contains the structures required by the ZiLOG File System
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

#ifndef _ZFSSTRUCT_H_
#define _ZFSSTRUCT_H_

//include header files
#include "zfstypes.h"

/////////////////////////////////////////////////////
// different structure sizes
/////////////////////////////////////////////////////

typedef struct {
	UINT8 CB[ ZFS_BLK_INFO_STR_SIZE ] ;
} ZFS_BLK_INFO_CB_t ;

typedef struct {
	UINT8 CB[ ZFS_VOL_INFO_STR_SIZE ] ;
} ZFS_VOL_INFO_CB_t ;

typedef struct {
	UINT8 CB[ ZFS_OPEN_REC_STR_SIZE ] ;
} ZFS_OPEN_REC_CB_t ;

typedef struct {
	UINT8 CB[ ZFS_DIR_LIST_STR_SIZE ] ;
} ZFS_DIR_LIST_CB_t ;


/////////////////////////////////////////////////////

#endif //_ZFSSTRUCT_H_


