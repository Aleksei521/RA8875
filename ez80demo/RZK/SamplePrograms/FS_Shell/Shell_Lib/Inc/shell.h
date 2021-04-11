/************************************************************************************
 * File       :     shell.h
 *
 * Description:		This file contains the shell related structure defintions and constants
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
/* shell.h */
#ifndef _SHELL_H_
#define _SHELL_H_

//////////////////////////////////////////////////////////////
// added for RZK Shell
//////////////////////////////////////////////////////////////
#define INT16		INT16
#define INT16		INT16
//#define SYSERR 		(-1)
#define static		static
#define TRUE		1
#define	FALSE		0

#ifndef NULL
#define NULL (void *) 				0
#endif

#ifndef OK
#define	OK		1
#endif

#define SHELL_STACK_SIZE		2048
#define SHELL_PRIORITY1			10
#define SHELL_RR_TICKS			10


#ifndef ZTP_EOF
#define ZTP_EOF	-2
#endif

typedef RZK_THREADHANDLE_t 	PID;
//typedef	RZK_DEVICE_CB_t *	RZK_DEVICE_CB_t * ;
//typedef void * 				void *;


//////////////////////////////////////////////////////////////
// end of RZK Shell
//////////////////////////////////////////////////////////////


/* Declarations for the Xinu shell */

#define	SHEXIT		-5		/* shell exit return		*/
#define	SHNAMLEN	9		/* length of user name + 1	*/
#define	SHBUFLEN	80		/* length of general buffer	*/
#define	SHARGLEN	82		/* length of area for arguments	*/
#define	SHMAXTOK	16		/* maximum tokens per line	*/
#define	SHMLEN		64		/* maximum length of mach. name	*/
#define	SHCMDSTK	8192		/* size of command proc. stack	*/
#define	SHCMDPRI	50		/* command process priority	*/

// MCB L92 #define SHLLSTK		2048
#define SHLLSTK		6000
#define SHLLPRI		11          //AJ-20
#define SHLLNAM		"SHL"

struct	shvars	{				/* shell variables		*/
	UINT32 shlast;		/* time shell last touched	*/
	UINT32 shlogon;		/* time user logged on		*/
	UINT8	shused;					/* TRUE => user using shell	*/
	INT8	shuser[SHNAMLEN];		/* current user name		*/
	INT8	shmach[SHMLEN];		/* name of this machine		*/
	INT8	shbuf[SHBUFLEN];		/* general purpose buffer	*/
	INT8	*shtok[SHMAXTOK];		/* pointers to input tokens	*/
	INT8	shtktyp[SHMAXTOK];	/* type of token in shtok[i]	*/
	INT8	shargst[SHARGLEN];	/* actual strings of arguments	*/
};


typedef INT16 (* SHELL_CMD)(struct shvars *, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[] );

struct	cmdent	{			/* entry in command table	*/
	/** A pointer to the name of the command.
	 * @type char *
	 */
	INT8	*cmdnam;
	/** A flag indicating wether the shell should create a new thread
	 * for this command or run it directly (ie builtin). Currently, only
	 * builtin commands are supported, so this should always be set to TRUE.
	 * @type unsigned char
	 */
	UINT8	cbuiltin;		/* Is this a builtin command?	*/
	/** A pointer to the function that implements the command.
	 */																				   
   SHELL_CMD cproc;
	/** A pointer to the next command in the list */
    struct cmdent	*cnext;

	/** Meenakshi added newly to have help description for each command */
	INT8	*helpDesc ;
};




/* lexan.c */
extern INT16 lexan(struct shvars *Shl, INT8 *line);

/* shell.c */
extern INT16 shell(RZK_DEVICE_CB_t * dev);
extern struct cmdent *ShellCommands;
extern void shell_add_commands(struct cmdent *cmds, UINT16 ncmds,UINT8 commandType);
extern INT16 shell_init(RZK_DEVICE_CB_t * dev);

/* shell_conf.c */
extern struct cmdent	defaultcmds[];
extern UINT16	ndefaultcmds;


/** MEENAKSHI ADDED NEWLY */
#define SHELL_ZTP 		( 1 )
#define SHELL_TELNET 	( 2 )
#define SHELL_FTP 		( 3 )

#define ZSHELL_SUCCESS  ( 0 )
#define ZSHELL_FAILURE  ( -1 )

#define ZSHELL_FALSE  ( 0 )
#define ZSHELL_TRUE 	( 1 )

extern UINT8 	g_commandType ;
extern struct cmdent	*g_telnetShellCommands;


#endif
