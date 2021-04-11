/*
 * File       :     zppp.h
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
#ifndef	_ZPPP_H
#define	_ZPPP_H


#define		PPP_CLIENT			1
#define		PPP_SERVER			2


#define		MAXNETBUF		1500	/* max network buffer length	*/
#define		MAXLRGBUF		1500	/* size of large net bufs	*/

#define 	PPP_SUCCESS				((short)1)
#define		PPP_FAILURE				((short)0)
#define		PPP_MALLOC_FAIL			((short)-1)
#define		PPP_DEVICE_INIT_FAIL	((short)-2)
#define		PPP_DEVICE_OPEN_FAIL	((short)-3)
#define		PPP_DEVICE_READ_FAIL	((short)-4)
#define		PPP_DEVICE_WRITE_FAIL	((short)-5)
#define		PPP_DEVICE_CLOSE_FAIL	((short)-6)
#define		PPP_ALREADY_INITIALIZED	((short)-7)


typedef INT16 ( * PPP_LINK_DEV_INIT )(void);
typedef INT16 ( * PPP_LINK_DEV_READ )(ETH_PKT_t **,TICK_t);
typedef INT16 ( * PPP_LINK_DEV_WRITE )(ETH_PKT_t *,UINT16);
typedef INT16 ( * PPP_LINK_DEV_CLOSE )(void);

typedef void  ( * PPP_STATE_HANDLER_FUNCTION )(ETH_PKT_t	*PppFrame);

typedef enum { PPP_DEAD_PHASE=0,PPP_LINK_PHASE, PPP_AUTHENTICATE_PHASE,
		PPP_IPCP_PHASE, PPP_NETWORK_PHASE, PPP_TERMINATE_PHASE} PppPhase;


struct PppConf {
	/** A string that is the Peer-ID that will be sent for authentication.
	 * Typically, this will be a username that is used when connecting to
	 * the remote system.
	 */
	INT8	*myuser;

	/** A string that is the password used for authentication. 
	 */
	INT8	*mypassword;
	
	/** A value that specifies the authentication protocol to use. A value
	 * of 0 means that no authentication is required by the remote end.
	 * Typical values would be ZTP_PPP_PAP or ZTP_PPP_PAP 
	 */
	UINT16 	auth;

	/** The Maximun Receive Unit (MRU) specifies the largest packet size
	 * that can be received.
	 */
	UINT16 	MRU;

	/** Time to wait between configuration request packets when no
	 * response is received.
	 */
	UINT16	ConfigTimer;

	/** Max number of times a Configuration Request packet is sent and
	 * unanswered before the connection is terminated.
	 */
	UINT16	MaxConfigRequest;

	/** The Mode field indicates the current operating mode of PPP:
	 *	PPP_CLIENT => ZTP is acting as a PPP client.
	 * 	PPP_SERVER => ZTP is acting as a PPP Server.
	 */
	UINT8	ppp_mode;
	};

struct PppNetworkConf {

	/** A string that contains the dotted quad IP address that should be
	 * used for the local end of the connection. A NULL should be used
	 * if eZ80 is acting as client and the IP address to be obtained 
	 * from the other end of the connection.
	 */
	INT8	*myaddress;

	/** A string that contains the dotted quad IP address that should be
	 * used for the remote end of the connection. A NULL should be used
	 * to indicate that the remote if eZ80 is acting as client and the IP 
	 *  address to be obtained from the other end of the connection.
	 */
	INT8	*peeraddress;

	 /** A string that contains the dotted quad Primary DNS IP address that 
	 * should be used for the remote end of the connection. A NULL should be used
	 * to indicate that the remote if eZ80 is acting as client and the IP 
	 *  address to be obtained from the other end of the connection.
	 */
	INT8	*PrimaryDns;

	 /** A string that contains the dotted quad Secondary DNS IP address that 
	 * should be used for the remote end of the connection. A NULL should be used
	 * to indicate that the remote if eZ80 is acting as client and the IP 
	 *  address to be obtained from the other end of the connection.
	 */
	INT8	*SecondaryDns;

	 /** A string that contains the dotted quad Primary NBNS IP address that 
	 * should be used for the remote end of the connection. A NULL should be used
	 * to indicate that the remote if eZ80 is acting as client and the IP 
	 *  address to be obtained from the other end of the connection.
	 */
	INT8	*PrimaryNbns;

	 /** A string that contains the dotted quad Secondary NBNS IP address that 
	 * should be used for the remote end of the connection. A NULL should be used
	 * to indicate that the remote if eZ80 is acting as client and the IP 
	 *  address to be obtained from the other end of the connection.
	 */
	INT8	*SecondaryNbns;
};

struct PppGlobal {
	PppPhase	Phase;
	UINT32	MyIP;
	UINT32	PeerIP;
	UINT32	PppPriDns;
	UINT32	PppSecDns;
	UINT32	PppPriNbns;
	UINT32	PppSecNbns;
	struct 	netif *PppNif;
	struct 	If  *PppIf;
	UINT16	IsPPPInitialized;
};

/**
  * This structure will hold the PPPoE or HDLC layer Init, Read, Write and 
  * Colse AIP's that PPP should call.
  */
typedef struct  PPP_LINK_DEVICE{ 
	 PPP_LINK_DEV_INIT	Init;
	 PPP_LINK_DEV_READ	Read;
	 PPP_LINK_DEV_WRITE	Write;
	 PPP_LINK_DEV_CLOSE	Close;
	}PPP_LINK_DEVICE_t;


extern PPP_LINK_DEVICE_t	PPP_Link_Device;
extern INT16	g_PPPInitialized;
extern UINT8	g_EnablePppDebug;
extern UINT8	g_PPPTerminatAckReceived;	
extern UINT8	g_PppServerAutoInitialize;
extern struct 	PppGlobal	PppStatus;
extern struct 	PppConf PPP_CONF;
extern struct 	PppNetworkConf PppNwConf;


extern DDF_STATUS_t Ppp_Write (struct RZK_DEVICE_CB_t *pdev,RZK_DEV_BUFFER_t *buf,RZK_DEV_BYTES_t nBytes);
extern INT16 	ztpPPPInit(void);
extern INT16 	ztpPPPStop(void);
extern void 	PppThread( struct netif *pnif );
extern void 	ztpAddPPPShellCmds( void );

extern void PPPState_Dead( ETH_PKT_t	*PppFrame );
extern void PPPState_Link( ETH_PKT_t	*PppFrame );
extern void PPPState_Authenticate( ETH_PKT_t	*PppFrame );
extern void PPPState_Ipcp( ETH_PKT_t	*PppFrame );
extern void PPPState_Network( ETH_PKT_t	*PppFrame );
extern void PPPState_Terminate( ETH_PKT_t	*PppFrame );

#endif	/* _ZPPP_H */
