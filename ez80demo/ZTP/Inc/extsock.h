/*
 * File       :     Extsock.h
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

/*
 *  94.04.20 by MF
 *  fds - file descriptor
 *  lds - listen file descriptor
 */
#ifndef EXTSOCK_H
#define EXTSOCK_H

#ifdef SSL_SOCKET
#include "ssl.h"
#endif

typedef struct {
  sock_type s;
  UINT16 type;
  UINT16 port;
  INT fds, lds;
  UINT8 blocking; 
#ifdef SSL_SOCKET
	SSL_SESSION *pSession ;
#endif
} _ext_socket;
#endif
