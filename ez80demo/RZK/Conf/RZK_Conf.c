/*********************************************************************************
 * File       : RZK_Conf.c
 *
 * Description: Defines the configurable system parameters for RZK and its
 *              sub components
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
 *******************************************************************************/
// RZK CORE CONFIGURATION




#ifdef _EZ80F91
#include <eZ80F91.h>
#endif
#ifdef _EZ80F92
#include <eZ80F92.h>
#endif
#ifdef _EZ80F93
#include <eZ80F93.h>
#endif
#ifdef _EZ80L92
#include <eZ80L92.h>
#endif
#ifdef _EZ80190
#include <eZ80190.h>
#endif

#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZThread.h"
#include "ZInterrupt.h"
#include "ZSemaphore.h"
#include "ZMessageQ.h"
#include "ZMemory.h"
#include "ZTimer.h"
#include "ZEventGroup.h"
#include "ZRegion.h"
#include "ZDevice.h"

//#define RZK_MINI /* Use this in case of Mini Development Board*/

#ifdef RZK_MINI
#ifdef ZDOT_WLAN
	#define MAX_THREADSH                    (15)
	#define MAX_MESSAGEQSH                  (8)
	#define MAX_SEMAPHORESH                 (8)
	#define MAX_EVENTGROUPSH                (4)
	#define MAX_TIMERSH                     (2)
	#define MAX_PARTITIONSH                 (1)
	#define MAX_REGIONSH                    (2)
	#define MAX_REGIONS_TABH                (100)
	#define RZK_MAX_DCBH                    (15)
#else
	#define MAX_THREADSH                    (17)
	#define MAX_MESSAGEQSH                  (20)
	#define MAX_SEMAPHORESH                 (40)
	#define MAX_EVENTGROUPSH                (5)
	#define MAX_TIMERSH                     (10)
	#define MAX_PARTITIONSH                 (5)
	#define MAX_REGIONSH                    (3)
	#define MAX_REGIONS_TABH                (100)
	#define RZK_MAX_DCBH                    (15)
#endif	
#else
	#define MAX_THREADSH                    (30)
	#define MAX_MESSAGEQSH                  (28)
	#define MAX_SEMAPHORESH                 (40)
	#define MAX_EVENTGROUPSH                (20)
	#define MAX_TIMERSH                     (20)
	#define MAX_PARTITIONSH                 (20)
	#define MAX_REGIONSH                    (20)
	#define MAX_REGIONS_TABH                (2000)
	#define RZK_MAX_DCBH                    (25)
#endif

// **************************
// CHANGE THE BELOW VALUES IF YOU KNOW WHAT YOU ARE DOING,
// REFER TO REFERENCE MANUAL FOR DESCRIPTIONS
// **************************

/* notes */
// notes for RZK Core macros. This describes the each macro and 
// minimum value required to work with. FOR MORE INFORMATION,
// PLEASE REFER TO RZK REFERENCE MANUAL PART OF THE RELEASE

// Thread parameters
// MAX_THREADSH - Contains the maximum number of threads to be created 
// (minimum value is 4 (for RZK stand alone without any components added)
// RZK_TIME_SLICEH - Default round robin time slice for the threads 
// created in round robin
// mode.
//#define MAX_THREADSH                    (30)
#define RZK_TIME_SLICEH                 (20)

// IPC parameters
// MAX_MESSAGEQSH - Maximum number of message queues that can be created. 
// (Minimum value is 0 if messagequeue objects are not used in the system)
// MAX_EVENTGROUPSH - Maximum number of eventgroup objects that can be created.
// (Minimum value is 0 if eventgroup objects are not used in the system)
//#define MAX_MESSAGEQSH                  (28)
//#define MAX_EVENTGROUPSH                (20)

// Semaphore parameters
// MAX_SEMAPHORESH - Maximum number of semaphore objects that can be created. 
// (Minimum value is 0 if semaphores are not used in the system)
//#define MAX_SEMAPHORESH                 (40)

// MAX_TIMERSH - Maximum number of software timer objects that can be created.
// (Minimum value is 0 if software timers are not used in the system)
//#define MAX_TIMERSH                     (20)

// memory object parameters
// MAX_PARTITIONSH - Maximum number of partition objects that can be created. 
// (Minimum value is 0 if partition objects are not used in the system)
// MAX_REGIONSH - Maximum number of regions objects that can be created. 
// (Minimum value is 0 if region objects are not used in the system)
// MAX_REGIONS_TABH - Maximum number of region segment objects that can be created.
// (Minimum value is 0 if region segment objects are not used in the system)
// If regions are used, this macro should be set to the appropriate values.

//#define MAX_PARTITIONSH                 (20)
//#define MAX_REGIONSH                    (20)
//#define MAX_REGIONS_TABH                (2000)

// RZK support threads stack size
// RZK_STACK_SIZEH - RZK Kernel thread stack size. Minimum value is 200, 
// depends upon the layer depth of functions being called by a function 
// in the thread. Normally 512 or 1024 is used.
// RZK_SYSTIMERSTACK_SIZEH - RZK Kernel timer thread stack size. Minimum
// value is 200, depends upon the layer depth of functions being called 
// by a function in the thread. Normally 512 or 1024 is used.
#define RZK_STACK_SIZEH                 (2048)
#define RZK_SYSTIMERSTACK_SIZEH         (2048)

// different system parameters
// RZK_SYSTICKS_INSECH - RZK system ticks per seconds. This depends upon
// the system response to the scheduling of tasks. Normally 100 is used.
// RZK_DEVTICKS_PERSYSTICKH - RZK system ticks per seconds related value.
// If this value is 2, then 2 system timer interrupts represent 1 tick.
// SYSTEM_CLOCKH - System clock frequency
#define RZK_SYSTICKS_INSECH             (100)
#define RZK_DEVTICKS_PERSYSTICKH        (1)

#ifdef RZK_MINI

	#ifdef _EZ80F91
	#define SYSTEM_CLOCKH                   (50000000L)
	#endif
	#ifdef _EZ80F92
	#define SYSTEM_CLOCKH                   (20000000L)
	#endif
	#ifdef _EZ80F93
	#define SYSTEM_CLOCKH                   (20000000L)
	#endif
	#ifdef _EZ80L92
	#define SYSTEM_CLOCKH                   (48000000L)
	#endif
	#ifdef _EZ80190
	#define SYSTEM_CLOCKH                   (40000000L)
	#endif
/*CR#7816 fix */
#else
	extern UINT32 SYS_CLK_FREQ;
#endif

// HWTIMER_TO_USE - Hardware timer to be used for system timer. Can 
// contain value from 0-3
#define HWTIMER_TO_USE                  (0)

// Device driver components
// RZK_DEVICE_DRIVERH - Enabling the RZK Device driver framework with 
// device driver added to the system. Should use (1) for enabling, 
// (0) for disabling.

// RZK_MAX_DCBH - Maximum number of device control blocks supported if
// device driver is enabled. (Minimum value should be 1).
//#define RZK_MAX_DCBH                    (25)
#define RZK_DEVICE_DRIVERH              (1)

// File system related kernel parameters
// RZK_MAX_CWD_PATH_LEN - Maximum length of the current working directory
// stored in the TCB including the directory separator and volume name.
// RZK_CWD_PATH - Current working directory set for the threads that are 
// craeted in main or RZKApplicationEntry function 
// (application entry for standalone RZK)
#define RZK_MAX_CWD_PATH_LEN            (128)
#define RZK_CWD_PATH                    "EXTF:/"







// **************************
// DO NOT CHANGE ANYTHING BELOW IN THIS SECTION
// **************************

// RZK CORE
UINT      RZK_TIME_SLICE                           = RZK_TIME_SLICEH ;
UINT      MAX_THREADS                              = MAX_THREADSH;
UINT      MAX_SEMAPHORES                           = MAX_SEMAPHORESH;
UINT      MAX_MESSAGEQS                            = MAX_MESSAGEQSH; 
UINT      MAX_EVENTGROUPS                          = MAX_EVENTGROUPSH;
UINT      MAX_TIMERS                               = MAX_TIMERSH;
UINT      MAX_PARTITIONS                           = MAX_PARTITIONSH;
UINT      MAX_REGIONS                              = MAX_REGIONSH ;
UINT      MAX_REGIONS_TAB                          = MAX_REGIONS_TABH ;
UINT      RZK_SYSTICKS_INSEC                       = RZK_SYSTICKS_INSECH;
UINT      RZK_DEVTICKS_PERSYSTICK                  = RZK_DEVTICKS_PERSYSTICKH;

#ifdef RZK_MINI
UINT32    SYSTEM_CLOCK                             = SYSTEM_CLOCKH;
#else
UINT32    SYSTEM_CLOCK                             = (UINT32)&SYS_CLK_FREQ;
#endif

UINT      RZK_STACK_SIZE                           = RZK_STACK_SIZEH ;
UINT      RZK_SYSTIMERSTACK_SIZE                   = RZK_SYSTIMERSTACK_SIZEH;

UINT      RZK_MAX_DCB                              = RZK_MAX_DCBH;
UINT      RZK_DEVICE_DRIVER                        = RZK_DEVICE_DRIVERH;


RZK_DEVICE_CB_t                                    nDCB[ RZK_MAX_DCBH ] ;
RZK_THREAD_CB_t                                 nInterruptThread[1];


#if MAX_THREADSH
RZK_THREAD_CB_t                                    nTcb[MAX_THREADSH];
#endif // MAX_THREADSH

#if MAX_EVENTGROUPSH
RZK_EG_CB_t                                        nEventGroup[MAX_EVENTGROUPSH];
#endif // MAX_EVENTGROUPSH

#if MAX_PARTITIONSH
RZK_PARTITION_CB_t                                 nPartition[MAX_PARTITIONSH];
#endif // MAX_PARTITIONSH

#if MAX_MESSAGEQSH
RZK_MQ_CB_t                                        nMessageQueue[MAX_MESSAGEQSH];
#endif // MAX_MESSAGEQSH

#if MAX_SEMAPHORESH
RZK_SEMAPHORE_CB_t                                 nSemaphore[MAX_SEMAPHORESH];
#endif // MAX_SEMAPHORESH

#if MAX_TIMERSH
RZK_TIMER_CB_t                                     nTimer[MAX_TIMERSH];
#endif // MAX_TIMERSH

#if MAX_REGIONSH
RZK_REGION_CB_t                                    nRegion[MAX_REGIONSH];
#endif // MAX_REGIONSH

#if MAX_REGIONS_TABH
RZK_SEGMENT_CB_t                                   nRegion_Tab[MAX_REGIONS_TABH];
#endif // MAX_REGIONS_TABH

#if RZK_DEVICE_DRIVERH
RZK_DEVICE_CB_t	 									usrDevBlk[RZK_MAX_DCBH] ;
#endif

#ifdef RZKPI
UINT      priorityinheritance                      = 1;
#else
UINT      priorityinheritance                      = 0;
#endif // RZK_PRIORITYINHERITANCE

#ifdef RZKDBG
UINT8     cDebug                                   = 1;
#else
UINT8     cDebug                                   = 0;
#endif // RZKMACRODEBUG

#if HWTIMER_TO_USE==0
UINT8     cTim                                     = 0;
UINT8      cIer                                     = 0x61;
UINT8      cIir                                     = 0x62;
UINT8      cCtl                                     = 0x80;
#endif // TIMER0

#if HWTIMER_TO_USE==1
UINT8     cTim                                     = 1;
UINT8      cIer                                     = 0x66;
UINT8      cIir                                     = 0x67;
UINT8      cCtl                                     = 0x83;
#endif // TIMER1

#if HWTIMER_TO_USE==2
UINT8     cTim                                     = 2;
UINT8      cIer                                     = 0x70;
UINT8      cIir                                     = 0x71;
UINT8      cCtl                                     = 0x86;
#endif // TIMER2

#if HWTIMER_TO_USE==3
UINT8     cTim                                     = 3;
UINT8      cIer                                     = 0x75;
UINT8      cIir                                     = 0x76;
UINT8      cCtl                                     = 0x89;
#endif // TIMER3

#if HWTIMER_TO_USE==4
UINT8     cTim                                     = 4;
UINT8      cCtl                                     = 0x8C;
#endif // TIMER3

#if HWTIMER_TO_USE==5
UINT8     cTim                                     = 5;
UINT8      cCtl                                     = 0x8F;
#endif // TIMER3

// CWD IMPLEMENTATION
#ifdef RZKFS
UINT8     fsEnable                                = 1;
UINT      nRzkMaxCwdPathLen                       = RZK_MAX_CWD_PATH_LEN;
INT8      fsCwdPath[RZK_MAX_CWD_PATH_LEN]         = RZK_CWD_PATH ;
INT8                                              fsCwdArray[MAX_THREADSH][RZK_MAX_CWD_PATH_LEN] ;
#else
UINT8     fsEnable                                = 0;
UINT      nRzkMaxCwdPathLen                       = RZK_MAX_CWD_PATH_LEN;
INT8                                              fsCwdPath[1] ;//
INT8                                              fsCwdArray[1][1] ; 
#endif // RZKFS


RZK_DEVICE_CB_t*                                  CONSOLE = (RZK_DEVICE_CB_t*)NULL ;

/*
* Function		:	IdleThread
*
* Description	:	It is the entry point function for idle thread. It keeps looping infinitely.User can change this
*					function for power saving purposes
* 
* Inputs		:	None	
*
* Outputs		:	None
*
* Dependencies	:	None
*/
void IdleThread()
{
 	while(1)
	{
	}
}

/** END OF RZK CORE CONFIGURATION**/
