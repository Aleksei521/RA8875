/********************************************************************************
 * File       : ZTimer.h
 *
 * Description: Provides the declaration of RZK Software Timer structures
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
 *********************************************************************************/

#ifndef _ZTIMER
#define _ZTIMER

extern UINT RZK_SYSTICKS_INSEC ;

/* STRUCTURE FOR TIMER PARAMETERS */	
typedef struct TIMERPARAMS_STRUCT_t	
{
	TICK_t tInitialDelay;            /*  Specifies the initial delay */
	TICK_t tPeriod;                  /*  Specifies the period */
	
} RZK_TIMERPARAMS_t;


#define RZKGetTimerResolution() RZK_SYSTICKS_INSEC

/* STRUCTURE FOR TCB MEMORY ALLOCATION */

typedef struct RZK_TIMER_CB
{
    UINT8 CB[RZK_TIMER_CB_SIZE] ;
} RZK_TIMER_CB_t;

/********************************************
*	syntax for Function Prototype:
*	<return type> <function name>
*  (<type><argument name>)
*****************************************/

RZK_TIMERHANDLE_t	RZKCreateTimer
			( 
			RZK_NAME_t  szName[MAX_OBJECT_NAME_LEN],									
			FNP_TIMER_FUNCTION pTimerFunction,
			TICK_t tInitialDelay,
			TICK_t tPeriod
			);

RZK_STATUS_t	RZKDeleteTimer
			( 
			 RZK_TIMERHANDLE_t hTimer
			 );

RZK_STATUS_t	RZKEnableTimer
			(
			 RZK_TIMERHANDLE_t hTimer
			 );

RZK_STATUS_t	RZKDisableTimer
			( 
			 RZK_TIMERHANDLE_t hTimer
			 );

RZK_STATUS_t	RZKGetTimerParameters
			( 
			 RZK_TIMERHANDLE_t hTimer,
			 RZK_TIMERPARAMS_t *pTimerParams
			);

#endif  /* _ZTIMER */
