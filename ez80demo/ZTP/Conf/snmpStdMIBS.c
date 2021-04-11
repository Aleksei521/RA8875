/*
 * File       :     snmpStdMIBs.c
 *
 * Description:  This file contains the initialization of all the mib variables.
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

#include "ZTypes.h" 

const UINT16 g_snmpMgmt[]				= {2};
const UINT16 g_snmpMib2[] 				= {2,1} ;
const UINT16 g_snmpSystem[] 		    = {2,1,1} ;
const UINT16 g_snmpSystemDecsr[] 	    = {2,1,1,1,0} ;
const UINT16 g_snmpSysObjectID[] 		= {2,1,1,2,0} ;
const UINT16 g_snmpSysUpTime[] 			= {2,1,1,3,0}; 
const UINT16 g_snmpSysContact[]			= {2,1,1,4,0} ;
const UINT16 g_snmpSysName[] 			= {2,1,1,5,0} ;
const UINT16 g_snmpSysLocation[] 		= {2,1,1,6,0} ;
const UINT16 g_snmpSysServices[] 		= {2,1,1,7,0} ;
const UINT16 g_snmpIfGrp[] 				= {2,1,2}  ;
const UINT16 g_snmpIfNumber[] 			= {2,1,2,1,0} ;
const UINT16 g_snmpIfTable[] 			= {2,1,2,2} ;
const UINT16 g_snmpIfEntry[] 			= {2,1,2,2,1} ;
const UINT16 g_snmpAtGrp[] 				= {2,1,3} ;
const UINT16 g_snmpAtTable[] 			= {2,1,3,1} ;
const UINT16 g_snmpAtEntry[] 			= {2,1,3,1,1} ;

// IP Group 
const UINT16 g_snmpIpGrp[] 				= {2,1,4} ;
const UINT16 g_snmpIpForwarding[] 		= {2,1,4,1,0} ;
const UINT16 g_ipDefaultTTL[] 			= {2,1,4,2,0};
const UINT16 g_ipInReceives[] 			= {2,1,4,3,0} ;
const UINT16 g_ipInHdrErrors[] 			= {2,1,4,4,0} ;
const UINT16 g_ipInAddrErrors[] 		= {2,1,4,5,0} ;
const UINT16 g_ipForwDatagrams[] 		= {2,1,4,6,0} ;
const UINT16 g_ipInUnknownProtos[] 		= {2,1,4,7,0} ;
const UINT16 g_ipInDiscards[] 			= {2,1,4,8,0} ;
const UINT16 g_ipInDelivers[]         	= {2,1,4,9,0} ;
const UINT16 g_ipOutRequests[] 			= {2,1,4,10,0} ;
const UINT16 g_ipOutDiscards[] 			= {2,1,4,11,0} ;
const UINT16 g_ipOutNoRoutes[] 			= {2,1,4,12,0} ;
const UINT16 g_ipReasmTimeout[] 		= {2,1,4,13,0} ;
const UINT16 g_ipReasmReqds[] 			= {2,1,4,14,0} ;
const UINT16 g_ipReasmOKs[] 			= {2,1,4,15,0} ;
const UINT16 g_ipReasmFails[] 			= {2,1,4,16,0} ;
const UINT16 g_ipFragOKs[] 				= {2,1,4,17,0} ;
const UINT16 g_ipFragFails[] 			= {2,1,4,18,0} ;
const UINT16 g_ipFragCreates[] 			= {2,1,4,19,0} ;
const UINT16 g_ipAddrTable[] 			= {2,1,4,20} ; 
const UINT16 g_ipAddrEntry[] 			= {2,1,4,20,1} ; 	
const UINT16 g_ipRoutingDiscards[] 		= {2,1,4,23,0} ;

// ICMP Group
const UINT16 g_icmp[] 					= {2,1,5} ;
const UINT16 g_icmpInMsgs[] 			= {2,1,5,1,0} ;
const UINT16 g_icmpInErrors[]			= {2,1,5,2,0} ;
const UINT16 g_icmpInDestUnreachs[] 	= {2,1,5,3,0} ;
const UINT16 g_icmpInTimeExcds[] 		= {2,1,5,4,0} ;
const UINT16 g_icmpInParmProbs[] 		= {2,1,5,5,0} ;
const UINT16 g_icmpInSrcQuenchs[] 		= {2,1,5,6,0} ;
const UINT16 g_icmpInRedirects[] 		= {2,1,5,7,0} ;
const UINT16 g_icmpInEchos[] 			= {2,1,5,8,0} ;
const UINT16 g_icmpInEchoReps[] 		= {2,1,5,9,0} ;
const UINT16 g_icmpInTimestamps[] 		= {2,1,5,10,0} ;
const UINT16 g_icmpInTimestampReps[] 	= {2,1,5,11,0} ;
const UINT16 g_icmpInAddrMasks[] 		= {2,1,5,12,0} ;
const UINT16 g_icmpInAddrMaskReps[] 	= {2,1,5,13,0} ;
const UINT16 g_icmpOutMsgs[] 			= {2,1,5,14,0} ;
const UINT16 g_icmpOutErrors[] 			= {2,1,5,15,0} ;
const UINT16 g_icmpOutDestUnreachs[] 	= {2,1,5,16,0} ;
const UINT16 g_icmpOutTimeExcds[] 		= {2,1,5,17,0} ;
const UINT16 g_icmpOutParmProbs[] 		= {2,1,5,18,0} ;
const UINT16 g_icmpOutSrcQuenchs[] 		= {2,1,5,19,0} ;
const UINT16 g_icmpOutRedirects[] 		= {2,1,5,20,0} ;
const UINT16 g_icmpOutEchos[] 			= {2,1,5,21,0} ;
const UINT16 g_icmpOutEchoReps[] 		= {2,1,5,22,0} ;
const UINT16 g_icmpOutTimestamps[] 		= {2,1,5,23,0} ;
const UINT16 g_icmpOutTimestampReps[] 	= {2,1,5,24,0} ;
const UINT16 g_icmpOutAddrMasks[] 		= {2,1,5,25,0} ;
const UINT16 g_icmpOutAddrMaskReps[] 	= {2,1,5,26,0} ;

// TCP Group
const UINT16 g_tcp[] 					= {2,1,6} ;
const UINT16 g_tcpRtoAlgorithm[] 		= {2,1,6,1,0} ;
const UINT16 g_tcpRtoMin[] 				= {2,1,6,2,0} ;
const UINT16 g_tcpRtoMax[] 				= {2,1,6,3,0} ;
const UINT16 g_tcpMaxConn[] 			= {2,1,6,4,0} ;
const UINT16 g_tcpActiveOpens[] 		= {2,1,6,5,0} ;
const UINT16 g_tcpPassiveOpens[] 		= {2,1,6,6,0} ;
const UINT16 g_tcpAttemptFails[] 		= {2,1,6,7,0} ;
const UINT16 g_tcpEstabResets[] 		= {2,1,6,8,0} ;
const UINT16 g_tcpCurrEstab[] 			= {2,1,6,9,0} ;
const UINT16 g_tcpInSegs[] 				= {2,1,6,10,0} ;
const UINT16 g_tcpOutSegs[] 			= {2,1,6,11,0} ;
const UINT16 g_tcpRetransSegs[] 		= {2,1,6,12,0} ;
const UINT16 g_tcpConnTable[] 			= {2,1,6,13} ;
const UINT16 g_tcpConnEntry[] 			= {2,1,6,13,1} ;
const UINT16 g_tcpInErrors[] 			= {2,1,6,14,0} ;
const UINT16 g_tcpOutRsts[] 			= {2,1,6,15,0} ;

// UDP Group
const UINT16 g_udp[] 					= {2,1,7} ;
const UINT16 g_udpInDatagrams[] 		= {2,1,7,1,0} ;
const UINT16 g_udpNoPorts[] 			= {2,1,7,2,0} ;
const UINT16 g_udpInErrors[] 			= {2,1,7,3,0} ;
const UINT16 g_udpOutDatagrams[] 		= {2,1,7,4,0} ;
const UINT16 g_udpTable[] 				= {2,1,7,5} ;
const UINT16 g_udpEntry[] 				= {2,1,7,5,1} ;

// SNMP Group
const UINT16 g_snmp[] 					= {2,1,11} ;
const UINT16 g_snmpInPkts[] 			= {2,1,11,1,0} ;
const UINT16 g_snmpOutPkts[] 			= {2,1,11,2,0} ;
const UINT16 g_snmpInBadVersions[] 		= {2,1,11,3,0} ;
const UINT16 g_snmpInBadCommunityNames[]= {2,1,11,4,0} ;
const UINT16 g_snmpInBadCommunityUses[] = {2,1,11,5,0} ;
const UINT16 g_snmpInASNParseErrors[] 	= {2,1,11,6,0} ;
const UINT16 g_snmpInTooBigs[] 			= {2,1,11,8,0} ;
const UINT16 g_snmpInNoSuchNames[] 		= {2,1,11,9,0} ;
const UINT16 g_snmpInBadValues[] 		= {2,1,11,10,0} ;
const UINT16 g_snmpInReadOnlys[] 		= {2,1,11,11,0} ;
const UINT16 g_snmpInGenErrs[] 			= {2,1,11,12,0} ;
const UINT16 g_snmpInTotalReqVars[] 	= {2,1,11,13,0} ;
const UINT16 g_snmpInTotalSetVars[] 	= {2,1,11,14,0} ;
const UINT16 g_snmpInGetRequests[] 		= {2,1,11,15,0} ;
const UINT16 g_snmpInGetNexts[] 		= {2,1,11,16,0} ;
const UINT16 g_snmpInSetRequests[] 		= {2,1,11,17,0} ;
const UINT16 g_snmpInGetResponses[] 	= {2,1,11,18,0} ;
const UINT16 g_snmpInTraps[] 			= {2,1,11,19,0} ;
const UINT16 g_snmpOutTooBigs[] 		= {2,1,11,20,0} ;
const UINT16 g_snmpOutNoSuchNames[] 	= {2,1,11,21,0} ;
const UINT16 g_snmpOutBadValues[] 		= {2,1,11,22,0} ;
const UINT16 g_snmpOutGenErrors[] 		= {2,1,11,24,0} ;
const UINT16 g_snmpOutGetRequests[] 	= {2,1,11,25,0} ;
const UINT16 g_snmpOutGetNexts[] 		= {2,1,11,26,0} ;
const UINT16 g_snmpOutSetRequests[] 	= {2,1,11,27,0} ;
const UINT16 g_snmpOutGetResponses[] 	= {2,1,11,28,0} ;
const UINT16 g_snmpOutTraps[] 			= {2,1,11,29,0} ;
const UINT16 g_snmpEnableAuthenTraps[] 	= {2,1,11,30,0} ;

// Enterprises MIBs 
const UINT16 g_expExtn[] 			= { 3 };
const UINT16 g_privateExtn[] 		= {	4 } ;
const UINT16 g_EnterprisesGrp[] 	= { 4,1 };
const UINT16 g_zilogGrp[] 			= { 4,1,12897 } ;
const UINT16 g_ztpGrp[]      		= {	4,1,12897,1	} ;
const UINT16 g_snmpVersion[] 		= {	4,1,12897,1,1,0	} ;
