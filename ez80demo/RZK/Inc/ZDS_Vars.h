/**************************************************************************************
 * File       : ZDS_Vars.h
 *
 * Description: ZDS variables and ZDS reference
 *              
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
 **************************************************************************************/

#ifndef _ZDS_VARS_H_
#define _ZDS_VARS_H_

#include "ZTypes.h"

extern UINT32 _heapbot;
#define HEAP_BOT ((UINT8 *)(&_heapbot))
extern UINT32 _heaptop;
#define HEAP_TOP ((UINT8 *)(&_heaptop))

extern UINT32 SysClkFreq;
#define masterclock SysClkFreq

extern UINT8 _CS0_BMC_INIT_PARAM;
#define CS0_BMC_INIT_PARAM (UINT8)((UINT32)(&_CS0_BMC_INIT_PARAM))
extern UINT8 _CS1_BMC_INIT_PARAM;
#define CS1_BMC_INIT_PARAM (UINT8)((UINT32)(&_CS1_BMC_INIT_PARAM))
extern UINT8 _CS2_BMC_INIT_PARAM;
#define CS2_BMC_INIT_PARAM (UINT8)((UINT32)(&_CS2_BMC_INIT_PARAM))
extern UINT8 _CS3_BMC_INIT_PARAM;
#define CS3_BMC_INIT_PARAM (UINT8)((UINT32)(&_CS3_BMC_INIT_PARAM))

#endif _ZDS_VARS_H_
