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
 * File       : _eZ80F93.h
 *
 * Description: Register and Macro definitions specific to eZ80F93
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


#ifndef __EZ80F93_H_
#define __EZ80F93_H_

#include <ez80F93.h>

#define UART_RBR(base)			(*((IORegExt8)(base+0)))
#define UART_THR(base)			(*((IORegExt8)(base+0)))
#define BRG_DLRL(base)			(*((IORegExt8)(base+0)))
#define BRG_DLRH(base)			(*((IORegExt8)(base+1)))
#define UART_IER(base)			(*((IORegExt8)(base+1)))
#define UART_IIR(base)			(*((IORegExt8)(base+2)))
#define UART_FCTL(base)			(*((IORegExt8)(base+2)))
#define UART_LCTL(base)			(*((IORegExt8)(base+3)))
#define UART_MCTL(base)			(*((IORegExt8)(base+4)))
#define UART_LSR(base)			(*((IORegExt8)(base+5)))
#define UART_MSR(base)			(*((IORegExt8)(base+6)))
#define UART_SPR(base)			(*((IORegExt8)(base+7)))


/*
 * Timer Control Register bit definitions
 */
#define TCR_PRT_IRQ				0x80
#define TCR_IRQ_EN				0x40
#define TCR_BRK_STP				0x20
#define TCR_PRT_MODE				0x10
#define TCR_CLKDIV_MASK 		0x0C
#define TCR_RST_EN				0x02
#define TCR_PRT_EN				0x01


#define TMR0						0x80
#define TMR1						0x83
#define TMR2						0x86
#define TMR3						0x89
#define TMR4						0x8C
#define TMR5						0x8F

#define TMR_CTL(base)			(*((IORegExt8)(base+0)))
#define TMR_DRL(base)			(*((IORegExt8)(base+1)))
#define TMR_DRH(base)			(*((IORegExt8)(base+2)))
#define TMR_RRL(base)			(*((IORegExt8)(base+1)))
#define TMR_RRH(base)			(*((IORegExt8)(base+2)))


/* Ram Control */
#define RAM_EN						0x80

/* Flash Control */
#define FLASH_CTRL_7_WS			0xE0
#define FLASH_CTRL_6_WS			0xC0
#define FLASH_CTRL_5_WS			0xA0
#define FLASH_CTRL_4_WS			0x80
#define FLASH_CTRL_3_WS			0x60
#define FLASH_CTRL_2_WS			0x40
#define FLASH_CTRL_1_WS			0x20
#define FLASH_CTRL_0_WS			0x00
#define FLASH_ENABLE				0x08
#define FLASH_DISABLE			0x00

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


/* Interrupt vector table offsets */
#define IV_RSV1					0x00
#define IV_RSV2					0x02
#define IV_RSV3					0x04
#define IV_RSV4					0x06
#define IV_FLASH					0x08
#define IV_TMR0					0x0A
#define IV_TMR1					0x0C
#define IV_TMR2					0x0E
#define IV_TMR3					0x10
#define IV_TMR4					0x12
#define IV_TMR5					0x14
#define IV_RTC						0x16
#define IV_UART0					0x18
#define IV_UART1					0x1A
#define IV_I2C						0x1C
#define IV_SPI						0x1E
#define IV_RSV6					0x20
#define IV_RSV7					0x22
#define IV_RSV8					0x24
#define IV_RSV9					0x26
#define IV_RSV10					0x28
#define IV_RSV11					0x2A
#define IV_RSV12					0x2C
#define IV_RSV13					0x2E
#define IV_PB0						0x30
#define IV_PB1						0x32
#define IV_PB2						0x34
#define IV_PB3						0x36
#define IV_PB4						0x38
#define IV_PB5						0x3A
#define IV_PB6						0x3C
#define IV_PB7						0x3E
#define IV_PC0						0x40
#define IV_PC1						0x42
#define IV_PC2						0x44
#define IV_PC3						0x46
#define IV_PC4						0x48
#define IV_PC5						0x4A
#define IV_PC6						0x4C
#define IV_PC7						0x4E
#define IV_PD0						0x50
#define IV_PD1 					0x52
#define IV_PD2						0x54
#define IV_PD3 					0x56
#define IV_PD4 					0x58
#define IV_PD5 					0x5A
#define IV_PD6 					0x5C
#define IV_PD7 					0x5E
#define IV_RSV14					0x60
#define IV_RSV15					0x62
#define IV_RSV16					0x64
#define IV_NMI	   				0x66

#endif
