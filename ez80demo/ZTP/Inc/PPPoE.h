/*
 * File       :     PPPoE.h
 *
 * Description:		This file contains PPPoE Exposed constants, error codes etc.
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

#ifndef PPPoE_H
#define PPPoE_H 

#include "ZTypes.h"
#include "EtherMgr.h"

/** PADI Retransmission Timeout **/
#define ZTP_PPPoE_PADI_RETRANSMIT_COUNT 3

/** Block Time to retransmit a PADI Frame ( in ticks ) **/
#define ZTP_PPPoE_PADI_BLOCK_TIME      20

/** PPPoE Error Codes **/
typedef enum{
	ZTP_PPPoE_ERR_SUCCESS,
	ZTP_PPPoE_ERR_ALREADY_INITIALIZED = -25,
	ZTP_PPPoE_ERR_INTERNAL ,
	ZTP_PPPoE_MEM_ALLOC_FAILURE ,
	ZTP_PPPoE_ERR_INITIALIZATION,
	ZTP_PPPoE_ERR_TIMEOUT,
	ZTP_PPPoE_ERR_INVALID_STATE,
	ZTP_PPPoE_ERR_INVALID_PACKET,
	ZTP_PPPoE_ERR_FATAL,
	ZTP_PPPoE_ERR_SEND
}ZTP_PPPoE_ERROR_CODES;

/** PPPoE Header structure **/
typedef struct PPPoE_Header{
		UINT8 		type_Version;      // PPPoE Type&Version
		UINT8		code;              // PPPoE Code
		UINT16 		sessionId;         // PPPoE SessionId
		UINT16		payloadLength;     // Length of payload
} PPPoE_Header_t;

/*****************************************************************************
 *
 * Function Name			:	PPPoECreateSession()
 * 
 * Return Values			:	Success
 *								Failure < 0
 *
 * Formal Parameters		:	None
 *				
 ****************************************************************************/
extern INT16 PPPoECreateSession( VOID );
/*****************************************************************************
 *
 * Function Name			:	PPPoERecvSessionData()
 * 
 * Return Values			:	Success - 0
 *								Failure < 0
 *
 * Formal Parameters		:	waitPeriod -- Waits to receive a packet
 *								for a specified time count
 *
 ****************************************************************************/
extern INT16 PPPoERecvSessionData(  ETH_PKT_t 	**dataToBeRead, 
									INT32 		blockTimeOut
						         );
/*****************************************************************************
 *
 * Function Name			:	PPPoETerminateSession()
 * 
 * Return Values			:	Success - 0
 *								Failure < 0
 *								
 * Formal Parameters		:	None
 *				
 ****************************************************************************/
extern INT16 PPPoETerminateSession();
/*****************************************************************************
 *
 * Function Name			:	PPPoESendSessionData()
 * 
 * Return Values			:	Success - 0
 *								Failure < 0
 *
 * Formal Parameters		:	length -- Length of the data
 *								dataToBeSent -- Actual data to be sent
 ****************************************************************************/

extern INT16 PPPoESendSessionData(  ETH_PKT_t 	*dataToBeSent, 
									UINT16 		length
								 );

#endif
