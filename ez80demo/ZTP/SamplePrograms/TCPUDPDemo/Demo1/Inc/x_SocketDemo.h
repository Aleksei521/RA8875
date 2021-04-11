/*
 * File       :     x_SocketDemo.h
 *
 * Description:  This file contains demo command handlers APIs.
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
#ifndef _X_SOCKETDEMO_H
#define _X_SOCKETDEMO_H

INT16 x_TCLIENT(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, INT nargs, INT8 *args[]);
INT16 x_TSERVER(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, INT nargs, INT8 *args[]);
INT16 x_UCLIENT(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, INT nargs, INT8 *args[]);
INT16 x_USERVER(struct shvars *Shl, RZK_DEVICE_CB_t * stdin, RZK_DEVICE_CB_t * stdout, RZK_DEVICE_CB_t * stderr, INT nargs, INT8 *args[]); 
#endif   /* _X_SOCKETDEMO_H */
