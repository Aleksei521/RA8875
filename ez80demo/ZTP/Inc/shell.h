/*
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
 */
 /*
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
 */

/* shell.h */
#ifndef _SHELL_H_
#define _SHELL_H_
#include "ztypes.h"
#include "zdevice.h"
#include "xc_lib.h"
/* Declarations for the Xinu shell */

#define	SHEXIT		-5		/* shell exit return		*/
#define	SHNAMLEN	9		/* length of user name + 1	*/
#define	SHBUFLEN	128		/* length of general buffer	*/
#define	SHARGLEN	130		/* length of area for arguments	*/
#define	SHMAXTOK	16		/* maximum tokens per line	*/
#define	SHMLEN		64		/* maximum length of mach. name	*/
#define	SHCMDSTK	6196		/* size of command proc. stack	*/
#define	SHCMDPRI	50		/* command process priority	*/

// MCB L92 #define SHLLSTK		2048
//#define SHLLSTK		6000
#define SHLLSTK		4096 // MK $ CR XXXX $ 10-JAN-2006 $ 4K is enough
#define SHLLPRI		15	/*AJ $ CR#5167 $ 21FEB05$ shift the band , 20*/
#define SHLLNAM		"SHL"

struct	shvars	{				/* shell variables		*/
	UINT32 shlast;		/* time shell last touched	*/
	UINT32 shlogon;		/* time user logged on		*/
	BOOL	shused;					/* TRUE => user using shell	*/
	INT8		shuser[SHNAMLEN];		/* current user name		*/
	INT8 	shmach[SHMLEN];		/* name of this machine		*/
	INT8		shbuf[SHBUFLEN];		/* general purpose buffer	*/
	INT8		*shtok[SHMAXTOK];		/* pointers to input tokens	*/
	INT8		shtktyp[SHMAXTOK];	/* type of token in shtok[i]	*/
	INT8		shargst[SHARGLEN];	/* actual strings of arguments	*/
};


typedef INT16 (* SHELL_CMD)(struct shvars *, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
struct	cmdent	{			/* entry in command table	*/
	/** A pointer to the name of the command.
	 * @type char *
	 */
	INT8	*cmdnam;
	/** A flag indicating wether the shell should create a new thread
	 * for this command or run it directly (ie builtin). Currently, only
	 * builtin commands are supported, so this should always be set to TRUE.
	 * @type Bool
	 */
	UINT8	cbuiltin;		/* Is this a builtin command?	*/
	/** A pointer to the function that implements the command.
	 */																				   
   SHELL_CMD cproc;
	/** A pointer to the next command in the list */
    struct cmdent	*cnext;

	/** Meenakshi added newly to have help description for each command */
	INT8 	*helpDesc ;
};

#ifndef ZTP_EOF
#define ZTP_EOF	-2
#endif

#ifndef TIMEOUT
#define TIMEOUT	-3
#endif

#ifndef INTRMSG
#define INTRMSG	-4
#endif

#define	IP_CLASSA(x)	(((x) & 0x80) == 0x00)	/* IP Class A address	*/
#define	IP_CLASSB(x)	(((x) & 0xc0) == 0x80)	/* IP Class B address	*/
#define	IP_CLASSC(x)	(((x) & 0xe0) == 0xc0)	/* IP Class C address	*/
#define	IP_CLASSD(x)	(((x) & 0xf0) == 0xe0)	/* IP Class D address	*/
#define	IP_CLASSE(x)	(((x) & 0xf8) == 0xf0)	/* IP Class E address	*/
extern INT16 x_keyIndex(	struct shvars *Shl, RZK_DEVICE_CB_t * stdin,RZK_DEVICE_CB_t * stdout,RZK_DEVICE_CB_t * stderr,UINT16 nargs,INT8 *args[]);
extern INT16 x_WepCmd(	struct shvars *Shl, RZK_DEVICE_CB_t * stdin,RZK_DEVICE_CB_t * stdout,RZK_DEVICE_CB_t * stderr,UINT16 nargs,INT8 *args[]);
extern INT16 x_setWlanParams(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
extern INT16 x_setDataRate(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
extern INT16 x_Scan(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
extern INT16 x_Join(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
extern INT16 x_Phrase(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
extern INT16 x_Pass(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
extern INT16 x_Disconnect(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);

/* addarg.c */
extern INT16 addarg(struct shvars* Shl, void * pid, UINT16 nargs, UINT16 len);
/* ascdate.c */
extern INT16 xc_ascdate(UINT32 time, INT8 *str);
/* bootp.c */
#ifdef _not_yet
extern INT16 parse_bootp(struct boot_type *tmp);
extern INT16 parse_vendor(INT8 vendor[]);
extern INT16 printbootp(struct boot_type *tmp);
#endif
/* lexan.c */
extern INT16 lexan(struct shvars *Shl, INT8 *line);
/* login.c */
extern INT16 login(struct shvars *Shl, RZK_DEVICE_CB_t * dev);
/* netcmds.c */
extern struct cmdent	netcmds[];
extern UINT16	nnetcmds;
/* shell.c */
extern INT16 shell(RZK_DEVICE_CB_t * dev);
extern struct cmdent *ShellCommands;
extern void shell_add_commands(struct cmdent *cmds, UINT16 ncmds);
extern INT16 shell_init(RZK_DEVICE_CB_t * dev);
/* shell_conf.c */
extern struct cmdent	defaultcmds[];
extern UINT16	ndefaultcmds;
/* x_arp.c */
extern INT16 x_arp(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_boot.c */
extern INT16 x_boot(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_bpool.c */
extern INT16 x_bpool(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_cat.c */
extern INT16 x_cat(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_close.c */
extern INT16 x_close(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_conf.c */
extern INT16 x_conf(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
extern INT8 *FindName(INT8 *strIP);
/* x_cp.c */
extern INT16 x_cp(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_create.c */
extern INT16 x_creat(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_date.c */
extern INT16 x_date(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_devs.c */
extern INT16 x_devs(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_dg.c */
extern INT16 x_dg(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_dumper.c */
extern INT16 x_dumper(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_echo.c */
extern INT16 x_echo(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_exit.c */
extern INT16 x_exit(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_finger.c */
extern INT16 x_finger(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
extern INT16 rfing(INT8 *user, INT8 *rhost, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr);
extern INT16 lfing(INT8 *nam, RZK_DEVICE_CB_t * output);
/* x_hang.c */
extern INT16 x_hang(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_help.c */
extern INT16 x_help(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_ifstat.c */
extern INT16 x_ifstat(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_igmp.c */
extern INT16 x_igmp(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_kbuf.c */
extern INT16 x_kbuf(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_kill.c */
extern INT16 x_kill(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_mail.c */
extern INT16 x_mail(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_mem.c */
extern INT16 x_mem(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_mount.c */
extern INT16 x_mount(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_mv.c */
extern INT16 x_mv(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_net.c */
extern INT16 x_net(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_ns.c */
extern INT16 x_ns(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_ospf.c */
extern INT16 x_ospf(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_ping.c */
extern INT16 x_ping(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_port.c */
extern INT16 x_port(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_pppopt.c */
extern INT16 x_pppopt(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_pppstat.c */
extern INT16 x_pppstat(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_ps.c */
extern INT16 x_ps(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_reboot.c */
extern INT16 x_reboot(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_rf.c */
extern INT16 x_rf(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_rls.c */
extern INT16 x_rls(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_rm.c */
extern INT16 x_rm(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_route.c */
extern INT16 x_route(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_routes.c */
extern INT16 x_routes(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_sem.c */
extern INT16 x_sem(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_set.c */
extern INT16 x_set(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
extern INT16 debug_level(INT8 *option);
/* x_sleep.c */
extern INT16 x_sleep(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_tftpdemo.c */
extern INT16 x_tftpdemo(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_snap.c */
extern INT16 x_snap(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_snmp.c */
extern INT16 x_snmp(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_timerq.c */
extern INT16 x_timerq(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_udplisten.c */
extern INT16 x_udplisten(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_udpping.c */
extern INT16 x_udpping(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_unmou.c */
extern INT16 x_unmou(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_uptime.c */
extern INT16 x_uptime(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
/* x_who.c */
extern INT16 x_who(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
extern INT16 cmd_tftp_get (struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout,RZK_DEVICE_CB_t * stderr,UINT16 nargs,INT8 *args[]);
extern INT16 cmd_tftp_put (struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout,RZK_DEVICE_CB_t * stderr,UINT16 nargs,INT8 *args[]);
extern INT16 x_addUsr(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
extern INT16 x_DeleteUsr(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
extern INT16 x_ArpDisplay(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
extern INT16 x_netstat(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
extern INT16 x_getdatetime(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
extern INT16 x_setdatetime(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
extern INT16 x_set_ip_params(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, UINT16 nargs, INT8 *args[]);
extern INT16
x_SNTPdatetime
(
	struct shvars *				Shl,
	RZK_DEVICE_CB_t *								stdin,
	RZK_DEVICE_CB_t *								stdout, 
	RZK_DEVICE_CB_t *								stderr, 
	UINT16								nargs,
	INT8 *							args[]
);

/** MK $ CR 6840 $ 9-DEC-2005 */
extern INT16 Shell_Telnet(
	struct shvars *			Shl1,
	RZK_DEVICE_CB_t *						stdin,
	RZK_DEVICE_CB_t *						stdout, 
	RZK_DEVICE_CB_t *						stderr, 
	UINT16					nargs,
	INT8					     *argv[]
);
/** MEENAKSHI ADDED NEWLY */
#define SHELL_ZTP 		( 1 )
#define SHELL_TELNET 	( 2 )
#define SHELL_FTP 		( 3 )

#define ZSHELL_SUCCESS  ( 0 )
#define ZSHELL_FAILURE  ( -1 )

#define ZSHELL_FALSE  ( 0 )
#define ZSHELL_TRUE 	( 1 )


#endif
