/*****************************************************************************
 * File       : Rtc.h
 *
 * Description: Contains macros and structure declarations for RTC driver
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
 ****************************************************************************/ 


#ifndef _RTC_H
#define _RTC_H

#include "ZTypes.h"
#include "ZSysgen.h"
#include "ZDevice.h"


typedef struct TIME{
	UINT8 sec ;
	UINT8 minutes ;
	UINT8 hrs ;
	UINT8 dayOfMonth ;
	UINT8 dayOfWeek ;
	UINT8 mon ;
	UINT8 year ;
	UINT8 cent ;
}TIME ;


enum RTC_ERROR_et{
	RTC_ERR_SUCCESS = 0,
	RTC_ERR_RESOURCE_NOT_CREATED = -5,
	RTC_ERR_RESOURCE_NOT_DELETED
} ;


/* Control Functions */

#define RTC_SET_SEC						((UINT)0x01)
#define RTC_SET_MIN						((UINT)0x02)
#define RTC_SET_HRS						((UINT)0x03)
#define RTC_SET_MON						((UINT)0x04)
#define RTC_SET_DOW						((UINT)0x05)
#define RTC_SET_DOM						((UINT)0x06)
#define RTC_SET_YEAR					((UINT)0x07)
#define RTC_SET_CENT					((UINT)0x08)
#define RTC_ENABLE_BCD					((UINT)0x09)
#define RTC_DISABLE_BCD					((UINT)0x0A)
#define RTC_SET_ALL						((UINT)0x0B)
#define RTC_CLK_SEL_CRYSTAL_OSCILL		((UINT)0x0C)
#define RTC_CLK_SEL_POWER_LINE_FREQ		((UINT)0x0D)
#define RTC_ENABLE_ALARM				((UINT)0x0E)
#define RTC_DISABLE_ALARM				((UINT)0x0F)
#define RTC_RESET_CONTROL				((UINT)0x10)
#define RTC_POWER_LINE_FREQUENCY60HZ 	((UINT)0x13)
#define RTC_POWER_LINE_FREQUENCY50HZ 	((UINT)0x14)



/* RTC Control Settings */

#define ALARAM_INTERRUPT_ACTIVE		0x80
#define ALARM_INTERRUPT_ENABLED		0x40
#define BINARY_FORMAT_ENABLED		0x20
#define POWER_LINE_FREQUENCY_INPUT	0x10
#define POWER_LINE_FREQUENCY_SELECT	0x08
#define DAYLIGHT_SAVINGS_ENABLED	0x04
#define SLEEP_MODE_RECOVERY_RESET	0x02
#define COUNT_REGISTERS_UNLOCK		0x01

/* RTC Alarm Settings */

#ifdef _EZ80F91
#define RTC_ALARM_VECTOR_OFFSET		0x6C
#else
#define RTC_ALARM_VECTOR_OFFSET		0x16
#endif
#define RTC_ALARM_ENABLE			0x0F

/* RTC Function Declarations */

DDF_STATUS_t AddRtc(void) ;

DDF_STATUS_t RTCRead(RZK_DEVICE_CB_t *pdev, RZK_DEV_BUFFER_t *buf, RZK_DEV_BYTES_t nBytes) ;
DDF_STATUS_t RTCControl(RZK_DEVICE_CB_t *pdev, RZK_DEV_BYTES_t uOperation, void *addr1, void *addr2) ;


#endif