#ifndef _SOCKADDR_H
#define _SOCKADDR_H
/*
 * File       :     sockaddr.h
 *
 * Description:  This file contains socket related definitions.
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
/* Berkeley format socket address structures. These things were rather
 * poorly thought out, but compatibility is important (or so they say).
 * Note that all the sockaddr variants must be of the same size, 16 bytes
 * to be specific. Although attempts have been made to account for alignment
 * requirements (notably in sockaddr_ax), porters should check each
 * structure.
 */
#include "ZTypes.h"

/* Generic socket address structure */
struct sockaddr {
	INT16 sa_family;
	INT8 sa_data[14];
};

/* This is a structure for "historical" reasons (whatever they are) */
struct in_addr {
	UINT32 s_addr;
};

/* Socket address, DARPA Internet style */
struct sockaddr_in {
	INT16 sin_family;
	UINT16 sin_port;
	struct in_addr sin_addr;
	INT8 sin_zero[8];
};

#define	SOCKSIZE	(sizeof(struct sockaddr))
#define MAXSOCKSIZE	SOCKSIZE /* All sockets are of the same size for now */

#endif /* _SOCKADDR_H */
