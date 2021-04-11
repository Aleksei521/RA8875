 /*******************************************************************************************
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
 *******************************************************************************************/
/*******************************************************************************************
 * File       : AMD79C874_phy.h.h
 *
 * Description: Contains macros pertaining to AMD PHY
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
 *******************************************************************************************/ 

/******************************************************************************
**	src: AMD79C874_phy.h
**
**	DPC	7/19/01	Initial
**
** Description:  header file for ethernet phy (AMD 79c874)
**
******************************************************************************/

#ifndef _AMD79C874_PHY_H_
#define _AMD79C874_PHY_H_

#include "generic_phy.h"

/*
 * Default Phy address
 */
#define PHY_ADDRESS							0x001F

/*
 * Phy registers unique to the AMD79C874.
 */
#define PHY_DIAG_REG							18
#define PHY_MODE_CTRL_REG					21

/*
 * ID register values to identify the Phy.
 * PHY_ID1 is the expected value from PHY_ID1_REG.
 * PHY_ID2 is the expected value from PHY_ID2_REG.
 */
#define PHY_ID1								0x0022
#define PHY_ID2								0x561B

/*
 * Diagnostic Register (Register 18) bit definitions
 */
#define PHY_FULL_DUPLEX						0x0800
#define PHY_100_MBPS							0x0400
#define PHY_RX_PASS							0x0200
#define PHY_RX_LOCK							0x0100

/*
 * Mode Control Register (Register 21) bit definitions
 */
#define PHY_GPSI_EN							0x0800



#endif
