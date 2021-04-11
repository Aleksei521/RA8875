/****************************************************************************************
 * File			:	AddUart.c
 *
 * Description	:	Adds a UART device to DDF device table.
 *					
 * Copyright 2016, ZiLOG Inc.
 * All Rights Reserved
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of ZiLOG Inc., and might
 * contain proprietary, confidential and trade secret information of
 * ZiLOG, our partners and parties from which this code has been licensed.
 * 
 * The contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of ZiLOG Inc.
 **************************************************************************************/

#include "ZTypes.h"
#include "ZSysgen.h"
#include "ZDevice.h"

extern RZK_DEVICE_CB_t Serial0Dev ;
extern RZK_DEVICE_CB_t Serial1Dev ;


DDF_STATUS_t AddUART0(void)
{
	return RZKDevAttach( &Serial0Dev ) ;

}

DDF_STATUS_t AddUART1(void)
{
	return RZKDevAttach( &Serial1Dev ) ;
}

