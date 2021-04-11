/*
 * File       :     snmpv1.h
 *
 * Description:  This file contains the structure definitions of the MIBs.
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

#ifndef _snmpv1_h_
#define _snmpv1_h_

#include "ZTypes.h"


/** SNMP Macroes**/

/** SNMP Version definition macroes**/
#define SNMP_VERSION_1				0x00

#ifdef SNMPV2
#define SNMP_VERSION_2		1
#endif

#ifdef SNMPV3
#define SNMP_VERSION_3		3
#endif

#define READ_ONLY							0x01
#define READ_WRITE							0x02
#define SN_MAX_PHYS_ADDR_SIZE				6
#define ZTPSNMP_RECVD_DATA_LEN 				4048 

/*
 * Trap Type Codes
 */
#define SN_TRAP_COLD_START					0
#define SN_TRAP_WARM_START					1
#define SN_TRAP_LINK_DOWN				   	2
#define SN_TRAP_LINK_UP					   	3
#define SN_TRAP_AUTH_FAILURE				4
#define SN_TRAP_EGP_NWIGHBOR_LOSS			5
#define SN_TRAP_ENTERPRISE_SPECIFIC			6	// for User-defined Traps


/* SNMP server stack size 	*/
#define SNMPMAXSZ							4048 
#define	SNMP_STK_SIZE						2048	
#define ZTPSNMP_RECVD_DATA_LEN 				4048 

/* SNMP server name 	*/
#define SNMP_AGENT_NAME						"SNMP_AGENT"
#define SNMP_AGENT_PORT						161		


/* Table specific constants	*/
#define SNUMF_IFTAB							22	/* 22 fields in an Interface Entry	*/
#define SNUMF_ATTAB							3	/* 3 fields in an Addr Transl. Entry	*/
#define SNUMF_AETAB							5	/* 5 fields in an Addr Entry 		*/
#define SNUMF_RTTAB							13	/* 13 fields in a Route Table Entry	*/
#define SNUMF_NMTAB							4	/* 4 fields in a IP NetToMedia Entry    */
#define SNUMF_TCTAB							5	/* 5 fields in a TCP Connection Entry	*/
#define SNUMF_UETAB							2	/* 2 fields in a UDP Entry */
#define SIF_OIDLEN							5	/* all sif variables start with 2.1.2.2.1	 */
#define SAT_OIDLEN							5	/* all sat variables start with 2.1.3.1.1	 */
#define SAE_OIDLEN							5	/* all sae variables start with 2.1.4.20.1 */
#define SRT_OIDLEN							5	/* all srt variables start with 2.1.4.21.1 */
#define SNM_OIDLEN							5	/* all snm variables start with 2.1.4.22.1 */
#define STC_OIDLEN							5	/* all stc variables start with 2.1.6.13.1 */
#define SUE_OIDLEN							5	/* all sue variables start with 2.1.7.5.1	 */
#define SIF_INDEX_LEN						1	/* index is Interface(1) */
#define SAT_INDEX_LEN						6	/* index is Interface(1), "1"(1), IP Address(4) */ 
#define SAE_INDEX_LEN						4	/* index is IP Address(4) */
#define SRT_INDEX_LEN						4	/* index is IP Address(4) */
#define SNM_INDEX_LEN						6	/* index is Interface(1), "1"(1), IP Address(4) */ 
#define STC_INDEX_LEN						10	/* index is Local IP(4), Local Port(1), Remote IP(4), Remote Port(1) */
#define SUE_INDEX_LEN						5	/* index is Local IP(4), Local Port(1)	*/


/** Notify Application when Trap is generated using this call back **/
typedef void (*ZTPSNMP_TRAP_NOTIFY)(INT8) ;


/** Exposed SNMP Strcutures **/
typedef struct 	sn_phys_addrress
{
	UINT8		Data[SN_MAX_PHYS_ADDR_SIZE];
} SN_PhysAddress_s;

/** Object Id structure **/		 
typedef struct objId
{
	UINT16    objId[32];
	UINT16    oidLen ;
}OID ;

/** SNMP Descriptor structure **/
typedef struct sn_descr_s
{
	void *		pData;
	UINT16		Length;
} SNMPDisplayStr;

/** SNMP Object Value **/
typedef union	snmpObjVal
{
	void 				*pData;
	OID		 			*pOid;	   // Object Identifier
	SNMPDisplayStr		*pDescr;	// Octet String, big Integer, Display String (Octet String)
	INT8	 			*pInt8;	
	INT16 				*pInt16;
	INT32 				*pInt24;
	INT32				*pInt32;
	UINT8				*pUint8;
	UINT16				*pUint16;
	UINT32				*pUint24;
	UINT32				*pUint32;	// Counter, Gauge, TimeTicks (encoded as an Integer)
	SN_PhysAddress_s	*pPhys;	// Physical Address (encoded as an Octet string)
	UINT32				*pIP;
	UINT32				*pCounter;
	UINT32				*pGauge;
	UINT32				*pTimeTicks;
} SNMPObjValue;

/** SNMP Object Structure **/
typedef struct snmpObj
{
	UINT16				*objId ;	
	UINT8				objIdLen ;	
	UINT8				objValType;
	SNMPObjValue		objVal;	
}SNMPObj ;

typedef struct snmpObjList
{
	OID					objId ;	
	SNMPObjValue		objVal;
	UINT8				objValType;	
	UINT8				*objMem ; // allocate memory and assign 
}SNMPObjLs;

/** SNMP Error Codes **/

typedef enum ztpsnmpErr
{
	ZTPSNMP_ERR_SUCCESS ,
	ZTPSNMP_ERR_DAEMON_CREATE = -100,	
	ZTPSNMP_ERR_SOCK_CREATE ,
	ZTPSNMP_ERR_SOCK_BIND ,
	ZTPSNMP_ERR_WRONG_ASN1_CODING,
	ZTPSNMP_ERR_INVALID_COMMUNITY_NAME,
	ZTPSNMP_ERR_INVALID_REQ_TYPE,
	ZTPSNMP_ERR_INVALID_VERSION,
	ZTPSNMP_ERR_MEM_ALLOC_FAILURE,
	ZTPSNMP_ERR_BUILD_VAR_BIND,
	ZTPSNMP_ERR_FRAME_REPLY,
	ZTPSNMP_ERR_INVALID_REQ_OP,
	ZTPSNMP_ERR_SOCK_SEND,
	ZTPSNMP_ERR_NO_SUCH_MIB,
	ZTPSNMP_ERR_INVALID_MIB_OPERATION,
	ZTPSNMP_ERR_MATCH_NOT_FOUND
}ZTP_SNMP_ERR_CODES ;


/** SNMP standard error codes **/
typedef enum ztpSNMPStdErr
{
	SNMP_OK,
	SERR_TOO_BIG,
	SERR_NO_SUCH,
	SERR_BAD_VALUE,
	SERR_READ_ONLY,
	SERR_GEN_ERR,
	SERR_NO_ACCESS,
	SERR_WRONG_TYPE,
	SERR_WRONG_LENGTH,
	SERR_WRONG_ENCODING,
	SERR_WRONG_VALUE,
	SERR_NO_CREATION,
	SERR_INCONSISTENT_VALUE,
	SERR_RESOURCE_UNAVAILABLE,
	SERR_COMMIT_FAILED,
	SERR_UNDO_FAILED,
	SERR_AUTHORIZATION_ERROR,
	SERR_NOT_WRITABLE,
	SERR_INCONSISTENT_NAME	
}ZTP_SNMP_RFC_ERR_CODES ;


#define SNMP_AUTH_TRAPS_ENABLED  		1
#define SNMP_AUTH_TRAPS_DISABLED 		2

/* Interface Table */

/**************************************************************************
 *
 *	Function Name	: sif_get()
 *					  
 *	Parameters		: SNMPObjLs	*pObj   -- SNMP Object structure pointer
 *					  
 *	Return Values	: SERR_NO_SUCH -- If the variable is not found  
 *					  
 *	Description		: This function performs a get on a variable in the 
 *					  Interfaces Table
 *
 ***************************************************************************/
extern INT16 sif_get(SNMPObjLs * pObj);

/**************************************************************************
 *
 *	Function Name	: sif_next()
 *					  
 *	Parameters		: UINT16 *pSubId   -- Next Id in the table
 *					  
 *	Return Values	: SERR_NO_SUCH -- If the variable is not found  
 *					  
 *	Description		: This function returns the next variable in the 
 *					  Interfaces Table
 *
 ***************************************************************************/
extern INT16 sif_next(	UINT16 * pSubId );

/**************************************************************************
 *
 *	Function Name	: sif_set()
 *					  
 *	Parameters		: SNMPObjLs *pObj -- Object Identifier structure
 *					  
 *	Return Values	: SERR_NO_SUCH -- If the variable is not found  
 *					  
 *	Description		: This function sets a particular variable in the 
 *					  Interfaces Table
 *
 ***************************************************************************/
 
extern INT16 sif_set(SNMPObjLs * pObj);

/**************************************************************************
 *
 *	Function Name	: sif_match()
 *					  
 *	Parameters		: SNMPObjLs	*pObj   -- SNMP Object structure pointer
 *					  UINT16 	*iface  -- Interface number
 *					  UINT16 	*field  -- Index inthe object Id 
 *					  
 *	Return Values	: ZTPSNMP_ERR_FAILURE / ZTPSNMP_ERR_SUCCESS
 *					  
 *	Description		: This function returns the particular Index number of
 *                    ifTable if the macth is found.
 *
 ***************************************************************************/
extern INT16 sif_match(SNMPObjLs * pObj, UINT16 *iface, UINT16 *field);

/* Address Translation Table */

/**************************************************************************
 *
 *	Function Name	: sat_get()
 *					  
 *	Parameters		: SNMPObjLs	*pObj   -- SNMP Object structure pointer
 *					  
 *	Return Values	: ZTPSNMP_ERR_FAILURE / ZTPSNMP_ERR_SUCCESS
 *					  
 *	Description		: This function performs get on a variable in the 
 *					  Address Translation Table
 *
 ***************************************************************************/

extern INT16 sat_get(SNMPObjLs * pObj);

/**************************************************************************
 *
 *	Function Name	: sat_next()
 *					  
 *	Parameters		: UINT16 *pSubId -- Next Id in the Table
 *					  
 *	Return Values	: ZTPSNMP_ERR_FAILURE / ZTPSNMP_ERR_SUCCESS
 *					  
 *	Description		: This function performs gets the variable in the 
 *					  Address Translation Table
 *
 ***************************************************************************/
 
extern INT16 sat_next(	UINT16 * pSubId );

/**************************************************************************
 *
 *	Function Name	: sat_set()
 *					  
 *	Parameters		: SNMPObjLs	*pObj   -- SNMP Object structure pointer
 *					  
 *	Return Values	: ZTPSNMP_ERR_FAILURE / ZTPSNMP_ERR_SUCCESS
 *					  
 *	Description		: This function performs a set operation on a variable 
 *					  in the Address Translation Table
 *
 ***************************************************************************/
 
extern INT16 sat_set(SNMPObjLs * pObj);

/**************************************************************************
 *
 *	Function Name	: sat_match()
 *					  
 *	Parameters		: SNMPObjLs	*pObj   -- SNMP Object structure pointer
 *					  UINT16 	*iface  -- Interface number
 *					  UINT16 	*entry  -- Interface number
 *					  UINT16 	*field  -- Index inthe object Id 
 *					  
 *	Return Values	: ZTPSNMP_ERR_FAILURE / ZTPSNMP_ERR_SUCCESS
 *					  
 *	Description		: This function returns the particular Index number of
 *                    atTable if the macth is found.
 *
 ***************************************************************************/
extern INT16 sat_match( SNMPObjLs * pObj, 
						UINT16 *iface, 
						UINT16 *entry, 
						UINT16 *field
					  );

/* IP Address Table */

/**************************************************************************
 *
 *	Function Name	: sae_get()
 *					  
 *	Parameters		: SNMPObjLs	*pObj   -- SNMP Object structure pointer
 *					  
 *	Return Values	: ZTPSNMP_ERR_FAILURE / ZTPSNMP_ERR_SUCCESS
 *					  
 *	Description		: This function performs get on a variable in the 
 *					  IP Address Entry Table
 *
 ***************************************************************************/

extern INT16 sae_get(SNMPObjLs * pObj);

/**************************************************************************
 *
 *	Function Name	: sae_next()
 *					  
 *	Parameters		: UINT16 *pSubId -- Next Id in the Table
 *					  
 *	Return Values	: ZTPSNMP_ERR_FAILURE / ZTPSNMP_ERR_SUCCESS
 *					  
 *	Description		: This function gets the variable in the 
 *					  IP Address Entry Table
 *
 ***************************************************************************/
extern INT16 sae_next(	UINT16 * pSubId );

/**************************************************************************
 *
 *	Function Name	: sae_set()
 *					  
 *	Parameters		: SNMPObjLs	*pObj   -- SNMP Object structure pointer
 *					  
 *	Return Values	: ZTPSNMP_ERR_FAILURE / ZTPSNMP_ERR_SUCCESS
 *					  
 *	Description		: This function performs a set operation on a variable 
 *					  in the IP Address entry Table
 *
 ***************************************************************************/
 
extern INT16 sae_set(SNMPObjLs * pObj);

/**************************************************************************
 *
 *	Function Name	: sae_match()
 *					  
 *	Parameters		: SNMPObjLs	*pObj   -- SNMP Object structure pointer
 *					  UINT16 	*iface  -- Interface number
 *					  UINT16 	*field  -- Index inthe object Id 
 *					  
 *	Return Values	: ZTPSNMP_ERR_FAILURE / ZTPSNMP_ERR_SUCCESS
 *					  
 *	Description		: This function returns the particular Index number of
 *                    IP Address Entry Table if the macth is found.
 *
 ***************************************************************************/
extern INT16 sae_match(SNMPObjLs * pObj, UINT16 *iface, UINT16 *field);

/* TCP Connection Table */

/**************************************************************************
 *
 *	Function Name	: stc_get()
 *					  
 *	Parameters		: SNMPObjLs	*pObj   -- SNMP Object structure pointer
 *					  
 *	Return Values	: ZTPSNMP_ERR_FAILURE / ZTPSNMP_ERR_SUCCESS
 *					  
 *	Description		: This function performs get on a variable in the 
 *					  TCP connections table
 *
 ***************************************************************************/
 
extern INT16 stc_get(SNMPObjLs * pObj);

/**************************************************************************
 *
 *	Function Name	: stc_next()
 *					  
 *	Parameters		: UINT16 *pSubId -- Next Id in the Table
 *					  
 *	Return Values	: ZTPSNMP_ERR_FAILURE / ZTPSNMP_ERR_SUCCESS
 *					  
 *	Description		: This function gets the next variable in the 
 *					  TCP Connection Table
 *
 ***************************************************************************/
 
extern INT16 stc_next(	UINT16 * pSubId );

/**************************************************************************
 *
 *	Function Name	: stc_set()
 *					  
 *	Parameters		: SNMPObjLs	*pObj   -- SNMP Object structure pointer
 *					  
 *	Return Values	: ZTPSNMP_ERR_FAILURE / ZTPSNMP_ERR_SUCCESS
 *					  
 *	Description		: This function performs a set operation on a variable 
 *					  in the TCP Connection Table
 *
 ***************************************************************************/
extern INT16 stc_set(SNMPObjLs * pObj);

/**************************************************************************
 *
 *	Function Name	: stc_match()
 *					  
 *	Parameters		: SNMPObjLs	*pObj   -- SNMP Object structure pointer
 *					  UINT16 	*iface  -- Interface number
 *					  UINT16 	*field  -- Index inthe object Id 
 *					  
 *	Return Values	: ZTPSNMP_ERR_FAILURE / ZTPSNMP_ERR_SUCCESS
 *					  
 *	Description		: This function returns the particular Index number of
 *                    TCP Connection Table if the macth is found.
 *
 ***************************************************************************/
extern INT16 stc_match(SNMPObjLs * pObj, UINT16 *field, UINT16 *tcbn);

/* UDP Listener Table */

/**************************************************************************
 *
 *	Function Name	: sue_get()
 *					  
 *	Parameters		: SNMPObjLs	*pObj   -- SNMP Object structure pointer
 *					  
 *	Return Values	: ZTPSNMP_ERR_FAILURE / ZTPSNMP_ERR_SUCCESS
 *					  
 *	Description		: This function performs get on a variable in the 
 *					  UDP Listener table
 *
 ***************************************************************************/
extern INT16 sue_get(SNMPObjLs * pObj);

/**************************************************************************
 *
 *	Function Name	: sue_next()
 *					  
 *	Parameters		: UINT16 *pSubId -- Next Id in the Table
 *					  
 *	Return Values	: ZTPSNMP_ERR_FAILURE / ZTPSNMP_ERR_SUCCESS
 *					  
 *	Description		: This function gets the variable in the 
 *					  UDP Listener Table
 *
 ***************************************************************************/
 
extern INT16 sue_next(	UINT16 * pSubId );

/**************************************************************************
 *
 *	Function Name	: sue_set()
 *					  
 *	Parameters		: SNMPObjLs	*pObj   -- SNMP Object structure pointer
 *					  
 *	Return Values	: ZTPSNMP_ERR_FAILURE / ZTPSNMP_ERR_SUCCESS
 *					  
 *	Description		: This function performs a set operation on a variable 
 *					  in the UDP Listener Table
 *
 ***************************************************************************/
 
extern INT16 sue_set(SNMPObjLs * pObj);

/**************************************************************************
 *
 *	Function Name	: sue_match()
 *					  
 *	Parameters		: SNMPObjLs	*pObj   -- SNMP Object structure pointer
 *					  UINT16 	*iface  -- Interface number
 *					  UINT16 	*field  -- Index inthe object Id 
 *					  
 *	Return Values	: ZTPSNMP_ERR_FAILURE / ZTPSNMP_ERR_SUCCESS
 *					  
 *	Description		: This function returns the particular Index number of
 *                    UDP Listener Table if the macth is found.
 *
 ***************************************************************************/
extern INT16 sue_match(SNMPObjLs * pObj, UINT16 *field, UINT16 *dgn);

/**************************************************************************
 *
 *	Function Name	: ztpSnmpV1Init()
 *					  
 *	Parameters		: snTrapNotifyFunc -- Callback function provided
 *					  by the application to notify whenever any trap is
 *					  generated.
 *					  
 *	Return Values	: ZTPSNMP_ERR_SUCCESS if everything goes ok, 
 *					  otherwise ZTPSNMP_ERR_DAEMON_CREATE
 *					  will be returned.
 *					  
 *	Description		: This function creates an SNMP Agent thread, which inturn
 *					  responds for the manegers requests.
 *
 ***************************************************************************/
extern INT16
ztpSnmpV1Init( ZTPSNMP_TRAP_NOTIFY snTrapNotifyFunc ) ;

/**************************************************************************
 *
 *	Function Name	: ztpSnmpV2Init()
 *					  
 *	Parameters		: snTrapNotifyFunc -- Callback function provided
 *					  by the application to notify whenever any trap is
 *					  generated.
 *					  
 *	Return Values	: ZTPSNMP_ERR_SUCCESS if everything goes ok, 
 *					  otherwise ZTPSNMP_ERR_DAEMON_CREATE
 *					  will be returned.
 *					  
 *	Description		: This function creates an SNMPV2 Agent thread, which inturn
 *					  responds for the manegers requests.
 *
 ***************************************************************************/
extern INT16 
ztpSnmpV2Init(ZTPSNMP_TRAP_NOTIFY snTrapNotifyFunc);

/**************************************************************************
 *
 *	Function Name	: snmpGenerateTrap()
 *					  
 *	Parameters		: UINT8  	Type   			-- Type of TRAP
 *					  UINT32    Code   			-- Code of TRAP msg	
 *					  INT32		snmpVersionNum	-- Version Number
 *					  UINT16    NumObjects		-- Number of Objects 
 *					  SNMPObj	*pObjList		-- List of Objects
 *					  
 *	Return Values	: One of the error codes present in ZTP_SNMP_ERR_CODES
 *					  enumeration
 *					  
 *	Description		: This function genrates a TRAP message
 *
 ***************************************************************************/
 
INT16
snmpGenerateTrap
( 
	INT8					*userName,
	UINT8					*pMgrAddress,
	UINT8              			Type, 
	UINT32        				Code,
	UINT16       				NumObjects, 
	SNMPObj					*pObjList
);


#endif
