/************************************************************************************
 * File       :     shell_conf.c
 *
 * Description:		This file contains the configuration details for the
 *					shell.
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
 /************************************************************************************
 * Copyright 2001, Metro Link, Inc.
 * All Rights Reserved
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Metro Link, Inc. and may
 * contain proprietary, confidential and trade secret information of
 * Metro Link, Inc. and/or its partners.
 *  
 * The contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of Metro Link, Inc.
 *
 ************************************************************************************/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZDevice.h"
#include "ZThread.h"
#include "shell.h"
#include "fs_shell.h"

/**
 * @name shell_conf.c
 * @memo Default Debug Shell Commands
 * @doc These are default settings used by the Debug Shell. This file should
 * be included in a project, and modified as needed.
 */

/*@{*/
/**
 * @doc A default set of all non-network related commands.  These commands are
 * made available by default - there is no need to call shell_add_commands()
 * to make them available.
 */
struct	cmdent	defaultcmds[]  = {
	{ "cd",		TRUE,		(SHELL_CMD)Shell_ZFSChdir,		(struct cmdent*)NULL, "Changes the current working directory\n" },
	{ "copy",	TRUE,		(SHELL_CMD)Shell_ZFSCopyMoveFile,	(struct cmdent*)NULL, "Copies a file from current directory to the directory provided\n" },
	{ "create",	TRUE,		(SHELL_CMD)Shell_ZFSCreateFile,		(struct cmdent*)NULL, "Creates a file\n" },
	{ "cwd",	TRUE,		(SHELL_CMD)Shell_ZFSGetCWD,			(struct cmdent*)NULL, "Prints the current working directory on the console\n" },
	{ "del",	TRUE,		(SHELL_CMD)Shell_ZFSDelFile,		(struct cmdent*)NULL, "Deletes a file\n" },
	{ "deltree",TRUE,		(SHELL_CMD)Shell_ZFSDelDirAll,		(struct cmdent*)NULL, "Deletes all files and sub-directories present in the given directory\n" },
	{ "deldir",	TRUE,		(SHELL_CMD)Shell_ZFSDelDir,			(struct cmdent*)NULL, "Deletes an empty directory\n" },
	{ "dir",	TRUE,		(SHELL_CMD)Shell_ZFSListFileDirs,	(struct cmdent*)NULL, "Displays the files/directories present in the current directory\n" },
	{ "format", TRUE,		(SHELL_CMD)Shell_ZFSFormat,  		(struct cmdent*)NULL, "Formats the volume with Zilog File System native format\n" },
	{ "gettime",TRUE,		(SHELL_CMD)Shell_getdatetime,		(struct cmdent*)NULL, "The gettime shell command prints the current date and time to a standard output\n" },
	{ "help",	TRUE,		(SHELL_CMD)Shell_ZFSHelp,			(struct cmdent*)NULL, "The ? shell command without any argument displays the set of commands that can be executed from the shell's command prompt. When ? is followed by a command name it prints help for that specific command\n" },
	{ "md",		TRUE,		(SHELL_CMD)Shell_ZFSMkdir,			(struct cmdent*)NULL, "Creates a directory in the specified path\n" },
	{ "move",	TRUE,		(SHELL_CMD)Shell_ZFSCopyMoveFile,	(struct cmdent*)NULL, "Moves a file from current directory to the directory provided\n" },
	{ "ren",	TRUE,		(SHELL_CMD)Shell_ZFSRenFile,		(struct cmdent*)NULL, "Renames a file to new name provided\n" },
	{ "rendir",	TRUE,		(SHELL_CMD)Shell_ZFSRenDir,			(struct cmdent*)NULL, "Renames the directory to new name provided\n" },
	{ "settime",TRUE,		(SHELL_CMD)Shell_setdatetime,		(struct cmdent*)NULL, "The settime shell command sets the current date and time to a standard outputsets current date and time\n" },
	{ "type",	TRUE,		(SHELL_CMD)Shell_ZFSDispFile,		(struct cmdent*)NULL, "Displays the contents of the file onto the console\n" },
	{ "vol",	TRUE,		(SHELL_CMD)Shell_ZFSGetVolumeInfo,	(struct cmdent*)NULL, "Displays the volume information on the console\n" },
	{ "?",		TRUE,		(SHELL_CMD)Shell_ZFSHelp,			(struct cmdent*)NULL, "The ? shell command without any argument displays the set of commands that can be executed from the shell's command prompt. When ? is followed by a command name it prints help for that specific command\n" },
};	/* shell commands */

/**
 * @doc The number of entries in the defaultcmds[] array.  If the defaultcmds
 * array is modified, ndefaultcmds must be adjusted accordingly.
 */
UINT16 ndefaultcmds=sizeof(defaultcmds)/sizeof(struct cmdent);
/*@}*/

