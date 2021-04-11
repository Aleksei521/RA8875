/*********************************************************************************
* File			:	RtcRead.c
*
* Description	:	Reads the current date and time from RTC
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
#include "Rtc.h"



DDF_STATUS_t RTCRead(RZK_DEVICE_CB_t *pdev,		// device id
					 RZK_DEV_BUFFER_t *buf,		// buffer pointer where the data need to be stored
					 RZK_DEV_BYTES_t nBytes)	// input-bytes to read, output-bytes read)
{
	TIME *pTime = (TIME *)buf ;
	
	pTime->sec = RTC_SEC ;
	pTime->minutes = RTC_MIN ;
	pTime->hrs = RTC_HRS ;
	pTime->mon = RTC_MON ;
	pTime->dayOfWeek = RTC_DOW ;
	pTime->dayOfMonth = RTC_DOM ;
	pTime->year = RTC_YR ;
	pTime->cent = RTC_CEN ;
	
	return RTC_ERR_SUCCESS ;
}
