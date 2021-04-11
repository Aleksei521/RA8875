/**********************************************************************************
* File       : SerialDemo.c
*
* Description: Read large amount of data from key borad and echoes it back to  serial window
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
**********************************************************************************/


#include <stdio.h>
#include <string.h>
#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZThread.h"
#include "ZDevice.h"
#include "serial.h"


#define STACK_SIZE 		1024
#define THD_PRIORITY	8

extern void InitUART(void) ;

void SerialDemoTask(void) ;


RZK_DEVICE_CB_t *       serHdl1 ;
RZK_THREADHANDLE_t	    hThread1,hThread2;
INT8                    rxBuff[STACK_SIZE] ;
INT8 g_szstr[] = "Please type in any data or send any data through UART, the data is reflected back on screen\n\n";
void InitializeResources()
{
    /*creating a preemptable thread with AUTOSTART Option*/
    
    hThread1=  RZKCreateThreadEnhanced(( RZK_NAME_t * )"Thread1",
        (RZK_PTR_t)SerialDemoTask, NULL,
        STACK_SIZE, 
        THD_PRIORITY,20 ,
        RZK_THREAD_PREEMPTION |RZK_THREAD_AUTOSTART,
        0); 
    
    
}/*End of InitilizeThreads*/


void RZKApplicationEntry()
{
    
    RZK_STATUS_t    status ;
    INT8            mode   ;
    
    
    /*The following function create 
    and initilize threads and event group*/    
    InitializeResources();
    
    /* Attach the UART1 device to the device table */
    status = AddUART0() ;
    
    /* Opening the UART1 device */
    serHdl1 = RZKDevOpen( 
        (RZK_DEV_NAME_t *)"SERIAL0",
        &mode 
        );
    
}	/*End of main*/




void SerialDemoTask()
{
    
    RZK_STATUS_t status ;
    INT len = 0 ;

    
    RZKDevWrite(serHdl1, g_szstr, strlen( g_szstr)	);	
    
    while(1)
    {
        
        /* Gets the number of bytes present in the Rx buffer */
        len = UARTPeek(serHdl1) ;
        
        
        /* Reading the number of bytes present in the UARTs Rx Buffer */
        status = RZKDevRead( 
            serHdl1,	
            rxBuff,		
            len	
            );
        
        
        status =  RZKDevWrite( 
            serHdl1, 
            rxBuff,
            len	
            );	
        
    }
    
    
}/*End of SerialDemoTask*/


/*main function*/
INT main( INT argc, void *argv[] )
{
    
    RZK_KernelInit() ;
    
    RZKApplicationEntry() ;
    
    RZK_KernelStart() ;
    
    return 0 ;
}
