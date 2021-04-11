/******************************************************************************************
 * File       : externvar.h
 *
 * Description: All the external variables are declared here.
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
 ******************************************************************************************/

#include "ZTypes.h"

#ifndef _ZEXTERNVAR
#define _ZEXTERNVAR  

extern UINT RZK_TIME_SLICE; /* Specifies the default time slice for round robin mode*/
extern UINT32 SYSTEM_CLOCK ;
extern UINT MAX_THREADS ; /* Specifies the maximum threads the RZK will create */
extern UINT MAX_INTERRUPT_THREADS;
extern UINT MAX_SEMAPHORES;

extern UINT MAX_MESSAGEQS;/* Specifies the maximum message queues that will be created*/
extern UINT MAX_MAILBOXES;
extern UINT MAX_EVENTGROUPS;
extern UINT MAX_TIMERS	;
extern UINT MAX_PARTITIONS;
extern UINT MAX_REGIONS_TAB;
extern UINT MAX_REGIONS; 


extern UINT RZK_SYSTICKS_INSEC;  /* Specifies the number of Ticks resolutions per seconds */
extern UINT RZK_DEVTICKS_PERSYSTICK;/* Specifies the device system ticks */

extern UINT RZK_MAX_DCB;
extern UINT RZK_DEVICE_DRIVER;
extern UINT8 priorityinheritance;
extern TICK_t tRZKTicks;

extern UINT RZK_STACK_SIZE ;
extern UINT RZK_SYSTIMERSTACK_SIZE ;

extern UINT nRzkMaxCwdPathLen ;

#endif
