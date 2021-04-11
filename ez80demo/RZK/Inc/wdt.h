/*********************************************************************************
 * File       :     wdt.h
 *
 * Description:  This file contains the Watch Dog defines.
 *
 * Copyright 2004 ZiLOG Inc.  ALL RIGHTS RESERVED.
 *
 * This file contains unpublished confidential and proprietary information
 * of ZiLOG, Inc.
 * NO PART OF THIS WORK MAY BE DUPLICATED, STORED, PUBLISHED OR DISCLOSED 
 * IN ANY FORM WITHOUT THE PRIOR WRITTEN CONSENT OF ZiLOG, INC.
 * This is not a license and no use of any kind of this work is authorized
 * in the absence of a written license granted by ZiLOG, Inc. in ZiLOG's 
 * sole discretion 
 *********************************************************************************/

#ifndef _ZWDT
#define _ZWDT  

#define	WDT_EN		0x80 
#define	WDT_NMI		0x40
#define	WDT_SMCLK	0x00
#define	WDT_RTCLK	0x04
#define	WDT_INTCLK	0x08
#define	WDT_27OUT	0x00
#define	WDT_25OUT	0x01
#define	WDT_22OUT	0x02
#define	WDT_18OUT	0x03

void wdt_init(UINT8 bWdtCtrl);
void wdt_reset();

#endif
