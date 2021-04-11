/******************************************************************************
 * File       : ez80def.h
 *
 * Description: Contains register definitions for eZ80
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
 *******************************************************************************/ 


#ifndef	_EZ80DEF_H
#define	_EZ80DEF_H


typedef	unsigned INT8 bool;
#define	FALSE	0
#define	TRUE	1


typedef unsigned INT8 UINT8;
typedef unsigned short UINT16;
typedef unsigned long dword;


#define	PTR_INTIO(x)	(*((__INTIO volatile UINT8*) x ))

/****************************************************************/
// Programmable Reload Counter/Timers

#define	TMR_CTL0	PTR_INTIO(0x80)
#define	TMR_DRL0	PTR_INTIO(0x81)
#define	TMR_DRH0	PTR_INTIO(0x82)
#define	TMR_RRL0	PTR_INTIO(0x81)
#define	TMR_RRH0	PTR_INTIO(0x82)

#define	TMR_CTL1	PTR_INTIO(0x83)
#define	TMR_DRL1	PTR_INTIO(0x84)
#define	TMR_DRH1	PTR_INTIO(0x85)
#define	TMR_RRL1	PTR_INTIO(0x84)
#define	TMR_RRH1	PTR_INTIO(0x85)

#define	TMR_CTL2	PTR_INTIO(0x86)
#define	TMR_DRL2	PTR_INTIO(0x87)
#define	TMR_DRH2	PTR_INTIO(0x88)
#define	TMR_RRL2	PTR_INTIO(0x87)
#define	TMR_RRH2	PTR_INTIO(0x88)

#define	TMR_CTL3	PTR_INTIO(0x89)
#define	TMR_DRL3	PTR_INTIO(0x8a)
#define	TMR_DRH3	PTR_INTIO(0x8b)
#define	TMR_RRL3	PTR_INTIO(0x8a)
#define	TMR_RRH3	PTR_INTIO(0x8b)

#define	TMR_CTL4	PTR_INTIO(0x8c)
#define	TMR_DRL4	PTR_INTIO(0x8d)
#define	TMR_DRH4	PTR_INTIO(0x8e)
#define	TMR_RRL4	PTR_INTIO(0x8d)
#define	TMR_RRH4	PTR_INTIO(0x8e)

#define	TMR_CTL5	PTR_INTIO(0x8f)
#define	TMR_DRL5	PTR_INTIO(0x90)
#define	TMR_DRH5	PTR_INTIO(0x91)
#define	TMR_RRL5	PTR_INTIO(0x90)
#define	TMR_RRH5	PTR_INTIO(0x91)

/****************************************************************/
#if 0
// Watch Dog Timer

#define	WDT_CTL		PTR_INTIO(0x93)
#define	WDT_RR		PTR_INTIO(0x94)

/****************************************************************/
// General Purpose I/O

#define	PA_DR		PTR_INTIO(0x96)
#define	PA_DDR		PTR_INTIO(0x97)
#define	PA_ALT1		PTR_INTIO(0x98)
#define	PA_ALT2		PTR_INTIO(0x99)

#define	PB_DR		PTR_INTIO(0x9a)
#define	PB_DDR		PTR_INTIO(0x9b)
#define	PB_ALT1		PTR_INTIO(0x9c)
#define	PB_ALT2		PTR_INTIO(0x9d)

#define	PC_DR		PTR_INTIO(0x9e)
#define	PC_DDR		PTR_INTIO(0x9f)
#define	PC_ALT1		PTR_INTIO(0xa0)
#define	PC_ALT2		PTR_INTIO(0xa1)

#define	PD_DR		PTR_INTIO(0xa2)
#define	PD_DDR		PTR_INTIO(0xa3)
#define	PD_ALT1		PTR_INTIO(0xa4)
#define	PD_ALT2		PTR_INTIO(0xa5)

/****************************************************************/
// Chip Select / Wait State Generator

#define	CS_LBR0		PTR_INTIO(0xa8)
#define	CS_UBR0		PTR_INTIO(0xa9)
#define	CS_CTL0		PTR_INTIO(0xaa)

#define	CS_LBR1		PTR_INTIO(0xab)
#define	CS_UBR1		PTR_INTIO(0xac)
#define	CS_CTL1		PTR_INTIO(0xad)

#define	CS_LBR2		PTR_INTIO(0xae)
#define	CS_UBR2		PTR_INTIO(0xaf)
#define	CS_CTL2		PTR_INTIO(0xb0)

#define	CS_LBR3		PTR_INTIO(0xb1)
#define	CS_UBR3		PTR_INTIO(0xb2)
#define	CS_CTL3		PTR_INTIO(0xb3)

/****************************************************************/
// Ram Control

#define	RAM_CTL0	PTR_INTIO(0xb4)
#define	RAM_CTL1	PTR_INTIO(0xb5)

/****************************************************************/
#endif



// Univeral ZiLOG Interface

#define	SPI_CTL0	PTR_INTIO(0xb6)
#define	SPI_SR0		PTR_INTIO(0xb7)
#define	SPI_RBR0	PTR_INTIO(0xb8)
#define	SPI_TSR0	PTR_INTIO(0xb8)

#define	SPI_CTL1	PTR_INTIO(0xba)
#define	SPI_SR1		PTR_INTIO(0xbb)
#define	SPI_RBR1	PTR_INTIO(0xbc)
#define	SPI_TSR1	PTR_INTIO(0xbc)

#define	BRG_DLRL0	PTR_INTIO(0xc0)
#define	BRG_DLRH0	PTR_INTIO(0xc1)
#define	UART_RBR0	PTR_INTIO(0xc0)
#define	UART_THR0	PTR_INTIO(0xc0)
#define	UART_IER0	PTR_INTIO(0xc1)
#define	UART_IIR0	PTR_INTIO(0xc2)
#define	UART_FCTL0	PTR_INTIO(0xc2)
#define	UART_LCTL0	PTR_INTIO(0xc3)
#define	UART_MCTL0	PTR_INTIO(0xc4)
#define	UART_LSR0	PTR_INTIO(0xc5)
#define	UART_MSR0	PTR_INTIO(0xc6)
#define	UART_SPR0	PTR_INTIO(0xc7)
#define	I2C_SAR0	PTR_INTIO(0xc8)
#define	I2C_XSAR0	PTR_INTIO(0xc9)
#define	I2C_DR0		PTR_INTIO(0xca)
#define	I2C_CTL0	PTR_INTIO(0xcb)
#define	I2C_SR0		PTR_INTIO(0xcc)
#define	I2C_SRR0	PTR_INTIO(0xcd)
#define	UZI_CTL0	PTR_INTIO(0xcf)

#define	BRG_DLRL1	PTR_INTIO(0xd0)
#define	BRG_DLRH1	PTR_INTIO(0xd1)
#define	UART_RBR1	PTR_INTIO(0xd0)
#define	UART_THR1	PTR_INTIO(0xd0)
#define	UART_IER1	PTR_INTIO(0xd1)
#define	UART_IIR1	PTR_INTIO(0xd2)
#define	UART_FCTL1	PTR_INTIO(0xd2)
#define	UART_LCTL1	PTR_INTIO(0xd3)
#define	UART_MCTL1	PTR_INTIO(0xd4)
#define	UART_LSR1	PTR_INTIO(0xd5)
#define	UART_MSR1	PTR_INTIO(0xd6)
#define	UART_SPR1	PTR_INTIO(0xd7)
#define	I2C_SAR1	PTR_INTIO(0xd8)
#define	I2C_XSAR1	PTR_INTIO(0xd9)
#define	I2C_DR1		PTR_INTIO(0xda)
#define	I2C_CTL1	PTR_INTIO(0xdb)
#define	I2C_SR1		PTR_INTIO(0xdc)
#define	I2C_SRR1	PTR_INTIO(0xdd)
#define	UZI_CTL1	PTR_INTIO(0xdf)

/****************************************************************/
// Multiply Accumulator

#define	MAC_XSTART	PTR_INTIO(0xe0)
#define	MAC_XEND	PTR_INTIO(0xe1)
#define	MAC_XRELOAD	PTR_INTIO(0xe2)
#define	MAC_LENGTH	PTR_INTIO(0xe3)
#define	MAC_YSTART	PTR_INTIO(0xe4)
#define	MAC_YEND	PTR_INTIO(0xe5)
#define	MAC_YRELOAD	PTR_INTIO(0xe6)
#define	MAC_CTL		PTR_INTIO(0xe7)
#define	MAC_AC0		PTR_INTIO(0xe8)
#define	MAC_AC1		PTR_INTIO(0xe9)
#define	MAC_AC2		PTR_INTIO(0xea)
#define	MAC_AC3		PTR_INTIO(0xeb)
#define	MAC_AC4		PTR_INTIO(0xec)
#define	MAC_SR		PTR_INTIO(0xed)

/****************************************************************/
// DMA Controller

#define	DMA_SARL0	PTR_INTIO(0xee)
#define	DMA_SARM0	PTR_INTIO(0xef)
#define	DMA_SARH0	PTR_INTIO(0xf0)
#define	DMA_DARL0	PTR_INTIO(0xf1)
#define	DMA_DARM0	PTR_INTIO(0xf2)
#define	DMA_DARH0	PTR_INTIO(0xf3)
#define	DMA_BCL0	PTR_INTIO(0xf4)
#define	DMA_BCH0	PTR_INTIO(0xf5)
#define	DMA_CTL0	PTR_INTIO(0xf6)

#define	DMA_SARL1	PTR_INTIO(0xf7)
#define	DMA_SARM1	PTR_INTIO(0xf8)
#define	DMA_SARH1	PTR_INTIO(0xf9)
#define	DMA_DARL1	PTR_INTIO(0xfa)
#define	DMA_DARM1	PTR_INTIO(0xfb)
#define	DMA_DARH1	PTR_INTIO(0xfc)
#define	DMA_BCL1	PTR_INTIO(0xfd)
#define	DMA_BCH1	PTR_INTIO(0xfe)
#define	DMA_CTL1	PTR_INTIO(0xff)


extern UINT8 vector_timer0 ;
extern UINT8 vector_uart0 ;
extern UINT8 vector_uart1 ;

/****************************************************************/

#endif	/* _EZ80DEF_H */

/** end of file */


