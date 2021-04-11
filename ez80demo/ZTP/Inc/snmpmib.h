/*
 * File       :     snmpmib.h
 *
 * Description:  This file contains the structure definitions of the MIBs.
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

#ifndef _snmp_mib__h_
#define _snmp_mib__h_

#define	T_IFTABLE		0x0	/* var is the interface table	*/
#define	T_ATTABLE		0x1	/* var is the addr translat tbl	*/
#define	T_AETABLE		0x2	/* var is the address entry tbl	*/
#define	T_TCPTABLE		0x3	/* var is the TCP conn. table	*/
#define T_SUETABLE		0x4	/* var is the UDP Entry table	*/

#ifdef SNMPV3
#define 	T_USMTABLE		0x5	/** Var is the USM Table Entry **/
#endif

#define	T_AGGREGATE		0x10	/* var is an aggregate		*/

/* this type specifies in mib.c that the object is a table.  The value is
   different than any of the ASN.1 types that SNMP uses. */
#define T_TABLE			01

/** Function typedefs **/
typedef  INT16 ( *SNMP_GET_SET_MIB_FUNC )  ( SNMPObjLs *, 
											 struct snmpMIBs *,  
											 UINT8
										   ) ;

/** MIB structure **/
typedef struct snmpMIBs 
{
	/* MIB object */
	SNMPObj				mi_obj;	
	/** Read/write status of the MIB **/
	BOOL				mi_writable;
	/** Fucntion to implement the get/set status of the MIB **/	
	SNMP_GET_SET_MIB_FUNC	mi_func ;
	/** pointer to the next MIB **/
	struct snmpMIBs *		mi_next; 	
}SNMPMIBData;

extern SNMPMIBData g_snmpMIBInfo[];

/* Information about MIB tables.  Contains functions to implement 	*/
/* operations upon variables in the tables.				*/
typedef 	INT16	(*SNMP_GET_FUNC)
(
	SNMPObjLs	*pObj
);
typedef 	INT16 	(*SNMP_NEXT_FUNC)
(	
	UINT16 		*pSubId
);
typedef 	INT16	(*SNMP_SET_FUNC)
(	
	SNMPObjLs	*pObj
);

/** Structure for a TABLE **/
typedef struct SNMP_TABLE_S
{
	SNMP_GET_FUNC		ti_get;		/* get operation  */
	SNMP_NEXT_FUNC		ti_next;	/* get next index	*/
	SNMP_SET_FUNC		ti_set;		/* set operation  */
	UINT16 				max_fields; /* number of 'colums' in the table        */
	UINT16 				index_len;	/* number of sub-identifiers in the index */
} SNMP_TABLE_S;

/**************************************************************************
 *
 *	Function Name : snleaf()
 *					  
 *	Parameters	  : SNMPObjLs	*pObj -- received object
 *					SNMPMIBData *mip  -- MIB from the MIB table
 *					UINT8		op    -- type of operation Get/Set/GetNext
 *					  
 *	Return Values : One of the error codes present in 
 * 					ZTP_SNMP_RFC_ERR_CODES enumeration
 *					  
 *	Description	  : This function performs get/set/get next operation on 
 *					the requested MIB
 *
 ***************************************************************************/
extern INT16 
snleaf
(
	SNMPObjLs				*pObj,
	SNMPMIBData 			*mip,
	UINT8					op
);

/**************************************************************************
 *
 *	Function Name	: sntable()
 *					  
 *	Parameters		: SNMPObjLs		*pObj -- Received Object 
 *					  SNMPMIBData 	*mip  -- Corresponding MIB in the table
 *					  UINT8			op    -- Get/Get next/Set operation
 *					  
 *	Return Values	: SNMP_OK or SYSERR
 *					  
 *	Description		: This function manipulates the Objects in a Table
 *
 ***************************************************************************/
extern INT16 
sntable
(
	SNMPObjLs				*pObj,
	SNMPMIBData 			*mip,
	UINT8					op
);

#endif
