/**********************************************************************
 * File       : ZMessageQ.h
 *
 * Description: Provides declaration of RZK Message Queue structures
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
 **********************************************************************/

#ifndef _ZMESSAGEQ
#define _ZMESSAGEQ


/* STRUCTURE FOR MESSAGE QUEUE PARAMETERS */
typedef struct MESSAGEQPARAMS_STRUCT_t
{
	RZK_STATE_t uState;					 /* State of Message Queue*/
	COUNT_t uQueueLength;                /* Length of queue*/
	COUNT_t uMaxMessageSize;             /*Maximum size of each message*/ 
	COUNT_t nNumThreads;                 /* Number of threads waiting to post and receive*/
	COUNT_t uMessageSpaceLeft;           /*Specifies  the number of message slots are free in the control block */
	RZK_MESSAGE_PTR_t pStart;			 /* Pointer to Start position of Message Queue Area  */	
} RZK_MESSAGEQPARAMS_t;

/* STRUCTURE FOR MQ CB MEMORY ALLOCATION */

typedef struct RZK_MQ_CB
{
    UINT8 CB[RZK_MQ_CB_SIZE] ;
} RZK_MQ_CB_t;

/***************************************************************************
*	syntax for Function Prototype:
*	<return type> <function name>
*  (<type><argument name>)
**************************************************************************/

RZK_MESSAGEQHANDLE_t RZKCreateQueue
								 ( 
									RZK_NAME_t  szName[MAX_OBJECT_NAME_LEN],										
									 COUNT_t uQueueLength,
									 RZK_PTR_t pMessage,
									 COUNT_t  uMaxSizeOfMessage,
									 RZK_RECV_ATTRIB_et etAttrib
								 );

RZK_STATUS_t RZKDeleteQueue
						 ( 
							 RZK_MESSAGEQHANDLE_t hMessageQueue
						 );

RZK_STATUS_t   RZKSendToQueue
						 ( 
							  RZK_MESSAGEQHANDLE_t hMessageQueue,
							  RZK_PTR_t	 pMessage,
							  COUNT_t uSize,
							  TICK_t tBlockTime
						 );

RZK_STATUS_t RZKSendToQueueFront
						 ( 
			 				  RZK_MESSAGEQHANDLE_t hMessageQueue,
			 				  RZK_PTR_t pMessage,
							  COUNT_t uSize
						 );

RZK_STATUS_t RZKReceiveFromQueue
						 ( 
							  RZK_MESSAGEQHANDLE_t hMessageQueue,
							  RZK_PTR_t pMessage,
							  COUNT_t *uSize,
							  TICK_t tBlockTime
						 );

RZK_STATUS_t RZKGetQueueParameters
						 ( 
			 				  RZK_MESSAGEQHANDLE_t hMessageQueue,
							  RZK_MESSAGEQPARAMS_t	*pQueueParams
						 );

RZK_STATUS_t RZKPeekMessageQueue
							 ( 
								  RZK_MESSAGEQHANDLE_t hMessageQueue,
								  RZK_PTR_t pMessage,
								  COUNT_t *uSize
							 );
RZK_STATUS_t   RZKSendToQueueUnique
						 ( 
							  RZK_MESSAGEQHANDLE_t hMessageQueue,
							  RZK_PTR_t	 pMessage,
							  COUNT_t uSize,
							  TICK_t tBlockTime
						 );

RZK_STATUS_t   ResetQueue
						 ( 
							  RZK_MESSAGEQHANDLE_t hMessageQueue
						 );

#endif /*_ZMESSAGEQ */