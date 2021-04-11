/*
 * File       :     shell_conf.c
 *
 * Description:		This file contains the configuration details for the
 *					shell.
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

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZThread.h"
#include "shell.h"
#include "ftp_client.h"

/**
 * @name shell_conf.c
 * @memo Default Debug Shell Commands
 * @doc These are default settings used by the Debug Shell. This file should
 * be included in a project, and modified as needed. If any of commands are
 * not needed, they can be commented to remove from the system.
 */

/*@{*/
/**
 * @doc A default set of all non-network related commands.  These commands are
 * made available by default - there is no need to call shell_add_commands()
 * to make them available.
 */
#ifdef HELP_REQUIRED 
INT8	*g_ShellHelpStrings[] = {
	"Displays information about system's buffer pools.\n" ,
	"Prints device information in the device table.\n" ,
	"Echos text entered after the echo command to the stdout\n" ,
	"Exit the shell process.\n",
	"suspends the shell.\n",
	"Displays the set of commands that can be executed from the shell's command prompt. When help is followed by a command name it prints help for that specific command\n",
	"Kills a specified process.\n",
	"Prints a summary of the state of system memory.\n",
	"Formats and prints information about all message ports currently in use.\n", 
	"Displays information about all active semaphores\n",
	"Places the shell process to sleep for a specified number of seconds.\n",
	"prints the current date and time to a standard output\n",
	"sets the current date and time to a standard outputsets current date and time\n",
	"Get a file from the server and store it in current working directory\n",
	"Upload the file stored in current working directory to the server\n",
	"Initiates a FTP client connection to specified server\n",
	"Add a new user to FTP server\n",
	"Delete an existing user of FTP server\n",
	"Provides access to Telnet\n",
	"Sends mail using SMTP\n",
	"Causes the operating system to begin its initialization sequence.\n",
	"Displays information about all processes in the system\n",
	"Same as Help\n",
	"Sets the IP, gateway, netmask, emac addresses along with DHCP \n",
	"Displays all existing ARP entries\n" ,
	"Displays status of all the active and passive sockets\n",
	"Ping to a specified host\n",
	"Displays configuration information of all interfaces\n",
	"Join/Leave the igmp group on that particular ethernet interface\n",
} ;	
INT8	*g_ftpClientHelpStrings[] = 
{
	"set ascii transfer type",
	"set binary transfer type",
	"terminate ftp session and exit",
	"change remote working directory",
	"termiate ftp session",
	"delete remote file",
	"list contents of remote directory",
	"receive file",
	"toggle printing `#' for each buffer transferred",
	"print local help information",
	"change working directory on local machine",
	"list contents of remote directory",
	"list contents of remote directory",
	"make directory on remote machine",
	"mode",
	"list contents of remote directory",
	"connect to remote ftp",
	"send one file",
	"print working directory on remote machine",
	"terminate ftp session and exit",
	"receive file",
	"remove directory on remote machine",
	"remove file on remote machine",
	"rename the file on remote machine",
	"show remote system type",
	"send new user information"
};
/** MK $ CR 6840 $ 10-DEC-2005 $ Added Telnet Help strings */
INT8	*g_ShellTelnetStrings[] = 
{
	"Help for Telnet commands\n",
	"Opens a telnet connection\n",
	"Ends a Telnet session\n",
	"Quits Telnet\n",
	"Help for Telnet commands\n"	
} ;
#else
INT8	*g_ShellHelpStrings[] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL, 
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
	
};	
INT8	*g_ftpClientHelpStrings[] = 
{
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};
/** MK $ CR 6840 $ 10-DEC-2005 $ Added Telnet Help strings */
INT8	*g_ShellTelnetStrings[] = 
{
	NULL,
	NULL,
	NULL,
	NULL,
	NULL	
} ;

#endif

//RK :4th Jan 2006: Changed the below intialization during IAR
struct	cmdent	defaultcmds[]  = {
#ifndef EVB_F91_MINI
//	{ "bpool",  TRUE,   (SHELL_CMD)x_bpool, NULL,(INT8*)(g_ShellHelpStrings)},		
	{ "devs",   TRUE,   (SHELL_CMD)x_devs,  NULL,(INT8*)(g_ShellHelpStrings+1)}, 		
//	{ "echo",   TRUE,   (SHELL_CMD)x_echo,  NULL,(INT8*)(g_ShellHelpStrings+2)}, 		
	{ "exit",   TRUE,   (SHELL_CMD)x_exit,  NULL, (INT8*)(g_ShellHelpStrings+3)},			
//	{ "hang",   TRUE,   (SHELL_CMD)x_hang,  NULL, (INT8*)(g_ShellHelpStrings+4)},		
	{ "help",   TRUE,   (SHELL_CMD)x_help,  NULL, 	(INT8*)(g_ShellHelpStrings+5)},		
//	{ "kill",   TRUE,   (SHELL_CMD)x_kill,  NULL, (INT8*)(g_ShellHelpStrings+6)},			
//	{ "mem",    TRUE,   (SHELL_CMD)x_mem,   NULL, (INT8*)(g_ShellHelpStrings+7)},		
	{ "port",   TRUE,   (SHELL_CMD)x_port,  NULL, 	(INT8*)(g_ShellHelpStrings+8)},		
	{ "sem",    TRUE,   (SHELL_CMD)x_sem,   NULL, (INT8*)(g_ShellHelpStrings+9)},		
	{ "sleep",  TRUE,   (SHELL_CMD)x_sleep, NULL,	 (INT8*)(g_ShellHelpStrings+10)},		
	{ "gettime",TRUE,   (SHELL_CMD)x_getdatetime,NULL, (INT8*)(g_ShellHelpStrings+11)	},
	{ "settime",TRUE,   (SHELL_CMD)x_setdatetime,NULL, (INT8*)(g_ShellHelpStrings+12)	},
//	{ "tftp_get",   TRUE,       (SHELL_CMD)cmd_tftp_get,    NULL,	(INT8*)(g_ShellHelpStrings+13)},	
//	{ "tftp_put",   TRUE,       (SHELL_CMD)cmd_tftp_put,    NULL,	(INT8*)(g_ShellHelpStrings+14)},	
//	{ "ftp",        TRUE,       (SHELL_CMD)cmd_ftp,         NULL,	(INT8*)(g_ShellHelpStrings+15)},	
	{ "addusr",     TRUE,       (SHELL_CMD)x_addUsr,        NULL,	(INT8*)(g_ShellHelpStrings+16)},	
	{ "deleteusr",	TRUE,       (SHELL_CMD)x_DeleteUsr,     NULL,	(INT8*)(g_ShellHelpStrings+17)},
		/** MK $ CR 6840 $ 9-DEC-2005 */
	{ "telnet", TRUE, (SHELL_CMD)Shell_Telnet, NULL, (INT8*)(g_ShellHelpStrings+18)},
	{ "mail",   TRUE,   (SHELL_CMD)x_mail,  NULL, (INT8*)(g_ShellHelpStrings+19)},
#endif
	{ "reboot", TRUE,   (SHELL_CMD)x_reboot,NULL, (INT8*)(g_ShellHelpStrings+20)},		
	{ "ps",     TRUE,   (SHELL_CMD)x_ps,    NULL, (INT8*)(g_ShellHelpStrings+21)},		
	{ "?",      TRUE,   (SHELL_CMD)x_help,  NULL, (INT8*)(g_ShellHelpStrings+22)},
	
#ifndef SSL_F91_MINI
	{ "setipparams",	TRUE,       (SHELL_CMD)x_set_ip_params, NULL,	(INT8*)(g_ShellHelpStrings+23)},
#endif
	{ "arp",        TRUE,       (SHELL_CMD)x_ArpDisplay,    NULL,		(INT8*)(g_ShellHelpStrings+24)},
	{ "netstat",    TRUE,       (SHELL_CMD)x_netstat,       NULL,		(INT8*)(g_ShellHelpStrings+25)},
	{ "ping",       TRUE,       (SHELL_CMD)x_ping,          NULL,		(INT8*)(g_ShellHelpStrings+26)},
	{ "ifstat",     TRUE,       (SHELL_CMD)x_ifstat,        NULL,			(INT8*)(g_ShellHelpStrings+27)},
#ifndef SSL_F91_MINI
	{ "igmp",       TRUE,       (SHELL_CMD)x_igmp,          NULL,		(INT8*)(g_ShellHelpStrings+28)},
#endif
	};	/* shell commands */

/**
 * @doc The number of entries in the defaultcmds[] array.  If the defaultcmds
 * array is modified, ndefaultcmds must be adjusted accordingly.
 */
UINT16 ndefaultcmds=sizeof(defaultcmds)/sizeof(struct cmdent);

/*@}*/
