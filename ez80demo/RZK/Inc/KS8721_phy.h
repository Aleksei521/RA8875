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
 * Copyright 2004, ZiLOG Inc.
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

#ifndef _KS8721_PHY_H_
#define _KS8721_PHY_H_

#include "generic_phy.h"


/*
 * Default Phy address
 */
#define PHY_ADDRESS							0x0001

/*
 * Phy registers unique to the KS8721.
 */
#define PHY_TX_CONTROL						31

/*
 * ID register values to identify the Phy.
 * PHY_ID1 is the expected value from PHY_ID1_REG.
 * PHY_ID2 is the expected value from PHY_ID2_REG.
 */
#define PHY_ID1								0x0022
#define PHY_ID2								0x1619

/*
 * 100BaseTx PHY Control Register (Register 31) bit definitions
 */
#define PHY_MII_ISOLATE						0x001C
#define PHY_MODE_MASK						0x001C
#define PHY_MODE_100_FD						0x0018
#define PHY_MODE_10_FD						0x0014
#define PHY_MODE_DEFAULT					0x000C
#define PHY_MODE_100_HD						0x0008
#define PHY_MODE_10_HD						0x0004



#endif