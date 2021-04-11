/*
 * File       : i2c_multimaster_demo.c
 * Scope      : 
 *
 * Description:  This contains implementations for I2C protocol
 *
 *
 * Copyright 2003 ZiLOG Inc.  ALL RIGHTS RESERVED.
 *
 * This file contains unpublished confidential and proprietary information
 * of ZiLOG, Inc.
 * NO PART OF THIS WORK MAY BE DUPLICATED, STORED, PUBLISHED OR DISCLOSED 
 * IN ANY FORM WITHOUT THE PRIOR WRITTEN CONSENT OF ZiLOG, INC.
 * This is not a license and no use of any kind of this work is authorized
 * in the absence of a written license granted by ZiLOG, Inc. in ZiLOG's 
 * sole discretion 
 */


#include <stdio.h>
#include <string.h>
#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZThread.h"
#include "ZDevice.h"
#include "i2c.h"

#include "ZInterrupt.h"
#define STACK_SIZE		2048	/** stack size for the thread */
#define PB0_PRIORITY	1	    /** thread priority */
#define DATA_SIZE       64

extern void InitUART(void) ;
extern void pb0_isr(void);
void I2cDemoTask(void) ;
void init_GPIO_interrupt();	/* Configure a GPIO for a interrupt*/
extern void GPIOIntTask();

extern RZK_DEVICE_CB_t I2cDevice;
RZK_THREADHANDLE_t g_hPb0Thread ;
RZK_DEVICE_CB_t *       i2cHdl1 ;
RZK_THREADHANDLE_t	    hThread1,hThread2;
extern UINT8                    i2cRxBuff[STACK_SIZE] ;
extern UINT8 				i2cTxBuff[STACK_SIZE];

INT8                    txBuff[] = "abta Txed to EEPROM and read back to verify working of I2C***"  ;
void InitializeResources()
{
	RZK_STATUS_t    status ;
	INT8            mode = I2C_MULTI_MASTER;		/*I2C_MASTER or I2C_MULTI_MASTER*/
	INT8            slvAddr = 0x50 ;		/*Slave adress*/
	UINT16          subAddr = 0x0000 ;      /*Slave Sub-address*/
	INT8            subAddrLen = I2C_SUBADDR_TWO_BYTE ;        /*Slave sub adress lenth*/
	INT8			busSpeed = 0x64 ;       /*I2C bus speed*/
    
	init_GPIO_interrupt();	/* Configure a GPIO for a interrupt*/
	InitUART() ;
	
	/*create a interrupt thread for the gpio interrupt*/
	g_hPb0Thread = RZKCreateThreadEnhanced(( RZK_NAME_t *) "PB0IT",
					(RZK_PTR_t)GPIOIntTask,
					NULL,
					STACK_SIZE,
					PB0_PRIORITY,
					0, 
					RZK_THREAD_PREEMPTION | RZK_THREAD_INTERRUPT,0  ) ; 
	if( g_hPb0Thread == NULL )
	{
		printf("\nUnable to create the GPIO interrupt thread "); 
		RZKFormatError( RZKGetErrorNum() ) ;
		return ;
	}    
   
   	
	/*Attach I2C*/
	status = RZKDevAttach( &I2cDevice );

	/* Open the I2C bus for communication */
    i2cHdl1 = RZKDevOpen( 
        (RZK_DEV_NAME_t *)"I2C",
        &mode 
        );


	/* Setting the Slave Address */
	RZKDevIOCTL(i2cHdl1, I2C_SET_SLAVE_ADDR, &slvAddr, NULL ) ;

	/* Setting the SubAddress for EEPROM */
	RZKDevIOCTL(i2cHdl1,I2C_SUBADDR_USED /*I2C_SUBADDR_NOT_USED*/, (INT8*)&subAddr, NULL ) ;

	/* Setting the Sub Address Length 1,2,3 bytes */
	RZKDevIOCTL(i2cHdl1, I2C_SUBADDR_LEN, &subAddrLen, NULL ) ;

	/* Setting the bus speed */
	RZKDevIOCTL(i2cHdl1, I2C_SET_SPEED, &busSpeed, NULL ) ;
	
	printf("\nI2C Multimaster Demo ready");
    
}/*End of InitilizeThreads*/


void RZKApplicationEntry()
{
    
    RZK_STATUS_t    status ;
    
    /*The following function create 
    and initilize threads, GPIO interrupt and UART*/    
    InitializeResources();

	printf("\nI2C Multimaster functionality Tests");
	printf("\n===================================\n");  
    
}	/*End of main*/


void init_GPIO_interrupt()
{
	RZKInstallInterruptHandler(pb0_isr, PORTB0_IVECT);	
  /* Initialize the PB0 pin for raising edge trigger interrupt */
	PB_DR 	|= 0x01; 
	PB_DDR	|= 0x01;
	PB_ALT1 |= 0x01;
	PB_ALT2 |= 0x01;	
}

/*main function*/
INT main( INT argc, void *argv[] )
{
    
    RZK_KernelInit() ;
    
    RZKApplicationEntry() ;
    
    RZK_KernelStart() ;
    
    return 0 ;
}


