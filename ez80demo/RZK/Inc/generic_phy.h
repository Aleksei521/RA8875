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
 */

#ifndef _GENERIC_PHY_H_
#define _GENERIC_PHY_H_


/*
 * Common Phy registers.
 */
#define PHY_CREG								0
#define PHY_SREG								1
#define PHY_ID1_REG							2
#define PHY_ID2_REG							3
#define PHY_ANEG_ADV_REG					4

/*
 * MII Management Control Register (Register 0) bit definitions
 */
#define PHY_RST								0x8000
#define PHY_LOOPBACK							0x4000
#define PHY_100BT								0x2000
#define PHY_AUTO_NEG_ENABLE				0x1000
#define PHY_POWER_DOWN						0x0800
#define PHY_ISOLATE							0x0400
#define PHY_RESTART_AUTO_NEG				0x0200
#define PHY_FULLD								0x0100
#define PHY_COLLISION_TEST					0x0080
#define PHY_10BT								0
#define PHY_HALFD								0

/*
 * MII Management Status Register (Register 1) bit definitions
 */
#define PHY_AUTO_NEG_COMPLETE				0x0020
#define PHY_CAN_AUTO_NEG  					0x0008
#define PHY_LINK_ESTABLISHED				0x0004

/*
 * Aut-Negotiation Advertisement Register (Register 4) bit definitions
 */
#define PHY_ANEG_100_FD						0x0100
#define PHY_ANEG_100_HD						0x0080
#define PHY_ANEG_10_FD						0x0040
#define PHY_ANEG_10_HD						0x0020
#define PHY_ANEG_802_3						0x0001



/*
 * Function prototypes
 */
extern UINT16 WritePhyReg(UINT16 phy_reg, UINT16 data);
extern UINT16 ReadPhyReg(UINT16 phy_reg, UINT16* data);


/*
 * Global Variables
 */
extern const F91_EMAC_CONF_t	F91_emac_config;



#endif
