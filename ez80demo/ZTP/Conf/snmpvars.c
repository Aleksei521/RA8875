/*
 * File       :     snmpvars.c
 *
 * Description:  This file contains snmp mibs.
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

#include "snmpV1.h"

UINT32 IfNumber;


/*
 * The SNMP Group
 */
UINT32 			SnmpInPkts					;
UINT32 			SnmpOutPkts					;
UINT32 			SnmpInBadVersions			;
UINT32 			SnmpInBadCommunityNames		;
UINT32 			SnmpInBadCommunityUses		;
UINT32 			SnmpInASNParseErrors		;
UINT32 			SnmpInTooBigs				;
UINT32 			SnmpInNoSuchNames			;
UINT32 			SnmpInBadValues				;
UINT32 			SnmpInReadOnlys				;
UINT32 			SnmpInGenErrs				;		
UINT32 			SnmpInTotalReqVars			;
UINT32 			SnmpInTotalSetVars			;
UINT32 			SnmpInGetRequests			;
UINT32 			SnmpInGetNexts				;
UINT32 			SnmpInSetRequests			;
UINT32 			SnmpInGetResponses			;
UINT32 			SnmpInTraps					;
UINT32 			SnmpOutTooBigs				;
UINT32 			SnmpOutNoSuchNames			;
UINT32 			SnmpOutBadValues			;
UINT32 			SnmpOutGenErrors			;
UINT32 			SnmpOutGetRequests			;
UINT32 			SnmpOutGetNexts				;
UINT32 			SnmpOutSetRequests			;
UINT32 			SnmpOutGetResponses			;
UINT32 			SnmpOutTraps				;
UINT32			SnmpEnableAuthenTraps		= SNMP_AUTH_TRAPS_ENABLED; 

/* ICMP MIB */
UINT32 IcmpInMsgs, IcmpInErrors, IcmpInDestUnreachs, IcmpInTimeExcds,
	IcmpInParmProbs, IcmpInSrcQuenchs, IcmpInRedirects, IcmpInEchos,
	IcmpInEchoReps, IcmpInTimestamps, IcmpInTimestampReps,
	IcmpInAddrMasks, IcmpInAddrMaskReps, IcmpOutMsgs, IcmpOutErrors,
	IcmpOutDestUnreachs, IcmpOutTimeExcds, IcmpOutParmProbs,
	IcmpOutSrcQuenchs, IcmpOutRedirects, IcmpOutEchos,
	IcmpOutEchoReps, IcmpOutTimestamps, IcmpOutTimestampReps,
	IcmpOutAddrMasks, IcmpOutAddrMaskReps;

/* IP MIBs */
UINT32 IpForwarding, IpDefaultTTL, IpInReceives, IpInHdrErrors,
	IpInAddrErrors, IpForwDatagrams, IpInUnknownProtos, IpInDiscards,
	IpInDelivers, IpOutRequests, IpOutDiscards, IpOutNoRoutes,
	IpReasmTimeout, IpReasmReqds, IpReasmOKs, IpReasmFails, IpFragOKs,
	IpFragFails, IpFragCreates, IpRoutingDiscards;

/* TCP MIB */
UINT32 TcpRtoAlgorithm, TcpRtoMin, TcpRtoMax, TcpMaxConn,
	TcpActiveOpens, TcpPassiveOpens, TcpAttemptFails, TcpEstabResets,
	TcpCurrEstab, TcpInSegs, TcpOutSegs, TcpRetransSegs, TcpInErrors, 
	TcpOutRsts;

/* UDP MIB */
UINT32 UdpInDatagrams, UdpNoPorts, UdpInErrors, UdpOutDatagrams;

