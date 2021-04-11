/*
 * File       :     snmpStdMIBs.h
 *
 * Description:  This file contains the extern declarations of  all the 
 *				 mib variables.
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

#include "ZTypes.h"

extern UINT16 g_snmpMgmt[] ;
extern UINT16 g_snmpMib2[] 			    ;
extern UINT16 g_snmpSystem[] 		    ;
extern UINT16 g_snmpSystemDecsr[] 	    ;
extern UINT16 g_snmpSysObjectID[] 		;
extern UINT16 g_snmpSysUpTime[] 		; 
extern UINT16 g_snmpSysContact[]		;
extern UINT16 g_snmpSysName[] 			;
extern UINT16 g_snmpSysLocation[] 		;
extern UINT16 g_snmpSysServices[] 		;
extern UINT16 g_snmpIfGrp[] 			;
extern UINT16 g_snmpIfNumber[] 		    ;
extern UINT16 g_snmpIfTable[] 			;
extern UINT16 g_snmpIfEntry[] 			;
extern UINT16 g_snmpAtGrp[] 			;
extern UINT16 g_snmpAtTable[] 			;
extern UINT16 g_snmpAtEntry[] 			;

// IP Group 
extern UINT16 g_snmpIpGrp[] 			 ;
extern UINT16 g_snmpIpForwarding[] 	     ;
extern UINT16 g_ipDefaultTTL[] 		     ;
extern UINT16 g_ipInReceives[] 		     ;
extern UINT16 g_ipInHdrErrors[] 		 ;
extern UINT16 g_ipInAddrErrors[] 		 ;
extern UINT16 g_ipForwDatagrams[] 		 ;
extern UINT16 g_ipInUnknownProtos[] 	 ;
extern UINT16 g_ipInDiscards[] 		     ;
extern UINT16 g_ipInDelivers[]           ;
extern UINT16 g_ipOutRequests[] 		 ;
extern UINT16 g_ipOutDiscards[] 		 ;
extern UINT16 g_ipOutNoRoutes[] 		 ;
extern UINT16 g_ipReasmTimeout[] 		 ;
extern UINT16 g_ipReasmReqds[] 		     ;
extern UINT16 g_ipReasmOKs[] 			 ;
extern UINT16 g_ipReasmFails[] 		     ;
extern UINT16 g_ipFragOKs[] 			 ;
extern UINT16 g_ipFragFails[] 			 ;
extern UINT16 g_ipFragCreates[] 		 ;
extern UINT16 g_ipAddrTable[] 			 ; 
extern UINT16 g_ipAddrEntry[] 			 ; 	
extern UINT16 g_ipRoutingDiscards[] 	 ;

// ICMP Group
extern UINT16 g_icmp[] 				     ;
extern UINT16 g_icmpInMsgs[] 			 ;
extern UINT16 g_icmpInErrors[]			 ;
extern UINT16 g_icmpInDestUnreachs[] 	 ;
extern UINT16 g_icmpInTimeExcds[] 		 ;
extern UINT16 g_icmpInParmProbs[] 		 ;
extern UINT16 g_icmpInSrcQuenchs[] 	     ;
extern UINT16 g_icmpInRedirects[] 		 ;
extern UINT16 g_icmpInEchos[] 			 ;
extern UINT16 g_icmpInEchoReps[] 		 ;
extern UINT16 g_icmpInTimestamps[] 	     ;
extern UINT16 g_icmpInTimestampReps[] 	 ;
extern UINT16 g_icmpInAddrMasks[] 		 ;
extern UINT16 g_icmpInAddrMaskReps[] 	 ;
extern UINT16 g_icmpOutMsgs[] 			 ;
extern UINT16 g_icmpOutErrors[] 		 ;
extern UINT16 g_icmpOutDestUnreachs[] 	 ;
extern UINT16 g_icmpOutTimeExcds[] 	     ;
extern UINT16 g_icmpOutParmProbs[] 	     ;
extern UINT16 g_icmpOutSrcQuenchs[] 	 ;
extern UINT16 g_icmpOutRedirects[] 	     ;
extern UINT16 g_icmpOutEchos[] 		     ;
extern UINT16 g_icmpOutEchoReps[] 		 ;
extern UINT16 g_icmpOutTimestamps[] 	 ;
extern UINT16 g_icmpOutTimestampReps[]   ;
extern UINT16 g_icmpOutAddrMasks[] 	     ;
extern UINT16 g_icmpOutAddrMaskReps[] 	 ;

// TCP Group
extern UINT16 g_tcp[] 					 ;
extern UINT16 g_tcpRtoAlgorithm[] 		 ;
extern UINT16 g_tcpRtoMin[] 			 ;
extern UINT16 g_tcpRtoMax[] 			 ;
extern UINT16 g_tcpMaxConn[] 			 ;
extern UINT16 g_tcpActiveOpens[] 		 ;
extern UINT16 g_tcpPassiveOpens[] 		 ;
extern UINT16 g_tcpAttemptFails[] 		 ;
extern UINT16 g_tcpEstabResets[] 		 ;
extern UINT16 g_tcpCurrEstab[] 		     ;
extern UINT16 g_tcpInSegs[] 			 ;
extern UINT16 g_tcpOutSegs[] 			 ;
extern UINT16 g_tcpRetransSegs[] 		 ;
extern UINT16 g_tcpConnTable[] 		     ;
extern UINT16 g_tcpConnEntry[] 		     ;
extern UINT16 g_tcpInErrors[] 			 ;
extern UINT16 g_tcpOutRsts[] 			 ;

// UDP Group
extern UINT16 g_udp[] 					 ;
extern UINT16 g_udpInDatagrams[] 		 ;
extern UINT16 g_udpNoPorts[] 			 ;
extern UINT16 g_udpInErrors[] 			 ;
extern UINT16 g_udpOutDatagrams[] 		 ;
extern UINT16 g_udpTable[] 			     ;
extern UINT16 g_udpEntry[] 			 	 ;

// SNMP Group
extern UINT16 g_snmp[] 					 	 ;
extern UINT16 g_snmpInPkts[] 				 ;
extern UINT16 g_snmpOutPkts[] 				 ;
extern UINT16 g_snmpInBadVersions[] 		 ;
extern UINT16 g_snmpInBadCommunityNames[] 	 ;
extern UINT16 g_snmpInBadCommunityUses[] 	 ;
extern UINT16 g_snmpInASNParseErrors[] 	 	 ;
extern UINT16 g_snmpInTooBigs[] 			 ;
extern UINT16 g_snmpInNoSuchNames[] 		 ;
extern UINT16 g_snmpInBadValues[] 			 ;
extern UINT16 g_snmpInReadOnlys[] 			 ;
extern UINT16 g_snmpInGenErrs[] 			 ;
extern UINT16 g_snmpInTotalReqVars[] 		 ;
extern UINT16 g_snmpInTotalSetVars[] 		 ;
extern UINT16 g_snmpInGetRequests[] 		 ;
extern UINT16 g_snmpInGetNexts[] 			 ;
extern UINT16 g_snmpInSetRequests[] 		 ;
extern UINT16 g_snmpInGetResponses[] 		 ;
extern UINT16 g_snmpInTraps[] 				 ;
extern UINT16 g_snmpOutTooBigs[] 			 ;
extern UINT16 g_snmpOutNoSuchNames[] 		 ;
extern UINT16 g_snmpOutBadValues[] 		 	 ;
extern UINT16 g_snmpOutGenErrors[] 		     ;
extern UINT16 g_snmpOutGetRequests[] 		 ;
extern UINT16 g_snmpOutGetNexts[] 			 ;
extern UINT16 g_snmpOutSetRequests[] 		 ;
extern UINT16 g_snmpOutGetResponses[] 		 ;
extern UINT16 g_snmpOutTraps[] 			 	 ;
extern UINT16 g_snmpEnableAuthenTraps[] 	 ;

// Enterprises MIBs 
extern UINT16 g_expExtn[] 					 ;
extern UINT16 g_privateExtn[] 		 		 ;
extern UINT16 g_EnterprisesGrp[] 			 ;
extern UINT16 g_zilogGrp[] 					 ;		
extern UINT16 g_ztpGrp[]  					 ;    		
extern UINT16 g_snmpVersion[] 	             ;	


extern SNMPDisplayStr		g_sysContact, g_sysName, g_sysLocation, g_sysDescr;
extern UINT32	 		g_sysServices, IfNumber;
extern OID g_sysObjectID ;
extern UINT32			SysUpTime;

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

/** SNMP MIBs **/
extern UINT32 			SnmpInPkts					;
extern UINT32 			SnmpOutPkts					;
extern UINT32 			SnmpInBadVersions			;
extern UINT32 			SnmpInBadCommunityNames		;
extern UINT32 			SnmpInBadCommunityUses		;
extern UINT32 			SnmpInASNParseErrors		;
extern UINT32 			SnmpInTooBigs				;
extern UINT32 			SnmpInNoSuchNames			;
extern UINT32 			SnmpInBadValues				;
extern UINT32 			SnmpInReadOnlys				;
extern UINT32 			SnmpInGenErrs				;		
extern UINT32 			SnmpInTotalReqVars			;
extern UINT32 			SnmpInTotalSetVars			;
extern UINT32 			SnmpInGetRequests			;
extern UINT32 			SnmpInGetNexts				;
extern UINT32 			SnmpInSetRequests			;
extern UINT32 			SnmpInGetResponses			;
extern UINT32 			SnmpInTraps					;
extern UINT32 			SnmpOutTooBigs				;
extern UINT32 			SnmpOutNoSuchNames			;
extern UINT32 			SnmpOutBadValues			;
extern UINT32 			SnmpOutGenErrors			;
extern UINT32 			SnmpOutGetRequests			;
extern UINT32 			SnmpOutGetNexts				;
extern UINT32 			SnmpOutSetRequests			;
extern UINT32 			SnmpOutGetResponses			;
extern UINT32 			SnmpOutTraps				;
extern UINT32			SnmpEnableAuthenTraps ;

#ifdef SNMPV3
extern UINT32		SnmpusmStatsUnsupportedSecLevels ;
extern UINT32		SnmpusmStatsNotInTimeWindows 	 ; // read-only
extern UINT32		SnmpusmStatsUnknownUserNames 	 ; // read-only
extern UINT32		SnmpusmStatsUnknownEngineIDs  ; // read-only
extern UINT32		SnmpusmStatsWrongDigests 	 ; // read-only
extern UINT32		SnmpusmStatsDecryptionErrors ; // read-only
#endif

