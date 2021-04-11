/*
 * File       :     asn1.h
 *
 * Description:  This file contains ASN formate related defines.
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
#ifndef _asn1_h_
#define _asn1_h_
#include "ztypes.h"

/* ASN.1 object types */
#define ASN1_SEQ				0x30	/* sequence object	*/
#define ASN1_INT				0x02	/* integer			*/
#define ASN1_OCTSTR				0x04	/* octet string		*/
#define ASN1_NULL				0x05	/* null				*/
#define ASN1_OBJID				0x06	/* OID				*/
#define ASN1_IPADDR				0x40	/* ip address		*/
#define ASN1_COUNTER			0x41	/* counter			*/
#define ASN1_GAUGE				0x42	/* gauge			*/
#define ASN1_TIMETICKS			0x43	/* time ticks		*/

#define ASN1_LONG_LEN	    (0x80)
/*
 * Implementation Specific Tags
 */
#define SN_DISPLAY_STR   		0xC0		
#define SN_PHYS_ADDR 			0xC1		
#define SN_INT8					0xC2		
#define SN_INT16				0xC3
#define SN_INT24				0xC4
#define SN_INT32				0xC5
#define SN_UINT8				0xC6		
#define SN_UINT16				0xC7
#define SN_UINT24				0xC8
#define SN_UINT32				0xC9

/*
 * Implementation specific Maximum lengths
 */

#define PDU_GETF		0x01
/* Protocol Data Unit types -- SNMP specific				*/
#define PDU_GET			0xA0	/* get request			*/
#define PDU_GETN		0xA1	/* get-next request		*/
#define PDU_RESP		0xA2	/* response			*/
#define PDU_SET			0xA3	/* set request			*/
#define PDU_TRAP		0xA4	/* trap message 		*/

#if	defined ( SNMPV2 ) || defined ( SNMPV3 )
#define PDU_GETBULK		0xA5
#define PDU_TRAP2		0xA7	/* trap message 		*/

#endif

#if defined ( SNMPV2 ) || defined ( SNMPV3 )
#define PDU_REPORT 	0xA8
#endif

#endif
