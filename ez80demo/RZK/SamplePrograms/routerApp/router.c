/********************************************************************************************
 * File       : router.c
 *
 * Description: The Router application, as its name suggests, 
 *				simulates a router using relevant RZK objects 
 *				to demonstrate the routing of packets and messages.
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
 *******************************************************************************************/

#include <string.h>
#include <stdio.h>
#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZThread.h"
#include "ZMessageQ.h"
#include "ZSemaphore.h"
#include "ZMemory.h"
#include "ZTimer.h"
#include "ZInterrupt.h"
#include "ZEventgroup.h"
#include "ZDevice.h"
#include "ZRegion.h"

extern void Toggle( void ) ;
extern void Timer1ISREntry( void );
extern void blkcopy(UINT8 *,UINT8 *,RZK_LENGTH_t);
typedef volatile UINT8 __INTIO *PBINTIO;	/* ON CHIP I/O */

/** F91 port */
#ifdef _EZ80F91

	#define	tmr1_ctl			(*(PBINTIO)( (UINT8)( 0x65 )))
	#define	tmr1_ier			(*(PBINTIO)( (UINT8)( 0x66 )))
	#define	tmr1_iir			(*(PBINTIO)( (UINT8)( 0x67 )))
	#define	tmr1_rr_l			(*(PBINTIO)((UINT8)( 0x68 )))
	#define	tmr1_rr_h			(*(PBINTIO)((UINT8)( 0x69 )))
	
	#define	tmr1_cap_ctl		(*(PBINTIO)((UINT8)( 0x6A )))
	#define	tmr1_capa_l			(*(PBINTIO)((UINT8)( 0x6B )))
	#define	tmr1_capa_h			(*(PBINTIO)((UINT8)( 0x6C )))
	#define	tmr1_capb_l			(*(PBINTIO)((UINT8)( 0x6D )))
	#define	tmr1_capb_h			(*(PBINTIO)((UINT8)( 0x6E )))
	
	/** timer control register */
	#define TMRCTL_BRK_STOP				0x80 /** Timer stops operation during debug break points */
	#define TMRCTL_CLK_SEL_SCLK			0x00 /** Source is master clock divided by the prescalar */
	#define TMRCTL_CLK_DIV64			0x10 /** System clock divider = 64 */
	#define TMRCTL_TIM_CONT				0x04 /** Continuous mode */
	#define TMRCTL_RLD					0x02 /** Reload function enabled */
	#define TMRCTL_TIM_EN				0x01 /** timer enabled */
	
	/** timer interrupt enable register */
	#define TMRIER_IRQ_EOC_EN			0x01 /** Interrupt on end count enabled */
	
	/** timer1 capture disable */
	#define TMRCAP_EDGE_A_DISABLE		0x00
	#define TMRCAP_EDGE_B_DISABLE		0x00
	#define TIMER1_CLOCK_DIVISOR		64
	#define	TIMER1_INTRID				(0x58)
	
#else

	#define TMR1_CONT		((PBINTIO) 0x0083)  /* TIMER CONTROL REGISTER */
	#define TMR1_RLL		((PBINTIO) 0x0084)  /* TIMER0 RELOAD LOW REGISTER */
	#define TMR1_RLH		((PBINTIO) 0x0085)  /* TIMER0 RELOAD HIGH REG.    */
	
	#define tmr1_cont		(TMR1_CONT[0])
	#define tmr1_rll		(TMR1_RLL[0])
	#define tmr1_rlh		(TMR1_RLH[0])

	#ifdef _EZ80190	
		#define	TIMER1_CLOCK_DIVISOR	16
		#define	TIMER1_INTRID			(0x0A)
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
   
#define	STACK_SIZE			1024 
#define	STACK_SIZE1			1024
  
#define	MAX_MESSAGE_SIZE		16L
#define	MAX_MESSAGES			16L

#define	TOTAL_BUFFER_SIZE (MAX_MESSAGE_SIZE * MAX_MESSAGES)

/* TIMER CONSTANTS */
#define	TIMER1_INIT_DELAY		0 
#define	TIMER1_TICKS			5
#define	TIMER2_INIT_DELAY		5
#define	TIMER2_TICKS			10

/* BUFFER CONSTANTS */
#define	BUFFER_SIZE			256

/* MEMORY CONSTANTS */
#define	MAX_MEM_POOLS		3

#define	MAX_BLOCKS1			7L
#define	MAX_BLOCKS2			4L
#define	MAX_BLOCKS3			5L

/* EVENT CONSTANTS */
#define	WAIT_TO_RESUME		5

/* EVENT GROUP CONSTANTS */
#define	EVENT_GROUP1_MASK	0xffff

/* MESSAGEQ CONSTANTS */
#define	TOTAL_QUEUE_BUFFER_SIZE (MAX_MESSAGES*(MAX_MESSAGE_SIZE + sizeof(COUNT_t)))
#define	QUEUE1BLOCKTIME		20
#define	QUEUE2BLOCKTIME		20

#define NO_OF_MESSAGES  4L
#define SIZE_OF_MESSAGE 3L
#define	TOTAL_QUEUE_BUFFER (NO_OF_MESSAGES*(SIZE_OF_MESSAGE + sizeof(COUNT_t)))

/* THREAD CONSTANTS */
/* PRIORITIES */
#define TIMER1_PRIORITY	1		/** thread priority */
#define	PRIORITY1	11
#define	PRIORITY23	9
#define	PRIORITY456	7
#define	PRIORITY78	5
#define	PRIORITY9	4
#define	PRIORITY10	3
#define	PRIORITY11	10
#define PRIORITY2	2
/* ROUND ROBIN TIME */
#define	ROUNDROBIN1		1
#define	ROUNDROBIN2		1
#define	ROUNDROBIN3		1
#define	ROUNDROBIN4		1
#define	ROUNDROBIN5		1
#define	ROUNDROBIN6		1
#define	ROUNDROBIN7		1
#define	ROUNDROBIN8		1
#define	ROUNDROBIN9		1
#define	ROUNDROBIN10	1
#define	ROUNDROBIN11	1

/* THREAD SUSPENSION TIME */
#define	SUSPENSIONTIME2	10
#define	SUSPENSIONTIME3	15
#define	SUSPENSIONTIME4	35
#define	SUSPENSIONTIME5	35
#define	SUSPENSIONTIME6	35
#define	SUSPENSIONTIME7	35
#define	SUSPENSIONTIME8	35


/** extern variables */
extern void Timer1ISRProlog(void);
extern RZK_THREADHANDLE_t hCurrentThread;
/* THREAD GLOBAL VARIABLES */
UINT Thread9_PendMQ = 0;
UINT PARTITIONS_DELETED = 0;
extern UINT32 SYSTEM_CLOCK;
extern UINT RZK_SYSTICKS_INSEC ;
extern void InitUART(void);
/*******************************************************
	ALL THE HANDLES
********************************************************/

/* THREAD HANDLES */
RZK_THREADHANDLE_t		hThread1;
RZK_THREADHANDLE_t		hThread2;
RZK_THREADHANDLE_t		hThread3;
RZK_THREADHANDLE_t		hThread4;
RZK_THREADHANDLE_t		hThread5;
RZK_THREADHANDLE_t		hThread6;
RZK_THREADHANDLE_t		hThread7;
RZK_THREADHANDLE_t		hThread8;
RZK_THREADHANDLE_t		hThread9;
RZK_THREADHANDLE_t		hThread10;
RZK_THREADHANDLE_t 		g_hTimer1Thread ;


/* QUEUE HANDLES */
RZK_MESSAGEQHANDLE_t		hMessageQueue1;
RZK_MESSAGEQHANDLE_t		hMessageQueue2;
RZK_MESSAGEQHANDLE_t		hMessageQueue3;
RZK_MESSAGEQHANDLE_t		hMessageQueue4;
RZK_MESSAGEQHANDLE_t 		hMessageQueue5;

/* MAILBOX HANDLE */


/* SEMAPHORE HANDLE */
RZK_SEMAPHOREHANDLE_t	hSemaphore1;
RZK_SEMAPHOREHANDLE_t	hSemaphore2;

/* EVENTGROUP HANDLE */
RZK_EVENTHANDLE_t			hEventGroup1;
RZK_EVENTHANDLE_t 			hThreadEvent;

/* TIMER HANDLES */
RZK_TIMERHANDLE_t			hTimer1;
RZK_TIMERHANDLE_t			hTimer2;

/* PARTITION HANDLES */
RZK_PARTITIONHANDLE_t	hPartition[MAX_MEM_POOLS];


/**************************************************************
	 
	   STACK DECLARATION FOR THREADS

***************************************************************/
UINT32							pArgv23[1]; /* arg for thread2 & thread 3 */
UINT32							pArgv456[1]; /* arg for thread4,5 & thread 6 */
UINT32							pArgv78[1]; /* arg for thread7 & thread 8 */
	

/** new msgQ changes */
COUNT_t	g_msgsize ; 

/*****************************************************

		CLEANUP FUNCTION DECLARATION

*****************************************************/
void pCleanupfunc1( void );
void pCleanupfunc9( void );
void pCleanupfunc78( void );

FNP_THREAD_ENTRY Cleanup9[] = {pCleanupfunc9,NULL}; 
FNP_THREAD_ENTRY Cleanup78[] = {pCleanupfunc78,NULL};

/*****************************************************

				ENTRY FUNCTIONS

*****************************************************/
/* Thread entry functions */
void Thread1Entry( void );
void Thread23Entry( void );
void Thread456Entry( void );
void Thread78Entry( void );
void Thread9Entry( void );
void Thread10Entry( void );

/* Timer functions */
void Timer1Entry( void );
void Timer2Entry( void );

void WriteToMemory(INT8 *);
void SortMessage( void );

/*****************************************************

		GLOBAL VARIABLES DECLARED HERE

******************************************************/


INT8	bSignalReceived;
UINT	AppResult;
INT32	nAppEntryCount = 0;
INT8	Sourcebuffer[TOTAL_BUFFER_SIZE+1];
INT8	Destibuffer[TOTAL_BUFFER_SIZE+1];
INT8* pBufferRead;
INT8* pBufferEnd;
INT8* pBufferWrite;
INT8	QueueID=0;
INT8	memid = 0;

/* buffers for partitions */
INT8	pMemory1[MAX_BLOCKS1 * (MAX_MESSAGE_SIZE+sizeof(RZK_PTR_t))];
INT8	pMemory2[MAX_BLOCKS2 * (MAX_MESSAGE_SIZE+sizeof(RZK_PTR_t))];
INT8	pMemory3[MAX_BLOCKS3 * (MAX_MESSAGE_SIZE+sizeof(RZK_PTR_t))];

INT8	*pMemPtr[MAX_MESSAGES];
RZK_PARTITIONHANDLE_t hMemPart[MAX_MESSAGES];


INT8	pMessage1[MAX_MESSAGE_SIZE];
INT8	pMessage23[MAX_MESSAGE_SIZE+1];	
INT8	pMessage4[MAX_MESSAGE_SIZE+1];	
INT8	pMessage5[MAX_MESSAGE_SIZE+1];
INT8	pMessage6[MAX_MESSAGE_SIZE+1];
INT8	pMessage7[MAX_MESSAGE_SIZE+1];
INT8	pMessage8[MAX_MESSAGE_SIZE+1];
INT8	pMessage9[MAX_MESSAGE_SIZE];
INT8	pMessage10[MAX_MESSAGE_SIZE];
INT8	pMessageCommon[MAX_MESSAGE_SIZE];

/* buffers for message queues */
INT8	pMessageQbuff1[TOTAL_QUEUE_BUFFER_SIZE];
INT8	pMessageQbuff2[TOTAL_QUEUE_BUFFER_SIZE];
INT8	pMessageQbuff3[TOTAL_QUEUE_BUFFER_SIZE];
INT8	pMessageQbuff4[TOTAL_QUEUE_BUFFER_SIZE];
INT8	pMessageQbuff5[TOTAL_QUEUE_BUFFER];


void* pMailboxMessage[4];
void* pMailboxSendMessage[4];

UINTRMASK mInterruptMask;

/* Timer1 initialization */
void Timer1Init( void )
{
	static UINT nCount = 0;
	UINT timer_val;
	if(nCount == 0)
	{
		RZKInstallInterruptHandler((RZK_FNP_ISR)Timer1ISRProlog, TIMER1_INTRID);	
	
		timer_val = SYSTEM_CLOCK / (TIMER1_CLOCK_DIVISOR * RZK_SYSTICKS_INSEC);
		
	#ifdef _EZ80F91
		tmr1_rr_l = timer_val; 
		tmr1_rr_h = timer_val >> 8 ;
	
		/** no capture functionality for timer1 */
		tmr1_cap_ctl = TMRCAP_EDGE_A_DISABLE | TMRCAP_EDGE_A_DISABLE ;
	
		/** write into tmr_ctl register */
		tmr1_ctl = ( TMRCTL_BRK_STOP | TMRCTL_CLK_SEL_SCLK | TMRCTL_CLK_DIV64 | TMRCTL_TIM_CONT |
								TMRCTL_RLD | TMRCTL_TIM_EN ) ;
	
		/** enable timer interrupts */
		tmr1_ier = TMRIER_IRQ_EOC_EN ;
	#else	
		tmr1_rll = timer_val;
		tmr1_rlh = timer_val >> 8;
	
		#ifdef _EZ80190
			tmr1_cont |= (  TIMER1_ENABLE |TIMER1_INTR_ENABLE |CLK_DIV_16|TIMER1_LDRST|TIMER1_SM|TIMER1_DISABLE_DEBUG);
		#else
			tmr1_cont |= (  TIMER1_ENABLE |TIMER1_INTR_ENABLE |CLK_DIV_256|TIMER1_LDRST|TIMER1_SM|TIMER1_DISABLE_DEBUG);
		#endif
	#endif
		g_hTimer1Thread = RZKCreateThreadEnhanced(
		                                ( RZK_NAME_t *) "TIMER1ISR",
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
	}
	else
	{
		if(!(nCount % 50))
			RZK_SYSTICKS_INSEC = 100;
		else if(!(nCount % 20))
			RZK_SYSTICKS_INSEC = 200;
		else if(!(nCount % 10))
			RZK_SYSTICKS_INSEC = 500;
			
		timer_val = (SYSTEM_CLOCK / (TIMER1_CLOCK_DIVISOR * RZK_SYSTICKS_INSEC)) + nCount;
		if(timer_val == 0xFFFF)
			nCount = 0;
	}

	nCount++;
}


/****************************************************************************/

/* APPLICATION ENTRY */
void RZKApplicationEntry( void )
{
	InitUART();

	hThread10 = RZKCreateThreadEnhanced( ( RZK_NAME_t * ) "Thread10",
			(RZK_PTR_t)Thread10Entry,
			(FNP_THREAD_ENTRY *)NULL,
			STACK_SIZE,
			PRIORITY10,
			ROUNDROBIN10,
			RZK_THREAD_ROUNDROBIN|RZK_THREAD_PREEMPTION,0); 

	if(hThread10 == NULL)
		printf("\n Thread10 could not be created  \n");
	
	hThread1 = RZKCreateThreadEnhanced( ( RZK_NAME_t * ) "Thread1",
			(RZK_PTR_t)Thread1Entry,
			(FNP_THREAD_ENTRY *)NULL,
			STACK_SIZE1,
			PRIORITY1,
			ROUNDROBIN1,
			RZK_THREAD_ROUNDROBIN|RZK_THREAD_AUTOSTART|RZK_THREAD_PREEMPTION,0);

	if(hThread1 == NULL)
		printf("\n Thread1 could not be created \n");

}



/* ENTRY FUNCTION FOR THREAD 1 */
void Thread1Entry(void)
{
	INT i,j;

	
	UINT roundrobin2 = ROUNDROBIN2;
	UINT roundrobin3 = ROUNDROBIN3;
	UINT roundrobin4 = ROUNDROBIN4;
	UINT roundrobin5 = ROUNDROBIN5;
	UINT roundrobin6 = ROUNDROBIN6;
	UINT roundrobin7 = ROUNDROBIN7;
	UINT roundrobin8 = ROUNDROBIN8;
	UINT roundrobin9 = ROUNDROBIN9;
	UINT timer1_ticks = TIMER1_TICKS;
	UINT timer2_ticks = TIMER2_TICKS;

	for(;;) 
	{
		/* Vary round robin timer ticks/sw timer ticks after every iteration */
		Timer1Init();
		roundrobin2+=ROUNDROBIN2;
		roundrobin3+=ROUNDROBIN3;
		roundrobin4+=ROUNDROBIN4;
		roundrobin5+=ROUNDROBIN5;
		roundrobin6+=ROUNDROBIN6;
		roundrobin7+=ROUNDROBIN7;
		roundrobin8+=ROUNDROBIN8;
		roundrobin9+=ROUNDROBIN9;

		if(roundrobin2 == ROUNDROBIN2 * 100)
			roundrobin2 = roundrobin3 = roundrobin4 = roundrobin5 = roundrobin6 = roundrobin7 = roundrobin8 = roundrobin9 = 1;

		timer1_ticks += TIMER1_TICKS;
		timer2_ticks += TIMER2_TICKS;

		if(timer1_ticks == TIMER1_TICKS * 100)
			timer1_ticks = TIMER1_TICKS;

		if(timer2_ticks == TIMER2_TICKS * 100)
			timer2_ticks = TIMER2_TICKS;

		PARTITIONS_DELETED = 0;
		printf("\nprocessing.......\n");


		/* create message queues */	
		hMessageQueue1 = RZKCreateQueue( ( RZK_NAME_t * ) "Queue1",
				MAX_MESSAGES,
				pMessageQbuff1,
				MAX_MESSAGE_SIZE,
				RECV_ORDER_PRIORITY);

		if(hMessageQueue1 == NULL)
			RZKFormatError(RZKGetErrorNum());
	
		hMessageQueue2 = RZKCreateQueue( ( RZK_NAME_t * ) "Queue2",
				MAX_MESSAGES,
				pMessageQbuff2,
				MAX_MESSAGE_SIZE,
				RECV_ORDER_FIFO);

		if(hMessageQueue2 == NULL)
			RZKFormatError(RZKGetErrorNum());

		hMessageQueue3 = RZKCreateQueue( ( RZK_NAME_t * ) "Queue3",
				MAX_MESSAGES,
				pMessageQbuff3,
				MAX_MESSAGE_SIZE,
				RECV_ORDER_FIFO);

		if(hMessageQueue3 == NULL)
			RZKFormatError(RZKGetErrorNum());

	
		hMessageQueue4 = RZKCreateQueue( ( RZK_NAME_t * ) "Queue4",
				MAX_MESSAGES,
				pMessageQbuff4,
				MAX_MESSAGE_SIZE,
				RECV_ORDER_FIFO);	

		if(hMessageQueue4 == NULL)
			RZKFormatError(RZKGetErrorNum());

	/* create mailbox */
		hMessageQueue5 = RZKCreateQueue( ( RZK_NAME_t * ) "Queue5",
				NO_OF_MESSAGES,
				pMessageQbuff5,
				SIZE_OF_MESSAGE,
				RECV_ORDER_FIFO);	

		if(hMessageQueue5 == NULL)
			RZKFormatError(RZKGetErrorNum());

	/* create binary semaphores */
		hSemaphore1 = RZKCreateSemaphore( ( RZK_NAME_t * ) "Semaphore1",
				1,
				RECV_ORDER_FIFO);

		if(hSemaphore1 == NULL)
			RZKFormatError(RZKGetErrorNum());


		hSemaphore2 = RZKCreateSemaphore( ( RZK_NAME_t * ) "Semaphore2",
				1,
				RECV_ORDER_FIFO);

		if(hSemaphore2 == NULL)
			RZKFormatError(RZKGetErrorNum());

		
	/* create eventgroup */
		hEventGroup1 = RZKCreateEventGroup( ( RZK_NAME_t * ) "Eventgroup1",
				EVENT_GROUP1_MASK);

		if(hEventGroup1 == NULL)
			RZKFormatError(RZKGetErrorNum());

	/* create timers */
		hTimer1 = RZKCreateTimer( ( RZK_NAME_t * ) "Timer1",
				Timer1Entry,
				TIMER1_INIT_DELAY,
				timer1_ticks);

		if(hTimer1 == NULL)
			RZKFormatError(RZKGetErrorNum());

		hTimer2 = RZKCreateTimer( ( RZK_NAME_t * ) "Timer2",
				Timer2Entry,
				TIMER2_INIT_DELAY,
				timer2_ticks);

		if(hTimer2 == NULL)
			RZKFormatError(RZKGetErrorNum());


	/* create partition */
		hPartition[0] =  RZKCreatePartition( ( RZK_NAME_t * ) "Partition1",
				pMemory1,
				MAX_BLOCKS1,
				MAX_MESSAGE_SIZE);

		if(hPartition[0] == NULL)
			RZKFormatError(RZKGetErrorNum());

		hPartition[1] =  RZKCreatePartition( ( RZK_NAME_t * ) "Partition2",
				pMemory2,
				MAX_BLOCKS2,
				MAX_MESSAGE_SIZE);

		if(hPartition[1] == NULL)
			RZKFormatError(RZKGetErrorNum());

		hPartition[2] =  RZKCreatePartition( ( RZK_NAME_t * ) "Partition3",
				pMemory3,
				MAX_BLOCKS3,
				MAX_MESSAGE_SIZE);

		if(hPartition[2] == NULL)
			RZKFormatError(RZKGetErrorNum());

		bSignalReceived = RZK_FALSE;

		memset(Sourcebuffer,'0',TOTAL_BUFFER_SIZE);
		memset(Destibuffer,'0',TOTAL_BUFFER_SIZE);

	/* initialise the source buffer with 256 bytes of data */
		pBufferEnd = Sourcebuffer + TOTAL_BUFFER_SIZE;
		pBufferRead = Sourcebuffer;
		pBufferWrite = Destibuffer;


		j=0;
		for(i=0;i < MAX_MESSAGES;i++)
		{
			Sourcebuffer[j] = i+'0';
			j++;
			blkcopy( (UINT8 * ) Sourcebuffer+j, (UINT8*)"RZK  REAL  TIME",MAX_MESSAGE_SIZE-1);
			j += (MAX_MESSAGES-1);
		}
		j++;
		blkcopy((UINT8*)Sourcebuffer+j,(UINT8*)'\0',1);
		
			/* create threads */
		pArgv23[0] = 2;
		hThread2 = RZKCreateThreadEnhanced( ( RZK_NAME_t * ) "Thread2",
				(RZK_PTR_t)Thread23Entry,
				(FNP_THREAD_ENTRY *)NULL,
				STACK_SIZE,
				PRIORITY23,
				roundrobin2,
				RZK_THREAD_ROUNDROBIN|RZK_THREAD_AUTOSTART|RZK_THREAD_PREEMPTION,0);

		if(hThread2 == NULL)
			RZKFormatError(RZKGetErrorNum());
		
		pArgv23[0] = 3;
		hThread3 = RZKCreateThreadEnhanced( ( RZK_NAME_t * ) "Thread3",
				(RZK_PTR_t)Thread23Entry,
				(FNP_THREAD_ENTRY *)NULL,
				STACK_SIZE,
				PRIORITY23,
				roundrobin3,
				RZK_THREAD_ROUNDROBIN|RZK_THREAD_AUTOSTART|RZK_THREAD_PREEMPTION,0);

		if(hThread3 == NULL)
			RZKFormatError(RZKGetErrorNum());
		
		pArgv456[0] = 4;
		hThread4=RZKCreateThreadEnhanced( ( RZK_NAME_t * ) "Thread4",
				(RZK_PTR_t)Thread456Entry,
				(FNP_THREAD_ENTRY *)NULL,
				STACK_SIZE,
				PRIORITY456,
				roundrobin4,
				RZK_THREAD_ROUNDROBIN|RZK_THREAD_AUTOSTART|RZK_THREAD_PREEMPTION,0);

		if(hThread4 == NULL)
			RZKFormatError(RZKGetErrorNum());

		pArgv456[0] = 5;
		hThread5 = RZKCreateThreadEnhanced( ( RZK_NAME_t * ) "Thread5",
				(RZK_PTR_t)Thread456Entry,
				(FNP_THREAD_ENTRY *)NULL,
				STACK_SIZE,
				PRIORITY456,
				roundrobin5,
				RZK_THREAD_ROUNDROBIN|RZK_THREAD_AUTOSTART|RZK_THREAD_PREEMPTION,0);

		if(hThread5 == NULL)
			RZKFormatError(RZKGetErrorNum());

		pArgv456[0] = 6;
		hThread6 = RZKCreateThreadEnhanced( ( RZK_NAME_t * ) "Thread6",
				(RZK_PTR_t)Thread456Entry,
				(FNP_THREAD_ENTRY *)NULL,
				STACK_SIZE,
				PRIORITY456,
				roundrobin6,
				RZK_THREAD_ROUNDROBIN|RZK_THREAD_AUTOSTART|RZK_THREAD_PREEMPTION,0);

		if(hThread6 == NULL)
			RZKFormatError(RZKGetErrorNum());

		pArgv78[0] = 7;
		hThread7 = RZKCreateThreadEnhanced( ( RZK_NAME_t * ) "Thread7",
				(RZK_PTR_t)Thread78Entry,
				Cleanup78,
				STACK_SIZE,
				PRIORITY78,
				roundrobin7,
				RZK_THREAD_ROUNDROBIN|RZK_THREAD_AUTOSTART|RZK_THREAD_PREEMPTION,0);

		if(hThread7 == NULL)
			RZKFormatError(RZKGetErrorNum());

		pArgv78[0] = 8;
		hThread8 = RZKCreateThreadEnhanced( ( RZK_NAME_t * ) "Thread8",
				(RZK_PTR_t)Thread78Entry,
				Cleanup78,
				STACK_SIZE,
				PRIORITY78,
				roundrobin8,
				RZK_THREAD_ROUNDROBIN|RZK_THREAD_AUTOSTART|RZK_THREAD_PREEMPTION,0);

		if(hThread8 == NULL)
			RZKFormatError(RZKGetErrorNum());

		hThread9 = RZKCreateThreadEnhanced( ( RZK_NAME_t * ) "Thread9",
				(RZK_PTR_t)Thread9Entry,
				Cleanup9,
				STACK_SIZE,
				PRIORITY9,
				roundrobin9,
				RZK_THREAD_ROUNDROBIN|RZK_THREAD_AUTOSTART|RZK_THREAD_PREEMPTION,0);

		if(hThread9 == NULL)
			RZKFormatError(RZKGetErrorNum());

		RZKEnableTimer(hTimer1);
		
		RZKEnableTimer(hTimer2);	

		(RZKResumeThread(hThread10));
		
						/* start all other threads created at this point */
		{
			COUNT_t nNumThreads;
			RZK_STATUS_t status;
			RZK_MESSAGEQPARAMS_t QueueParams = {0,0,0,0,0,NULL};
			RZK_MESSAGEQPARAMS_t *pQueueParams = &QueueParams;
			RZKSetThreadPriority(hCurrentThread,PRIORITY2);
			RZKGetQueueParameters(hMessageQueue1,pQueueParams);
			
			for(nNumThreads = 0 ; nNumThreads < pQueueParams->nNumThreads ; nNumThreads++)
			{
			        //printf("\nkickstart by sending message to queue1");
				status = RZKSendToQueue(hMessageQueue1,
				(RZK_PTR_t)"kikstrt",
				sizeof("kikstrt"),
				MAX_INFINITE_SUSPEND);
				
				if(status != RZKERR_SUCCESS)
				RZKFormatError(status);
			}
			
			RZKSetThreadPriority(hCurrentThread,PRIORITY1);
		}
		
				/* pend on mailbox */
		{
			COUNT_t uSize = 3;
			(RZKReceiveFromQueue(hMessageQueue5,pMailboxMessage,
			&uSize,MAX_INFINITE_SUSPEND));
		}
	
		if((memcmp(Sourcebuffer, pMailboxMessage[0],(MAX_MESSAGE_SIZE * MAX_MESSAGES))) ==0 )
			AppResult = RZK_TRUE;
		else
			AppResult = RZK_FALSE;
		
		
		(RZKPostToEventGroup(hThreadEvent,WAIT_TO_RESUME,EVENT_OR));

		hThread10 = RZKCreateThreadEnhanced( ( UINT8 * ) "Thread10",
				(RZK_PTR_t)Thread10Entry,
				(FNP_THREAD_ENTRY *)NULL,
				STACK_SIZE,
				PRIORITY10,
				ROUNDROBIN10,
				RZK_THREAD_ROUNDROBIN|RZK_THREAD_PREEMPTION,0); 
		
		if(hThread10 == NULL)
			printf("\n Thread10 could not be created  \n");
		
			/*Delete Objects*/
		(RZKDeleteQueue(hMessageQueue4));
		(RZKDeleteQueue(hMessageQueue5));
		(RZKDeleteEventGroup(hThreadEvent));
		(RZKDeleteThreadEnhanced(hThread9));
		(RZKDeleteSemaphore(hSemaphore1));
		(RZKDeleteSemaphore(hSemaphore2));

	}/* end of for loop*/
}

/* COMMON ENTRY FUNCTION FOR THREADS 2 & 3 */
void Thread23Entry()
{
	/* for kick starting */
	COUNT_t msgsize = MAX_MESSAGE_SIZE ;

	/** receive the kickstart message */
	

	RZKReceiveFromQueue( hMessageQueue1, 
			pMessageCommon, 
			&msgsize /*MAX_MESSAGE_SIZE*/, 
			MAX_INFINITE_SUSPEND );

	RZKYieldThread();

	while (1)
	{
		
		
		RZKAcquireSemaphore( hSemaphore1, MAX_INFINITE_SUSPEND );
		
	

		if(pBufferRead == pBufferEnd)
		{
        		
			RZKReleaseSemaphore( hSemaphore1 );
			break;
		}

		
		blkcopy( (UINT8*)pMessage23, (UINT8*)pBufferRead, MAX_MESSAGE_SIZE );
		
		pMessage23[ MAX_MESSAGE_SIZE ] = '\0' ;


		pBufferRead += MAX_MESSAGE_SIZE;
		if( QueueID == 0 )
		{
			
			RZKSendToQueue( hMessageQueue1,
					pMessage23,
					MAX_MESSAGE_SIZE,
					QUEUE1BLOCKTIME) ;
		}
		else
		{
			
			RZKSendToQueue( hMessageQueue2,
					pMessage23,
					MAX_MESSAGE_SIZE,
					QUEUE2BLOCKTIME) ;
		}

		
		
		RZKSuspendThread( hCurrentThread, SUSPENSIONTIME2) ;	
		
		RZKReleaseSemaphore( hSemaphore1 );
	}

	bSignalReceived	= RZK_TRUE;
}


/* COMMON ENTRY FUNCTION FOR THREADS 4, 5 & 6 */
void Thread456Entry()
{
	INT8 *pmsg = NULL;
	COUNT_t msgsize = MAX_MESSAGE_SIZE ;	
	COUNT_t msgsize1 = MAX_MESSAGE_SIZE ;	
	COUNT_t msgsize2 = MAX_MESSAGE_SIZE ;	
	
	/* for kick starting */
	/** receive the kickstart message */
	
	RZKReceiveFromQueue( hMessageQueue1,
			pMessageCommon,
			&msgsize /*MAX_MESSAGE_SIZE */,
			MAX_INFINITE_SUSPEND ) ;

	if(hCurrentThread == hThread4) pmsg = pMessage4; 
	if(hCurrentThread == hThread5) pmsg = pMessage5; 
	if(hCurrentThread == hThread6) pmsg = pMessage6; 

	while(bSignalReceived != RZK_TRUE)
	{
		

		msgsize1 = MAX_MESSAGE_SIZE ;
		msgsize2 = MAX_MESSAGE_SIZE ;

                    if( (RZKReceiveFromQueue(hMessageQueue1,pmsg,&msgsize1,QUEUE1BLOCKTIME) == RZKERR_SUCCESS) ||
			(RZKReceiveFromQueue(hMessageQueue2,pmsg,&msgsize2,QUEUE2BLOCKTIME) == RZKERR_SUCCESS))
		{	
		        
			WriteToMemory(pmsg);	
		}

                
		RZKSuspendThread( hCurrentThread, SUSPENSIONTIME4) ;
	} /* end of while */


	/* loop until both queues are empty */

		msgsize1 = MAX_MESSAGE_SIZE ;
		msgsize2 = MAX_MESSAGE_SIZE ;

	while((RZKReceiveFromQueue(hMessageQueue1,pmsg,&msgsize1,0) == RZKERR_SUCCESS)
		  ||(RZKReceiveFromQueue(hMessageQueue2,pmsg,&msgsize2,0) == RZKERR_SUCCESS))
	{
		       
			WriteToMemory(pmsg);	
			msgsize1 = MAX_MESSAGE_SIZE ;
			msgsize2 = MAX_MESSAGE_SIZE ;

	}

}


/* COMMON ENTRY FUNCTION FOR THREADS 7 & 8 */
void Thread78Entry()
{
	INT8 *pmsg = NULL;	
	COUNT_t msgsize ;
	RZK_STATUS_t status ;

	

	/* for kickstart */
	msgsize = MAX_MESSAGE_SIZE ;
	status = RZKReceiveFromQueue( hMessageQueue1,
				pMessageCommon,
				&msgsize /** MAX_MESSAGE_SIZE */,
				MAX_INFINITE_SUSPEND ) ;

	if(hCurrentThread == hThread7) pmsg = pMessage7; 
	if(hCurrentThread == hThread8) pmsg = pMessage8;

	while(1)
	{
		
		msgsize = MAX_MESSAGE_SIZE ;
		status = RZKReceiveFromQueue( hMessageQueue3,
				pmsg,
				&msgsize /** MAX_MESSAGE_SIZE */,
				MAX_INFINITE_SUSPEND ) ;

		if( status != RZKERR_SUCCESS ) {
			
			break ;
		}

		pmsg[ MAX_MESSAGE_SIZE ] = '\0' ;
	
		RZKAcquireSemaphore( hSemaphore2, MAX_INFINITE_SUSPEND ) ;

		blkcopy((UINT8*)pBufferWrite,(UINT8*)pmsg,MAX_MESSAGE_SIZE);
		pBufferWrite += MAX_MESSAGE_SIZE;

                
		RZKPostToEventGroup( hEventGroup1,
				(1 << (*pmsg - '0')), 
				EVENT_OR ) ;
		
                
		RZKSuspendThread( hCurrentThread, SUSPENSIONTIME7) ;

                
		RZKReleaseSemaphore( hSemaphore2 ) ;

	}

} 

/* ENTRY FUNCTION FOR THREAD 9 */
void Thread9Entry()
{	
	INT8 *pptr;
	COUNT_t msgsize ;
	
	msgsize = MAX_MESSAGE_SIZE ;
	RZKReceiveFromQueue( hMessageQueue1,
			pMessageCommon,
			&msgsize /** MAX_MESSAGE_SIZE */,
			MAX_INFINITE_SUSPEND ) ;


	
	RZKPendOnEventGroup( hEventGroup1,
			EVENT_GROUP1_MASK,
			MAX_INFINITE_SUSPEND,
			EVENT_AND ) ;


	
	RZKDeleteThreadEnhanced( hThread7 ) ;
	RZKDeleteThreadEnhanced( hThread8 ) ;

	pBufferRead = Destibuffer;
	pBufferEnd = Destibuffer + BUFFER_SIZE;
	
	
	SortMessage();
	pptr = Destibuffer;
	
       
	RZKSendToQueue( hMessageQueue4, 
			&pptr, 
			sizeof(pptr),
			MAX_INFINITE_SUSPEND ) ;

	{
		COUNT_t uSize = 3;
		(RZKReceiveFromQueue(hMessageQueue5,pMailboxMessage,
		&uSize,MAX_INFINITE_SUSPEND));
	}

}

/* ENTRY FUNCTION FOR THREAD 10 */
void Thread10Entry()
{
	COUNT_t msgsize ;
	hThreadEvent = RZKCreateEventGroup((RZK_NAME_t *)"EventGroup",WAIT_TO_RESUME);

	
	msgsize = MAX_MESSAGE_SIZE ;
	RZKReceiveFromQueue( hMessageQueue1,
			pMessageCommon,
			&msgsize /** MAX_MESSAGE_SIZE */,
			MAX_INFINITE_SUSPEND ) ;

	/* for kick starting */
	

	(RZKPendOnEventGroup( hThreadEvent,WAIT_TO_RESUME,MAX_INFINITE_SUSPEND,(EVENT_AND | EVENT_CONSUME) )) ;
	

	printf("\nSOURCE BUFFER:\n");
	printf("%s\n",Sourcebuffer);
	printf("\nDESTINATION BUFFER:\n");
	printf("\n%s\n",Destibuffer); //commented for time calculation

	printf("Entry count = %d\n", ++nAppEntryCount );
	if(AppResult == RZK_TRUE)
	{
		printf("Application successful \n");
	}
	else
		printf("Application failure\n");

}
 
/* ENTRY FUNCTION FOR INTERRUPT THREAD */

/************************************************************

					GENERAL FUNCTIONS 

************************************************************/


void WriteToMemory(pmsg)
INT8 *pmsg;
{ 
	UINT8 tempMemID = memid;
	RZK_PTR_t pBlock;
	static UINT8 bufidx = 0;
	

	RZKSendToQueueFront( hMessageQueue3, pmsg, MAX_MESSAGE_SIZE ) ;
	for(;;)
	{
		if((pBlock = RZKAllocFixedSizeMemory( hPartition[tempMemID]) ) != NULL)
		{
			if(bufidx == MAX_MESSAGES) bufidx = 0;
			pMemPtr[bufidx] = pBlock;
			hMemPart[bufidx++] = hPartition[tempMemID];
			break;
		}
		tempMemID ++;
		if(tempMemID == MAX_MEM_POOLS)
			tempMemID = 0;
	}
	blkcopy((UINT8*)pBlock, (UINT8*)pmsg, MAX_MESSAGE_SIZE); 
}

void SortMessage( void )
{
	INT i,j;
	INT8 temp[MAX_MESSAGE_SIZE];

	for(i=0;i < ((MAX_MESSAGE_SIZE * MAX_MESSAGES) - MAX_MESSAGE_SIZE);i+=MAX_MESSAGE_SIZE)
	{
		for(j=i+MAX_MESSAGE_SIZE;j<= ((MAX_MESSAGE_SIZE * MAX_MESSAGES) - MAX_MESSAGE_SIZE);j+=MAX_MESSAGE_SIZE)
		{	
			if(Destibuffer[i]>Destibuffer[j])
			{
				blkcopy((UINT8*)temp,(UINT8*)&Destibuffer[i],MAX_MESSAGE_SIZE);
				blkcopy((UINT8*)&Destibuffer[i],(UINT8*)&Destibuffer[j],MAX_MESSAGE_SIZE);
				blkcopy((UINT8*)&Destibuffer[j],(UINT8*)temp,MAX_MESSAGE_SIZE);
			}
		}
	}
}
/**************************************************************************

						TIMER FUNCTIONS 

**************************************************************************/

void Timer1Entry( void )
{
	QueueID ^= 1;
}

void Timer2Entry( void )
{
	memid++;
	if(memid == MAX_MEM_POOLS )
		memid = 0;
}


/****************************************************************

						CLEANUP FUNCTIONS

****************************************************************/

void pCleanupfunc1( void )
{
}

void pCleanupfunc9( void )
{
	/* Delete all the redundant objects */

	(RZKDisableTimer(hTimer1));
	(RZKDisableTimer(hTimer2));

	RZKSuspendThread(hCurrentThread, TIMER2_TICKS + 1);

	(RZKDeleteTimer(hTimer1));
	(RZKDeleteTimer(hTimer2));

	(RZKDeleteQueue(hMessageQueue1));
	(RZKDeleteQueue(hMessageQueue2));
	(RZKDeleteQueue(hMessageQueue3));

	(RZKDeleteEventGroup(hEventGroup1));

	(RZKDeleteThreadEnhanced(hThread2));
	(RZKDeleteThreadEnhanced(hThread3));
	(RZKDeleteThreadEnhanced(hThread4));
	(RZKDeleteThreadEnhanced(hThread5));
	(RZKDeleteThreadEnhanced(hThread6));

}

void pCleanupfunc78( void )
{
	/* FREEING OF MEMORY BLOCKS */
	volatile INT cntmb;

	/* Set the global variable for thread9*/
	Thread9_PendMQ = 1;

	if(!PARTITIONS_DELETED)
	{
		/* Free memory back to memory partitions */
		for (cntmb = 0; cntmb < MAX_MESSAGES; cntmb++)
		{
			/*RZKFormatError*/(RZKFreeFixedSizeMemory(hMemPart[cntmb], pMemPtr[cntmb]));
		}

		/* Delete partitions */
		
		cntmb = 0;
		while( cntmb <  MAX_MEM_POOLS )
		{
			(RZKDeletePartition(hPartition[cntmb]));
			cntmb++;
		}


		PARTITIONS_DELETED = 1;
	}
}

INT main( INT argc, void *argv[] )
{
	RZK_KernelInit() ;

	RZKApplicationEntry() ;

	RZK_KernelStart() ;

	return 0 ;
}

