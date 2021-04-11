/**************************************************************************************
 * File       : ZThreadStatistics.h
 *
 * Description: Provides the declaration of RZK Thread Statistics structure
 * and API.
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
 **********************************************************************************/

#ifndef _ZTHREADSTATISTICS
#define _ZTHREADSTATISTICS

typedef struct RZK_THREADSTATISTICS
{
	TICK_t tTotalTimeRun;
	TICK_t tActualTimeRun;
	COUNT_t nNumTimesBlocked;
} RZK_THREADSTATISTICS_t; 




/********************************************
*	syntax for Function Prototype:
*	<return type> <function name>
*  (<type><argument name>)
*****************************************/

RZK_STATUS_t RZKGetThreadStatistics
						(
						   RZK_THREADHANDLE_t pThread, 
						   RZK_THREADSTATISTICS_t *pThreadStatistics
						);

#endif