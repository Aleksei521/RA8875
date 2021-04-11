/*
 * File       :     ftp_client.h
 *
 * Description:  This file contains the Telnet Client side API definitions.
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


#ifndef	_FTP_CLIENT_H
#define	_FTP_CLIENT_H

#define FTP_PORT 21
#define FTPBUFSIZE 255

#define ZSHELL_TRY_AGAIN ( 2 )
#define	hibyte(x)	((UINT8)((x) >> 8))
#define	lobyte(x)	((UINT8)(x))
#define	hiword(x)	((UINT16)((x) >> 16))
#define	loword(x)	((UINT16)(x))

enum {WRITE_MODE, READ_MODE};

typedef enum
{
	ZFTP_SUCCESS = 0x00,
	ZFTP_ERR_CONNECT_FAILURE = -2,
	ZFTP_ERR_NO_GREETING_MSG = -3 	
}ZFTP_ERROR_VALUES ;



INT16 cmd_ftp (struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout,RZK_DEVICE_CB_t *	stderr,UINT16	nargs,INT8 *	args[]);
INT do_ftp(RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, INT8 *Addr);
INT do_login(RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout);
INT remote_command(INT8* cmd, RZK_DEVICE_CB_t * stdout);

#endif	/* _FTPCLI_H */
