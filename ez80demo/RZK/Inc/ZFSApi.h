/************************************************************************************
 * File       : zfsapi.h
 * Description: This file contains the api prototypes provided in ZiLOG File System
 * Author     : 
 * Created on : 21-APR-2004
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

#ifndef _ZFSAPI_H_
#define _ZFSAPI_H_


#include "zfstypes.h"

// structures used by the API's 

// file/directory information for LIST API
typedef struct {
	INT8				fd_name[ ZFS_MAX_FILE_NAME_SIZE + 1 ] ;
	UINT8				fd_type ;  /*DIRECTORY or FILE */
	UINT32				fd_size ; /* size of the file */
	UINT8				fd_sec ;
	UINT8				fd_min ;
	UINT8				fd_hrs ;
	UINT8				fd_day ;
	UINT8				fd_mon ;
	UINT8				fd_year ;
	UINT8				fd_century ;
#ifdef RZK_ZNEO
	UINT8                           dummy;
#endif
} ZFS_FD_LIST_t ;

// volume information
typedef struct {
	UINT8	vol_name[ ZFS_MAX_FILE_NAME_SIZE + 1 ] ;
	UINT8 	is_valid ;
	UINT32	vol_size ;
	UINT32	free_space ;
	UINT32	used_space ;
	UINT32	dirty_space ;
} ZFS_VOL_PARAMS_t, *PZFS_VOL_PARAMS_t ;

//Function Name: ZFSChdir
//Decription: This API changes the current working directory to a new directory provided in 
// the arguments
ZFS_STATUS_t ZFSChdir( IN INT8 *dir ) ;

//Function Name: ZFSClose
//Description: This API closes an opened file
ZFS_STATUS_t ZFSClose( IN ZFS_HANDLE_t handle ) ;

//Function Name: ZFSDelete
//Description: This API deletes a file
ZFS_STATUS_t ZFSDelete( IN INT8 *file_name ) ;

//Function Name: ZFSDeleteDir
//Description: This API deletes a empty directory or directory including all subdirectories and files present
// within
ZFS_STATUS_t ZFSDeleteDir( IN INT8 *dir_name, IN UINT8 del_all ) ;

//Function Name: ZFSFormat
//Description: This API formats the appropriate volume.
ZFS_STATUS_t ZFSFormat( IN INT8 *volname ) ;

//Function Name: ZFSGetCwd
//Description: This API gets the current working directory for the thread
ZFS_STATUS_t ZFSGetCwd( IN_OUT INT8 *pcwd_path ) ;

//Function Name: ZFSGetCwdLen
//Description: This API gets the length of the current working directory in absolute path
INT ZFSGetCwdLen( void ) ;

//Function Name: ZFSGetDirFileCount
//Description: This API will return the number of files/directories present in the given directory path
INT32 ZFSGetDirFileCount( IN INT8 *dir_path ) ;

//Function Name: ZFSGetErrNum
//Description: This API will get the error number stored in the current executing thread.
ZFS_STATUS_t ZFSGetErrNum( void ) ;

//Function Name: ZFSGetVolumeCount
//Description: This API returns the Number of volumes present
ZFS_STATUS_t ZFSGetVolumeCount( void ) ;

//Function Name: ZFSGetVolumeParams
//Description: This API returns the volume parameters for user information
ZFS_STATUS_t ZFSGetVolumeParams( IN INT8 *vol_name, ZFS_VOL_PARAMS_t *vol_params, UINT8 get_all ) ;

//Function Name: ZFSInit
//Description: This API initializes the file system
ZFS_STATUS_t ZFSInit( PZFS_VOL_PARAMS_t pvol_params ) ;

//Function Name: ZFSList
//Description: This API will return the files and directories present within a directory. This will
// return 8 files at a time.
ZFS_STATUS_t ZFSList( IN INT8 * path, IN_OUT ZFS_FD_LIST_t * list, IN UINT8 startCnt  ) ;

//Function Name: ZFSMkdir
//Description: This API creates a directory in a given path
ZFS_STATUS_t ZFSMkdir( IN INT8 *path, IN INT8 *dirname ) ;

//Function Name: ZFSOpen
//Description: This API will open a file for reading/writing
ZFS_HANDLE_t ZFSOpen( IN INT8 *filename, IN UINT8 mode, IN UINT8 type ) ;

//Function Name: ZFSRead
//Description: This API will read the specified number of bytes from the file
INT32 ZFSRead( IN ZFS_HANDLE_t, IN_OUT UINT8 *buf, IN UINT bytes ) ;

//Function Name: ZFSRename
//Description: This API renames a existing file
ZFS_STATUS_t ZFSRename( IN INT8 *src_file_path, IN INT8 *dst_file_name ) ;

//Function Name: ZFSRenameDir
//Description: This API renames a existing directory
ZFS_STATUS_t ZFSRenameDir( IN INT8 *src_dir_path, IN INT8* dst_dir_name ) ;

//Function Name: ZFSSeek
//Description: This API will change the offset in the file
ZFS_STATUS_t ZFSSeek( IN ZFS_HANDLE_t handle, IN INT32 offset, IN INT8 origin ) ;

//Function Name: ZFSShutdown
//Decription: This API shuts down the ZFS.
// the arguments
ZFS_STATUS_t ZFSShutdown( void ) ;

//Function Name: ZFSWrite
//Description: This API will write the data onto the file
INT32 ZFSWrite( IN ZFS_HANDLE_t handle, IN UINT8 *buf, IN UINT bytes ) ;

#endif //_ZFSAPI_H_


