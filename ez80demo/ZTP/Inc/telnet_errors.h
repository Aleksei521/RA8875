/*
 * File       :     telnet_errors.h
 *
 * Description:		This file contains the errors returned by 
 *					Telnet Client to the application.
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
#ifndef TELNET_ERRORS_H
#define TELNET_ERRORS_H

#include "ZTypes.h"

typedef UINT16 TELNET_RET ;

/** Lists the error numbers returned by Telnet client to the application */
typedef enum{
	TELNET_SUCCESS,
	TELNET_BEGIN_ERROR_CODE = 0x400,
	TELNET_INVALID_ARG = TELNET_BEGIN_ERROR_CODE,
	TELNET_CONNECT_FAILURE,
	TELNET_CLOSE_FAILURE,
	TELNET_NO_CONNECTION,
	TELNET_ALREADY_CONNECTED,
	TELNET_OVER_SIZED_DATA,
	TELNET_ALREADY_INITIALIZED,
	TELNET_LOWER_LAYER_FAILURE,
	TELNET_FAILURE
}TELNET_RET_VALUE;

/** TELNET TYPEDEFINTIONS */

/** Telnet client application handle */
typedef		UINT8 		TELNET_HANDLE;

/** Telnet Data size */
typedef	 	UINT16	 	TELNET_DATA_SIZE;

/** Telnet Data */
typedef 	UINT8 		TELNET_DATA ;

/** IP Address */
typedef 	UINT32 		IP_ADDRESS ;

#endif /* TELNET_ERRORS_H */
