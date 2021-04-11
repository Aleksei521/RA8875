/**********************************************************************************
 * File       : MiniInterruptDemo.c
 *
 * Description: This sample program is an Mini interrupt demo program
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

/** standard header files */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


/** RZK header files */
#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZThread.h"
#include "ZInterrupt.h"
#include <ez80.h>

/** macros */
/** macros for threads */
#define PRIORITY		20		/** thread priority */
#define PB_PRIORITY		10		/** thread priority */
#define STACK_SIZE		1024   /** stack size for the thread */
#define RR_TICK			1		/** round robin tick for the schedular */

/* Port B interrupt vectors for the various eZ80 and eZ80Acclaim! devices */
//#ifdef EVB_F91_MINI
#ifdef _EZ80F91
#define PB3_IVECT 0xAC
#endif
//#endif

/** extern function declarations */
/** function for initializing the UART port for printing the messages on console */
extern void InitUART(void);
extern void Pb3_isr_prolog( void ) ;

/** global variables */
/** thread handles to store */
RZK_THREADHANDLE_t g_hthd1 ;
RZK_THREADHANDLE_t g_hpb3 ;

/*****************************************************************************
 *  Buttons initialization routine
 *  configure buttons for input (falling edge interrupt)
 ****************************************************************************/
void buttons_init( void )
{
    UINT8 pb_tmp;
	UINTRMASK intr_mask ;

	intr_mask = RZKDisableInterrupts();
    /* set port B 3 interrupt vector */
    RZKInstallInterruptHandler( Pb3_isr_prolog, PB3_IVECT );
	
	
/* Initialize the PB0 pin for raising edge trigger interrupt @modified by Venkat */

	PB_DR = 0x08; 
	PB_DDR = 0x08;
	PB_ALT1 = 0x08;
	PB_ALT2 = 0x08;

	RZKEnableInterrupts( intr_mask );

}

void ButtonThreadEntry( void )
{
	UINT cnt = 1 ;
	while(1)
	{
		printf("\nYou have pressed the TEST button %d time\n", cnt);
		printf("\nPress <TEST> button for interrupt generation\n");
		cnt++;

		/* enable PB0 interrupt @Added by Venkat */
		PB_ALT2 = 0x08;

		RZKSuspendThread( RZKGetCurrentThread(), INFINITE_SUSPEND);
	}

	return ;
}



/** Thread entry function for thread1 */
void MiniInterruptDemoMainThread()
{
	RZK_STATUS_t status ;
	UINT uyear ;

	printf("\nStarted Main thread\n");

	g_hpb3 = RZKCreateThreadEnhanced( ( UINT8 *) "SETTIME",
					(RZK_PTR_t)ButtonThreadEntry,
					NULL,
					STACK_SIZE,
					PB_PRIORITY,
					RR_TICK, 
					RZK_THREAD_PREEMPTION,0 ) ; 
	/** validate the thread handle */
	if( g_hpb3 == NULL )
	{
		printf("\nUnable to create thread for <PB0> button"); 
		return;
	}

	printf("\nPress <TEST> button for interrupt generation");

	buttons_init() ;
	printf("\n");
	while(1) 
	{
		// Do nothing
	}
}





INT main( INT argc, void *argv[] )
{
	RZK_STATUS_t status ;
	RZK_KernelInit() ;

	/** Initialize the UART port to print the messages on console */
	InitUART();

	printf("\nStarting to create RZK threads" ) ; 

	/**
	 * create two threads with the same priority and the attributes of the 
	 * the thread as round robin mode
	 */
	printf("\nCreating thread #1" ) ;
	g_hthd1 = RZKCreateThreadEnhanced( ( UINT8 *) "Thread1",
					(RZK_PTR_t)MiniInterruptDemoMainThread,
					NULL,
					STACK_SIZE,
					PRIORITY,
					RR_TICK, 
					RZK_THREAD_AUTOSTART | RZK_THREAD_PREEMPTION | RZK_THREAD_ROUNDROBIN,0 ) ; 
	/** validate the thread handle */
	if( g_hthd1 == NULL )
	{
		printf("\nUnable to create the threads"); 
		return 0;
	}

	RZK_KernelStart() ;

	return 0 ;
}


