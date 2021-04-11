/*
 * File       :     zmodem.h
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
 
struct chatscript {
	/** A pointer to a string that will be send to the device. NULL if
	 * no string should be sent.
	 */
	INT8 *SendScript;
	/** A pointer to a string that is expected from the device. NULL if
	 * no response is expected.
	 */
	INT8 *ReceiveScript;
	/*
	 * Timeout period in seconds. This is the amount of time that we have
	 * to wait for receiving the response string. If the timeout is 0 then 
	 * it is an infinite wait.
	 */
	UINT16 TimeOutValue;
};

struct AuthPair {
	 /** The User Name to be authenticated.
	 */
	INT8 *Id;
	/** The password for the above UserName.
	 */
	INT8 *Pwd;
};

//extern INT16 ModemInit( INT8*  Dev );
extern INT16 ModemInit( RZK_DEVICE_CB_t* SerDevHndle );

