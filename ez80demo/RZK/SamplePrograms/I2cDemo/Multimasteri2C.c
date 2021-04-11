
/*
 * File       : i2c_multimaster.c
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


#define STACK_SIZE 1024
void GPIOIntTask();
extern INT8                    rxBuff[STACK_SIZE] ;
//INT8                    txBuff[100] ;
 extern INT8 txBuff[];


extern RZK_DEVICE_CB_t I2cDevice;

extern RZK_DEVICE_CB_t *       i2cHdl1 ;
 

void GPIOIntTask()
{
	RZK_STATUS_t status ;    
	int len = 0;

          	
	while(1)
	{
		UINTRMASK mInterruptMask;
		
		//mInterruptMask = RZKDisableInterrupts();
		printf("\nMultimaster demo starts Master 1\n");
		len = 2; // No of bytes to be transmitted/Read
	
#if 0		
			status =  RZKDevWrite( 
	        i2cHdl1, 
	        txBuff,
	        len	
	        );
			if(status == len)
			{
				printf("\nI2C Device Write: Passed");
			}
			else
			{
				printf("\nI2C Device Write: Failed = %d",status);
				switch(status)
				{
					case I2CERR_SLAWTXD_ACKNRXD:
						printf("\nSlave Address/Write transmitted but ACK not received");
						break;
					case I2CERR_SLARTXD_ACKNRXD:
						printf("\nSlave Address/Read transmitted but ACK not received");
						break;
		
					case I2CERR_ARBLOST:
						printf("\nArbitration Lost");
						break;
					case I2CERR_ARBLOST_SLAWRXD_ACKTXD:
						printf("\nArbitration Lost, Slave Adress/Write received and Ack transmitted");
						break;
					case I2CERR_ARBLOST_GLCLADRXD_ACKTXD:
						printf("\nArbitration Lost, General Call Address received and Ack transmitted");
						break;
					case I2CERR_ARBLOST_SLARRXD_ACKTXD:
						printf("\nArbitration Lost, Slave Adress/Read received and Ack transmitted");
						break;
					case I2CERR_FAILURE:
						printf("\nI2C Failure");
						break;
					case I2CERR_TIMEOUT:
						printf("\n Time Out");
						break;
					case I2CERR_DATABYTETXDMMODE_ACKNRXD:
						printf("\nData byte transmitted but ACK not received");
						break;
					case I2CSTAT_SLDATATXD_ACKNRXD:
						printf("\nSlave Data transmitted but ACK not received");
						break;
					case I2CERR_IDLE:
						printf("\nI2C Idle");
						break;
					case I2CERR_BUSERROR:
						printf("\nBus Error");
				}
		
			}
#endif			
 		
			status =  RZKDevRead( 
	        i2cHdl1, 
	        txBuff,
	        len	
	        );
			if(status == len)
			{
				printf("\nI2C Device Read: Passed");
			}
			else
			{
				printf("\nI2C Device Read: Failed = %d",status);
				switch(status)
				{
					case I2CERR_DATABYTERXDMMODE_NACKTXD:
						printf("\nData byte recieved in Master mode and NACK transmitted");
						break;
					case I2CERR_ARBLOST:
						printf("\nArbitration Lost");
						break;
					case I2CERR_ARBLOST_SLAWRXD_ACKTXD:
						printf("\nArbitration Lost, Slave Adress/Write received and Ack transmitted");
						break;
					case I2CERR_ARBLOST_GLCLADRXD_ACKTXD:
						printf("\nArbitration Lost, General Call Address received and Ack transmitted");
						break;
					case I2CERR_ARBLOST_SLARRXD_ACKTXD:
						printf("\nArbitration Lost, Slave Adress/Read received and Ack transmitted");
						break;
					case I2CERR_FAILURE:
						printf("\nI2C Failure");
						break;
					case I2CERR_TIMEOUT:
						printf("\n Time Out");
						break;
					case I2CERR_SLARTXD_ACKNRXD:
						printf("\nSlave Address/Read transmitted but ACK not received");
						break;
					case I2CERR_BUSERROR:
						printf("\nBus Error");
				}
			
			}

	  //}  
    

        /* enable PB0 interrupt*/
		PB_ALT2 = 0x01;

	/* suspend interrupt thread. 
		It just removes thread from DQ and does not call scheduler */
		RZKSuspendInterruptThread();

/*		If the interrupt thread is not infinitely suspended then resumeinterruptthread 
		returns without doing anything. So RZKDI/RZKEI combination is not required here*/
		//RZKEnableInterrupts(mInterruptMask);
	}
}
 

