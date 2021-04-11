/*
 * File       :     PPPoE_Internals.h
 *
 * Description:		This file contains PPPoE Constants, structures etc.
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
 */

#ifndef PPPoE_INTERNALS_H
#define PPPoE_INTERNALS_H 
#include "ZTypes.h"

/** PPPoE Version **/
#define PPPoE_VERSION 		0x01

/** PPPoE Type **/
#define PPPoE_TYPE 			0x01

/** PPPoE Codes for different Discovery packets **/
#define PPPoE_CODE_PADI		(UINT8)0x09
#define PPPoE_CODE_PADO		(UINT8)0x07
#define PPPoE_CODE_PADR		(UINT8)0x19
#define PPPoE_CODE_PADS		(UINT8)0x65
#define PPPoE_CODE_PADT		(UINT8)0xa7

/** PPPoE Ethernet codes for discovery and Session Stages **/
#define PPPoE_ETH_TYPE_DISCOVERY		0x8863
#define PPPoE_ETH_TYPE_SESSION			0x8864

/** Session Id during Discovery stage **/
#define PPPoE_DISCOVERY_SESSION_ID	0x0000

/** PPPoE States **/
typedef enum{
	PPPoE_IDLE,
	PPPoE_TERMINATE = PPPoE_IDLE,
	PPPoE_INITIALIZED,
	PPPoE_TX_PADI,
	PPPoE_RX_PADO,
	PPPoE_TX_PADR,
	PPPoE_RX_PADS,
	PPPoE_SESSION	
}PPPoE_STATES;

#define PPPoE_STATES UINT8

/** PPPoE TAG Structure **/
typedef struct PPPoE_TAG{
	UINT16 tagType;
	UINT16 tagLength ;
}PPPoE_TAG_t;

#define PPPoE_TAG_SIZE sizeof( PPPoE_TAG_t )

/* PPPoE tag types */
#define PPPoE_TAG_END_OF_LIST        0x0000
#define PPPoE_TAG_SERVICE_NAME       0x0101
#define PPPoE_TAG_AC_NAME            0x0102
#define PPPoE_TAG_HOST_UNIQ          0x0103
#define PPPoE_TAG_AC_COOKIE          0x0104
#define PPPoE_TAG_VENDOR_SPECIFIC    0x0105
#define PPPoE_TAG_RELAY_SESSION_ID   0x0110
#define PPPoE_TAG_SERVICE_NAME_ERROR 0x0201
#define PPPoE_TAG_AC_SYSTEM_ERROR    0x0202
#define PPPoE_TAG_GENERIC_ERROR      0x0203


extern UINT8  g_PPPoE_PADI_RexmitCount ;
extern UINT32 g_PPPoE_PADI_BlockTime ;


/** Extern Variables **/
extern INT8 			f91_mac_addr[] ;
#endif
