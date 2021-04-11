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
 * File       : _eZ80190.h
 *
 * Description: Register and Macro definitions specific to ez80190
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

#ifndef __EZ80190_H_
#define __EZ80190_H_

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

/* GPIO */
#define PA_DR		(*(IORegExt8)0x96)
#define PA_DDR		(*(IORegExt8)0x97)
#define PA_ALT0		(*(IORegExt8)0xA6) 
#define PA_ALT1		(*(IORegExt8)0x98)
#define PA_ALT2		(*(IORegExt8)0x99)
#define PB_DR		(*(IORegExt8)0x9A)
#define PB_DDR		(*(IORegExt8)0x9B)
#define PB_ALT0		(*(IORegExt8)0xA7)
#define PB_ALT1		(*(IORegExt8)0x9C)
#define PB_ALT2		(*(IORegExt8)0x9D)
#define PC_DR		(*(IORegExt8)0x9E)
#define PC_DDR		(*(IORegExt8)0x9F)
#define PC_ALT0		(*(IORegExt8)0xCE)
#define PC_ALT1		(*(IORegExt8)0xA0)
#define PC_ALT2		(*(IORegExt8)0xA1)
#define PD_DR		(*(IORegExt8)0xA2)
#define PD_DDR		(*(IORegExt8)0xA3)
#define PD_ALT0		(*(IORegExt8)0xCF)
#define PD_ALT1		(*(IORegExt8)0xA4)
#define PD_ALT2		(*(IORegExt8)0xA5)

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


#define I2C0						0xC8
#define I2C1						0xD8
#define SPI0						0xB6
#define SPI1						0xBA

/* Watch Dog Timer */

#define WDT_CTL					(*(IORegInt8)(UINT8)0x93)
#define WDT_RR						(*(IORegInt8)(UINT8)0x94)

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

/* Ram Control */
#define GPRAM_EN					0x80
#define MACRAM_EN					0x40

/* Interrupt vector table offsets */
#define IV_MACC					0x00
#define IV_DMA0					0x02
#define IV_DMA1					0x04
#define IV_TMR0					0x06
#define IV_TMR1					0x08
#define IV_TMR2					0x0A
#define IV_TMR3					0x0C
#define IV_TMR4					0x0E
#define IV_TMR5					0x10
#define IV_UZI0					0x12
#define IV_UZI1					0x14
#define IV_PA0						0x16
#define IV_PA1						0x18
#define IV_PA2						0x1A
#define IV_PA3						0x1C
#define IV_PA4						0x1E
#define IV_PA5						0x20
#define IV_PA6						0x22
#define IV_PA7						0x24
#define IV_PB0						0x26
#define IV_PB1						0x28
#define IV_PB2						0x2A
#define IV_PB3						0x2C
#define IV_PB4						0x2E
#define IV_PB5						0x30
#define IV_PB6						0x32
#define IV_PB7						0x34
#define IV_PC0						0x36
#define IV_PC1						0x38
#define IV_PC2						0x3A
#define IV_PC3						0x3C
#define IV_PC4						0x3E
#define IV_PC5						0x40
#define IV_PC6						0x42
#define IV_PC7						0x44
#define IV_PD0						0x46
#define IV_PD1						0x48
#define IV_PD2						0x4A
#define IV_PD3						0x4C
#define IV_PD4						0x4E
#define IV_PD5						0x50
#define IV_PD6						0x52
#define IV_PD7						0x54
#define IV_RSV1					0x56
#define IV_RSV2					0x58
#define IV_RSV3					0x5A
#define IV_RSV4					0x5C
#define IV_RSV5					0x5E



#endif 
