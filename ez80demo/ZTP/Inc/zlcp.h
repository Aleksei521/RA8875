/*
 * File       :     zlcp.h
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
#ifndef	_ZLCP_H
#define	_ZLCP_H



#define LCP_CONFIGURE_REQUEST				 1
#define LCP_CONFIGURE_ACK					 2
#define LCP_CONFIGURE_NACK					 3
#define LCP_CONFIGURE_REJECT				 4
#define LCP_TERMINATE_REQUEST				 5
#define LCP_TERMINATE_ACK					 6
#define LCP_CODE_REJECT						 7
#define LCP_PROTOCOL_REJECT				 	 8
#define LCP_ECHO_REQUEST	 				 9
#define LCP_ECHO_REPLY		 				 10


#define LCP_OP_MAX_RECEIVE_UNIT								  1
#define LCP_OP_ASYNC_CONTROL_CHARECTER_MAP				  	  2
#define LCP_OP_AUTHENTICATION_PROTOCOL						  3
#define LCP_OP_QUALITY_PROTOCOL								  4
#define LCP_OP_MAGIC_NUMBER									  5
#define LCP_OP_PROTOCOL_FIELD_COMPRESSION					  7
#define LCP_OP_ADDRESS_AND_CONTROL_FIELD_COMPRESSION	  	  8
#define LCP_OP_FCS_ALTERNATIVES								  9
#define LCP_OP_IDENTIFICATION								  12
#define LCP_OP_CALLBACK										  13
#define LCP_OP_MULTILINK_MRRU								  17
#define LCP_OP_MULTILINK_SHORT_SEQ_NUM_HDR_FRMT			  	  18
#define LCP_OP_MULTILINK_ENDPOINT_DISCRIMINATOR			  	  19




typedef struct PppLcp {
	UINT8		Code;
	UINT8		Identifier;
	UINT16		Length;
	UINT8		Data[1];
	}PppLcp_t;

typedef struct LCP_Protocol_Reject {
	UINT8		Code;
	UINT8		Identifier;
	UINT16		Length;
	UINT16		Protocol;
	UINT8		Information[1];
	}LCP_Protocol_Reject_t;

typedef struct LCP_ECHO {
	UINT8	Code;
	UINT8	Id;
	UINT16	Length;
	UINT32	Magic;
	UINT8	Data[1];
	}LCP_ECHO_t;


typedef struct LCP_OP_MRU {
	UINT8		Type;
	UINT8		Length;
	UINT16		MRU;
	}LCP_OP_MRU_t;

typedef struct LCP_OP_ACCM {
	UINT8		Type;
	UINT8		Length;
	UINT32		ACCM;
	}LCP_OP_ACCM_t;

typedef struct LCP_OP_AUTHENTICATION {
	UINT8		Type;
	UINT8		Length;
	UINT16		Protocol;
	UINT8		Algorithm[1];
	}LCP_OP_AUTHENTICATION_t;

typedef struct LCP_OP_LINK_QUALITY {
	UINT8		Type;
	UINT8		Length;
	UINT16		Quality;
	UINT8		Data[1];
	}LCP_OP_LINK_QUALITY_t;

typedef struct LCP_OP_MAGIC {
	UINT8		Type;
	UINT8		Length;
	UINT32		MAGIC_NUM;
	}LCP_OP_MAGIC_t;

typedef struct LCP_OP_ProtFieldComp {
	UINT8	Type;
	UINT8	Length;
	}LCP_OP_ProtFieldComp_t;

typedef struct LCP_OP_AddrCtlFieldComp{
	UINT8	Type;
	UINT8	Length;
	}LCP_OP_AddrCtlFieldComp_t;

typedef struct LCP_OP_MultiLinkMRRU {
	UINT8	Type;
	UINT8	Length;
	UINT16	MRRU;
	}LCP_OP_MultiLinkMRRU_t;

typedef struct LCP_OP_CallBack {
	UINT8	Type;
	UINT8	Length;
	UINT8	Operation;
	UINT8	Data[1];
	}LCP_OP_CallBack_t;

typedef struct LCP_OP_Generic {
	UINT8	Type;
	UINT8	Length;
	}LCP_OP_Generic_t;

extern void Lcp_Handler(ETH_PKT_t *pep);
extern void Send_Lcp_Frame(ETH_PKT_t *pep);
extern void Lcp_Parse_Request(ETH_PKT_t *pep);
extern INT16 Lcp_Send_Req(void);
//extern void ppp_lcp_init(void);
extern void PPPStartNegotiations( INT16 authType ) ;

extern void In_Lcp(ETH_PKT_t *pep);
extern INT16 Lcp_Send_TerminateAck(ETH_PKT_t *pep);
extern INT16 Lcp_Send_TerminateRqst(void);
extern void Do_Protocol_Reject(ETH_PKT_t *pep);



#endif	/* _ZLCP_H */
