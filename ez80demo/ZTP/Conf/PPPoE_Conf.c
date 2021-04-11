/*
 * File       :     PPPoE_Conf.c
 *
 * Description:		This file contains the configuration details of PPPoE.
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
#include "ZTypes.h"
#include "PPPoE.h"

UINT8  g_PPPoE_PADI_RexmitCount = ZTP_PPPoE_PADI_RETRANSMIT_COUNT ;
UINT32 g_PPPoE_PADI_BlockTime   = ZTP_PPPoE_PADI_BLOCK_TIME ;