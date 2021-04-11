/********************************************************************************
 * File       : rtc_conf.c
 *
 * Description: Defines the configurable system parameters for RTC device
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

#include <stdio.h>

#include "ZTypes.h"
#include "ZSysgen.h"
#include "ZDevice.h"
#include "ZThread.h"
#include "rtc.h"

#define RTC_TASK_PRIOH                  (10) /*CR#5167 */
#define RTC_THD_STACK_SIZEH             (1024)

extern DDF_STATUS_t IONULL( void );
extern RZK_STATUS_t DummyDevInit( RZK_DEVICE_CB_t *) ;

/* RTC Device Control Block Initialization Structure */
RZK_DEVICE_CB_t RtcDev = {
		0, "RTC", 
		(FNPTR_RZKDEV_INIT)IONULL, (FNPTR_RZKDEV_STOP)IONULL,
		(FNPTR_RZKDEV_OPEN)IONULL, (FNPTR_RZKDEV_CLOSE)IONULL, 
		(FNPTR_RZKDEV_READ)RTCRead,(FNPTR_RZKDEV_WRITE)IONULL,
		(FNPTR_RZKDEV_SEEK)IONULL, (FNPTR_RZKDEV_GETC)IONULL, 
		(FNPTR_RZKDEV_PUTC)IONULL, (FNPTR_RZKDEV_IOCTL)RTCControl, 
		(RZK_PTR_t)IONULL,0000, NULL, 0, 0
	} ;


// **************************
// DO NOT CHANGE ANYTHING BELOW IN THIS SECTION
// **************************
UINT8     RTC_TASK_PRIO                           = RTC_TASK_PRIOH ;
UINT      RTC_THD_STACK_SIZE                      = RTC_THD_STACK_SIZEH ;

/* RTC Alarm Task*/

/* This task will be scheduled whenevr an RTC Alarm interrupt fires*/
/* Code corresponding to the functionality desired should be added by the user */
void RtcTask(void)
{
	while(1)
	{
      /* Add logic For Alarm Control */

		printf("\nTin-Tin-Tin\n");
        RZKSuspendInterruptThread() ;
	}
}
