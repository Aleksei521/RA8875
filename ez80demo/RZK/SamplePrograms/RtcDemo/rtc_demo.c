/******************************************************************************************
 * File       : rtc_demo.c
 *
 * Description: This sample program is an RTC driver demo that displays the time
 *              and also provides an alarm.
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
 ******************************************************************************************/

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

#include "rtc.h"

/** macros */
/** macros for threads */
#define PRIORITY		20		/** thread priority */
#define PB_PRIORITY		10		/** thread priority */
#define STACK_SIZE		1024    /** stack size for the thread */
#define RR_TICK			1		/** round robin tick for the schedular */
/** extern function declarations */
/** function for initializing the UART port for printing the messages on console */
extern void InitUART(void);
extern void buttons_init( void );
extern RZK_DEVICE_CB_t *CONSOLE ;

/** global variables */
/** thread handles to store */
RZK_THREADHANDLE_t g_hthd1 ;
RZK_THREADHANDLE_t g_hpb1 ;
RZK_THREADHANDLE_t g_hpb2 ;

RZK_DEVICE_CB_t *g_hrtc_driver ;

// setting up the time for Jul-1-2004, 10:05:04
const TIME g_time_struct = {
	4, // sec
	5, // min
	10, // hrs
	1, // dayofMonth
	4, // dayOfWeek
	7, // mon
	4, // year
	20 // century
	} ;

void PrintTime( TIME *pst_time, UINT8 by_alarm )
{
	UINT uyear ;
	uyear = pst_time->year + ( ( UINT) pst_time->cent * 100 ) ;
	/* display the time */
	if( by_alarm == 0 )
		printf("%2d/%2d/%4d, ",
			pst_time->mon, 
			pst_time->dayOfMonth, 
			uyear ) ;

	printf("%2d:%2d:%2d",
		pst_time->hrs, 
		pst_time->minutes, 
		pst_time->sec ) ;

	printf("  DOW = %d", pst_time->dayOfWeek ) ;

}

void GetAndPrintTime( void )
{
	TIME st_time ;
	/* get the time */
	RZKDevRead( g_hrtc_driver, (RZK_DEV_BUFFER_t*) &st_time, sizeof( TIME ) ) ;
	PrintTime( &st_time, 0 ) ;

}

UINT GetNum( UINT nmax)
{
	UINT num = 0 ;
	UINT ctr = nmax -1 ;
	UINT nmult = 1 ; 
	UINT8 ch ;

	for(; ctr > 0 ; ctr-- )
		nmult *= 10 ;

	while( nmax > 0 )
	{
		RZKDevRead( CONSOLE, (RZK_DEV_BUFFER_t*) &ch, 1 ) ;
		if( isdigit(ch) )
		{
			printf("%c",ch);
			num += nmult * ( ch - '0' ) ;
		    nmax-- ;
			nmult /= 10 ;
		}
	}
	return num;
}


INT8 GetTimeFrmUsr( TIME *pst_time )
{
	UINT sec ;
	UINT min ;
	UINT hrs ;

	pst_time->sec = 0 ;
	pst_time->minutes = 0 ;
	pst_time->hrs = 0 ;

	printf("\nHour (2) : " ) ;
	hrs = GetNum(2) ;
	if( hrs > 23 )
		return 0 ;
	printf("\nMinutes (2) : " ) ;
	min = GetNum(2) ;
	if( min > 59 )
		return 0 ;
	printf("\nSeconds (2) : " ) ;
	sec = GetNum(2) ;
	if( sec > 59 )
		return 0 ;

	pst_time->sec = sec ;
	pst_time->minutes = min ;
	pst_time->hrs = hrs ;

	return 1 ;
}

INT8 GetDateFrmUsr( TIME *pst_time )
{
	UINT day ;
	UINT mon ;
	UINT year ;
	UINT cent ;

	pst_time->dayOfMonth = 0 ;
	pst_time->mon = 0 ;
	pst_time->year = 0 ;
	pst_time->cent = 0 ;

	printf("\nYear (4) : " ) ;
	year = GetNum(4) ;
	if( year > 3000 || year == 0 )
		return 0 ;
	printf("\nMonth (2) : " ) ;
	mon = GetNum(2) ;
	if( mon > 12 || mon == 0 )
		return 0 ;
	printf("\nDay of Month (2) : " ) ;
	day = GetNum(2) ;
	if( !( year % 4 ) || ( year % 400 ) )
	{
		if( mon == 2 && day > 29 )
			return 0 ;
	}
	else
	{
		if( mon == 2 && day > 28 )
			return 0 ;
	}
	if( mon == 1 || 
	    mon == 3 ||
	    mon == 5 ||
	    mon == 7 ||
	    mon == 8 ||
	    mon == 10 ||
	    mon == 12 )
	{
	 	if( day > 31 || day == 0 )
			return 0 ;
	}
	else if( mon == 4 || 
	    mon == 6 ||
	    mon == 9 ||
	    mon == 11 )
	{
	 	if( day > 30 || day == 0 )
			return 0 ;
	}
	pst_time->dayOfMonth = day ;
	pst_time->mon = mon ;
	pst_time->year = ( year % 100 ) ;
	pst_time->cent = year / 100 ;

	return 1 ;
}

INT8 GetTimeDateFrmUsr( TIME *pst_time, UINT8 by_alarm )
{
	UINT day_week ;

	memset( pst_time, 0x00, sizeof( TIME ) ) ;

	if( by_alarm == 0 )
	{
		// for setting date and time
		if( !GetDateFrmUsr( pst_time ) )
			return 0 ;
	}

	printf("\nDay of Week (1) : " ) ;
	day_week = GetNum(1) ;
	if( day_week > 7 )
		return 0 ;
	pst_time->dayOfWeek = day_week ;

	// for setting date and time and as well for alarm
	return GetTimeFrmUsr( pst_time ) ;
}

	
void RtcSetTimeThreadEntry( void )
{
	TIME st_time ;
	while(1)
	{
		RZKSuspendThread( g_hthd1, INFINITE_SUSPEND ) ;
		printf("\nPlease set a new date and Time (enter all digits): " ) ;
		if( GetTimeDateFrmUsr( &st_time, 0 ) )
		{
			printf("\nSetting entered date and time to :");
			PrintTime( &st_time, 0 ) ;
			printf("\nPress <PB0> to set Date & time, <PB1> to set Alarm\n");
			printf("\nCurrent Date and Time is : \n" ) ;
			RZKDevIOCTL( g_hrtc_driver, RTC_RESET_CONTROL, ( void * ) &st_time, ( void * ) &st_time ) ;
			RZKDevIOCTL( g_hrtc_driver, RTC_SET_ALL, ( void * ) &st_time, ( void * ) &st_time ) ;

			RZKResumeThread( g_hthd1 ) ;
			// successfull;
		}
		else
		{
			printf("\n Invalid date and time entered.");
			printf("\nPress <PB0> to set Date & time, <PB1> to set Alarm\n");
			printf("\nCurrent Date and Time is : \n" ) ;
			RZKResumeThread( g_hthd1 ) ;
			// unsuccessfull
		}
		RZKSuspendThread( RZKGetCurrentThread(), INFINITE_SUSPEND);
	}

	return ;
}

void RtcSetAlarmThreadEntry( void )
{
	TIME st_time ;
	while(1)
	{
		RZKSuspendThread( g_hthd1, INFINITE_SUSPEND ) ;
		printf("\nPlease set a new date and Time for the alarm. Note that you can set " ) ;
		printf("\nalarm for a week only. " ) ;
		if( GetTimeDateFrmUsr( &st_time, 1 ) )
		{
			printf("\nSetting the alarm to: ");
			PrintTime( &st_time, 1 ) ;
			printf("\nPress <PB0> to set Date & time, <PB1> to set Alarm\n");
			printf("\nCurrent Date and Time is : \n" ) ;
			RZKDevIOCTL( g_hrtc_driver, RTC_ENABLE_ALARM, ( void * ) &st_time, ( void * ) &st_time ) ;
			RZKResumeThread( g_hthd1 ) ;
			// successfull;
		}
		else
		{
			printf("\nInvalid date and time entered.");
			printf("\nPress <PB0> to set Date & time, <PB1> to set Alarm\n");
			printf("\nCurrent Date and Time is : \n" ) ;
			RZKResumeThread( g_hthd1 ) ;
			// unsuccessfull
		}
		RZKSuspendThread( RZKGetCurrentThread(), INFINITE_SUSPEND);
	}

	return ;
}

/** Thread entry function for thread1 */
void RtcDemoThreadEntry()
{
	RZK_STATUS_t status ;
	UINT uyear ;

	printf("\nStarted RTC Demo Thread\n");
	/* open the RTC driver */
	g_hrtc_driver = RZKDevOpen( "RTC", NULL ) ;
	if( g_hrtc_driver == NULL )
	{
		printf("\nRTC Driver could not be opened.. exiting..");
		return ;
	}

	g_hpb1 = RZKCreateThreadEnhanced( ( UINT8 *) "SETTIME",
					(RZK_PTR_t)RtcSetTimeThreadEntry,
					NULL,
					STACK_SIZE,
					PB_PRIORITY,
					RR_TICK, 
					RZK_THREAD_PREEMPTION,0 ) ; 
	/** validate the thread handle */
	if( g_hpb1 == NULL )
	{
		printf("\nUnable to create thread for <PB0> button"); 
		return;
	}

	g_hpb2 = RZKCreateThreadEnhanced( ( UINT8 *) "ALARM",
					(RZK_PTR_t)RtcSetAlarmThreadEntry,
					NULL,
					STACK_SIZE,
					PB_PRIORITY,
					RR_TICK, 
					RZK_THREAD_PREEMPTION,0 ) ; 
	/** validate the thread handle */
	if( g_hpb2 == NULL )
	{
		printf("\nUnable to create thread for <PB1> button"); 
		return;
	}

	printf("\nCurrent Date and Time is : " ) ;
	GetAndPrintTime() ;
	printf("\nPress <PB0> to set Date & time, <PB1> to set Alarm");

	buttons_init() ;
	printf("\n");
	while(1) 
	{
		GetAndPrintTime();
		RZKSuspendThread( RZKGetCurrentThread(), 50 ) ;
		printf("\r");
	}
}





INT main( INT argc, void *argv[] )
{
	RZK_STATUS_t status ;
	RZK_KernelInit() ;

	/** Initialize the UART port to print the messages on console */
	InitUART();

	/* add the RTC driver */
	if( AddRtc() != RZKERR_SUCCESS )
	{
		printf("\nRTC driver is not initialized properly");
		return 0 ;
	}

	printf("\nStarting to create RZK threads" ) ; 

	/**
	 * create two threads with the same priority and the attributes of the 
	 * the thread as round robin mode
	 */
	printf("\nCreating thread #1" ) ;
	g_hthd1 = RZKCreateThreadEnhanced( ( UINT8 *) "Thread1",
					(RZK_PTR_t)RtcDemoThreadEntry,
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


