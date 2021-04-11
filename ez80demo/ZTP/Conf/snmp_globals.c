/*
 * File       :     snmp_gloabls.c
 *
 * Description:  This file contains SNMP Global variables
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



#include "netif.h"
#include "snmpv1.h"
#include "snmpvars.h"

#ifdef SNMPV3
#include "snmpv3_internals.h"
#endif

#include "snmpmib.h"
#include "snmpStdMIBs.h"
#include "asn1.h"

const UINT8 myIfTable[SNUMF_IFTAB] =
{
	ASN1_INT, 			/* ifIndex */
	SN_DISPLAY_STR ,	/* ifDescr */
	ASN1_INT,			/* ifType */
	ASN1_INT,			/* ifMtu */
	ASN1_GAUGE ,		/* ifSpeed */
	ASN1_OCTSTR ,		/* ifPhysAddress */
	ASN1_INT ,			/* ifAdminStatus */
	ASN1_INT,			/* ifOperStatus */
	ASN1_TIMETICKS,		/* ifLastChange */
	ASN1_COUNTER ,		/* ifInOctets */
	ASN1_COUNTER ,		/* ifInUcastPkts */
	ASN1_COUNTER ,		/* ifInNUcastPkts */
	ASN1_COUNTER ,		/* ifInDiscards */
	ASN1_COUNTER ,		/* ifInErrors */
	ASN1_COUNTER ,		/* ifInUnknownProtos */
	ASN1_COUNTER ,   	/* ifOutOctets */
    ASN1_COUNTER ,   	/* ifOutUcastPkts */
	ASN1_COUNTER ,   	/* ifOutNUcastPkts */
	ASN1_COUNTER ,  	/* ifOutDiscards */
	ASN1_COUNTER , 		/* ifOutErrors */
	ASN1_GAUGE , 		/* ifOutQLen */
	ASN1_OBJID 			/* ifSpecific */
};
const UINT8 myAtTable[] =
{
	ASN1_INT, 	 /* atIfIndex */
	ASN1_OCTSTR, /* atPhysAddress */
	ASN1_IPADDR  /* atNetAddress */
};


