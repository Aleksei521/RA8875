/**************************************************************************************
 * File       : ZThread.h
 *
 * Description: Provides the declaration of RZK Thread structures
 * and APIs.
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
 *********************************************************************************/



#ifndef _ZTHREAD
#define _ZTHREAD  

#include "ZContext.h"

/*  THREAD OPERATION MODES */
#define RZK_THREAD_ROUNDROBIN	(UINT8)0x01		/* Specifies RoundRobin time slice is enabled */
#define RZK_THREAD_AUTOSTART	(UINT8)0x02     /* Starts the thread at the creation time */
#define RZK_THREAD_PREEMPTION	(UINT8)0x04     /* Current thread can be preempted by the higher priority thread */
#define RZK_THREAD_REGISTER		(UINT8)0x10		/* Runs the thread as a register thread */
#define RZK_THREAD_INTERRUPT	(UINT8)0x80		/* Runs the thread as a register thread */

/* STRUCTURE OF THE THREAD PARAMETERS */

typedef struct THREADPARAMS_STRUCT_t
{
	RZK_NAME_t  szName[MAX_OBJECT_NAME_LEN];  	 /* Name of the Thread */
	RZK_STATE_t uState;							 /*This specifies the state the thread is in */	
	UINT8 uBankSelector;						 /* specifies the count of the register bank */
	RZK_OPERATIONMODE_t  uOperationMode;	     /* Specifies the mode of the operation */
	TICK_t tQuantum;							 /* specifies the Time slice */
	RZK_THREAD_PRIORITY_t cPriority;			 /*Specifies the the priority of the thread */
	RZK_SIGNAL_t sSignalReceive;				 /*Specifies the recieve signal value for the thread*/ 
} RZK_THREADPARAMS_t;

/* STRUCTURE FOR TCB MEMORY ALLOCATION */

typedef struct RZK_THREAD_CB
{
    UINT8 CB[RZK_THREAD_CB_SIZE] ;
} RZK_THREAD_CB_t;


/*****************************************************************************
*	syntax for function Prototypes:
*	<return type> <function name>
*  (<type><argument name>)
****************************************************************************/

void RZK_KernelInit( void ) ;
void RZK_KernelStart( void ) ;
UINT8 init_malloc( void ) ;
RZK_THREADHANDLE_t   RZKCreateThread
				   (	
					RZK_NAME_t  szName[MAX_OBJECT_NAME_LEN],									
					RZK_PTR_t pEntryFunction,				
					FNP_THREAD_ENTRY *pCleanupFunction, 
					CADDR_t pInitialStack,				
					RZK_THREAD_PRIORITY_t etPriority, 
					TICK_t tQuantum,							
					RZK_OPERATIONMODE_t uOperationMode,
					UINT8	nArgs,
					...
				    );

RZK_THREADHANDLE_t   RZKCreateThreadEnhanced
				   (	
					RZK_NAME_t  szName[MAX_OBJECT_NAME_LEN],									
					RZK_PTR_t pEntryFunction,				
					FNP_THREAD_ENTRY *pCleanupFunction, 
					COUNT_t uStackSize,				
					RZK_THREAD_PRIORITY_t etPriority, 
					TICK_t tQuantum,							
					RZK_OPERATIONMODE_t uOperationMode,
					UINT8	nArgs,
					...
				    );

RZK_STATUS_t	RZKDeleteThread
				(
				  RZK_THREADHANDLE_t  hThread		
				);

RZK_STATUS_t	RZKDeleteThreadEnhanced
				(
				  RZK_THREADHANDLE_t  hThread		
				);

RZK_STATUS_t	RZKSuspendThread
				(
				 RZK_THREADHANDLE_t  hThread,	
				 TICK_t tTicks								   
				 );

RZK_STATUS_t	RZKResumeThread
				(
				 RZK_THREADHANDLE_t  hThread 
				 );

RZK_STATUS_t RZKYieldThread(void);

RZK_STATUS_t	RZKGetThreadParameters
				(
				 RZK_THREADHANDLE_t  hThread,	
				 RZK_THREADPARAMS_t *pThreadParams 
				);


RZK_PTR_t       RZKGetArgPtr(void) ;

RZK_THREADHANDLE_t RZKGetCurrentThread(void) ;

RZK_STATUS_t RZKGetErrorNum(void) ;
TIME_t RZKSystemTime(void);

void RZKFormatError(
                    RZK_STATUS_t num	
                    ) ;

RZK_STATUS_t RZKSetThreadPriority
				(
				RZK_THREADHANDLE_t	hThread,
				RZK_THREAD_PRIORITY_t threadPriority
				);

RZK_STATUS_t TaskUnsleep(RZK_THREADHANDLE_t	hThread);

void RZKSuspendInterruptThread();
void RZKResumeInterruptThread(RZK_THREADHANDLE_t hThread);

RZK_STATE_t RZKDisablePreemption( void );
void RZKEnablePreemption( void );
void RZKRestorePreemption(RZK_STATE_t);

void RZK_Reboot( void );

// CWD IMPLEMENTATION
RZK_STATUS_t RZKGetCwd
(
	RZK_THREADHANDLE_t hThread,
	RZK_PTR_t *pFsDir
) ;

RZK_STATUS_t RZKSetCwd
(
	RZK_PTR_t pFsDir
) ;

RZK_STATUS_t RZKGetHandleByIndex
(
	UINT nIndex,
	RZK_THREADHANDLE_t *hThread
) ;

RZK_STATUS_t RZKSetFSData
(
	UINT32 ulData
) ;

UINT32 RZKGetFSData
(
	void
) ;

void RZKThreadLockForDelete();
void RZKThreadUnLockForDelete();


#endif /* _ZTHREAD */
