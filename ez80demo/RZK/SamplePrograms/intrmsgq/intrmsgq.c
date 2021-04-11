/********************************************************************************************
 * File       : intrmsgq.c
 *
 * Description: This sample program creates the one thread which receives the
 *              messages from a timer isr and displays it.
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
#include "ZInterrupt.h"


/** timer related register definitions */
typedef volatile UINT8 __INTIO *PBINTIO;

#ifdef _EZ80F91

#define	tmr1_ctl			(*(PBINTIO)((UINT8)( 0x65 )))
#define	tmr1_rr_l			(*(PBINTIO)((UINT8)( 0x68 )))
#define	tmr1_rr_h			(*(PBINTIO)((UINT8)( 0x69 )))
#define tmr1_ier			(*(PBINTIO)((UINT8)( 0x66 )))
#define	tmr1_cap_ctl        (*(PBINTIO)((UINT8)( 0x6A )))

#define TMR1_IRQ_EOC_EN	     0x01
#define TMRCTL_RLD		     0x02 /** Reload function enabled */
#define TMRCTL_TIM_EN	     0x01 /** timer enabled */

/** Timer CTL register definitions */

#define TMRCTL_BRK_STOP					0x80 /** Timer stops operation during debug break points */
#define TMRCTL_BRK_NO_STOP				0x00 /** Timer does not stops operation during debug break points */
#define TMRCTL_MODE_CONT				0x04 /** Continuous mode */
#define TMRCTL_MODE_SINGLE				0x00 /** Single mode */
#define TMRCTL_CLK_DIV4					0x00 /** System clock divider = 4 */
#define TMRCTL_CLK_DIV16				0x08 /** System clock divider = 16 */
#define TMRCTL_CLK_DIV64				0x10 /** System clock divider = 64 */
#define TMRCTL_CLK_DIV256				0x18 /** System clock divider = 256 */
#define TMRCTL_RLD_ENABLE				0x02 /** Reload function enabled */
#define TMRCTL_RLD_DISABLE				0x00 /** Reload function disabled */
#define TMRCTL_TIM_ENABLE				0x01 /** timer enabled */
#define TMRCTL_TIM_DISABLE				0x00 /** timer disabled */
#define TMR1_CLK_SEL					0x00
#define TMRIER_IRQ_EOC_EN		        0x01 /** Interrupt on end count enabled */
#define TMRCTL_CLK_SEL_SCLK	            0x00 /** Source is master clock divided by the prescalar */
#define TMRCTL_TIM_CONT			        0x04 /** Continuous mode */

/** other constants */
#define TIMER1_INTRID					(0x58)
#define TIMER1_CLOCK_DIVISOR			256
#define TMRCAP_EDGE_A_DISABLE			0x00
#define TMRCAP_EDGE_B_DISABLE			0x00

#else

	#define TMR1_CONT		((PBINTIO) 0x0083)  /* TIMER CONTROL REGISTER */
	#define TMR1_RLL		((PBINTIO) 0x0084)  /* TIMER0 RELOAD LOW REGISTER */
	#define TMR1_RLH		((PBINTIO) 0x0085)  /* TIMER0 RELOAD HIGH REG.    */
	
	#define tmr1_cont		(TMR1_CONT[0])
	#define tmr1_rll		(TMR1_RLL[0])
	#define tmr1_rlh		(TMR1_RLH[0])

	#ifdef _EZ80190	
		#define	TIMER1_CLOCK_DIVISOR	16
		#define	TIMER1_INTRID			(0x08) 
		#define	CLK_DIV_16				0x0C
	#else
		#define	TIMER1_CLOCK_DIVISOR	256
		#define	TIMER1_INTRID			(0x0C) 
		#define	CLK_DIV_256				0x0C
	#endif
/** F91 port */
#endif

#define	TIMER1_ENABLE			0x01
#define	TIMER1_INTR_ENABLE		0x40
#define	TIMER1_DISABLE_DEBUG	0x20
#define	TIMER1_SM				0X10
#define	TIMER1_LDRST			0X02
#define	CLOCK_DIVISOR			8
   
/* TIMER CONSTANTS */
#define	TIMER1_INIT_DELAY		0 
#define	TIMER1_TICKS			5
#define	TIMER2_INIT_DELAY		5
#define	TIMER2_TICKS			10

/** macros */
/** macros for threads */
#define PRIORITY			2		/** thread priority */
#define TIMER1_PRIORITY		1		/** thread priority */
#define STACK_SIZE			2048    /** stack size for the thread */
#define RR_TICK				1		/** round robin tick for the schedular */

/** macros for message queues */
#define MAX_MSGQ_SIZE		10L
#define MAX_MESSAGE_SIZE	25L

/** extern function declarations */
/** function for initializing the UART port for printing the messages on console */
extern void InitUART(void);
extern void Timer1ISRProlog( void ) ;
extern void RZKFormatError(UINT num) ;
extern RZK_STATUS_t RZKGetErrorNum( ) ;
extern UINT32 SYSTEM_CLOCK;

/** global variables */
/** thread handles to store */
RZK_THREADHANDLE_t g_hthd1 ;
RZK_THREADHANDLE_t g_hTimer1Thread ;

/** message queue related global variables */
RZK_MESSAGEQHANDLE_t g_hmsgq1 ;

/**
 * buffer that is need to be passed to RZK to store the messages
 * received. Buffer size should be equals to ( MAX_MSGQ_SIZE * MAX_MESSAGE_SIZE 
 * added to ( MAX_MSGQ_SIZE * sizeof( void * ) ).
 * please see the RZK API manual.
 */
UINT8 g_buf1[ MAX_MSGQ_SIZE * MAX_MESSAGE_SIZE + ( MAX_MSGQ_SIZE * sizeof(COUNT_t) ) ] ;

/* Timer1 initialization */
void InitTimer1( void ) 
{
	UINT timer_val;

	RZKInstallInterruptHandler((RZK_FNP_ISR)Timer1ISRProlog, TIMER1_INTRID);	
	/** some random value */
	timer_val = 0xA000 ; 
#ifdef _EZ80F91
	tmr1_rr_l = timer_val;
	tmr1_rr_h = timer_val >> 8;
	tmr1_ctl = 0x00;
    tmr1_ctl = ( TMRCTL_BRK_STOP | TMRCTL_MODE_CONT | TMRCTL_CLK_DIV256 | 
				 TMRCTL_RLD_ENABLE | TMRCTL_TIM_ENABLE | TMR1_CLK_SEL ) ;
	
	tmr1_ier  = TMR1_IRQ_EOC_EN ;
	
#else	
	tmr1_rll = timer_val;
	tmr1_rlh = timer_val >> 8;

	#ifdef _EZ80190
		tmr1_cont |= (  TIMER1_ENABLE |TIMER1_INTR_ENABLE |CLK_DIV_16|TIMER1_LDRST|TIMER1_SM|TIMER1_DISABLE_DEBUG);
	#else
		tmr1_cont |= (  TIMER1_ENABLE |TIMER1_INTR_ENABLE |CLK_DIV_256|TIMER1_LDRST|TIMER1_SM|TIMER1_DISABLE_DEBUG);
	#endif
#endif
}

void Timer1ISR( void )
{
	RZK_STATUS_t status ;
	INT8 msg[ MAX_MESSAGE_SIZE ] = { 0 };
	static COUNT_t counter = 0 ;
	COUNT_t size ;

	memcpy(msg,0,MAX_MESSAGE_SIZE);
	sprintf( &msg[0], "Message number : %d", counter ) ;
	counter++ ;
	size = strlen( msg ) ;

	status = RZKSendToQueue( g_hmsgq1, 
								&msg[0],
								size,
								INFINITE_SUSPEND ) ;
	if( status != RZKERR_SUCCESS )
	{
		printf("\n\tUnable to send the message to thread #1: error descr is" ) ;
		RZKFormatError( status ) ;
		return ;
	}

	return ;
}

void Timer1ISREntry()
{
	while(1)
	{
		UINTRMASK mInterruptMask;

		Timer1ISR();

		mInterruptMask = RZKDisableInterrupts();

		/* Enable timer interrupt */
#ifdef _EZ80F91
		asm("IN0	A,	(0066H)");
		asm("SET	0, A");
		asm("OUT0	(0066H), A");
#else
		asm("IN0	A,	(0083H)");
		asm("SET	6, A");
		asm("OUT0	(0083H), A");
#endif
		/* suspend interrupt thread */
		RZKSuspendInterruptThread();

    	RZKEnableInterrupts(mInterruptMask);
	}
}

/** Thread entry function for thread1 */
void Thread1Entry()
{
	RZK_STATUS_t status ;
	INT8 msg[ MAX_MESSAGE_SIZE ] = { 0 };
	COUNT_t size ;

	printf("\nStarted Thread1\n");
	while(1) {

		/** wait to receive message from thread #2 */
		size = MAX_MESSAGE_SIZE ;
		printf("\nThread #1: Waiting to receive mesasge from timer ISR" ) ;
		status = RZKReceiveFromQueue( g_hmsgq1, 
									&msg[0],
									&size,
									INFINITE_SUSPEND ) ;
		if( status != RZKERR_SUCCESS )
		{
			printf("\n\tUnable to receive the message from timer ISR: error descr is") ;
			RZKFormatError( status ) ;
			return ;
		}

		printf("\nThread #1: Received message from timer ISR and Message is " ) ;
		printf("\n\t%s", msg ) ;
	}
}


/** RZK application entry */
void RZKApplicationEntry(void)
{

	RZK_STATUS_t status ;

	/** Initialize the UART port to print the messages on console */
	InitUART();

	/** initializing the timer1 */
	printf("\nInitializing the timer 1"); 

	InitTimer1() ;

	printf("\nStarting to create RZK threads" ) ; 

	/**
	 * create two threads with the same priority and the attributes of the 
	 * the thread as round robin mode
	 */
	printf("\nCreating thread #1" ) ;
	g_hthd1 = RZKCreateThreadEnhanced( ( UINT8 *) "Thread1",
					(RZK_PTR_t)Thread1Entry,
					NULL,
					STACK_SIZE,
					PRIORITY,
					RR_TICK, 
					RZK_THREAD_AUTOSTART | RZK_THREAD_PREEMPTION | RZK_THREAD_ROUNDROBIN,0 ) ; 
	/** validate the thread handle */
	if( g_hthd1 == NULL )
	{
		printf("\nUnable to create the thread #1, error description is"); 
		RZKFormatError( RZKGetErrorNum() ) ;
		return ;
	}

	printf("\nCreating timer thread" ) ;

	g_hTimer1Thread = RZKCreateThreadEnhanced(( RZK_NAME_t *) "TIMER1ISR",
					(RZK_PTR_t)Timer1ISREntry,
					NULL,
					STACK_SIZE,
					TIMER1_PRIORITY,
					0, 
					RZK_THREAD_PREEMPTION | RZK_THREAD_INTERRUPT,0  ) ; 
	/** validate the thread handle */
	if( g_hTimer1Thread == NULL )
	{
		printf("\nUnable to create the timer thread , error description is"); 
		RZKFormatError( RZKGetErrorNum() ) ;
		return ;
	}

	/** 
	 * create message queue object with the receiving order as FIFO
	 */
	printf("\nCreating message queue #1" ) ;
	g_hmsgq1 = RZKCreateQueue( ( UINT8 * ) "MSGQ1",
					MAX_MSGQ_SIZE,
					&g_buf1[0],
					MAX_MESSAGE_SIZE,
					RECV_ORDER_FIFO ) ;

	/** validate the thread handle */
	if( g_hmsgq1 == NULL )
	{
		printf("\nUnable to create the message queue #1, error description is");
		RZKFormatError( RZKGetErrorNum() ) ;

		/** now unable to create the thread #2, delete the thread #1 */
		printf("\nDeleting the threads created" ) ;
		status = RZKDeleteThreadEnhanced( g_hthd1 ) ;
		if( status != RZKERR_SUCCESS ) 
		{
			printf("\nCould not able to delete the thread #1, error description is");
			RZKFormatError( status ) ;
		}

		return ;
	}
}


INT main( INT argc, void *argv[] )
{
	RZK_KernelInit() ;

	RZKApplicationEntry() ;

	RZK_KernelStart() ;

	return 0 ;
}
