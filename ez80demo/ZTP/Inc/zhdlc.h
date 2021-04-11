/*
 * File       :     zhdlc.h
 *
 * Description: This file contains structure and defines for PPP.
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
#ifndef	_ZHDLC_H
#define	_ZHDLC_H

#define HDLC_BUF_SIZE   1600

/* HDCL define values */

#define HDLC_FLAG			0x7e
#define HDLC_ADDR			0xff
#define HDLC_CONTROL		0x03 
#define HDLC_ESCAPE			0x7d
#define HDLC_TRANS			0x20

#define HDLC_INIT_FCS		0xffff
#define HDLC_GOOD_FCS		0xf0b8


#define ZTP_PPP_IP			0x0021
#define ZTP_PPP_IPCP		0x8021
#define ZTP_PPP_LCP			0xC021
#define ZTP_PPP_PAP			0xC023
#define ZTP_PPP_LQR			0xC025
#define ZTP_PPP_CHAP		0xC223
#define ZTP_PPP_CCP			0x80fd

#define ZTP_PPP_IP_NETORDER				0x2100
#define ZTP_PPP_IPCP_NETORDER			0x2180
#define ZTP_PPP_LCP_NETORDER			0x21C0
#define ZTP_PPP_PAP_NETORDER			0x23C0
#define ZTP_PPP_LQR_NETORDER			0x25C0
#define ZTP_PPP_CHAP_NETORDER			0x23C2
#define ZTP_PPP_CCP_NETORDER			0xfd80

typedef struct chatscript {
	/** A pointer to a string that will be sent to the modem. NULL if
	 * no string should be sent.
	 */
	INT8 *SendScript;
	/** A pointer to a string that is expected from the modem. NULL if
	 * no response is expected.
	 */
	INT8 *ReceiveScript;
	/*
	 * Timeout period in seconds. This is the timeout value for receiving the 
	 * response from the modem.If the timeout is 0 then it is an infinite wait.
	 */
	UINT16 TimeOutValue;
}chatscript_t;

typedef struct AuthPair {
	 /** The User Name to be authenticated.
	 */
	INT8 *Id;
	/** The password for the above UserName.
	 */
	INT8 *Pwd;
}AuthPair_t;

typedef struct HdlcFrameStr {
	/** A flag indicating if the frame is initialized.
	 */
	INT8 IsFrame;
	/** A flag indicating if the previous charector was a escape charector.
	 */
	INT8 IsEscaped;
	/** Frame FCS value.
	 */
	UINT16 FrameFcs;
	/** Interface MRU.
	 */
	UINT16 MaxMRU;
	/** Pointer where the next data has to be place in Data buffer.
	 */
	UINT8*	DataPtr;
	/** Serial Port device handle
	 */
	RZK_DEVICE_CB_t *	SerDev;
	/** Frame Data buffer
	 */
	UINT8		Data[HDLC_BUF_SIZE];
}HdlcFrameStr_t;

typedef struct HdlcConfig {
	/** Serial device to use SERIAL0 or SERIAL1 .
	 */
	INT8 	 *SerDevName;
	/** Pointer to the chat script.
	 */
	struct chatscript *	chat;
	/** Pointer to the chat script.
	 */
	UINT16 	nchat;
}HdlcConfig_t;

extern HdlcConfig_t g_HdlcConf;

extern UINT16 CalculateFcs( UINT16 CurrentFcs, UINT8 charector );
extern INT16 HdlcInit( void );
extern INT16 HdlcRead( struct ETH_PKT * *ppep,INT32 len );
extern INT16 HdlcWrite( struct ETH_PKT *	pep, UINT16	len );
extern INT16 HdlcClose( void );


extern INT16 ModemInit( RZK_DEVICE_CB_t* SerDevHndle );
extern INT16 ModemReceive(RZK_DEVICE_CB_t* , INT8* , UINT16 );

#endif	/* _ZHDLC_H */




