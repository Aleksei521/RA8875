/*****************************************************************************************************
 * File       : eZ80eval.c
 *
 * Description: This file contains routines that are used to print the character
 *              onto the console and to initialize the UART port.
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
 ********************************************************************************************************/

#include "ZDevice.h"
#include "ZTypes.h"
#include "ZSysgen.h"
#include "ZInterrupt.h"
#include "serial.h"

extern RZK_DEVICE_CB_t *CONSOLE ;
 

/* Function			:	putch
*
* Description		:	puts characters to terminal
* 
* Inputs			:	int
*							
* Outputs			:	int
*
* Dependencies		:	Serial driver.
*/

INT putch(INT nch)
{
DDF_STATUS_t        nStatus =0;
RZK_DEVICE_CB_t     *pDev ;
UINTRMASK           uIntmask ;

	uIntmask = RZKDisableInterrupts() ;
	pDev = CONSOLE ;
	RZKEnableInterrupts(uIntmask) ;
	
	if(pDev)
	{
		nStatus = pDev->fnWrite(pDev, 
                               (RZK_DEV_BUFFER_t *) &nch,
                               1 
                               ) ;

		if ((UINT8)nch == '\n')
		{
			nch = (UINT8)'\r' ;
			nStatus = pDev->fnWrite(pDev, 
                                    (RZK_DEV_BUFFER_t *)&nch, 
                                    1 
                                    ) ;
		}
	}
	
	return nStatus ;
}

INT putchar(INT ch)
{
  putch(ch);
  return ch;
}

/* Function			:	getch
*
* Description		:	gets characters from terminal   
* 
* Inputs			:	void
*							
* Outputs			:	int
*
* Dependencies		:	Serial Driver.
*/
INT
getch(void)
{

INT                 nCh ;
RZK_DEVICE_CB_t     *pDev ;
UINTRMASK           uIntmask ;

	uIntmask = RZKDisableInterrupts() ;
	pDev = CONSOLE ;
	RZKEnableInterrupts(uIntmask) ;
	if(pDev)
	{
		pDev->fnRead(pDev, (RZK_DEV_BUFFER_t *) &nCh, 1) ;
	}
	return nCh ;

}

/* Function			:	InitUART
*
* Description		:	Initializes UART
* 
* Inputs			:	void
*							
* Outputs			:	void
*							
*
* Dependencies		:	Serial Driver.
*/
void InitUART ( void ) 
{ 

    INT8 cMode ;

    AddUART1() ;

    CONSOLE = RZKDevOpen( 
			 (RZK_DEV_NAME_t *)"SERIAL1",
			 &cMode
			 );
}
