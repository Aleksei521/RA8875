/***********************************************************************************
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


#include <stdio.h>
#include <string.h>
#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZThread.h">
#include "ZDevice.h"
#include "i2c.h"


#define STACK_SIZE 		1024
#define THD_PRIORITY	8
#define DATA_SIZE       64

extern void InitUART(void) ;

void I2cDemoTask(void) ;


RZK_DEVICE_CB_t *       i2cHdl1 ;
RZK_THREADHANDLE_t	    hThread1,hThread2;
INT8                    rxBuff[STACK_SIZE] ;
INT8                    txBuff[] = "**Data Txed to EEPROM and read back to verify working of I2C***"  ;

void InitializeResources()
{
    /*creating a preemptable thread with AUTOSTART Option*/
    
    hThread1=  RZKCreateThreadEnhanced(( RZK_NAME_t * )"Thread1",
        (RZK_PTR_t)I2cDemoTask, NULL,
        STACK_SIZE, 
        THD_PRIORITY,20 ,
        RZK_THREAD_PREEMPTION |RZK_THREAD_AUTOSTART,
        0); 
    
    
}/*End of InitilizeThreads*/


void RZKApplicationEntry()
{
    
    RZK_STATUS_t    status ;
    INT8            mode = I2C_MASTER;
	INT8            slvAddr = 0xA0 ;
	UINT16          subAddr = 0x0000 ;
	INT8            subAddrLen = 2 ;
	INT8			busSpeed = 0x58 ;
    
    
    /*The following function create 
    and initilize threads and event group*/    
    InitializeResources();


	InitUART() ;
    
        AddI2c() ;

	/* Open the I2C bus for communication */
    i2cHdl1 = RZKDevOpen( 
        (RZK_DEV_NAME_t *)"I2C",
        &mode 
        );


	/* Setting the Slave Address */
	RZKDevIOCTL(i2cHdl1, I2C_SET_SLAVE_ADDR, &slvAddr, NULL ) ;

	/* Setting the SubAddress for EEPROM */
	RZKDevIOCTL(i2cHdl1, I2C_SUBADDR_USED, (INT8*)&subAddr, NULL ) ;

	/* Setting the Sub Address Length 1,2,3 bytes */
	RZKDevIOCTL(i2cHdl1, I2C_SUBADDR_LEN, &subAddrLen, NULL ) ;

	/* Setting the bus speed */
	RZKDevIOCTL(i2cHdl1, I2C_SET_SPEED, &busSpeed, NULL ) ;
    
}	/*End of main*/




void I2cDemoTask()
{
    
    RZK_STATUS_t status ;
    INT len = 0 ;
	UINT16 addr = 0 ;
	UINT8 cnt = 16 ;
    
	    len = DATA_SIZE ;

        /* Writing into EEPROM device teh contents of Tx buffer */
        status =  RZKDevWrite( 
            i2cHdl1, 
            txBuff,
            len	
            );
        
        printf("Data sent\n") ;
        printf("%s\n\n",txBuff) ;

        /* Reading back from the EEPROM into Rx Buffer */
        status = RZKDevRead( 
            i2cHdl1,	
            rxBuff,		
            len	
            );

        printf("Data Received\n") ;
        printf("%s\n\n",rxBuff) ;


   
}/*End of SerialDemoTask*/



/*main function*/
INT main( INT argc, void *argv[] )
{
    
    RZK_KernelInit() ;
    
    RZKApplicationEntry() ;
    
    RZK_KernelStart() ;
    
    return 0 ;
}

