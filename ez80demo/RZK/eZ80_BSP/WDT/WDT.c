/*********************************************************************************
* File       : wdt_init.c
* Scope      : 
*
* Description: This file implements the wdt_init function
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
*******************************************************************************/


#include "ztypes.h"
#include <ez80.h>
#include "ZSysgen.h"
#include "wdt.h"

void wdt_init(UINT8 bWdtCtrl)
{
	WDT_CTL = bWdtCtrl;
	WDT_CTL |= WDT_EN;
}

void wdt_reset()
{
	WDT_RR = 0xA5;
	WDT_RR = 0x5A;
}