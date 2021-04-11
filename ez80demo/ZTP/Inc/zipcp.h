/*
 * File       :     zipcp.h
 *
 * Description: This file contains structure and defines for PPP LCP.
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
#ifndef	_ZIPCP_H
#define	_ZIPCP_H


#define IPCP_CONFIGURE_REQUEST			1
#define IPCP_CONFIGURE_ACK				2
#define IPCP_CONFIGURE_NACK				3
#define IPCP_CONFIGURE_REJECT			4
#define IPCP_TERMINATE_REQUEST			5
#define IPCP_TERMINATE_ACK				6
#define IPCP_TERMINATE_NACK				7
#define IPCP_CODE_REJECT				8


#define IPCP_OP_IP_ADDRESSES	  				3
#define IPCP_OP_IP_COMPRESSION_PROTOCOL	  		2
#define IPCP_OP_PRIMARY_DNS					  	0x81
#define IPCP_OP_SECONDARY_DNS					0x83
#define IPCP_OP_PRIMARY_NBNS					0x82
#define IPCP_OP_SECONDARY_NBNS				  	0x84


typedef struct IPCP_OP_GENERIC{
	UINT8		Type;
	UINT8		Length;
	}IPCP_OP_GENERIC_t;

typedef struct PppIpcp {
	UINT8  		Code;
	UINT8  		Identifier;
	UINT16 		Length;
	UINT8  		Data[1];
	}PppIpcp_t;

typedef struct IPCP_OP_IP_ADDR{
	UINT8		Type;
	UINT8		Length;
	UINT32		IPAddr;
	}IPCP_OP_IP_ADDR_t;

typedef struct IPCP_OP_IP_COMPRESSION{
	UINT8		Type;
	UINT8		Length;
	UINT16		CompProtocol;
	UINT8		Data[1];
	}IPCP_OP_IP_COMPRESSION_t;

extern RZK_THREADHANDLE_t IPCP_HANDLE;
extern void Ipcp_Parse_Request(ETH_PKT_t *pep);
extern void IpcpRqst( void );
extern void Ipcp_Handler(ETH_PKT_t *pep);
//extern void StartIpcp( void );
extern void Ipcp_Parse_Nack(ETH_PKT_t *pep);
extern void Ipcp_Parse_Reject(ETH_PKT_t *PppFrame);

#endif	/* _ZIPCP_H */
