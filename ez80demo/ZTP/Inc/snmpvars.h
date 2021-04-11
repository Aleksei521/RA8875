/*
 * File       :     snmpvars.h
 *
 * Description:  This file contains snmp mib variables.
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

#ifndef _snmpvars_h_
#define _snmpvars_h_

#include "ZTypes.h"
/* IP MIB */
extern UINT32
	IpForwarding, IpDefaultTTL, IpInReceives, IpInHdrErrors,
	IpInAddrErrors, IpForwDatagrams, IpInUnknownProtos, IpInDiscards,
	IpInDelivers, IpOutRequests, IpOutDiscards, IpOutNoRoutes,
	IpReasmTimeout, IpReasmReqds, IpReasmOKs, IpReasmFails, IpFragOKs,
	IpFragFails, IpFragCreates, IpRoutingDiscards;

/* ICMP MIB */
extern UINT32
	IcmpInMsgs, IcmpInErrors, IcmpInDestUnreachs, IcmpInTimeExcds,
	IcmpInParmProbs, IcmpInSrcQuenchs, IcmpInRedirects, IcmpInEchos,
	IcmpInEchoReps, IcmpInTimestamps, IcmpInTimestampReps,
	IcmpInAddrMasks, IcmpInAddrMaskReps, IcmpOutMsgs, IcmpOutErrors,
	IcmpOutDestUnreachs, IcmpOutTimeExcds, IcmpOutParmProbs,
	IcmpOutSrcQuenchs, IcmpOutRedirects, IcmpOutEchos,
	IcmpOutEchoReps, IcmpOutTimestamps, IcmpOutTimestampReps,
	IcmpOutAddrMasks, IcmpOutAddrMaskReps;

/* UDP MIB */
extern UINT32
	UdpInDatagrams, UdpNoPorts, UdpInErrors, UdpOutDatagrams;

/* TCP MIB */
extern UINT32
	TcpRtoAlgorithm, TcpRtoMin, TcpRtoMax, TcpMaxConn, TcpActiveOpens,
	TcpPassiveOpens, TcpAttemptFails, TcpEstabResets, TcpCurrEstab,
	TcpInSegs, TcpOutSegs, TcpRetransSegs, TcpInErrors, TcpOutRsts;

#ifdef SNMPV3
extern UINT32		SnmpusmStatsUnsupportedSecLevels ;
extern UINT32		SnmpusmStatsNotInTimeWindows 	 ; // read-only
extern UINT32		SnmpusmStatsUnknownUserNames 	 ; // read-only
extern UINT32		SnmpusmStatsUnknownEngineIDs  ; // read-only
extern UINT32		SnmpusmStatsWrongDigests 	 ; // read-only
extern UINT32		SnmpusmStatsDecryptionErrors ; // read-only
#endif

#endif
