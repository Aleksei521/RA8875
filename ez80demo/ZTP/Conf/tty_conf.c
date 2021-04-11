/*
 * File       :    tty_conf.c
 *
 * Description:		This file contains the configuration details
 *					of TTY
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
 */
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

//#include "kernel.h"
#include "tty.h"

// **************************
// CHANGE BELOW VALUES IF YOU KNOW WHAT YOU ARE DOING
// BELOW VALUES PRESENT IN STRUCTURE/MACRO/VARIABLES CAN BE
// CHANGED ACCORDING TO THE SYSTEM NEEDS. PLEASE REFER TO THE
// REFERENCE MANUAL FOR STRUCTURE/MACRO/VARIABLE DEFINITIONS
// **************************

// number of TTY devices present in the system
#define NUMBER_OF_TTY_DEVICES           (4)


// **************************
// PLEASE DO NOT CHANGE ANYTHING BELOW IN TTY CONFIGURATION
// **************************

/**
 * @memo Configure TTY driver resources.
 * @doc This file contains tunable elements that affect the TTY driver. The number
 * of TTY devices that are available can be adjusted here.
 */
/*@{*/
/**
 * @doc An entry is required in `#ttytab[]#' for each TTY device that will be
 * used by the system. Portions of IPworks, such as the console shell and
 * telnetd, may use TTY devices, so allow a few extras.
 */
struct  tty                                        ttytab[ NUMBER_OF_TTY_DEVICES ];

/*
 * @doc The number of TTY devices is contained in Ntty. This variable
 * should be treated as Read-Only, and it's declaration in tty_conf.c should
 * not be changed.
 */

/* Don't modify this declaration of Ntty */
UINT8 Ntty                                          = sizeof(ttytab)/sizeof(struct tty);

/*@}*/

