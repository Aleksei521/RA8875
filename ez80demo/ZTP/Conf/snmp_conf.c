/*
 * File       :     snmp_conf.c
 *
 * Description:  This file contains SNMP related configurable parameters.
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

#ifdef SNMP

#include "asn1.h"
#include "snmpv1.h"

#define ENTERPRISE_ID 			( 12897 )
#define PRODUCT_CODE  			( 0x01 )

/*** Following are the Configuration Macroes, which can be changed by the user ***/

/** SNMP RFC says that the maximum length of Display String data Type is 255,
    If the application does not require 255 bytes for those variables, 
    this can be reduced 
**/
#define SNMP_MAX_STRING_LENGTH	( 255 )

/** This macro indiactes the maximum size of each veriable setting **/
#define SNMP_MAX_OBJECT_SIZE 400

/** Textual description of the entity.**/
#define SNMP_SYS_DESCRIPTION			"eZ80 Embedded Webserver"

/** Identification of the contact person for this managed node, 
    together with information on how to contact this person */
#define SNMP_SYS_CONTACT     			"software@zilog.com"

/** "An administratively-assigned name for this managed node. **/
#define SNMP_SYS_NAME		  			"Zilog TCP/IP Software Suite"			

/** The physical location of this node **/
#define SNMP_SYS_LOCATION    			"Inside an eZ80 near you"

/* The SysObjectId OID contains he vendor’s authoritative identification of the
 * network management subsystem  This value is allocated within the SMI
 * enterprises subtree (1.3.6.1.4.1) Choose an
 * OID within your enterprise tree which is not being used by any other product
 * from your company.  To register for an enterprise subtree, contact the
 * Internet Assigned Numbers Authority (IANA) at www.iana.org.
 * @type struct oid 
 */
OID g_sysObjectID = { {0x01, 0x03, 0x6, 0x1, 0x4, 0x1, ENTERPRISE_ID, PRODUCT_CODE}, 8};

/*
 * The g_snmpTrapTargetIP indicates where Trap messages will be sent.  You may modify as
 * appropriate for your application.
 */
INT8  g_snmpTrapTargetIP[] = "172.16.6.40";
INT16 g_snmpTrapTargetPort = 162;


/*   g_sysServices is a value which indicates the set of services that
 *   this entity primarily offers. The value is a sum.
 *   Bit definitions:
 *		Bit 7	(0x80) - Not used
 *		Bit 6   (0x40) - Application Layer 
 *		Bit 5	(0x20) - Presentation Layer
 *		Bit 4	(0x10) - Session Layer
 *		Bit 3	(0x08) - Transport Layer
 *		Bit 2	(0x04) - Network Layer
 *		Bit 1	(0x02) - Data Link Layer
 *		Bit 0   (0x01) - Physical Layer
 */
UINT32 g_sysServices = 0x4C;	/* Application layer + TCP, UDP + IP */

/*
 * SNMP Community name
 * The community name defined below is compared against the community name of all inbound
 * SNMP requests (Get, Get Next and Set).  If the community name matches then the PDU is
 * acted upon.  If the community names do not match, an authentication Trap is generated
 * (if enabled)
 */
INT8 g_snmpCommunityName[] = "public";

UINT8 	g_snmpSysDescrStr[SNMP_MAX_STRING_LENGTH] 	= SNMP_SYS_DESCRIPTION;
UINT8	g_snmpSysContactStr[SNMP_MAX_STRING_LENGTH] = { SNMP_SYS_CONTACT };
UINT8	g_snmpSysNameStr[SNMP_MAX_STRING_LENGTH] 	= { SNMP_SYS_NAME };
UINT8	g_snmpSysLocationStr[SNMP_MAX_STRING_LENGTH]= { SNMP_SYS_LOCATION };


/** Following are the system related variables **/
SNMPDisplayStr	g_sysDescr 		=  {	&g_snmpSysDescrStr, 
										sizeof(SNMP_SYS_DESCRIPTION) 
							  	   };
SNMPDisplayStr	g_sysContact 	= { 	&g_snmpSysContactStr,
							     		sizeof(SNMP_SYS_CONTACT),
							   	  };
SNMPDisplayStr	g_sysName		= { 	&g_snmpSysNameStr,
							  			sizeof(SNMP_SYS_NAME),
								  };
SNMPDisplayStr	g_sysLocation 	= {		&g_snmpSysLocationStr,
										sizeof(SNMP_SYS_LOCATION),

								  };


UINT16	g_snmpMaxStrSize = SNMP_MAX_STRING_LENGTH ;
UINT16 g_snmpMaxObjectSize = SNMP_MAX_OBJECT_SIZE;

/** Following is the Enterprise OID **/
OID g_snmpEnterpriseOid  =
{
	{	0x01, 0x03, 0x06, 0x01, 0x06, 0x03, 0x01, 0x01, 0x04, 0x03, 0x00 },
		11
};

UINT8 g_snmpEnterpriseOIDLen = 11 ;

INT16 g_EnterpriseId = ENTERPRISE_ID ;
#endif
