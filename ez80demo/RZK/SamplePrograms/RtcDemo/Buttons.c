/*                                                   
 *****************************************************************************
 * Buttons.c
 *
 * Description: Functions for buttons on the ZiLOG Evaluation Board
 *
 * Copyright: 2016 ZiLOG Inc.  ALL RIGHTS RESERVED.
 *
 * This file contains unpublished confidential and proprietary information
 * of ZiLOG, Inc.
 * NO PART OF THIS WORK MAY BE DUPLICATED, STORED, PUBLISHED OR DISCLOSED 
 * IN ANY FORM WITHOUT THE PRIOR WRITTEN CONSENT OF ZiLOG, INC.
 * This is not a license and no use of any kind of this work is authorized
 * in the absence of a written license granted by ZiLOG, Inc. in ZiLOG's 
 * sole discretion 
 *****************************************************************************
 */

#include <ez80.h>
#include <stdio.h>

#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZThread.h"
#include "ZInterrupt.h"

/* Port B interrupt vectors for the various eZ80 and eZ80Acclaim! devices */
#ifdef _EZ80F91
#define PB0_IVECT 0xA0
#define PB1_IVECT 0xA4
#endif

#ifdef _EZ80F93
#define PB0_IVECT 0x30
#define PB1_IVECT 0x32
#endif

#ifdef _EZ80F92
#define PB0_IVECT 0x30
#define PB1_IVECT 0x32
#endif

#ifdef _EZ80L92
#define PB0_IVECT 0x30
#define PB1_IVECT 0x32
#endif

#ifdef _EZ80190
#define PB0_IVECT 0x2c
#define PB1_IVECT 0x2e
#endif



extern RZK_THREADHANDLE_t g_hpb1 ;
extern RZK_THREADHANDLE_t g_hpb2 ;
extern void Pb0_isr_prolog( void ) ;
extern void Pb1_isr_prolog( void ) ;
/*****************************************************************************
 *  Buttons initialization routine
 *
 *  configure buttons for input (falling edge interrupt)
 ******************************************************************************/
void buttons_init( void )
{
    UINT8 pb_tmp;
	UINTRMASK intr_mask ;

	intr_mask = RZKDisableInterrupts();
    /* set port B 3 interrupt vector */
    RZKInstallInterruptHandler( Pb0_isr_prolog, PB0_IVECT );
	
	/* set port B 4 interrupt vector */
    RZKInstallInterruptHandler( Pb1_isr_prolog, PB1_IVECT );
	
    //Set the Port B bits 4 and 5 to low level interrupt mode
	PB_ALT1 = 0xFF;
	PB_ALT2 = 0xFF;

	RZKEnableInterrupts( intr_mask );

}
