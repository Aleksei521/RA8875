/******************************************************************************
 * File       : ZSemaphore.h
 *
 * Description: Provides the declaration of RZK Semaphore structures
 * and APIs.
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
 ******************************************************************************/

#ifndef _ZSEMAPHORE
#define _ZSEMAPHORE

#define RZK_PRIORITY_INHERITANCE	0x80   /*  Priority Inheritance supported on the resource */

/* STRUCTURE FOR SEMAPHORE PARAMETERS */
typedef struct SEMAPHOREPARAMS_STRUCT_t
{
	RZK_STATE_t uState;  		/*Specifies the initial state of the semaphore */	
	COUNT_t uInitialCount;  	/*Specifies the initial count of the semaphore */	
	COUNT_t uDynamicCount;		/*Specifies the dynamic count of the counting semaphore*/
	COUNT_t nNumThreads;		/*Specifies the number of threads that are waiting on the semaphore*/
} RZK_SEMAPHOREPARAMS_t;

/* STRUCTURE FOR SEMAPHORE CB MEMORY ALLOCATION */

typedef struct RZK_SEMAPHORE_CB
{
    UINT8 CB[RZK_SEMAPHORE_CB_SIZE] ;
} RZK_SEMAPHORE_CB_t;

/********************************************
*	syntax for Function Prototype:
*	<return type> <function name>
*  (<type><argument name>)
*****************************************/

RZK_SEMAPHOREHANDLE_t RZKCreateSemaphore
						( 
							RZK_NAME_t  szName[MAX_OBJECT_NAME_LEN],										
							COUNT_t uInitialCount,				   
							RZK_RECV_ATTRIB_et etAttrib		
						);

RZK_STATUS_t RZKDeleteSemaphore
				 ( 
					 RZK_SEMAPHOREHANDLE_t hSemaphore		
				 );


RZK_STATUS_t RZKAcquireSemaphore
				 ( 
					 RZK_SEMAPHOREHANDLE_t	hSemaphore,  
					 TICK_t tBlockTime
				 );

RZK_STATUS_t RZKReleaseSemaphore
				 ( 
					 RZK_SEMAPHOREHANDLE_t hSemaphore  
				 );

RZK_STATUS_t RZKGetSemaphoreParameters
				 ( 
					 RZK_SEMAPHOREHANDLE_t	hSemaphore,
					 RZK_SEMAPHOREPARAMS_t *pSemaphoreParams 
				 );

RZK_STATUS_t ResetSemaphore
				 ( 
					 RZK_SEMAPHOREHANDLE_t	hSemaphore
				 );

RZK_STATUS_t SetSemaphoreParameters
				( 
					RZK_SEMAPHOREHANDLE_t hSemaphore,
					RZK_SEMAPHOREPARAMS_t *pSemaphoreParams
				);

#endif  /* _ZSEMAPHORE  */
