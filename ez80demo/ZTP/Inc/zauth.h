/*
 * File       :     zauth.h
 *
 * Description: This file contains structure and defines for PPP PAP and CHAP.
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
#ifndef	_ZAUTH_H
#define	_ZAUTH_H


#define PAP_OP_AUTH_REQUEST					  	  1
#define PAP_OP_AUTH_ACK			  				  2
#define PAP_OP_AUTH_NACK						  3


#define PAP_OP_AUTH_REQUEST					  	  1
#define PAP_OP_AUTH_ACK			  				  2
#define PAP_OP_AUTH_NACK						  3


#define CHAP_OP_AUTH_CHALLANGE				 	 1
#define CHAP_OP_AUTH_RESPONSE					 2
#define CHAP_OP_AUTH_SUCCESS					 3
#define CHAP_OP_AUTH_FAILURE 					 4


typedef struct PppPap {
	UINT8		Code;
	UINT8		Identifier;
	UINT16		Length;
	UINT8		Data[1];
	}PppPap_t;


typedef struct PppChap {
	UINT8		Code;
	UINT8		Identifier;
	UINT16		Length;
	UINT8		SizeOfData;
	UINT8		Algorithm[1];
	}PppChap_t;

extern RZK_THREADHANDLE_t g_PAP_AUTH_HANDLE;
extern RZK_THREADHANDLE_t g_CHAP_AUTH_HANDLE;

extern void PapAuth( void );
extern void ChapThread( void );
extern void Chap_Handler(ETH_PKT_t *pep);
extern void Pap_Handler(ETH_PKT_t *pep);
//extern void StartChap( void );
//extern void StartPap( void );

extern void PPPStartNegotiations( INT16 authType );



#endif	/* _ZAUTH_H */
