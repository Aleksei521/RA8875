/*************************************************************************************
 * File       : ZClock.h
 *
 * Description: Provides declarations for the clock parameters structure 
 * and Clock APIs
 *              
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
 ***********************************************************************************/

#ifndef _ZCLOCK
#define _ZCLOCK

#include "ZTypes.h"	
/********************************************
*	syntax for Function Prototypes:
*	<return type> <function name>
*  (<type><argument name>)
*****************************************/
/* STRUCTURE FOR CLOCK PARAMETERS */
typedef struct CLOCKPARAMS_t
{
	UINT	uCurr_Year;            
	UINT	uCurr_Month;           
	UINT	uCurr_Date;            
	UINT16  uCurr_Hour; 
	UINT16  uCurr_Minute; 
	UINT16  uCurr_Seconds; 
}RZK_CLOCKPARAMS_t;	


void RZKSetClock
		(
		 RZK_CLOCKPARAMS_t *pClockParams
		);

void RZKGetClock
		(		 
		 RZK_CLOCKPARAMS_t *pClockParams
		);

#endif   /* _ZCLOCK */
