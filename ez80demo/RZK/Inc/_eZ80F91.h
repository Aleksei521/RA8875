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
/*
 * File       : _eZ80F91.h
 *
 * Description: Register and Macro definitions specific to ez80F91
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

#ifndef __EZ80F91_H_
#define __EZ80F91_H_

#include <ez80f91.h>


/* EMAC */
#define EmacStad(q)			*((IORegExt8)(0x25+q))
#define EmacHtbl(q)			*((IORegExt8)(0x33+q))
#define EmacCtld				*((IORegInt16)((INT8)0x3c))
#define EmacRrp				*((IORegInt16)((INT8)0x49))

// test reg
#define TESTFIFO				0x40	// enable fifo test
#define TXSEL					0x20	// select fifo for test_fifo
#define RXSEL					0x00	// select fifo for test_fifo
#define SSTCT					0x10	// shorcut slot timer counter when 1
#define SIMR					0x08	// simulation reset - reset random number generator in PETFUN
#define FRC_OVR_ERR 			0x04	// force rx overrun error
#define FRC_UND_ERR 			0x02	// force tx underrun error
#define LPBK					0x01	// internal loop back enable

// cfg1 reg
#define PADEN					0x80	// 1=pad short packets
#define ADPAD					0x40	// Auto detect pad enable
#define VLPAD					0x20	// VLAN pad enable
#define CRCEN					0x10	// crc enable
#define FULLD					0x08	// Full Duplex - mac ignores crs and col when 1
#define FLCHK					0x04	// Frame length check
#define HUGEN					0x02	// huge packet enable
#define DCRCC					0x01	// Delayed crc check

// cfg2 reg
#define BPNB					0x80	// backpreswsure no back off - if set, mac will automatically retry
											// if a collision occurs
#define NOBO					0x40	// no back off
#define LCOL_MASK				0x3f	// late collision window

// cfg3 reg
#define LONGP					0x80	// Long preamble enforcement
#define PUREP					0x40	// Pure preamble enforcement
#define XSDFR					0x20	// Excess Defer
#define BITMD					0x10	// Bit mode 10 mb/s ENDEC mode
#define RETRY_MASK			0x0f	// Retry count mask

// cfg4 reg
#define TPCF					0x40	// tx pause control frame
#define THDF					0x20	// tx half duplex (jams incoming packets)
#define PARF					0x10	// Pass all Receive frames
#define RXFC					0x08	// rx flow control
#define TXFC					0x04	// tx flow control (allow pause frames)
#define TPAUSE					0x02	// Test pause - mac control is paused
#define RXEN					0x01	// allows mac to output srxen

// reset reg
#define SRST					0x20	// s/w reset
#define HRTFN					0x10	// host reset tx function
#define HRRFN					0x08	// host reset rx function
#define HRTMC					0x04	// host reset tx MCS
#define HRRMC					0x02	// host reset rx MCS
#define HRMGT					0x01	// host reet MII MGT

// mii reg
#define LCTLD					0x80	// load control data
#define RSTAT					0x40	// read status
#define SCINC					0x20	// Scan Increment across multiple PHY's
#define SCAN					0x10	// 1=scan mgmt cycles continuously
#define SPRE					0x08	//	suppress preamble
#define CLK_MASK				0x07	// clock select
#define CLKDIV28				0x07
#define CLKDIV20				0x06
#define CLKDIV14				0x05
#define CLKDIV10				0x04
#define CLKDIV8				0x03
#define CLKDIV6				0x02
#define CLKDIV4				0x01

// interrupt enable reg
#define TXFSMERRIEN			0x80
#define MGTDONEIEN			0x40
#define RXCFIEN				0x20
#define RXPCFIEN				0x10
#define RXDONEIEN				0x08
#define RXOVRRUNIEN			0x04
#define TXPCFIEN				0x02
#define TXDONEIEN				0x01

// define istat reg
#define TXFSMERR				0x80
#define MGTDONE				0x40
#define RXCF					0x20
#define RXPCF					0x10
#define RXDONE					0x08
#define RXOVRRUN				0x04
#define TXPCF					0x02
#define TXDONE					0x01

// bufsz reg
#define BUFSZMASK				0xc0
#define TPCF_LEVEL			0x3f

// SAL Address Selection Reg
#define PROMISCUOUS			0x08
#define AMULTICAST			0x04
#define QMULTICAST			0x02
#define BROADCAST				0x01
#define STAD					0x00

// mii mgt status
#define MGTBUSY				0x80
#define MIILF					0x40
#define RDNVALID				0x20
#define RDADDRMASK			0x1f

// mii mgt phy registers
#define MII_CREG				0x00
#define MII_SREG				0x01

// fifo flags bit definitions
#define TXFF					0x80
#define TXFAE					0x20
#define TXFE					0x10
#define RXFF					0x08
#define RXFAF					0x04
#define RXFAE					0x02
#define RXFE					0x01



/* TIMER */
#define TMR0					0x60
#define TMR1					0x65
#define TMR2					0x6F
#define TMR3					0x74
#define TMR_CTL(base)		(*((IORegExt8)(base+0)))
#define TMR_IER(base)		(*((IORegExt8)(base+1)))
#define TMR_IIR(base)		(*((IORegExt8)(base+2)))
#define TMR_DRL(base)		(*((IORegExt8)(base+3)))
#define TMR_DRH(base)		(*((IORegExt8)(base+4)))
#define TMR_RRL(base)		(*((IORegExt8)(base+3)))
#define TMR_RRH(base)		(*((IORegExt8)(base+4)))

/* Timer Control Register bit definitions */
#define TCR_BRK_STP			0x80
#define CLKSELSYS				0x00
#define TCR_CLKDIV_MASK 	0x18
#define TCR_CONT_EN			0x04
#define TCR_RLD_EN			0x02
#define TCR_PRT_EN			0x01
#define TMRCLKDIV4			0x00
#define TMRCLKDIV16			0x08		
#define TMRCLKDIV64			0x10
#define TMRCLKDIV256			0x18

/* Timer Interrupt enable register bit definitions */
#define TIER_IRQ_EOC_EN		0x01
#define TIER_IRQ_ICA_EN		0x02
#define TIER_IRQ_ICB_EN		0x04
#define TIER_IRQ_0C0_EN		0x08
#define TIER_IRQ_OC1_EN		0x10
#define TIER_IRQ_OC2_EN	 	0x20
#define TIER_IRQ_OC3_EN		0x40

/* Timer Interrupt Identification register bit definitions */
#define TIIR_EOC				0x01
#define TIIR_ICA				0x02
#define TIIR_ICB				0x04
#define TIIR_0C0				0x08
#define TIIR_OC1				0x10
#define TIIR_OC2		 		0x20
#define TIIR_OC3				0x40

/* Ram Control */
#define GPRM_EN				0x80
#define ERAM_EN				0x40

/* Flash Control */
#define FLASH_CTRL_7_WS		0xE0
#define FLASH_CTRL_6_WS		0xC0
#define FLASH_CTRL_5_WS		0xA0
#define FLASH_CTRL_4_WS		0x80
#define FLASH_CTRL_3_WS		0x60
#define FLASH_CTRL_2_WS		0x40
#define FLASH_CTRL_1_WS		0x20
#define FLASH_CTRL_0_WS		0x00
#define FLASH_ENABLE			0x08
#define FLASH_DISABLE		0x00

/* UART */
#define UART_RBR(base)		(*((IORegExt8)(base+0)))
#define UART_THR(base)		(*((IORegExt8)(base+0)))
#define BRG_DLRL(base)		(*((IORegExt8)(base+0)))
#define BRG_DLRH(base)		(*((IORegExt8)(base+1)))
#define UART_IER(base)		(*((IORegExt8)(base+1)))
#define UART_IIR(base)		(*((IORegExt8)(base+2)))
#define UART_FCTL(base)		(*((IORegExt8)(base+2)))
#define UART_LCTL(base)		(*((IORegExt8)(base+3)))
#define UART_MCTL(base)		(*((IORegExt8)(base+4)))
#define UART_LSR(base)		(*((IORegExt8)(base+5)))
#define UART_MSR(base)		(*((IORegExt8)(base+6)))
#define UART_SPR(base)		(*((IORegExt8)(base+7)))



/* CS Bus Mode Control bits */
#define CSBM_CTL_MASK						0xC0
#define CSBM_CTL_EZ80_MODE					0x00
#define CSBM_CTL_Z80_MODE					0x40
#define CSBM_CTL_INTEL_MODE				0x80
#define CSBM_CTL_MOTOROLA_MODE			0xC0
#define CSBM_CTL_AD_MUX						0x20
#define CSBM_CTL_EZ80_CLOCKS_MASK		0x0F
#define CSBM_CTL_1_EZ80_CLOCKS			0x01
#define CSBM_CTL_2_EZ80_CLOCKS			0x02
#define CSBM_CTL_3_EZ80_CLOCKS			0x03
#define CSBM_CTL_4_EZ80_CLOCKS			0x04
#define CSBM_CTL_5_EZ80_CLOCKS			0x05
#define CSBM_CTL_6_EZ80_CLOCKS			0x06
#define CSBM_CTL_7_EZ80_CLOCKS			0x07
#define CSBM_CTL_8_EZ80_CLOCKS			0x08
#define CSBM_CTL_9_EZ80_CLOCKS			0x09
#define CSBM_CTL_10_EZ80_CLOCKS			0x0A
#define CSBM_CTL_11_EZ80_CLOCKS			0x0B
#define CSBM_CTL_12_EZ80_CLOCKS			0x0C
#define CSBM_CTL_13_EZ80_CLOCKS			0x0D
#define CSBM_CTL_14_EZ80_CLOCKS			0x0E
#define CSBM_CTL_15_EZ80_CLOCKS			0x0F



/* PLL Control 0 */
#define PLL_CHARGE_PUMP_MASK				0xC0
#define PLL_100uA								0x00
#define PLL_500uA								0x40
#define PLL_1mA								0x80
#define PLL_1_5mA								0xC0

#define PLL_LOCK_MASK						0x0C
#define PLL_LOCK_8_CYCLES_OF_20ns		0x00
#define PLL_LOCK_16_CYCLES_OF_20ns		0x04
#define PLL_LOCK_8_CYCLES_OF_400ns		0x08
#define PLL_LOCL_16_CYCLES_OF_400ns		0x0C

#define PLL_CLK_MUX_MASK					0x03
#define PLL_CLK_IS_EXT						0x00
#define PLL_CLK_IS_PLL						0x01
#define PLL_CLK_IS_RTC						0x02

/* PLL Control 1 */
#define PLL_LOCKED_STATUS					0x20
#define PLL_LOCKED_INT						0x10
#define PLL_UNLOCKED_INT					0x08
#define PLL_LOCKED_IE						0x04
#define PLL_UNLOCKED_IE						0x02
#define PLL_ENABLE_PLL						0x01
#define PLL_DISABLE_PLL						0x00



/* Interrupt vector table offsets */
#define IV_ERX					0x40
#define IV_ETX					0x44
#define IV_ESYS				0x48
#define IV_PLL					0x4C
#define IV_FLSH				0x50
#define IV_TMR0				0x54
#define IV_TMR1				0x58
#define IV_TMR2				0x5C
#define IV_TMR3				0x60
#define IV_RSVD1				0x64
#define IV_RSVD2				0x68
#define IV_RTC					0x6C
#define IV_UART0				0x70
#define IV_UART1				0x74
#define IV_I2C					0x78
#define IV_SPI					0x7C
#define IV_PA0					0x80
#define IV_PA1					0x84
#define IV_PA2					0x88
#define IV_PA3					0x8C
#define IV_PA4					0x90
#define IV_PA5					0x94
#define IV_PA6					0x98
#define IV_PA7					0x9C
#define IV_PB0					0xA0
#define IV_PB1					0xA4
#define IV_PB2					0xA8
#define IV_PB3					0xAC
#define IV_PB4					0xB0
#define IV_PB5					0xB4
#define IV_PB6					0xB8
#define IV_PB7					0xBC
#define IV_PC0					0xC0
#define IV_PC1					0xC4
#define IV_PC2					0xC8
#define IV_PC3					0xCC
#define IV_PC4					0xD0
#define IV_PC5					0xD4
#define IV_PC6					0xD8
#define IV_PC7					0xDC
#define IV_PD0					0xE0
#define IV_PD1					0xE4
#define IV_PD2					0xE8
#define IV_PD3					0xEC
#define IV_PD4					0xF0
#define IV_PD5					0xF4
#define IV_PD6					0xF8
#define IV_PD7					0xFC


#endif
