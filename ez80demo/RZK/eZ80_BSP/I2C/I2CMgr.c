/*
 * File       : I2CMgr.c
 * Scope      : 
 *
 * Description:  This contains implementations for I2C manager which handles 
 * the i2c protocol.
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




#include <ez80.h>


#include <stdio.h>
#include "ZTypes.h"
#include "ZSysgen.h"
#include "ZInterrupt.h"
#include "ZThread.h"
#include "ZSemaphore.h"
#include "i2c.h"

extern UINT8 I2C_TASK_STACK[] ;
extern UINT8 i2cRxBuff[] ;
extern UINT8 i2cTxBuff[] ;
extern UINT I2C_TASK_STACK_SIZE ;
extern UINT8 I2C_TASK_PRIORITY ;
extern I2C_CONFIG_t    i2cConfigParams ;
extern UINT I2C_RX_MAX_BUFF_SIZE ;
extern UINT I2C_TX_MAX_BUFF_SIZE ;
RZK_SEMAPHOREHANDLE_t hSem_I2C;
void I2cInterruptTask(void) ;

INT rxAvail ;

CQ_t rxCQ ;
CQ_t txCQ ;

RZK_THREADHANDLE_t i2cIntThreadHdl ;

void CQ_IN( CQ_t *pCQ, UINT8 ch )
{    
    if( pCQ->rear == pCQ->bufLen - 1 )
    {         
        pCQ->rear = 0 ;
    }
    else
        pCQ->rear++ ;

    pCQ->pBuff[pCQ->rear] = ch ;
    pCQ->avail++ ;
}



UINT8 CQ_OUT(CQ_t *pCQ)
{     
    UINT8 data = 0;   
    
    data = pCQ->pBuff[pCQ->front] ;   
    if ( pCQ->front == pCQ->bufLen - 1 )        
        pCQ->front = 0 ;     
    else         
        pCQ->front++ ;    
    
    pCQ->avail-- ;
    return data ;
}

DDF_STATUS_t I2CPeek()
{
	DDF_STATUS_t avail ;
	UINTRMASK intmask ;

	intmask = RZKDisableInterrupts() ;
	avail = rxCQ.avail ;
	RZKEnableInterrupts(intmask) ;

	return avail ;
}



DDF_STATUS_t I2COpen( RZK_DEVICE_CB_t *pDCB, INT8 *devName, INT8 * devMode )
{
    UINTRMASK intmask;
    
    /**  Opens the I2C bus by initializing the ports and 
    setting up the registers to a known state
    *    Creates interrupt thread 
    *    Configures I2C based on whether it is a master or slave */
    
    if(RZKDEV_OPENED == pDCB->InUse)
        return I2CERR_SUCCESS ;

    i2cConfigParams.mode = *devMode ;
    
   	I2CDev_Init();
    
	hSem_I2C    = RZKCreateSemaphore((RZK_NAME_t*)"hSem_I2C", 1, RECV_ORDER_PRIORITY) ;//KE_SemCreate( 1 );
	if(hSem_I2C == NULL)
		printf("\nI2C Semaphore creation error");

    if((*devMode == I2C_SLAVE) ||(*devMode == I2C_MULTI_MASTER) )
    {
        i2cIntThreadHdl = RZKCreateThreadEnhanced((RZK_NAME_t*)"I2C", 
            (RZK_PTR_t)I2cInterruptTask, 
            NULL, 
            I2C_TASK_STACK_SIZE,
            I2C_TASK_PRIORITY,1, 
            RZK_THREAD_PREEMPTION|RZK_THREAD_INTERRUPT,0) ;
        if(i2cIntThreadHdl == NULL)
        {
            return I2CERR_KERNELERROR ;
        }

		rxCQ.rear = -1 ;
		rxCQ.front = 0 ;
		txCQ.rear = -1 ;
		txCQ.front = 0 ;
		rxCQ.pBuff = i2cRxBuff ;
		txCQ.pBuff = i2cTxBuff ;
		rxCQ.bufLen = I2C_RX_MAX_BUFF_SIZE ;
		txCQ.bufLen = I2C_TX_MAX_BUFF_SIZE ;
		rxCQ.avail = 0 ;
		txCQ.avail = 0 ;
    }
    
    intmask = RZKDisableInterrupts() ;
    pDCB->InUse = RZKDEV_OPENED ;
    RZKEnableInterrupts(intmask) ;    
    return 		I2CERR_SUCCESS ; 				
    
    
}


DDF_STATUS_t I2CClose( RZK_DEVICE_CB_t * pDCB)
{

UINTRMASK intmask ;
/** Close I2C bus
*   Deletes interrupt thread
    */
    if(RZKDEV_OPENED != pDCB->InUse)
        return I2CERR_INVALID_OPERATION ;
    
    
    RZKDeleteThreadEnhanced(i2cIntThreadHdl) ;
    intmask = RZKDisableInterrupts() ;
    pDCB->InUse &= ~RZKDEV_OPENED ;
    RZKEnableInterrupts(intmask) ;
    
    return I2CERR_SUCCESS ;
}



DDF_STATUS_t I2CWrite( RZK_DEVICE_CB_t * pDCB, INT8 * buf, RZK_DEV_BYTES_t size )
{
    INT stat = 0 ;
    INT32 i ; // INT changed to INT32
    UINT8 slvaddr = i2cConfigParams.currSlaveAddr ; 
	UINT8 useSubAddr = i2cConfigParams.useSubAddr ;
	UINT subAddr = i2cConfigParams.subAddr ;
//    UINTRMASK intmask ;   //Commented during IAR Porting
	UINT16 count ;
    
    /* If master 
    Send the start condition
    Send slave address with write bit
    Send data
    Send STOP condn
    If slave
    write the data to the Tx circular buffer in the I2C
    Actual transmission will happen from the interrupt thd context.
    */
    
     if((RZKDEV_OPENED & pDCB->InUse) != RZKDEV_OPENED)
        return I2CERR_INVALID_OPERATION ;
    
    if((pDCB->InUse & I2C_BUS_BUSY) == I2C_BUS_BUSY )
        return I2CERR_BUSBUSY ;
    else
    {
	RZKAcquireSemaphore(hSem_I2C,INFINITE_SUSPEND);
//      mask = RZKDisablePreemption() ;
        pDCB->InUse |= I2C_BUS_BUSY ;
	I2C_SRR = 0x01; // Reset the ez80 I2C peripheral 
        if( (i2cConfigParams.mode == I2C_MASTER) || (i2cConfigParams.mode == I2C_MULTI_MASTER))
        {
			stat = I2CDev_SendStart() ;
			if(stat<0)
			{
				pDCB->InUse &= ~I2C_BUS_BUSY ;
				RZKReleaseSemaphore(hSem_I2C);
//            		RZKRestorePreemption(mask) ;
				return stat ;
			}
			slvaddr &= I2CMODE_TRANSMIT ;   
			stat = I2CDev_TransmitDataByte(slvaddr) ;

			if(I2C_Status_Check(stat,pDCB) < 0)
			{
				return stat;
			}	
         	if(useSubAddr == RZK_TRUE)
            {
				
               
				switch(i2cConfigParams.addrLen)
				{
					case I2C_SUBADDR_THREE_BYTE:
			       /* Venkat - CR#75733 fix */
			        	if (i2cConfigParams.subAddrFormat == I2C_SUBADDR_BIG_ENDIAN)
						{
						    subAddr = (UINT8)(i2cConfigParams.subAddr >> 16);
						}
		                stat = I2CDev_TransmitDataByte(subAddr) ;
						
						if(I2C_Status_Check(stat,pDCB) < 0)
						{
							break;
						}
						if (i2cConfigParams.subAddrFormat == I2C_SUBADDR_LITTLE_ENDIAN)
						{
						    subAddr >>= 8 ;
						}
					case I2C_SUBADDR_TWO_BYTE:

				        if (i2cConfigParams.subAddrFormat == I2C_SUBADDR_BIG_ENDIAN)
						{
						    subAddr = (UINT8)(i2cConfigParams.subAddr >> 8);
						} 
	                   	stat = I2CDev_TransmitDataByte(subAddr) ;
					
						if(I2C_Status_Check(stat,pDCB) < 0)
						{
							break;
						}	
						if (i2cConfigParams.subAddrFormat == I2C_SUBADDR_LITTLE_ENDIAN)
						{
						    subAddr >>= 8 ;
						}
					case I2C_SUBADDR_SINGLE_BYTE:
				        if (i2cConfigParams.subAddrFormat == I2C_SUBADDR_BIG_ENDIAN)
						{
						    subAddr = (UINT8)(i2cConfigParams.subAddr);
						} 
						stat = I2CDev_TransmitDataByte(subAddr) ;
						break ;
				}


				if(I2C_Status_Check(stat,pDCB) < 0)
				{
					return stat;
				}
        	}
                
            for(i=0; i<size; i++)
            {
                stat = I2CDev_TransmitDataByte(*buf) ;
                if(stat<0)
                {	
				/*if arbitration is lost during the data phase 
				then quit the current execution and free the bus*/
					if(stat == I2CERR_ARBLOST)/* giri 05-12-08*/
					{
						pDCB->InUse &= ~I2C_BUS_BUSY ;
						RZKReleaseSemaphore(hSem_I2C);
						return stat;
					}
                    I2CDev_SendStop() ;
                    pDCB->InUse &= ~I2C_BUS_BUSY ;
					RZKReleaseSemaphore(hSem_I2C);
//                  RZKRestorePreemption(mask) ;
                    return stat ;
                }
                else 
                	buf++ ;
            }
                
            I2CDev_SendStop() ;
			count = COUNT_DELAY ;
			I2C_Delay(count) ;    
        }
	    else
	    {
    	    for(i=0; i<size; i++)
        	{
                CQ_IN(&txCQ, *buf++) ;
            }	
        }

    }
 
	pDCB->InUse &= ~I2C_BUS_BUSY ;
	RZKReleaseSemaphore(hSem_I2C);
//  RZKRestorePreemption(mask) ; 
    return size ;
    
}

DDF_STATUS_t I2CRead(RZK_DEVICE_CB_t *	pDCB, INT8 * buf, RZK_DEV_BYTES_t size)
{
    
    INT stat = 0 ;
    INT32 i ;   
    UINT8 slvaddr = i2cConfigParams.currSlaveAddr ; 
	UINT8 useSubAddr = i2cConfigParams.useSubAddr ;
	UINT subAddr = i2cConfigParams.subAddr ;
	UINT16 temp = 0 ;
	UINT16 count ;
    
    if((RZKDEV_OPENED & pDCB->InUse) != RZKDEV_OPENED)
        return I2CERR_INVALID_OPERATION ;
    
    if((pDCB->InUse & I2C_BUS_BUSY) == I2C_BUS_BUSY )
        return I2CERR_BUSBUSY ;
    else
    {
		RZKAcquireSemaphore(hSem_I2C,INFINITE_SUSPEND);
//      mask = RZKDisablePreemption() ;
        pDCB->InUse |= I2C_BUS_BUSY ;     
        I2C_SRR = 0x01; // Reset the ez80 I2C peripheral 
        if( (i2cConfigParams.mode == I2C_MASTER) || (i2cConfigParams.mode == I2C_MULTI_MASTER))
        {
 			if(useSubAddr == RZK_TRUE)
           	{
	    		stat = I2CDev_SendStart() ;
        		if(stat<0)
        		{
            		pDCB->InUse &= ~I2C_BUS_BUSY ;
					RZKReleaseSemaphore(hSem_I2C);
//                		RZKRestorePreemption(mask) ; 
            		return stat ;
        		}	
        		slvaddr &= I2CMODE_TRANSMIT ;
                stat = I2CDev_TransmitDataByte(slvaddr) ;
			 
 
				if(I2C_Status_Check(stat,pDCB) < 0)
				{
					return stat;
				}
				switch(i2cConfigParams.addrLen)
				{
					case 3:
				        /* CR#75733 fix */
				        if (i2cConfigParams.subAddrFormat == I2C_SUBADDR_BIG_ENDIAN)
						{
						    subAddr = (UINT8)(i2cConfigParams.subAddr >> 16);
						}
						
	                   	stat = I2CDev_TransmitDataByte(subAddr) ;
					
						if(stat < 0)
						//if(I2C_Status_Check(stat,pDCB) < 0)
						{
							break;
						}
						if (i2cConfigParams.subAddrFormat == I2C_SUBADDR_LITTLE_ENDIAN)
						{
						    subAddr >>= 8 ;
						}
				
					case 2:
				        if (i2cConfigParams.subAddrFormat == I2C_SUBADDR_BIG_ENDIAN)
						{
						    subAddr = (UINT8)(i2cConfigParams.subAddr >> 8);
						} 
	                   	stat = I2CDev_TransmitDataByte(subAddr) ;
						
						
						if(stat < 0)
						//if(I2C_Status_Check(stat,pDCB) < 0)
						{
							break;
						}	
						if (i2cConfigParams.subAddrFormat == I2C_SUBADDR_LITTLE_ENDIAN)
						{
						    subAddr >>= 8 ;
						}
						
				case 1:
				        if (i2cConfigParams.subAddrFormat == I2C_SUBADDR_BIG_ENDIAN)
						{
						    subAddr = (UINT8)(i2cConfigParams.subAddr);
						} 
				       	stat = I2CDev_TransmitDataByte(subAddr) ;
						break ;
				}

 
				if(I2C_Status_Check(stat,pDCB) < 0)
				{
					return stat;
				}
            }
            
            stat = I2CDev_SendStart() ;
            if(stat<0)
            {
				
                pDCB->InUse &= ~I2C_BUS_BUSY ;
				RZKReleaseSemaphore(hSem_I2C);
               // RZKRestorePreemption(mask) ; 
			    return stat ;
            }
			
            slvaddr |= I2CMODE_RECEIVE ;
            stat = I2CDev_TransmitDataByte(slvaddr) ;
 
			if(I2C_Status_Check(stat,pDCB) < 0)
			{
				return stat;
			}	
	
            for(i=0; i<size; i++)
            {

				
				if(i <= (size - 2))
				{
					I2C_SetAAKClearIFlag(temp) ;
				}
				else
				{
					I2C_ResetAAKClearIFlag() ;
				 /* CR#75733 fix */
					if (size != 1)
					{
					    stat = I2CDev_ReceiveDataByte(buf++) ;
						if(I2C_Status_Check(stat,pDCB) < 0)
						{
							if(stat == I2CERR_DATABYTERXDMMODE_NACKTXD)
							{
								break;
							}
							return stat;
						}
					}
 
				}
            	stat = I2CDev_ReceiveDataByte(buf++) ;
 
				if(I2C_Status_Check(stat,pDCB) < 0)
				{
					if(stat == I2CERR_DATABYTERXDMMODE_NACKTXD)
					{
						break;
					}
					return stat;
				}

   }
                
            I2CDev_SendStop() ;                  
			count = COUNT_DELAY ;
			I2C_Delay(count) ;                    
    	}
    	else
    	{
        	if(size > rxCQ.avail)
        	    size = rxAvail ;
			I2C_SetAAKClearIFlag(temp) ;

	        for(i=0; i<size; i++)
    	    {
        	    *buf++ = CQ_OUT(&rxCQ) ;
        	}
    	}
	}					

	pDCB->InUse &= ~I2C_BUS_BUSY ;
	RZKReleaseSemaphore(hSem_I2C);
//  RZKRestorePreemption(mask) ;    
	return size ;
}


DDF_STATUS_t I2C_Status_Check(DDF_STATUS_t stat, RZK_DEVICE_CB_t *	pDCB)
{
UINT16 temp = 0 ;
UINT8 ch;
	if(stat < 0)
	{
		pDCB->InUse &= ~I2C_BUS_BUSY ;				/* I2C peripheral to be made free */
		RZKReleaseSemaphore(hSem_I2C);				/* Release the semaphore*/
		
		if(i2cConfigParams.mode == I2C_MULTI_MASTER)
		{
			switch(stat)
			{
				case I2CERR_ARBLOST:   				/*If arbitration is lost then stay idle*/
					I2C_SRR = 0x01;
					return stat;
	
				case I2CERR_ARBLOST_SLAWRXD_ACKTXD:	/* slave addr with write command rxd, Now recieve the data and check for AAK and transmit the ack or nack*/	
					I2C_CTL |= 0x80;				/* Enable the I2C interrupt*/
					I2C_ClearIFlag() ;				/* Clear the IFLAG bit*/
					return stat;

				case I2CERR_ARBLOST_GLCLADRXD_ACKTXD:/*general call addr with write command rxd, Now recieve the data and check for AAK and transmit the ack or nack*/
				case I2CERR_ARBLOST_SLARRXD_ACKTXD:	 /* slave addr with write command 	Now recieve the data and check for AAK and transmit the ack or nack*/
					I2C_CTL |= 0x80;				 /* Enable the I2C interrupt*/
					ch = CQ_OUT(&txCQ) ;
					I2CDev_TransmitDataByte(ch) ;	 /*  Sending the first byte */
					return stat;
	
				default:
					I2CDev_SendStop() ;
					return stat;		 
			}	
		}
		else
		{
			I2CDev_SendStop() ;
		//  RZKRestorePreemption(mask) ;
			return stat;		
		}	
	}
	return stat;
}

DDF_STATUS_t I2CControl( RZK_DEVICE_CB_t *pDCB, RZK_DEV_BYTES_t uOperation, INT8 *addr1, INT8 *addr2 )
{
    UINTRMASK intmask ;
    
/* change the slave address, UseSubAddr, Subaddr, speed
    */
    
    if(RZKDEV_OPENED != pDCB->InUse)
        return I2CERR_INVALID_OPERATION ;
    
    if(I2C_BUS_BUSY == pDCB->InUse)
        return I2CERR_BUSBUSY ;
    else
    {
        intmask = RZKDisableInterrupts();
        pDCB->InUse |= I2C_BUS_BUSY ;
        RZKEnableInterrupts(intmask) ;
    }
    
    switch((UINT)uOperation)
    {
	    case I2C_SET_SLAVE_ADDR:
	        i2cConfigParams.currSlaveAddr = *addr1 ;
	        break ;
        
	    case I2C_SUBADDR_USED:
	        i2cConfigParams.useSubAddr = RZK_TRUE ;
	        i2cConfigParams.subAddr = *((UINT16 *)addr1) ;
	        break ;
        
	    case I2C_SUBADDR_NOT_USED:
	        i2cConfigParams.useSubAddr = RZK_FALSE ;
	        i2cConfigParams.subAddr = 0 ;
	        break ;

	    case I2C_SUBADDR_LEN :
	        i2cConfigParams.addrLen = 0 ;
	        i2cConfigParams.addrLen = *addr1 ;
	        break ;
        
	    case I2C_SET_SPEED:
	        i2cConfigParams.speed = *addr1 ;
	        I2C_CCR = i2cConfigParams.speed ;
	        break ;

    }
    
    intmask = RZKDisableInterrupts() ;
    pDCB->InUse &= ~I2C_BUS_BUSY ;
    RZKEnableInterrupts(intmask) ;
    
    return I2CERR_SUCCESS ;
    
}




UINT8* pBuff = i2cRxBuff ;

void I2cInterruptTask(void)
{
    
    UINT8 i2cStatus ;
    INT8 ch ;    
    UINTRMASK intmask, intrmask ;
//    RZK_STATE_t mask ;  //Commented during IAR Porting
	UINT16 temp ;
	INT stat;
    /*    look for the kind of interrupt
    *Validate the slave address wrt own address
    *Send ack
    
      *If slave read
      *Read the incoming data into a circular buffer
      *If a thread is pending on IO by calling I2C read then resume the thd
      *If slave write
      *
      *
    */

    
    while(1)
    {   
        i2cStatus = I2C_SR ;
     
	    switch(i2cStatus)
        {
     	   	case 	I2CSTAT_SLAWRXD_ACKTXD:

			case 	I2CSTAT_SLDATARXD_ACKTXD:
           		{
                /* receive the data into Rx circular buffer.
                    buffer will not check for full condition
                    and keep receiving the data if the 
                    buffer is not updated with fresh data.
                */

                    I2CDev_ReceiveDataByte(&ch) ;
					I2C_SetAAKClearIFlag(temp) ;
                    intrmask = RZKDisableInterrupts() ;
					CQ_IN(&rxCQ, ch) ;
					RZKEnableInterrupts(intrmask) ;
            	}
            break ;

        case 	I2CSTAT_SLARRXD_ACKTXD:

        case 	I2CSTAT_SLDATATXD_ACKRXD:            
            {
                /* Transmit the data present in the Tx buffer 
                    Circular Buffer will not check for empty condn
                    and keep transmitting the same data if the 
                    buffer is not updated with fresh data.
                */
					intrmask = RZKDisableInterrupts() ;
					ch = CQ_OUT(&txCQ) ;
                    intrmask = RZKDisableInterrupts() ;
					stat = I2CDev_TransmitDataByte(ch) ;
					
            }
            
            break ;
            
        case 	I2CSTAT_SLDATARXD_NACKTXD:
            I2C_SetAAKClearIFlag(temp) ;
            break ;
            
        case 	I2CSTAT_SLSTOPREPSTART_RXD:
            I2C_SetAAKClearIFlag(temp) ;
            break ;        
            
        case 	I2CSTAT_SLDATATXD_ACKNRXD:
            I2C_SetAAKClearIFlag(temp) ;
            break ;
            
        case 	I2CSTAT_SLDATALASTTXD_ACKRXD:
            I2C_SetAAKClearIFlag(temp) ;
            break ;
            
            
        case 	I2CSTAT_NORELEVANT_STATUS:	
            	I2C_SetAAKClearIFlag(temp) ;
				break ;
            
        default :
			printf("condition not handled \n") ;
            break ;
            
        }
        
        intmask = RZKDisableInterrupts() ;
        /* Enabling the I2C interrupts */
        I2C_CTL  |= I2CMASK_SET_INT ;
        
        RZKSuspendInterruptThread() ;
        
        RZKEnableInterrupts(intmask) ;
    }    
}




/*****************************************************/




