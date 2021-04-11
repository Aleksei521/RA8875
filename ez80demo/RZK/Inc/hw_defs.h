/*******************************************************************************
 * File       : hw_defs.h
 *
 * Description: Defines the configurable system parameters
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


#ifndef _HW_DEFS_H_
#define _HW_DEFS_H_


 /***************************
 PLEASE DO NOT CHANGE BELOW MACRO VALUES, as these are mapped to 
 eZ80 hardware.
 **************************/

/*
 * ===============================================================
 * HW Configuration for eZ80 Evaluation Board with internal NIC
 * ===============================================================
 */
#define MAX_CHIP_SELECTS	4	
#define MAX_GPIO_PORTS		4
/*
 * Chip select Control Register bit definitions
 */
#define CSCTL_WAIT_STATE_MASK			0xE0
#define CSCTL_7_WAIT_STATES				0xE0
#define CSCTL_6_WAIT_STATES				0xC0
#define CSCTL_5_WAIT_STATES				0xA0
#define CSCTL_4_WAIT_STATES				0x80
#define CSCTL_3_WAIT_STATES				0x60
#define CSCTL_2_WAIT_STATES				0x40
#define CSCTL_1_WAIT_STATES				0x20
#define CSCTL_0_WAIT_STATES				0x00
#define CSCTL_MEMORY_ACCESS				0x00
#define CSCTL_IO_ACCESS					0x10
#define CSCTL_ENABLE_CS					0x08
#define CSCTL_DISABLE_CS				0x00
/* Flash Control */
#define FLASH_CTRL_7_WS					0xE0
#define FLASH_CTRL_6_WS					0xC0
#define FLASH_CTRL_5_WS					0xA0
#define FLASH_CTRL_4_WS					0x80
#define FLASH_CTRL_3_WS					0x60
#define FLASH_CTRL_2_WS					0x40
#define FLASH_CTRL_1_WS					0x20
#define FLASH_CTRL_0_WS					0x00
#define FLASH_ENABLE					0x08
#define FLASH_DISABLE					0x00
/* Ram Control */
#define GPRM_EN							0x80
#define ERAM_EN							0x40
/* CS Bus Mode Control bits */
#define CSBM_CTL_MASK					0xC0
#define CSBM_CTL_EZ80_MODE				0x00
#define CSBM_CTL_Z80_MODE				0x40
#define CSBM_CTL_INTEL_MODE				0x80
#define CSBM_CTL_MOTOROLA_MODE			0xC0
#define CSBM_CTL_AD_MUX					0x20
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

/* PLL defines */
#define PLL_CHARGE_PUMP_MASK			0xC0
#define PLL_100uA						0x00
#define PLL_500uA						0x40
#define PLL_1mA							0x80
#define PLL_1_5mA						0xC0

#define PLL_LOCK_MASK					0x0C
#define PLL_LOCK_8_CYCLES_OF_20ns		0x00
#define PLL_LOCK_16_CYCLES_OF_20ns		0x04
#define PLL_LOCK_8_CYCLES_OF_400ns		0x08
#define PLL_LOCL_16_CYCLES_OF_400ns		0x0C

#define PLL_CLK_MUX_MASK				0x03
#define PLL_CLK_IS_EXT					0x00
#define PLL_CLK_IS_PLL					0x01
#define PLL_CLK_IS_RTC					0x02

#define PLL_LOCKED_STATUS				0x20
#define PLL_LOCKED_INT					0x10
#define PLL_UNLOCKED_INT				0x08
#define PLL_LOCKED_IE					0x04
#define PLL_UNLOCKED_IE					0x02
#define PLL_ENABLE_PLL					0x01
#define PLL_DISABLE_PLL					0x00


typedef struct CS_CONFIG_S
{
	UINT8	b_lower_bound;
	UINT8	b_upper_bound;
	UINT8	b_control;
} CS_CONFIG_S;

typedef struct 					GPIO_CONFIG_S
{
	UINT8	b_dr;
	UINT8	b_ddr;
	UINT8 	b_alt1;
	UINT8	b_alt2;
} GPIO_CONFIG_S;

typedef struct						INTERNAL_RAM_S
{
	UINT8	Control;
	UINT8	Addr;
} INTERNAL_RAM_S;

typedef struct						INTERNAL_FLASH_S
{
	UINT8	Control;
	UINT8	Addr;
} INTERNAL_FLASH_S;

typedef struct			F91_PLL_S
{
	UINT16				Divider;
	UINT8				Control_0;
	UINT8				Control_1;
} F91_PLL_CONFIG;


#endif /* _HW_DEFS_H_ */