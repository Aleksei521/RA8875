/***********************************************************************************
 * File       :eZ80Hw_conf_ZDS_Mini.c
 *
 * Description: Defines the configurable system parameters
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
 *********************************************************************************/

#include <eZ80.h>

#include "ZTypes.h"
#include "hw_defs.h"
#include "ZDS_Vars.h"

#ifdef _EZ80F91
void RZK_HW_Init( void )
{
	/*
	 * Ensure the Bus Mode registers are properly configured.
	 * This block of code can be removed once the ZDSII startup code
	 * initializes the Bus Mode control registers.
	 */
	CS0_BMC = CS0_BMC_INIT_PARAM;
	CS1_BMC = CS1_BMC_INIT_PARAM;
	CS2_BMC = CS2_BMC_INIT_PARAM;
	CS3_BMC = CS3_BMC_INIT_PARAM;

	/*
	* Reduce the Number of Wait states on Internal Flash
	 */
	FLASH_CTRL = (FLASH_CTRL & ~FLASH_CTRL_7_WS) | FLASH_CTRL_3_WS;

	/*
	 * GPIO Map for the eZ80F91_99C0942:
	 *
	 *		Port A	
	 *			PA0..PA7		All GPIOs are available to the user,
	 *							(Default setting = Mode 2, Input)
	 *		Port B
	 *			PB0..PB7		All GPIOs are available to the user,
	 *							(Default setting = Mode 2, Input)
	 *							Note: Push Button Switches 1..3 are connected to PB0..2
	 *		Port C
	 *			PC0			Modem (Uart 1) TxD, Mode 7, Alternate Function
	 *			PC1			Modem (Uart 1) RxD, Mode 7, Alternate Function
	 *			PC2			Modem (Uart 1) RTS, Mode 7, Alternate Function
	 *			PC3			Modem (Uart 1) CTS, Mode 7, Alternate Function
	 *			PC4			Modem (Uart 1) DTR, Mode 7, Alternate Function
	 *			PC5			Modem (Uart 1) DSR, Mode 7, Alternate Function
	 *			PC6			Modem (Uart 1) DCD, Mode 7, Alternate Function
	 *			PC7			Modem (Uart 1) RI,  Mode 7, Alternate Function
	 *		Port D
	 *			PD0			Console (Uart 0) TxD, Mode 7, Alternate Function
	 *			PD1			Console (Uart 0) RxD, Mode 7, Alternate Function
	 *			PD2			Console (Uart 0) RTS, Mode 7, Alternate Function
	 *			PD3			Console (Uart 0) CTS, Mode 7, Alternate Function
	 *			PD4			- available for user, Mode 2, Input
	 *			PD5			- available for user, Mode 2, Input
	 *			PD6			- available for user, Mode 2, Input
	 *			PD7			- available for user, Mode 2, Input
	 */
	PA_DR   = 0xFF;
	PA_DDR  = 0xFF;
	PA_ALT1 = 0x00;
	PA_ALT2 = 0x00;

	PB_DR   = 0xFF;
	PB_DDR  = 0xBF;		//	PB6    		RS232 Enable,  Mode 1, Output = 1
	PB_ALT1 = 0x00;
	PB_ALT2 = 0x00;

	PC_DR   = 0x00;
	PC_DDR  = 0xFF;
	PC_ALT1 = 0x00;
	PC_ALT2 = 0xFF;

	PD_DR   = 0xF0;
	PD_DDR  = 0xFF;
	PD_ALT1 = 0x00;
	PD_ALT2 = 0x0F;
}
#endif

#ifdef _EZ80F92
void RZK_HW_Init( void )
{
	/*
	 * Ensure the Bus Mode registers are properly configured.
	 * This block of code can be removed once the ZDSII startup code
	 * initializes the Bus Mode control registers.
	 */
	CS0_BMC = CS0_BMC_INIT_PARAM;
	CS1_BMC = CS1_BMC_INIT_PARAM;
	CS2_BMC = CS2_BMC_INIT_PARAM;
	CS3_BMC = CS3_BMC_INIT_PARAM;

	/*
	* Reduce the Number of Wait states on Internal Flash
	 */
	FLASH_CTRL = (FLASH_CTRL & ~FLASH_CTRL_7_WS) | FLASH_CTRL_1_WS;

	/*
	 * GPIO Map for the EZ80F920200ZCO
	 *
	 *		Port A	
	 *			PA0..PA7		Not Present
	 *		Port B
	 *			PB0..PB7		All GPIOs are available to the user,
	 *							(Default setting = Mode 2, Input)
	 *							Note: Push Button Switches 1..3 are connected to PB0..2
	 *		Port C
	 *			PC0			Modem (Uart 1) TxD, Mode 7, Alternate Function
	 *			PC1			Modem (Uart 1) RxD, Mode 7, Alternate Function
	 *			PC2			Modem (Uart 1) RTS, Mode 7, Alternate Function
	 *			PC3			Modem (Uart 1) CTS, Mode 7, Alternate Function
	 *			PC4			Modem (Uart 1) DTR, Mode 7, Alternate Function
	 *			PC5			Modem (Uart 1) DSR, Mode 7, Alternate Function
	 *			PC6			Modem (Uart 1) DCD, Mode 7, Alternate Function
	 *			PC7			Modem (Uart 1) RI,  Mode 7, Alternate Function
	 *		Port D
	 *			PD0			Console (Uart 0) TxD, Mode 7, Alternate Function
	 *			PD1			Console (Uart 0) RxD, Mode 7, Alternate Function
	 *			PD2			Console (Uart 0) RTS, Mode 7, Alternate Function
	 *			PD3			Console (Uart 0) CTS, Mode 7, Alternate Function
 	 *			PD4			CS8900A IRQ Line, 	 Mode 8, Interrupt actie high
	 *			PD5			- available for user, Mode 2, Input
	 *			PD6			CS8900A /LANLED Line, Mode 2, Input	   	(not used)
	 *			PD7			CS8900A /SLEEP Line,  Mode 1, Output = 1	(not used)
	 */
	PB_DR   = 0xFF;
	PB_DDR  = 0xFF;
	PB_ALT1 = 0x00;
	PB_ALT2 = 0x00;

	PC_DR   = 0x00;
	PC_DDR  = 0xFF;
	PC_ALT1 = 0x00;
	PC_ALT2 = 0xFF;

	PD_DR   = 0xF0;
	PD_DDR  = 0x6F;
	PD_ALT1 = 0x10;
	PD_ALT2 = 0x1F;
}

#endif


#ifdef _EZ80F93
void RZK_HW_Init( void )
{
	/*
    * Ensure the Bus Mode registers are properly configured
	 * There is a missing bracket in the definition of the CSx_BMC.
	 * Once this is corrected in a future ZDS release, it will be necessary to
	 * remove the extra bracker in the statements below.
	 */
	CS0_BMC = CS0_BMC_INIT_PARAM;
	CS1_BMC = CS1_BMC_INIT_PARAM;
	CS2_BMC = CS2_BMC_INIT_PARAM;
	CS3_BMC = CS3_BMC_INIT_PARAM;

	/*
    * Reduce the Number of Wait states on Internal Flash
	 */
	FLASH_CTRL = (FLASH_CTRL & ~FLASH_CTRL_7_WS) | FLASH_CTRL_1_WS;

	/*
	 * GPIO Map for the EZ80F93
	 *
	 *		Port A	
	 *			PA0..PA7		Not Present
	 *		Port B
	 *			PB0..PB7		All GPIOs are available to the user,
	 *							(Default setting = Mode 2, Input)
	 *							Note: Push Button Switches 1..3 are connected to PB0..2
	 *		Port C
	 *			PC0			Modem (Uart 1) TxD, Mode 7, Alternate Function
	 *			PC1			Modem (Uart 1) RxD, Mode 7, Alternate Function
	 *			PC2			Modem (Uart 1) RTS, Mode 7, Alternate Function
	 *			PC3			Modem (Uart 1) CTS, Mode 7, Alternate Function
	 *			PC4			Modem (Uart 1) DTR, Mode 7, Alternate Function
	 *			PC5			Modem (Uart 1) DSR, Mode 7, Alternate Function
	 *			PC6			Modem (Uart 1) DCD, Mode 7, Alternate Function
	 *			PC7			Modem (Uart 1) RI,  Mode 7, Alternate Function
	 *		Port D
	 *			PD0			Console (Uart 0) TxD, Mode 7, Alternate Function
	 *			PD1			Console (Uart 0) RxD, Mode 7, Alternate Function
	 *			PD2			Console (Uart 0) RTS, Mode 7, Alternate Function
	 *			PD3			Console (Uart 0) CTS, Mode 7, Alternate Function
	 *			PD4			CS8900A IRQ Line, 	 Mode 8, Interrupt actie high
	 *			PD5			- available for user, Mode 2, Input
	 *			PD6			CS8900A /LANLED Line, Mode 2, Input	   	(not used)
	 *			PD7			CS8900A /SLEEP Line,  Mode 1, Output = 1	(not used)
	 */
	PB_DR   = 0xFF;
	PB_DDR  = 0xFF;
	PB_ALT1 = 0x00;
	PB_ALT2 = 0x00;

	PC_DR   = 0x00;
	PC_DDR  = 0xFF;
	PC_ALT1 = 0x00;
	PC_ALT2 = 0xFF;

	PD_DR   = 0xF0;
	PD_DDR  = 0x6F;
	PD_ALT1 = 0x10;
	PD_ALT2 = 0x1F;

}

#endif


#ifdef _EZ80L92
void RZK_HW_Init( void )
{
	/*
	 * Ensure the Bus Mode registers are properly configured.
	 * This block of code can be removed once the ZDSII startup code
	 * initializes the Bus Mode control registers.
	 */
	CS_BMC0 = CS0_BMC_INIT_PARAM;
	CS_BMC1 = CS1_BMC_INIT_PARAM;
	CS_BMC2 = CS2_BMC_INIT_PARAM;
	CS_BMC3 = CS3_BMC_INIT_PARAM;

	/*
	 * GPIO Map for the EZ80L920210ZCO
	 *
	 *		Port A	
	 *			PA0..PA7		Not Present
	 *		Port B
	 *			PB0..PB7		All GPIOs are available to the user,
	 *							(Default setting = Mode 2, Input)
	 *							Note: Push Button Switches 1..3 are connected to PB0..2
	 *		Port C
	 *			PC0			Modem (Uart 1) TxD, Mode 7, Alternate Function
	 *			PC1			Modem (Uart 1) RxD, Mode 7, Alternate Function
	 *			PC2			Modem (Uart 1) RTS, Mode 7, Alternate Function
	 *			PC3			Modem (Uart 1) CTS, Mode 7, Alternate Function
	 *			PC4			Modem (Uart 1) DTR, Mode 7, Alternate Function
	 *			PC5			Modem (Uart 1) DSR, Mode 7, Alternate Function
	 *			PC6			Modem (Uart 1) DCD, Mode 7, Alternate Function
	 *			PC7			Modem (Uart 1) RI,  Mode 7, Alternate Function
	 *		Port D
	 *			PD0			Console (Uart 0) TxD, Mode 7, Alternate Function
	 *			PD1			Console (Uart 0) RxD, Mode 7, Alternate Function
	 *			PD2			Console (Uart 0) RTS, Mode 7, Alternate Function
	 *			PD3			Console (Uart 0) CTS, Mode 7, Alternate Function
	 *			PD4			CS8900A IRQ Line, 	 Mode 8, Interrupt actie high
	 *			PD5			- available for user, Mode 2, Input
	 *			PD6			CS8900A /LANLED Line, Mode 2, Input	   	(not used)
	 *			PD7			CS8900A /SLEEP Line,  Mode 1, Output = 1	(not used)
	 */
	PB_DR   = 0xFF;
	PB_DDR  = 0xFF;
	PB_ALT1 = 0x00;
	PB_ALT2 = 0x00;

	PC_DR   = 0x00;
	PC_DDR  = 0xFF;
	PC_ALT1 = 0x00;
	PC_ALT2 = 0xFF;

	PD_DR   = 0xF0;
	PD_DDR  = 0x6F;
	PD_ALT1 = 0x10;
	PD_ALT2 = 0x1F;
}

#endif
