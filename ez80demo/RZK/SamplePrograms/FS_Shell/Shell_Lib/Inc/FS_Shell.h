/************************************************************************************
 * File       :     fs_shell.h
 *
 * Description:		This file contains the prototypes of FS 
 *					Shell commands.
 *
 * Copyright 2004 ZiLOG Inc.  ALL RIGHTS RESERVED.
 *
 * This file contains unpublished confidential and proprietary information
 * of ZiLOG, Inc.
 * NO PART OF THIS WORK MAY BE DUPLICATED, STORED, PUBLISHED OR DISCLOSED 
 * IN ANY FORM WITHOUT THE PRIOR WRITTEN CONSENT OF ZiLOG, INC.
 * This is not a license and no use of any kind of this work is authorized
 * in the absence of a written license granted by ZiLOG, Inc. in ZiLOG's 
 * sole discretion 
 ************************************************************************************/


#ifndef _FS_SHELL_H_
#define _FS_SHELL_H_

extern
INT16 Shell_ZFSMkdir (
	struct shvars *			Shl,
	RZK_DEVICE_CB_t *						stdin,
	RZK_DEVICE_CB_t *						stdout, 
	RZK_DEVICE_CB_t *						stderr, 
	UINT16					nargs,
	INT8					*argv[]
);

extern
INT16 Shell_ZFSChdir (
	struct shvars *			Shl,
	RZK_DEVICE_CB_t *						stdin,
	RZK_DEVICE_CB_t *						stdout, 
	RZK_DEVICE_CB_t *						stderr, 
	UINT16					nargs,
	INT8					*argv[]
);
extern
INT16 Shell_ZFSDelFile (
	struct shvars *			Shl,
	RZK_DEVICE_CB_t *						stdin,
	RZK_DEVICE_CB_t *						stdout, 
	RZK_DEVICE_CB_t *						stderr, 
	UINT16					nargs,
	INT8					*argv[]
);

extern
INT16 Shell_ZFSDelDirAll (
	struct shvars *			Shl,
	RZK_DEVICE_CB_t *						stdin,
	RZK_DEVICE_CB_t *						stdout, 
	RZK_DEVICE_CB_t *						stderr, 
	UINT16					nargs,
	INT8					*argv[]
);

extern
INT16 Shell_ZFSDelDir (
	struct shvars *			Shl,
	RZK_DEVICE_CB_t *						stdin,
	RZK_DEVICE_CB_t *						stdout, 
	RZK_DEVICE_CB_t *						stderr, 
	UINT16					nargs,
	INT8					*argv[]
);

extern
INT16 Shell_ZFSRenFile (
	struct shvars *			Shl,
	RZK_DEVICE_CB_t *						stdin,
	RZK_DEVICE_CB_t *						stdout, 
	RZK_DEVICE_CB_t *						stderr, 
	UINT16					nargs,
	INT8					*argv[]
);

extern
INT16 Shell_ZFSRenDir (
	struct shvars *			Shl,
	RZK_DEVICE_CB_t *						stdin,
	RZK_DEVICE_CB_t *						stdout, 
	RZK_DEVICE_CB_t *						stderr, 
	UINT16					nargs,
	INT8					*argv[]
);

extern INT16 Shell_ZFSCopyMoveFile (
	struct shvars *			Shl,
	RZK_DEVICE_CB_t *						stdin,
	RZK_DEVICE_CB_t *						stdout, 
	RZK_DEVICE_CB_t *						stderr, 
	UINT16					nargs,
	INT8					      *argv[]
) ;

extern
INT16 Shell_ZFSGetCWD (
	struct shvars *			Shl,
	RZK_DEVICE_CB_t *						stdin,
	RZK_DEVICE_CB_t *						stdout, 
	RZK_DEVICE_CB_t *						stderr, 
	UINT16					nargs,
	INT8					*argv[]
);

extern
INT16 Shell_ZFSListFileDirs (
	struct shvars *			Shl,
	RZK_DEVICE_CB_t *						stdin,
	RZK_DEVICE_CB_t *						stdout, 
	RZK_DEVICE_CB_t *						stderr, 
	UINT16					nargs,
	INT8					*argv[]
);

extern
INT16 Shell_ZFSDispFile (
	struct shvars *			Shl,
	RZK_DEVICE_CB_t *						stdin,
	RZK_DEVICE_CB_t *						stdout, 
	RZK_DEVICE_CB_t *						stderr, 
	UINT16					nargs,
	INT8					*argv[]
);

extern
INT16 Shell_ZFSGetVolumeInfo (
	struct shvars *			Shl,
	RZK_DEVICE_CB_t *						stdin,
	RZK_DEVICE_CB_t *						stdout, 
	RZK_DEVICE_CB_t *						stderr, 
	UINT16					nargs,
	INT8					*argv[]
);

INT16	Shell_ZFSHelp
(
  	struct shvars *Shl,
	RZK_DEVICE_CB_t *								stdin,
	RZK_DEVICE_CB_t *								stdout, 
	RZK_DEVICE_CB_t *								stderr, 
	UINT16							nargs,
   	INT8	*                    				args[]
);

extern INT16 Shell_getdatetime
(
	struct shvars *				Shl,
	RZK_DEVICE_CB_t *								stdin,
	RZK_DEVICE_CB_t *								stdout, 
	RZK_DEVICE_CB_t *								stderr, 
	UINT16								nargs,
	INT8 *							args[]
) ;

extern INT16 Shell_setdatetime
(
	struct shvars *				Shl,
	RZK_DEVICE_CB_t *								stdin,
	RZK_DEVICE_CB_t *								stdout, 
	RZK_DEVICE_CB_t *								stderr, 
	UINT16								nargs,
	INT8 *							args[]
) ;


extern INT16 Shell_ZFSCreateFile (
	struct shvars *			Shl,
	RZK_DEVICE_CB_t *						stdin,
	RZK_DEVICE_CB_t *						stdout, 
	RZK_DEVICE_CB_t *						stderr, 
	UINT16					nargs,
	INT8					*argv[]
) ;

extern INT16 Shell_ZFSFormat (
	struct shvars *			Shl,
	RZK_DEVICE_CB_t *						stdin,
	RZK_DEVICE_CB_t *						stdout, 
	RZK_DEVICE_CB_t *						stderr, 
	UINT16					nargs,
	INT8					*argv[]
) ;

#endif //_FS_SHELL_H_
