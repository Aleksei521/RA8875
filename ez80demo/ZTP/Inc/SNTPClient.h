/*

 * File       :     SNTPClient.h
 *
 * Description:  This file contains the SNTP Client related structures
 *				 and macroes.
 *
 * Copyright 2006 ZiLOG Inc.  ALL RIGHTS RESERVED.
 * This file contains unpublished confidential and proprietary information
 * of ZiLOG, Inc.
 * NO PART OF THIS WORK MAY BE DUPLICATED, STORED, PUBLISHED OR DISCLOSED 
 * IN ANY FORM WITHOUT THE PRIOR WRITTEN CONSENT OF ZiLOG, INC.
 * This is not a license and no use of any kind of this work is authorized
 * in the absence of a written license granted by ZiLOG, Inc. in ZiLOG's 
 * sole discretion 
 */


#ifndef _SNTPClient_H_
#define _SNTPClient_H_

//These define the SNTP version number
#define SNTP_VERSION_NUMBER_IPV4 0x18 //Version 3

#define SNTP_VERSION_NUMBER_IPV6 0x20 //Version 4

//This enumerator defines the error values for the server reply
typedef enum{
	SNTP_SERVER_RETURN_SUCCESS=0,
	SNTP_VERSION_NUMBER_ERROR=1,
	SNTP_MODE_ERROR=2
}SNTP_SERVER_REPLY;

//This enumerator defines the errors occured during connection establishment
typedef enum{
	SNTP_SUCCESS = 0,
	SNTP_SOCKET_CREATION_ERROR = 1,
	SNTP_SEND_TO_ERROR = 2,
	SNTP_RECIEVE_FROM_ERROR = 3,
	SNTP_IOCTLSOCKET_FAIL,
	SNTP_RZK_DEV_OPEN_ERROR,
	SNTP_MEM_ALLOC_FAILURE
}SNTP_ERRORS;

//This enumerator defines the values for the Leap Indicator SNTP uses
typedef enum{
	SNTP_LI_NO_WARNINGS = 0x00,
	SNTP_LI_61_SECONDS = 0x40,
	SNTP_LI_59_SECONDS = 0x80,
	SNTP_LI_CLOCK_NOT_SYNCHRONISED = 0xC0
}SNTP_LEAP_INDICATOR;

//This enumerator defines the values for the Modes SNTP uses
typedef enum{
	SNTP_MODE_RESERVED = 0x00,
	SNTP_MODE_SYMMETRIC_PASSIVE = 0x01,
	SNTP_MODE_SYMMETRIC_ACTIVE = 0x02,
	SNTP_MODE_CLIENT = 0x03,
	SNTP_MODE_SERVER = 0x04,
	SNTP_MODE_BROADCAST = 0x05,
	SNTP_MODE_RESERVED_FOR_NTP_CONTROL_MESSAGE = 0x06,
	SNTP_MODE_RESERVED_FOR_PRIVATE_USE = 0x07
}SNTP_MODES;
//Structure which acts as the SNTP Message Format

typedef struct zSNTP_MessageFormat
{
	INT8 zLiVnMode;	//LeapIndiactor,VersionNumber,Mode fields
	INT8 zStratum;  //Stratum Field
	INT8 zPoll;     //Poll Field
	INT8 zPrecision;  //Precision Field
	INT32 zRootDelay; //RootDelay Field
	UINT32 zRootDispersion; //RootDispersion field
	UINT32 zRefernceID;     //ReferenceID     
	UINT32 zReferenceTimeStamp_I; //ReferenceTimeStamp Integer Field
	UINT32 zReferenceTimeStamp_F; //ReferenceTimeStamp Fractional Field
	UINT32 zOriginateTimeStamp_I; //OriginateTimeStamp Integer Field
	UINT32 zOriginateTimeStamp_F; //OriginateTimeStamp Fractional Field
	UINT32 zRecieveTimeStamp_I; //RecieveTimeStamp Integer Field
	UINT32 zRecieveTimeStamp_F; //RecieveTimeStamp Fractional Field
	UINT32 zTransmitTimeStamp_I; //TransmitTimeStamp Integer Field
	UINT32 zTransmitTimeStamp_F; //TransmitTimeStamp Fractional Field
}ZSNTP_MF;

#define SNTP_SERVER_MODE 0x04  //SERVER MODE			
#define SUCCESS 0              // RETURN VALUE
#define BUFFER_SIZE 50         //BUFFER  USED TO RECEIVE DATA FROM SERVER

#endif
