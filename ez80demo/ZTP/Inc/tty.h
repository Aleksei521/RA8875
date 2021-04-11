/*
 * File       :     tty.h
 *
 * Description:		This file contains the tty related structures 
 *					and the constants
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

#include "ztypes.h"
#include "zdevice.h"

#ifndef _TTY_H_
#define _TTY_H_
/* tty.h */

#define	IBLEN		256		/* input buffer size		*/
#define	OBLEN		256		/* output buffer size		*/


/* terminal special characters */
struct tchars {
	UINT8	tc_erase;
	UINT8	tc_intr;
	UINT8	tc_kill;
	UINT8	tc_lnext;
	UINT8	tc_reprint;
	UINT8	tc_eof;
	UINT8	tc_stop;
	UINT8	tc_start;
	UINT8	tc_susp;
	UINT8	tc_werase;
	UINT8	tc_eol;
};

#define	CNTRL(c) ((c) - 'a' + 1)

struct tty {
	UINT8	 			tty_state;	/* TTYS_* below			*/
	struct RZK_DEVICE_CB_t *	tty_pdev;	/* tty dev structure		*/
	struct RZK_DEVICE_CB_t *	tty_phw;	/* hardware dev structure	*/
	struct tchars	tty_tchars;	/* line editing characters	*/
	RZK_THREADHANDLE_t tty_cpid;	/* controlling process		*///AJ-PID changed to RZK_THREAD_HANDLE_t , XTLRM, 1.6.0

	/* TTY input fields */
	//AJ-XTLRM SID		tty_isema;	/* 1/0 semaphore for tty input	*/
	RZK_SEMAPHOREHANDLE_t					tty_isema;	/* 1/0 semaphore for tty input	*/
	UINT8	 			tty_iflags;	/* TIF_* below			*/
	UINT16	 			tty_istart;	/* index of first character	*/
	UINT16	 			tty_icount;	/* # characters in input buffer	*/
	
	/* TTY output fields */
	void *				tty_osema;	/* output buffer space semaphore*/
	UINT8	 			tty_oflags;	/* TOF_* below			*/
	UINT16	 			tty_ostart;	/* index of first character	*/
	UINT16	 			tty_ocount;	/* # characters in output buffer*/

	UINT8		 		tty_rows;
	UINT8		 		tty_cols;

	/* login information */
	void *				tty_uid;	/* user identifier		*/
	UINT32	 			tty_ltime;	/* login time			*/
	UINT32	 			tty_ctime;	/* last command time		*/
	UINT8	 			tty_in[IBLEN];
	UINT8	 			tty_out[OBLEN];

	/** Meenakshi added newly  to fix CR 4404 */
	UINT8				tty_lastChar ;
};

/* TTY states */

#define	TTYS_FREE	0
#define	TTYS_ALLOC	1

/* TTY input flags */

#define	TIF_NOBLOCK		0x0001		/* don't block on input		*/
#define	TIF_NOECHO		0x0002		/* echo input			*/
#define	TIF_RAW			0x0004
#define	TIF_CBREAK		0x0008
#define	TIF_EXCLUSIVE	0x0010
#define	TIF_EOF			0x0020		/* have seen EOF		*/
#define  TIF_DATA_AVAIL	0x0040		/* Rx data is available */
#define  TIF_7_BIT      0x0080      /* use 7-bit ASCII */

/* TTY output flags */

#define	TOF_NOBLOCK	0x0001		/* don't block on output	*/
#define	TOF_SYNC	0x0002		/* do synchronous output	*/
#define	TOF_RAW		0x0004		/* do raw output		*/

/* control() functions */

#define TTC_CPID		0x0001		/* set controlling PID		*/
#define	TTC_SYNC	0x0002		/* do synchronous output	*/
#define	TTC_GIF		0x0003		/* get input flags		*/
#define	TTC_GOF		0x0004		/* get output flags		*/
#define	TTC_NEXTC	0x0005		/* peek at next character	*/
#define	TTC_SUSER	0x0006		/* set uid for a tty		*/
#define	TTC_CUSER	0x0007		/* clear uid for a tty		*/
#define TTC_SIF		0x0008		/* set input flags		*/
#define TTC_SOF		0x0009		/* set output flags		*/

#define	NOCHAR		256

extern struct tty	ttytab[];
extern UINT8 Ntty;

/* ../tty/ttyalloc.c */
extern struct tty *ttyalloc(void);
/* ../tty/ttyclose.c */
extern INT16 ttyclose(struct RZK_DEVICE_CB_t *pdev);
/* ../tty/ttycntl.c */
extern DDF_STATUS_t
ttycntl
(
   RZK_DEVICE_CB_t		*pdev,
   RZK_DEV_BYTES_t		func,
   INT8					*arg1, 
   INT8     				*arg2
);
/* ../tty/ttyfree.c */
extern void ttyfree(struct tty *ptty);
/* ../tty/ttygetc.c */
extern DDF_STATUS_t ttygetc(struct RZK_DEVICE_CB_t *pdev);
/* ../tty/ttyiin.c */
extern INT16 ttyiin(struct RZK_DEVICE_CB_t *pdev, UINT8 ch);
/* ../tty/ttyinit.c */
extern INT16 ttyminit(struct RZK_DEVICE_CB_t *pdev);
extern INT16 ttyinit(struct RZK_DEVICE_CB_t *pdev);
/* ../tty/ttynew.c */
extern struct tty *ttynew(void);
/* ../tty/ttyoin.c */
extern INT16 ttyoin(struct tty *ptty, UINT8 ch);
/* ../tty/ttyopen.c */
//extern DID ttymopen(struct RZK_DEVICE_CB_t *pdev, 	DID pLowerDev);
extern DDF_STATUS_t ttymopen(struct RZK_DEVICE_CB_t *pdev, 	void *noUse ,void *pLowerDev);
/* ../tty/ttyputc.c */
extern DDF_STATUS_t ttyputc(struct RZK_DEVICE_CB_t *pdev, UINT8 ch);
/* ../tty/ttyread.c */
/*
extern SYSCALL ttyread(struct RZK_DEVICE_CB_t *pdev, unsigned char *buf, WORD len); */
extern
DDF_STATUS_t ttyread
(
	RZK_DEVICE_CB_t	*pdev,
	UINT8				*buf,
	RZK_DEV_BYTES_t			length 
) ;
/* ../tty/ttywrite.c */
/**
extern SYSCALL ttywrite(struct RZK_DEVICE_CB_t *pdev, unsigned char *buf, WORD len); */
extern 
DDF_STATUS_t 
ttywrite
(
	RZK_DEVICE_CB_t				*pdev,
	UINT8						*buf,
	RZK_DEV_BYTES_t				len
) ;
#endif /* _TTY_H_ */
