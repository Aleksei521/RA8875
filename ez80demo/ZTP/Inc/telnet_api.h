/*
 * File       :     telnet_api.h
 *
 * Description:		This file contains the Client side API prototypes.
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

#ifndef TELNET_API_H
#define TELNET_API_H

#include "telnet_errors.h"

/*****************************************************************************
 * Function Name			:	TelnetRead()
 * 
 * Return Values			:	None
 * Formal Parameters		:
 *				telnetAppHandle	    -   Unique handle returned by Telnet 
 *										Client to the application during
 *										connection establishment.
 *				telnetData			-   Data received from the server.
 *				telnetDataSize		-	Size of the data.
 *
 * Description				:	This is a callback function called by the
 *								Telnet Client to notify the application
 *								when received any data from the server.
 ****************************************************************************/

/** Prototype of Read callback to be supplied by Telnet client */
typedef void (*TELNETREAD) (  TELNET_HANDLE		telnetAppHandle,
 					    	  UINT8				*telnetData,
 		    				  UINT16			telnetDataSize 
						   );

/*****************************************************************************
 * Function Name			:	TelnetOpenConnection()
 * 
 * Return Values			:	TELNET_ALREADY_CONNECTED - Indicates that the
 *								Telnet connection already exists.
 *								TELNET_INVALID_ARG - Indicates that one or 
 *								more arguments are invalid.
 *								TELNET_LOWER_LAYER_FAILURE - Indicates that a
 *								lower layer failure happened.
 *								TELNET_SUCCESS - Telnet Connection has been
 *								established successfully.
 * Formal Parameters		:
 * 				IPAddress			-	Destination's IP Address
 *				telnetAppHandle	    -   Unique handle returned by Telnet 
 *										Client to the application.
 *				telnetReadCallback	-	Call back function provided by 
 *										application to Telnet Client to
 *										read any data coming from Server.
 *
 * Description				:	This is an API called by the application to
 *								establish a connection with Telnet server
 ****************************************************************************/
extern
TELNET_RET TelnetOpenConnection( IP_ADDRESS			IpAddress,
								 TELNET_HANDLE		*telnetAppHandle,
								 TELNETREAD			telnetReadCallback
							   );

/*****************************************************************************
 * Function Name			:	TelnetCloseConnection()
 * 
 * Return Values			:	TELNET_NO_CONNECTION - Indicates that the
 *								Telnet connection is not established.
 *								TELNET_INVALID_ARG - Indicates that one or 
 *								more arguments are invalid.
 *								TELNET_LOWER_LAYER_FAILURE - Indicates that a
 *								lower layer failure happened.
 *								TELNET_SUCCESS - Successfully terminated the
 *								Telnet connection
 * Formal Parameters		:
 *				telnetAppHandle	    -   Unique handle returned by Telnet 
 *										Client to the application during
 *										connection establishment.
 *
 * Description				:	This is an API called by the application to
 *								terminate a connection with Telnet server
 ****************************************************************************/
extern
TELNET_RET TelnetCloseConnection( TELNET_HANDLE telnetAppHandle );

/*****************************************************************************
 * Function Name			:	TelnetSendData()
 * 
 * Return Values			:	TELNET_NO_CONNECTION - Indicates that the
 *								Telnet connection is not established.
 *								TELNET_INVALID_ARG - Indicates that one or 
 *								more arguments are invalid.
 *								TELNET_LOWER_LAYER_FAILURE - Indicates that a
 *								lower layer failure happened.
 *								TELNET_SUCCESS - Successfully terminated the
 *								Telnet connection
 * Formal Parameters		:
 *				telnetAppHandle	    -   Unique handle returned by Telnet 
 *										Client to the application during
 *										connection establishment.
 *				telnetData			-   Data to be sent to the server.
 *				telnetDataSize		-	Size of the data.
 *
 * Description				:	This is an API called by the application to
 *								terminate a connection with Telnet server
 ****************************************************************************/
extern
TELNET_RET TelnetSendData( TELNET_HANDLE		telnetAppHandle,
						   TELNET_DATA			*telnetData,
						   TELNET_DATA_SIZE		telnetDataSize
						 );

/*****************************************************************************
 * Function Name			:	telnet_init()
 * 
 * Return Values			:	None
 * Formal Parameters		:
 *								None
 *
 * Global variables used    :	None
 *
 * Description				:	This is an API called by the application to
 *								initialize telnet Server
 ****************************************************************************/
extern
void telnet_init(void);

#endif /* TELNET_API_H */
