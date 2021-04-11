/************************************************************************************
 * File       : Timer1ISR.c
 *
 * Description: This is a Timer ISR which peeks into the MQ and sends 
 *				the message to another Queue
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
 ************************************************************************************/

#include <stdio.h>
#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZInterrupt.h"
#include "ZThread.h"
#include "ZMessageQ.h"

#define STACK_SIZE 1024

/** extern variables */
extern void IntThreadEntry( void );
extern RZK_MESSAGEQHANDLE_t hMessagequeue ;
extern void* pMailboxSendMessage[4];
extern UINT Thread9_PendMQ;
extern RZK_MESSAGEQHANDLE_t	hMessageQueue4;
extern RZK_MESSAGEQHANDLE_t	hMessageQueue5;

void Timer1ISR()
{

	COUNT_t msgsize ;
	msgsize = sizeof( pMailboxSendMessage[ 0 ] ) ;
	if((Thread9_PendMQ) && 
		( RZKPeekMessageQueue( hMessageQueue4,
				&pMailboxSendMessage[0],
				&msgsize ) == RZKERR_SUCCESS))
	{
		COUNT_t nNumThreads,uSize = 3;
	
		RZK_MESSAGEQPARAMS_t QueueParams = {0,0,0,0,0,NULL};
		RZK_MESSAGEQPARAMS_t *pQueueParams = &QueueParams;
	
		RZKGetQueueParameters(hMessageQueue5,pQueueParams);
	
		for(nNumThreads = 0 ; nNumThreads < pQueueParams->nNumThreads ; nNumThreads++)
			RZKSendToQueue(hMessageQueue5,&pMailboxSendMessage[ 0 ],uSize,MAX_INFINITE_SUSPEND);
		Thread9_PendMQ = 0;
	}
}