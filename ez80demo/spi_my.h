/****************************************************************************************
 * File       :     config.h
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
 ***************************************************************************************/

#ifndef _SPI_MY
#define _SPI_MY  
#include "ZTypes.h"
//typedef volatile UINT8 __INTIO *PBINTIO;

//#define IO8_WRITE(io_reg, b_val) io_reg=b_val



void spi_open(void(*touch_gui)(INT16 x,INT16 y,UINT8 state));
void SpiSendCmd(UINT8 cmd);
#endif
