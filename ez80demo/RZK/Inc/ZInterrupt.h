/**************************************************************************************
 * File       : ZInterrupt.h
 *
 * Description: Provides the declaration of RZK's Interrupt handling APIs.
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
 *************************************************************************************/

#ifndef _ZINTERRUPT
#define _ZINTERRUPT

typedef struct RZK_INTERRUPT_CB
{
    UINT8 CB[RZK_INTERRUPT_CB_SIZE] ;
} RZK_INTERRUPT_CB_t;

/********************************************
*	syntax for Function Prototype:
*	<return type> <function name>
*  (<type><argument name>)
*****************************************/

#ifndef RZK_ZNEO
RZK_FNP_ISR RZKInstallInterruptHandler
							(
								RZK_FNP_ISR pHandlerFunc,
								RZK_INTERRUPT_NUM_t nInterruptNum
							);
#else

#define RZKInstallInterruptHandler SET_VECTOR

#endif

void RZKEnableInterrupts
							  (
									UINTRMASK mInterruptMask
							  );

UINTRMASK RZKDisableInterrupts();

void RZKISRProlog(void) ;
void RZKISREpilog(void) ;

#endif  /* _ZINTERRUPT */