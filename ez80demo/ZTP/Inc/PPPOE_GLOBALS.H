/*
 * File       :     PPPoE_Globals.h
 *
 * Description:		This file contains PPPoE globals variables.
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

#ifndef PPPoE_GLOBALS_H
#define PPPoE_GLOBALS_H

#include "ZTypes.h"
#include "PPPoE.h"

/** PPPoE Global Variables **/
extern RZK_MESSAGEQHANDLE_t g_PPPoE_RxQueueHandle;

/** PPPoE Header received from the packet **/
extern PPPoE_Header_t  g_PPPoERxHeader ;

/** Global variable to save the Rx Header to be sent **/
extern PPPoE_Header_t  g_PPPoETxHeader ;

/** Discovery Ether Type **/
extern UINT16 g_PPPoE_EthTypeDiscovery ;

/** Session Ether Type **/
extern UINT16 g_PPPoE_EthTypeSession ;

#endif
