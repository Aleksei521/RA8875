/*
 * File       :     x_TCP.c
 *
 * Description:  This file contains the implementation of TCP client application.
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

/*Standard includes*/
#include <stdio.h>
#include "ztptcp.h"
#include "sockaddr.h"
#include "socket.h"

/* User includes */
#include "ZTypes.h"
#include "ZSysgen.h"
#include "SocketDemo.h"
#include "shell.h"

INT16
GetString
(
	RZK_DEVICE_CB_t * dev,
	INT8 	*buf,
	UINT16	bufsz
)
{
	INT16	   len;
	UINT16     idx;

	idx=0;
	memset(buf, 0, bufsz);
	
	while(!xc_index(buf, '\n')) 
	{
	    if( (len = RZKDevRead(dev, &buf[idx], bufsz-idx)) <= SYSERR )
			return -1;
			idx+=len;
		    if(idx == bufsz)
			return OK;
	}

	/* Remove trailing \n (and possibly \r before it) */
	if(*(xc_index(buf, '\n') - 1) == '\r')
		*(xc_index(buf, '\n') - 1) = '\0';
	else
		*(xc_index(buf, '\n')) = '\0';

	return OK;
}

