#ifndef _DATE_H_
#define _DATE_H_
/*

 * File       :     date.h

 *

 * Description:  This file contains the Telnet Client side API definitions.

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

 */


 /*

 * Copyright 2001, Metro Link, Inc.

 * All Rights Reserved

 *

 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Metro Link, Inc. and may

 * contain proprietary, confidential and trade secret information of

 * Metro Link, Inc. and/or its partners.

 *  

 * The contents of this file may not be disclosed to third parties, copied or

 * duplicated in any form, in whole or in part, without the prior written

 * permission of Metro Link, Inc.

 *

 */



/* date.h - net2xt, xt2net, isleap, ut2ltim */

/* Xinu stores time as seconds past Jan 1, 1970 (UNIX format), with	*/
/* 1 being 1 second into Jan. 1, 1970, GMT (universal time).  The	*/
/* Internet uses seconds past Jan 1, 1900 (also GMT or universal time)	*/

#define	net2xt(x)	((x)-2208988800UL)  /* convert net-to-xinu time	*/
#define	xt2net(x)	((x)+2208988800UL)  /* convert xinu-to-net time	*/

/* Days in months and month names used to format a date */

struct	datinfo	{
	UINT8	dt_msize[12];
	INT8	*dt_mnam[12];
	INT8	*dt_dnam[7];
};

struct daymonth{
	INT8	* string;
	UINT number;
};
extern	struct	datinfo	Dat;

/* Constants for converting time to month/day/year/hour/minute/second	*/

#define	isleap(x) (((x)%4==0) && (((x)%100!=0) || ((x)%400==0)))	/* MCB - CR# 2202, Leap Year correction */
#define	SECPERDY	(60L*60L*24L)	/* one day in seconds		*/
#define	SECPERHR	(60L*60L)	/* one hour in seconds		*/
#define	SECPERMN	(60L)		/* one minute in seconds	*/

/* date doesn't understand daylight savings time (it was built in	*/
/*	Indiana where we're smart enough to realize that renumbering	*/
/*	doesn't save anything).	However, the local time zone can be	*/
/*	set to EST, CST, MST,or PST.					*/

#define	ZONE_EST	5		/* Eastern Standard time is 5	*/
#define	ZONE_CST	6		/*  hours west of England	*/
#define	ZONE_MST	7
#define	ZONE_PST	8
#define	TIMEZONE	ZONE_EST	/* timezone for this system	*/

/* In-line procedures to convert universal-to-local time and vice versa	*/

#define	ut2ltim(x)	((x)-TIMEZONE*SECPERHR)
#define	ltim2ut(x)	((x)+TIMEZONE*SECPERHR)

extern INT16	 time_rqest(void);

#endif
