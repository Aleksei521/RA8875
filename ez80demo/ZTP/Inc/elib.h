/*
 * File       :     elib.h
 *
 * Description: 
 *
 * Copyright 2017 ZiLOG Inc.  ALL RIGHTS RESERVED.
 *
 * This file contains unpublished confidential and proprietary information
 * of ZiLOG, Inc.
 * NO PART OF THIS WORK MAY BE DUPLICATED, STORED, PUBLISHED OR DISCLOSED 
 * IN ANY FORM WITHOUT THE PRIOR WRITTEN CONSENT OF ZiLOG, INC.
 * This is not a license and no use of any kind of this work is authorized
 * in the absence of a written license granted by ZiLOG, Inc. in ZiLOG's 
 * sole discretion 
 */

#include "Ztypes.h" 
#ifndef _ELIB_H_
#define _ELIB_H_

#ifndef LEGACY_ZTP_TIMING
   //*** Deprecated ***VOID outch( INT8 ch );		/* print character to stdio */
   //*** Deprecated ***VOID outs( INT8 *s);		/* print a ASCIIZ string to stdio */
   //*** Deprecated ***VOID outhex( INT8 ch );
   //*** Deprecated ***VOID outhexes( INT8 *ch, INT n );
   //*** Deprecated ***UINT set_timeout(  UINT NumSec );
   //*** Deprecated ***UINT set_ttimeout( UINT NumTicks );
   //*** Deprecated ***BOOL chk_timeout(  UINT EndTick );

   /*
    * The RZK system timer (10ms period) is used for the legacy ZTP system timer.
    */
   #define ZTP_TICKS_PER_SEC     100


   extern TICK_t           tRZKTicks;
   #define ZTP_SYS_TIME    tRZKTicks


   typedef struct       ZTP_TIMER_s
   {
      UINT24            Ticks;
      UINT24            Start;
   } ZTP_TIMER;

   extern void CheckTimeout( UINT24 * Start, UINT24 NumTicks );

   #define CheckTimeout( Start, NumTicks )      \
      ((ZTP_SYS_TIME - Start) > NumTicks)

   extern void SetZtpTimerTick( ZTP_TIMER * tmr, UINT24 NumTick );

   #define SetZtpTimerTick( tmr, NumTick )      \
   {                                            \
      tmr.Ticks = NumTick;                      \
      tmr.Start = ZTP_SYS_TIME;                 \
   }

   extern void SetZtpTimerSec( ZTP_TIMER * tmr, UINT24 NumSec );

   #define SetZtpTimerSec( tmr, NumSec )        \
   {                                            \
      tmr.Ticks = NumSec * ZTP_TICKS_PER_SEC;   \
      tmr.Start = ZTP_SYS_TIME;                 \
   }

   extern BOOL CheckZtpTimer( ZTP_TIMER * pTmr );




#else
   UINT32 set_timeout( UINT seconds );
   UINT32 set_ttimeout( UINT ticks );
   INT chk_timeout( UINT32 timeout );
#endif

INT8 *itoa (INT val, INT8 *buf, INT radix);

UINT32 intel( UINT32 );
UINT16 intel16( UINT16 );
UINT16 inchksum( VOID *ptr, UINT16 len);	/* IP checksum */

#endif
