/************************************************************************************
 * File       : zfsapp.c
 * Description: This file contains a application that will test the FS
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
 ************************************************************************************/
// standard headers
#include <stdio.h>
#include <string.h>
// headers
#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZThread.h"
#include "ZDevice.h"

#include "zfstypes.h"
#include "zfsapi.h"
#include "cfileapi.h"

#include "rtc.h"


// macros

#define PRIORITY1		10		/** priority for thread #1 */
#define STACK_SIZE		2048	/** stack size for the thread */
#define RR_TICKS		1		/** round robin tick for the schedular */

/** extern function declarations */
/** function for initializing the UART port for printing the messages on console */
extern void InitUART( void );

/** global variables */
/** thread handles to store */
RZK_THREADHANDLE_t g_hthd1 ;

/** stack for the two threads */
INT8 g_thd1stack [ STACK_SIZE ];

// FILE System testing

ZFS_FD_LIST_t g_fd_list[ 256 ] ;

const INT8 g_txt_buf1 [] = \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog INDIA ELECTRONICS PVT LTD " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " \
"Zilog Inc, RACE Street, Sanjose " ;

INT8 g_fdname[ 16 ] ;
INT8 g_fdttlName1[ 256 ] ;
//ZFS_FD_LIST_t g_fd_list[ 256 ] ;

ZFS_VOL_PARAMS_t g_vol_params[10] ;

void CreateFiles( INT8* dir_name, INT cnt )
{
	INT i ;
	ZFS_HANDLE_t fs_handle ;
	INT32 num ;
	ZFS_STATUS_t status ;
	for( i = 0 ; i < cnt; i++ ) 
		{
		sprintf(&g_fdname[0], "/File.%d", i ) ;
		strcpy( (INT8*) g_fdttlName1, (const INT8*)dir_name ) ;
		strcat( (INT8*)g_fdttlName1, g_fdname ) ;
		printf("\n Creating the file : %s", g_fdttlName1 ) ;
		fs_handle = ZFSOpen( &g_fdttlName1[0], ZFS_WRITE, ZFS_MODE_BINARY ) ;
		if( fs_handle == NULL )
			{
			printf(" ERROR" ) ;
			break ;
			}
		else
			printf(" SUCCESS" ) ;

		// write 2KB data into it.
		printf("\n writing 2KB data into the file : %s", g_fdttlName1 ) ;
		num = ZFSWrite( fs_handle, (UINT8*)&g_txt_buf1[0], 512 ) ;
		if( num == 0 )
			{
			printf(" ERROR" ) ;
			break ;
			}
		else
			{
			printf(" SUCCESS :%ld", num ) ;
			}

		//close the file
		printf("\n Closing the file : %s", g_fdttlName1 ) ;
		status = ZFSClose( fs_handle ) ;
		if( status != ZFSERR_SUCCESS )
			{
			printf(" ERROR" ) ;
			break ;
			}
		else
			printf(" SUCCESS" ) ;
		}
	return ;
}	

ZFS_STATUS_t GetFilesPresentInDir( INT8 * dir_name )
{
	INT j = 0 ;
	INT startCnt = 0 ;
	INT fd_cnt ;
	ZFS_STATUS_t status =0 ;
	printf("\n Getting the directory list for : %s", dir_name ) ;
	// printing the fd names present in the names.
	fd_cnt = ZFSGetDirFileCount( &dir_name[0]);
	if( fd_cnt <= 0 )
		{
//		printf("\n ERROR: %d", fd_cnt ) ;
		return fd_cnt ;
		}
	else
		{
		printf("\n ZFSGetDirFileCount (%s) = %d", dir_name, fd_cnt ) ;
		}

	do
		{
		fd_cnt = ZFSList( &dir_name[0], &g_fd_list[0], startCnt ) ;
		if( fd_cnt > 0 )
			{	
			printf("\n ZFSList (%s) status = %d", dir_name, status ) ;
			for( j = 0 ; j < fd_cnt ; j++ )
				{
				printf("\n fd_name = %s    SIZE = %d", g_fd_list[j].fd_name, g_fd_list[j].fd_size ) ;
				}
			startCnt += fd_cnt ;
			}
		} while( fd_cnt > 0 ) ;
	return startCnt ;
}

void AppMain()
{
	ZFS_STATUS_t status ;
	INT cnt;
	INT i = 0 ;

	printf("\nInitializing FileSystem, Please Wait..." ) ;
	status = ZFSInit( &g_vol_params[0] ) ;
	if( status != ZFSERR_SUCCESS )
	{
		printf(" FAILED : %d", status ) ;

		for( i = 0 ; i < status ; i ++ )
			{
			printf("\n\nVolume Name: %s", g_vol_params[i].vol_name ) ;
			printf("\nVolume Validness : %s", ( g_vol_params[i].is_valid? "VALID":"INVALID" )) ;
			printf("\nFormatting the volume: %s",  g_vol_params[i].vol_name );
			status = ZFSFormat( ( INT8* ) &g_vol_params[i].vol_name[0] );
			if( status != ZFSERR_SUCCESS )
			{
			 	printf(" FAILED" );
				return ;
			}
			else
				printf(" SUCCESS");
			}
	}
	else
		printf(" DONE" ) ;

	// get first level director list
	cnt = GetFilesPresentInDir( "/" ) ;
	if( cnt > 0 )
		{
		GetFilesPresentInDir( "/Dir.0" ) ;
		GetFilesPresentInDir( "/Dir.1" ) ;
		GetFilesPresentInDir( "/Dir.2" ) ;
	
		// get the second level directory list
		GetFilesPresentInDir( "/Dir.0/Child.0" ) ;
		GetFilesPresentInDir( "/Dir.0/Child.1" ) ;
		GetFilesPresentInDir( "/Dir.2/Child.2" ) ;
	
		// get the third level directory list
		GetFilesPresentInDir( "/Dir.0/Child.1/Grand.0" ) ;
		GetFilesPresentInDir( "/Dir.2/Child.2/Grand.1" ) ;
		GetFilesPresentInDir( "/Dir.2/Child.2/Grand.2" ) ;
	
		// get fourth level directory listing
		GetFilesPresentInDir( "/Dir.0/Child.1/Grand.0/GGrand.0" ) ;
	
		// get fifth level directory listing
		GetFilesPresentInDir( "/Dir.0/Child.1/Grand.0/GGrand.0/GGGrand.0" ) ;
		GetFilesPresentInDir( "/Dir.0/Child.1/Grand.0/GGrand.0/GGGrand.1" ) ;
		}
	else
		{
		// no files are present in the volume, just create some files/directories

		// 1st level	///////////////
		printf("\n Creating directories at level 1" ) ;
		// now create 3 directories in root
		for( i = 0 ; i < 3 ; i++ )
			{
			sprintf(g_fdname, "Dir.%d", i ) ;
			printf("\n Creating the directory: %s", g_fdname ) ;
			status = ZFSMkdir("/", (INT8*)&g_fdname[0] ) ;
			if( status != ZFSERR_SUCCESS )
				{
				printf(" ERROR: %d", status ) ;
				break ;
				}
			else
				{
				printf(" SUCCESS" ) ;
				}
			}
		
		// create 5 file in dir1
		printf("\n Creating files at level 1" ) ;
		CreateFiles( "Dir.1", 5 ) ;
	
		// 2nd level	///////////////
		// related to dir.0
		// now create 2 directories in root
		printf("\n Creating directories at level 2" ) ;
		for( i = 0 ; i < 2 ; i++ )
			{
			sprintf(g_fdname, "Child.%d", i ) ;
			printf("\n Creating the directory: %s", g_fdname ) ;
			status = ZFSMkdir("Dir.0", (INT8*)&g_fdname[0] ) ;
			if( status != ZFSERR_SUCCESS )
				{
				printf(" ERROR: %d", status ) ;
				break ;
				}
			else
				{
				printf(" SUCCESS" ) ;
				}
			}
		// create a directory under dir.2
		printf("\n Creating the directory Dir.2/Child.2" ) ;
		status = ZFSMkdir("Dir.2", "Child.2" ) ;
		if( status != ZFSERR_SUCCESS )
			{
			printf(" ERROR: %d", status ) ;
			}
		else
			{
			printf(" SUCCESS" ) ;
			}
	
		// creating files uder level-2
		// create 5 file in dir1
		printf("\n Creating files at level 2" ) ;
		CreateFiles( "Dir.0/Child.0", 2 ) ;
		CreateFiles( "Dir.0/Child.1", 1 ) ;
		
		
		// 3rd level	///////////////
		// related to dir.0
		// now create 2 directories in root
		printf("\n Creating directories at level 3" ) ;
		// create a directory under dir.2
		printf("\n Creating the directory /Dir.0/Child.1/Grand.0" ) ;
		status = ZFSMkdir("/Dir.0/Child.1", "Grand.0" ) ;
		if( status != ZFSERR_SUCCESS )
			{
			printf(" ERROR: %d", status ) ;
			}
		else
			{
			printf(" SUCCESS" ) ;
			}
		
		for( i = 0 ; i < 2 ; i++ )
			{
			sprintf(g_fdname, "Grand.%d", i + 1 ) ;
			printf("\n Creating the directory: Dir.2/Child.2/%s", g_fdname ) ;
			status = ZFSMkdir("Dir.2/Child.2", (INT8*)&g_fdname[0] ) ;
			if( status != ZFSERR_SUCCESS )
				{
				printf(" ERROR: %d", status ) ;
				break ;
				}
			else
				{
				printf(" SUCCESS" ) ;
				}
			}
	
		// creating files uder level-2
		// create 5 file in dir1
		printf("\n Creating files at level 3" ) ;
		CreateFiles( "Dir.0/Child.1/Grand.0", 3 ) ;
		CreateFiles( "Dir.2/Child.2/Grand.1", 1 ) ;
		CreateFiles( "Dir.2/Child.2/Grand.2", 5 ) ;
	
	
		// 4th level	///////////////
		// related to dir.0
		// now create 2 directories in root
		printf("\n Creating directories at level 4" ) ;
		// create a directory under dir.2
		printf("\n Creating the directory : /Dir.0/Child.1/Grand.0/GGrand.0" ) ;
		status = ZFSMkdir("/Dir.0/Child.1/Grand.0", "GGrand.0" ) ;
		if( status != ZFSERR_SUCCESS )
			{
			printf(" ERROR: %d", status ) ;
			}
		else
			{
			printf(" SUCCESS" ) ;
			}
		
		// creating files uder level-4
		printf("\n Creating files at level 4" ) ;
		CreateFiles( "/Dir.0/Child.1/Grand.0/GGrand.0", 2 ) ;
	
	
		// 5th level	///////////////
		// related to dir.0
		// now create 2 directories in root
		printf("\n Creating directories at level 5" ) ;
	
		for( i = 0 ; i < 2 ; i++ )
			{
			sprintf(g_fdname, "GGGrand.%d", i ) ;
			printf("\n Creating the directory: %s", g_fdname ) ;
			status = ZFSMkdir("/Dir.0/Child.1/Grand.0/GGrand.0", (INT8*)&g_fdname[0] ) ;
			if( status != ZFSERR_SUCCESS )
				{
				printf(" ERROR: %d", status ) ;
				break ;
				}
			else
				{
				printf(" SUCCESS" ) ;
				}
			}
		
		// creating files uder level-5
		printf("\n Creating files at level 5" ) ;
		CreateFiles( "/Dir.0/Child.1/Grand.0/GGrand.0/GGGrand.0", 5 ) ;
		CreateFiles( "/Dir.0/Child.1/Grand.0/GGrand.0/GGGrand.1", 5 ) ;

		printf("\n Finished creating the files" ) ;

		printf("\n File/Directory listing" ) ;
		// get first level director list
		GetFilesPresentInDir( "/" ) ;
		GetFilesPresentInDir( "/Dir.0" ) ;
		GetFilesPresentInDir( "/Dir.1" ) ;
		GetFilesPresentInDir( "/Dir.2" ) ;
	
		// get the second level directory list
		GetFilesPresentInDir( "/Dir.0/Child.0" ) ;
		GetFilesPresentInDir( "/Dir.0/Child.1" ) ;
		GetFilesPresentInDir( "/Dir.2/Child.2" ) ;
	
		// get the third level directory list
		GetFilesPresentInDir( "/Dir.0/Child.1/Grand.0" ) ;
		GetFilesPresentInDir( "/Dir.2/Child.2/Grand.1" ) ;
		GetFilesPresentInDir( "/Dir.2/Child.2/Grand.2" ) ;
	
		// get fourth level directory listing
		GetFilesPresentInDir( "/Dir.0/Child.1/Grand.0/GGrand.0" ) ;
	
		// get fifth level directory listing
		GetFilesPresentInDir( "/Dir.0/Child.1/Grand.0/GGrand.0/GGGrand.0" ) ;
		GetFilesPresentInDir( "/Dir.0/Child.1/Grand.0/GGrand.0/GGGrand.1" ) ;
		}

	printf("\n\n Completed FS_App demo application") ;
	return ;
}


void FS_AppEntry( void )
{

//	RZK_STATUS_t status ;
	/** Initialize the UART port to print the messages on console */
	InitUART();

	/* creating a thread #1 with a higher priority than thread #2 */
	printf("\nCreating thread #1" ) ;
	g_hthd1 = RZKCreateThread( ( UINT8 *) "Thread1",
					(RZK_PTR_t)AppMain,
					NULL,
					(CADDR_t) ( g_thd1stack + STACK_SIZE ),
					PRIORITY1,
					RR_TICKS, 
					RZK_THREAD_AUTOSTART | RZK_THREAD_PREEMPTION,
					0) ; 

	/** validate the thread handle */
	if( g_hthd1 == NULL )
	{
		printf("\nUnable to create the thread #1, error description is");
		RZKFormatError( RZKGetErrorNum() ) ;
		return ;
	}
}

// Set the RTC date to a value
// setting up the time for Jul-1-2004, 10:05:04
const TIME g_timestr = {
	4, // sec
	5, // min
	10, // hrs
	1, // dayofMonth
	4, // dayOfWeek
	7, // mon
	4, // year
	20 // century
	} ;

void SetRTCTime( void )
{
	RZK_DEVICE_CB_t *hrtc ;
	hrtc = RZKDevOpen( "RTC", NULL ) ;
	if( hrtc == NULL )
		{
		// error in opening the driver.
		printf("\n RTC Could not be initialized" ) ;
		return ;
		}
		// Now the driver is opened, set a temp date.
		RZKDevIOCTL( hrtc, RTC_RESET_CONTROL, ( void * ) &g_timestr, ( void * ) &g_timestr ) ;
		RZKDevIOCTL( hrtc, RTC_SET_ALL, ( void * ) &g_timestr, ( void * ) &g_timestr ) ;

	return ;
}


INT main( INT argc, void *argv[] )
{
	RZK_KernelInit() ;
	// Initialize the RTC
	AddRtc() ;
	// Set RTC value
	SetRTCTime() ;

	FS_AppEntry() ;

	RZK_KernelStart() ;

	return 0 ;
}
 

