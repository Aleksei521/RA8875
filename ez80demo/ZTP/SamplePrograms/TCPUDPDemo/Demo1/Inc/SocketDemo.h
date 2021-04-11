/*
 * File       :     SocketDemo.h
 *
 * Description:  This file contains socket demo related APIs.
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

#ifndef _SOCKETDEMO_H
#define _SOCKETDEMO_H

/* Constants */
#ifndef ROUNDROBIN_AP
#define ROUNDROBIN_AP       0 // check whether this is OK
#endif


//Abstract types; not to allow user to create objects of below types
/*struct tcpconnection;  
struct udpclient;
struct udpserver; */

/* User is allowed to use these structure */

//extern INT16 priority;


/* Function prototypes */


extern void Tcpserver(int port);
extern void Tcpclient(INT8 *buf,int port);
extern void Udpclient(INT8 *buf,int port);
extern void Udpserver(int port);
extern INT16 GetString(RZK_DEVICE_CB_t * dev,INT8 	*buf, UINT16	bufsz);


/* User definitions */
#define ARGS	5
#define PORT	4000
#define TRUE	1

/* DON'T DEFINE PERFORMANCE_DEMO MACRO AS DEMO APPLICATIONS ARE NOT TESTED WITH THIS MACRO DEFINED.
   THE FUTURE RELEASES MAY CONTAIN DEMO APPLICATIONS WITH THIS MACRO DEFINED */

//#define PERFORMANCE_DEMO

#endif /* _SOCKETDEMO_H */
