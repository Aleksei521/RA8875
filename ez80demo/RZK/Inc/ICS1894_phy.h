/*
 * Copyright 2011, ZiLOG Inc.
 * All Rights Reserved
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of ZiLOG Inc., and might
 * contain proprietary, confidential and trade secret information of
 * ZiLOG, our partners and parties from which this code has been licensed.
 * 
 * The contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of ZiLOG Inc.
 */

#ifndef _ICS1894_PHY_H_
#define _ICS1894_PHY_H_

#include "generic_phy.h"


/*
 * Default Phy address
 */
#define PHY_ADDRESS                    0x11

/*
 * Phy registers unique to the ICS1894.
 * Existing code in F91PhyInit.c reads PHY_DIAG_REG to determine link speed
 * and duplex configuration.  Aliasing the detailed status register allows
 * the same code to be re-used on the ICS1894.
 */
#define PHY_EXT_STA_REG                17
#define PHY_DIAG_REG                   (PHY_EXT_STA_REG)

/*
 * ID register values to identify the Phy.
 * PHY_ID1 is the expected value from PHY_ID1_REG.
 * PHY_ID2 is the expected value from PHY_ID2_REG.
 */
#define PHY_ID1                        0x0015
#define PHY_ID2                        0xF450

/*
 * Quick Poll Detailed Status Register (Register 17) bit definitions
 */
#define PHY_100_MBPS                   0x8000
#define PHY_FULL_DUPLEX                0x4000


#endif
