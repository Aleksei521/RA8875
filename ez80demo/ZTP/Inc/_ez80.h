/*
 * File       :     _eZ80.h
 *
 * Description:  This file contains the Telnet Client side API definitions.
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

#ifndef __EZ80_H_
#define __EZ80_H_
#ifdef _EZ80F91
	#include "_eZ80F91.h"
#endif
#ifdef _EZ80F92
	#include "_eZ80F92.h"
#endif
#ifdef _EZ80F93
	#include "_eZ80F93.h"
#endif
#ifdef _EZ80L92
	#include "_eZ80L92.h"
#endif
#ifdef _EZ80190
	#include "_eZ80190.h"
#endif
/*
 * Pointer type definitions for the eZ80 Compiler to access I/O.
 * Note that all on-chip IO devices are internal.  However, some IO macros
 * use the __EXTIO keyword.  These macros are intended to be used with a 
 * variable that holds the base address of the peripheral.  This allows the 
 * same code fragment to operate on identical peripherals located at different 
 * places in the IO map.  For example, if your code neede to write the value 
 * 0x11 to either UART_THR0 or UART_THR1 depending on the value of a variable,
 * and that variable contained the base address of which UART to use, you could
 * either write:
 *
 *			if( uart_base == UART0 )
 *			{
 *				UART_THR0 = 0x11;
 *			}
 *			else
 *			{
 *				UART_THR1 = 0x11;
 *			}
 *
 * Or you could use the base-indexed macro like this:
 *			UART_THR(uart_base) = 0x11;
 *
 * However you must not use a constant with the base-index macros.  For example,
 * 	UART_THR( UART0 ) = 0x11; 
 * will produce unpredictable results.
 */
typedef volatile UINT8 __INTIO *	IORegInt8;
typedef volatile UINT8 __EXTIO *	IORegExt8;
typedef volatile UINT16 __INTIO *	IORegInt16;
typedef volatile UINT16 __EXTIO *	IORegExt16;

/* eZ80 Common Peripheral devices */
#define UART0						0xC0
#define UART1						0xD0

/* UART_IER bits */
#define IER_MIIE					0x08
#define IER_LSIE					0x04
#define IER_TIE					0x02
#define IER_RIE					0x01

/* UART_IIR bits */
#define IIR_FIFOEN				0xC0
#define IIR_ISCMASK				0x0E
#define IIR_RLS					0x06
#define IIR_RDR					0x04
#define IIR_CTO					0x0C
#define IIR_TBE					0x02
#define IIR_MS						0x00
#define IIR_INTBIT				0x01

/* UART_FCTL bits */
#define FCTL_TRIGMASK			0x00
#define FCTL_TRIG_1				0x00
#define FCTL_TRIG_4				0x40
#define FCTL_TRIG_8				0x80
#define FCTL_TRIG_14				0xC0
#define FCTL_CLRTXF				0x04
#define FCTL_CLRRXF				0x02
#define FCTL_FIFOEN				0x01

/* UART_LCTL bits */
#define LCTL_DLAB					0x80
#define LCTL_SB					0x40 		/* Send Break */
#define LCTL_FPE					0x20 		/* Force Parity Error */
#define LCTL_EPS					0x10 		/* Even Parity */
#define LCTL_PEN					0x08 		/* Parity Enable */
#define LCTL_2STOPBITS			0x04
#define LCTL_5DATABITS			0x00
#define LCTL_6DATABITS			0x01
#define LCTL_7DATABITS			0x02
#define LCTL_8DATABITS			0x03

/* UART_MCTL bits */
#define MCTL_LOOP					0x10
#define MCTL_OUT2					0x08
#define MCTL_OUT1					0x04
#define MCTL_RTS					0x02
#define MCTL_DTR					0x01

/* UART_LSR bits */
#define LSR_ERR					0x80
#define LSR_TEMT					0x40
#define LSR_THRE					0x20
#define LSR_BI						0x10
#define LSR_FE						0x08
#define LSR_PE						0x04
#define LSR_OE						0x02
#define LSR_DR						0x01

/* UART_MSR bits */
#define MSR_DCD					0x80
#define MSR_RI						0x40
#define MSR_DSR					0x20
#define MSR_CTS					0x10
#define MSR_DDCD					0x08
#define MSR_TERI					0x04
#define MSR_DDSR					0x02
#define MSR_DCTS					0x01

/* WDT_CTL bits */
#define WDTCTL_EN					0x80
#define WDTCTL_NMI				0x40
#define WDCTL_RST_FLAG			0x20
#define WDTCTL_TWO18				0x03
#define WDTCTL_TWO22				0x02
#define WDTCTL_TWO25				0x01
#define WDTCTL_TWO27				0x00

/* WDT_RR bits */
#define WDTRR_RESET1				0xA5
#define WDTRR_RESET2				0x5A

/*
 * Chip select Control Register bit definitions
 */
#define CSCTL_WAIT_STATE_MASK	0xE0
#define CSCTL_7_WAIT_STATES	0xE0
#define CSCTL_6_WAIT_STATES	0xC0
#define CSCTL_5_WAIT_STATES	0xA0
#define CSCTL_4_WAIT_STATES	0x80
#define CSCTL_3_WAIT_STATES	0x60
#define CSCTL_2_WAIT_STATES	0x40
#define CSCTL_1_WAIT_STATES	0x20
#define CSCTL_0_WAIT_STATES	0x00
#define CSCTL_MEMORY_ACCESS	0x00
#define CSCTL_IO_ACCESS			0x10
#define CSCTL_ENABLE_CS			0x08
#define CSCTL_DISABLE_CS		0x00

#endif
