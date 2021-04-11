/*******************************************************************************
 * File       : ZTimerStatistics.h
 *
 * Description: Provides the declaration of RZK timer statistics structure
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
 ******************************************************************************/

#ifndef _ZTIMERSTATISTICS
#define _ZTIMERSTATISTICS

typedef struct RZK_TIMERSTATISTICS
{
	TICK_t tJitter;
	TICK_t tDrift;
} RZK_TIMERSTATISTICS_t; 


/****************************************
*	syntax for Function Prototype:
*	<return type> <function name>
*  (<type><argument name>)
*****************************************/

RZK_STATUS_t RZKGetTimerStatistics
						(
						   RZK_TIMERHANDLE_t pTimer,
						   RZK_TIMERSTATISTICS_t *pTimerStatistics
						);

#endif