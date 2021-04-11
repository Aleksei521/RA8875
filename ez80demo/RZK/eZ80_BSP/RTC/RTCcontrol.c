/*********************************************************************************
* File			:	RtcControl.c
*
* Description	:	RTC control settings. Does time and alarm settings.
*					
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
********************************************************************************/


#include <ez80.h>
#include <stdio.h>
#include <stdlib.h>
#include "ZTypes.h"
#include "ZSysgen.h"
#include "ZDevice.h"
#include "ZThread.h"
#include "ZInterrupt.h"
#include "Rtc.h"


extern void rtcisr(void) ;
extern UINT8 RTC_TASK_PRIO ;
extern UINT RTC_THD_STACK_SIZE ;
extern UINT8 RTC_Task_Stack[] ;
void RtcTask(void) ;

RZK_THREADHANDLE_t rtcInterruptThdHdl ;



DDF_STATUS_t RTCControl(RZK_DEVICE_CB_t *pdev, RZK_DEV_BYTES_t uOperation, void *addr1, void *addr2)
{
	
	switch((UINT)uOperation)
	{
	case RTC_SET_SEC:
		RTC_CTRL |= COUNT_REGISTERS_UNLOCK;			// unlocks the RTC_UNLOCK
		RTC_SEC = *((INT8*)addr1) ;
		RTC_CTRL &= ~COUNT_REGISTERS_UNLOCK;		// locks the RTC_LOCK 
		break ;
		
	case RTC_SET_MIN:
		RTC_CTRL |= COUNT_REGISTERS_UNLOCK;			// unlocks the RTC_UNLOCK
		RTC_MIN = *((INT8*)addr1) ;
		RTC_CTRL &= ~COUNT_REGISTERS_UNLOCK;		// locks the RTC_LOCK 
		break ;
		
	case RTC_SET_HRS:
		RTC_CTRL |= COUNT_REGISTERS_UNLOCK;			// unlocks the RTC_UNLOCK
		RTC_HRS = *((INT8*)addr1) ;
		RTC_CTRL &= ~COUNT_REGISTERS_UNLOCK;		 // locks the RTC_LOCK 		
		break ;
		
	case RTC_SET_MON:
		RTC_CTRL |= COUNT_REGISTERS_UNLOCK;			// unlocks the RTC_UNLOCK
		RTC_MON = *((INT8*)addr1) ;
		RTC_CTRL &= ~COUNT_REGISTERS_UNLOCK;		 // locks the RTC_LOCK 
		break ;
		
	case RTC_SET_DOW:
		RTC_CTRL |= COUNT_REGISTERS_UNLOCK;			// unlocks the RTC_UNLOCK
		RTC_DOW = *((INT8*)addr1) ;
		RTC_CTRL &= ~COUNT_REGISTERS_UNLOCK;		 // locks the RTC_LOCK 		
		break ;
		
	case RTC_SET_DOM:
		RTC_CTRL |= COUNT_REGISTERS_UNLOCK;			// unlocks the RTC_UNLOCK
		RTC_DOM = *((INT8*)addr1) ;
		RTC_CTRL &= ~COUNT_REGISTERS_UNLOCK;		  // locks the RTC_LOCK 
		break ;
		
	case RTC_SET_YEAR:
		RTC_CTRL |= COUNT_REGISTERS_UNLOCK;			// unlocks the RTC_UNLOCK
		RTC_YR = *((INT8*)addr1) ;
		RTC_CTRL &= ~COUNT_REGISTERS_UNLOCK;		// locks the RTC_LOCK 
		break ;
		
	case RTC_SET_CENT:
		RTC_CTRL |= COUNT_REGISTERS_UNLOCK;			// unlocks the RTC_UNLOCK
		RTC_CEN = *((INT8*)addr1) ;
		RTC_CTRL &= ~COUNT_REGISTERS_UNLOCK;		 // locks the RTC_LOCK 
		break ;
		
	case RTC_SET_ALL:
		{
			TIME *pTime = (TIME *)addr1 ;
			
			RTC_CTRL |= COUNT_REGISTERS_UNLOCK;		// unlocks the RTC_UNLOCK
			
			RTC_SEC = pTime->sec ;
			RTC_MIN = pTime->minutes ;
			RTC_HRS = pTime->hrs ;
			RTC_MON = pTime->mon ;
			RTC_DOW = pTime->dayOfWeek ;
			RTC_DOM = pTime->dayOfMonth ;
			RTC_YR	= pTime->year ;
			RTC_CEN = pTime->cent ;
			
			RTC_CTRL &= ~COUNT_REGISTERS_UNLOCK;     // locks the RTC_LOCK 
			break ;
		}
		
	case RTC_ENABLE_BCD:
		RTC_CTRL |= BINARY_FORMAT_ENABLED ;
		break ;
		
	case RTC_DISABLE_BCD:
		RTC_CTRL &= ~BINARY_FORMAT_ENABLED ;
		break ;

	case RTC_CLK_SEL_CRYSTAL_OSCILL:
		RTC_CTRL &=~POWER_LINE_FREQUENCY_INPUT ;
		break ;

	case RTC_CLK_SEL_POWER_LINE_FREQ:
		RTC_CTRL |= POWER_LINE_FREQUENCY_INPUT ;
		break ;		

	case RTC_POWER_LINE_FREQUENCY50HZ:
		RTC_CTRL |= POWER_LINE_FREQUENCY_SELECT ;
		break ;		

	case RTC_POWER_LINE_FREQUENCY60HZ:
		RTC_CTRL &= ~POWER_LINE_FREQUENCY_SELECT ;
		break ;		
		

	case RTC_RESET_CONTROL:
		RTC_CTRL = 0 ;
		break ;
		
	case RTC_ENABLE_ALARM:
		{
			TIME *pTime = (TIME *)addr1 ;
			
			RZKInstallInterruptHandler( (RZK_FNP_ISR)rtcisr, RTC_ALARM_VECTOR_OFFSET ) ;
			
			rtcInterruptThdHdl = RZKCreateThreadEnhanced((RZK_NAME_t*)"RTC_TASK",
				(RZK_PTR_t)RtcTask, 
				(FNP_THREAD_ENTRY *) NULL, 
				RTC_THD_STACK_SIZE, 
				RTC_TASK_PRIO, 
				2,
				RZK_THREAD_INTERRUPT|RZK_THREAD_PREEMPTION,
				0 ) ;
			if( rtcInterruptThdHdl == NULL )
			{
				return RTC_ERR_RESOURCE_NOT_CREATED ;
			}
			RTC_ASEC = pTime->sec;
			RTC_AMIN = pTime->minutes;
			RTC_AHRS = pTime->hrs;
			RTC_ADOW = pTime->dayOfWeek ;
//#ifdef _IAR_CODE
//#ifdef _EZ80F91			
//			RTC_ACTR = RTC_ALARM_ENABLE;
//#else
//                        RTC_ACTRL = RTC_ALARM_ENABLE;
//#endif		
//#else
			RTC_ACTRL = RTC_ALARM_ENABLE;
//#endif			
			RTC_CTRL |= ALARM_INTERRUPT_ENABLED ;
			break ;
		}
	case RTC_DISABLE_ALARM:
		{
			RZK_STATUS_t status ;

//#ifdef _IAR_CODE
//#ifdef _EZ80F91
//			RTC_ACTR = 0 ;
//#else
//			RTC_ACTRL = 0 ;
//#endif						
//#else
			RTC_ACTRL = 0 ;
//#endif			
			RTC_CTRL &= ~ALARM_INTERRUPT_ENABLED ;

			status = RZKDeleteThreadEnhanced( rtcInterruptThdHdl) ;
			if(status != RZKERR_SUCCESS )
			{
				return RTC_ERR_RESOURCE_NOT_DELETED ;
			}
		}
		
	default:
		break ;
	}
	
	return RTC_ERR_SUCCESS ;
	
}




