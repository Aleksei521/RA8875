/*
 * File       :     fs_shell.h
 *
 * Description:		This file contains the prototypes of FS 
 *					Shell commands.
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
 */


/** ZTP Includes */ 
#include "shell.h"


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
	RZK_DEVICE_CB_t *				stdin,
	RZK_DEVICE_CB_t *				stdout, 
	RZK_DEVICE_CB_t *				stderr, 
	UINT16				nargs,
	INT8				*argv[]
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

extern
INT16 Shell_ZFSMoveFile (
	struct shvars *			Shl,
	RZK_DEVICE_CB_t *						stdin,
	RZK_DEVICE_CB_t *						stdout, 
	RZK_DEVICE_CB_t *						stderr, 
	UINT16					nargs,
	INT8					*argv[]
);

extern
INT16 Shell_ZFSCopyFile (
	struct shvars *			Shl,
	RZK_DEVICE_CB_t *						stdin,
	RZK_DEVICE_CB_t *						stdout, 
	RZK_DEVICE_CB_t *						stderr, 
	UINT16					nargs,
	INT8					*argv[]
);

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
void Shell_Add_FS_Commands( void );

