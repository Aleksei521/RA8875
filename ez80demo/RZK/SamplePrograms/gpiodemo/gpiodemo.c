/*******************************************************************************************
 * File       : gpiodemo.c
 *
 * Description: This sample program creates the one interrupt thread which 
 *              prints "Port B0 rising edge interrupt" when PB0 interrupt ocured.
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
 ********************************************************************************************/

/** standard header files */
#include <stdio.h>
#include <string.h>

/** RZK header files */
#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZThread.h"
#include "ZMessageQ.h"
#include "ZTimer.h"
#include "ZInterrupt.h"
#include <ez80.h>


/** macros for threads */
#define PRIORITY		2		/** thread priority */
#define PB0_PRIORITY	1		/** thread priority */
#define STACK_SIZE		2048    /** stack size for the thread */
#define RR_TICK			1		/** round robin tick for the schedular */
#ifdef _EZ80F91
#define PORTB_INTRID	0xA0
#else
#define PORTB_INTRID	0x30
#endif
/** extern function declarations */
/** function for initializing the UART port for printing the messages on console */
extern void InitUART(void);
extern void Timer1ISRProlog( void ) ;
extern void pb0_isr(void);
extern void RZKFormatError(UINT num) ;
extern RZK_STATUS_t RZKGetErrorNum( ) ;
extern UINT32 SYSTEM_CLOCK;
/** global variables */
/** thread handles to store */
RZK_THREADHANDLE_t g_hPb0Thread ;

void PB0IntTask()
{
    
	while(1)
	{
		UINTRMASK mInterruptMask;

		mInterruptMask = RZKDisableInterrupts();

		printf("\n Port B0 rising edge interrupt \n");  /* enable PB0 interrupt */
		PB_ALT2 = 0x01;

		/* suspend interrupt thread */
		RZKSuspendInterruptThread();
    	RZKEnableInterrupts(mInterruptMask);

	}
}

void InitPortB( void ) 
{
	RZKInstallInterruptHandler(pb0_isr, PORTB_INTRID);	
  /* Initialize the PB0 pin for raising edge trigger interrupt*/
	PB_DR = 0x01; 
	PB_DDR = 0x01;
	PB_ALT1 = 0x01;
	PB_ALT2 = 0x01;
	return ;
}
/* RZK application entry */
void RZKApplicationEntry(void)
{
	
	InitUART();		/** Initialize the UART port to print the messages on console */

	printf("\nDemo program to demo GPIO Interrupt operation." ) ; 

	printf("\nPress PB0 button on the board" ) ; 

	g_hPb0Thread = RZKCreateThreadEnhanced(( RZK_NAME_t *) "PB0IT",
					(RZK_PTR_t)PB0IntTask,
					NULL,
					STACK_SIZE,
					PB0_PRIORITY,
					0, 
					RZK_THREAD_PREEMPTION | RZK_THREAD_INTERRUPT,0  ) ; 
	/* validate the thread handle */
	if( g_hPb0Thread == NULL )
	{
		printf("\nUnable to create the timer thread , error description is"); 
		RZKFormatError( RZKGetErrorNum() ) ;
		return ;
	}

	InitPortB() ;
}


INT main( INT argc, void *argv[] )
{
	RZK_KernelInit() ;

	RZKApplicationEntry() ;

	RZK_KernelStart() ;

	return 0 ;
}
