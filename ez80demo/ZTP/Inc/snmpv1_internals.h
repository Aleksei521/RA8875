/*
 * File       :     snmpv1_internals.h
 *
 * Description:  This file contains the internal strcutres and macroes
 *				 used by SNMP V1
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


#ifndef SNMPV3
#include "snmpv1_struct.h"
#endif

#ifdef SNMPV3
#include "snmpv3_internals.h"
#endif


/** Macroes **/
#if 0
#define	SOP_GET		1		/* get operation 		*/
#define SOP_GETN	2		/* getnext operation		*/
#define SOP_GETF	3		/* get first operation		*/
#define SOP_SET		4		/* set operation		*/
#define SVERSION	0		/* current SNMP version		*/
#endif


/** Structure definitions **/
#ifndef SNMPV3
/* Structure that holds a complete description of an SNMP request 	*/
typedef struct snmpReq 
{
	UINT8	            versionNum ;
	UINT8	            reqType;			/* request type				*/
	INT32	            reqId;				/* request identifier		*/
	INT32	            errorStatus;		/* error status				*/
	INT32	            errorIndex;			/* error index				*/
	UINT8	            *errorStatPtr;		/* position of error status */
	UINT8               *errorIndexPtr;		/* position of error index	*/
	UINT8               *pdutypePosPtr;		/* position of pdu type		*/
	UINT8	            *varBindStartPointer ; /* Start of the varBinds */
	UINT8				*endPktPtr ;
	UINT8 	            *recvdPkt ;
	INT32	            recvdPktLen ;
    UINT8               *replyPkt ;
    UINT32              replyPktLen ;
	INT16				snmpAgentSockFd;     /** socket Id **/
	UINT8				varBindCount ;
	SNMPVarBindList     *pVarBinds ;
}SnmpReqMsg;
#endif


/** Extern variables ***/
extern 		INT8 		g_snmpCommunityName[] ;
extern 		INT8 		g_snmpTrapTargetIP[];
extern 		UINT8 		g_SNMPDPri ;
extern 		UINT8 		snmpVersionNum ;
extern 		INT16 		g_snmpTrapTargetPort;
extern 		UINT16 		mib_entries ;
extern 		UINT16 		g_snmpMaxObjectSize;
extern UINT16	g_snmpMaxStrSize ;

/** Extern function prototypes **/

/**************************************************************************
 *
 *	Function Name	: snmpSendTrap()
 *					  
 *	Parameters		: None
 *					  
 *	Return Values	: None.
 *					  
 *	Description		: This function sends a Cold Start Trap and Link Up Trap
 *					  if those options are enabled
 *
 ***************************************************************************/
 
extern INT16
snmpSendTrap
(
	UINT8			*pMgrIPAddress,
	UINT8 			*pData,
	UINT16			Length
) ;

/**************************************************************************
 *
 *	Function Name	: SnmpV1InitUDPSocket()
 *					  
 *	Parameters		: None
 *					  
 *	Return Values	: snmpSockFd -- socket Id if socket and bind calls are
 *					  successful
 *                    ZTPSNMP_ERR_SOCK_CREATE -- If socket creation fails
 *					  ZTPSNMP_ERR_SOCK_BIND -- If bind() fails
 *					  
 *	Description		: This function creates a socket and binds the socket
 *					  to the port 161.
 *
 ***************************************************************************/
extern
INT16 SnmpV1InitUDPSocket() ;

	
/**************************************************************************
 *
 *	Function Name	: SnmpReadObjVal()
 *					  
 *	Parameters		: SNMPObjValue *val  -- Value of the object Id 
 *					  UINT16	    type -- Type of the Obj Id
 *					  UINT16	    vallen -- Length of the object value 
 *                    UINT8         *reqMsg  -- Actual message
 *					  
 *	Return Values	: pointer to recvd packet 
 *					  
 *	Description		: This function reads the value of the object 
 *                    identifier and returns the pointer to the next
 *                    header/variable binding.
 *
 ***************************************************************************/
extern 
UINT8* SnmpReadObjVal
(
   SNMPObjValue     *val,
   UINT8		    *type,
   UINT8            *reqMsg
);

/**************************************************************************
 *
 *	Function Name	: SnmpParseObjId()
 *					  
 *	Parameters		: INT8 *reqMsg -- recvd packet
 *					  INT16 *objIdLen -- Lenghth of the Obj Id
 *					  INT16 *objId -- Object Id 
 *					  
 *	Return Values	: pointer to recvd packet 
 *					  
 *	Description		: This function parses the Object Id and 
 *					  returns the pointer to the next header.
 *
 ***************************************************************************/

extern
UINT8*   SnmpParseObjId
(
   UINT8	      *reqMsg,
   UINT16		  *objIdLen,
   UINT16	      *objId
) ;  

/**************************************************************************
 *
 *	Function Name	: SnmpASN1ParseLength()
 *					  
 *	Parameters		: INT8 *pack -- recvd packet
 *					  INT16 *hdrLength -- Lenghth of the hdr to be returned
 *					  INT16 *hdrValue -- Header value
 *					  
 *	Return Values	: pointer to recvd packet 
 *					  
 *	Description		: This function parses the length of the header and 
 *					  returns the pointer to the next header.
 *
 ***************************************************************************/
extern
UINT8*	SnmpASN1ParseLength(	UINT8	 *pack,
								INT16	 *hdrLength,
								INT16	 *hdrValue
						   );
/**************************************************************************
 *
 *	Function Name	: SnmpASN1ParseSequenceHeader()
 *					  
 *	Parameters		: INT8 *pack -- recvd packet
 *					  INT16 *hdrLength -- Lenghth of the hdr to be returned
 *					  INT16 *hdrValue -- Header value
 *					  
 *	Return Values	: pointer to recvd packet 
 *					  
 *	Description		: This function parses the sequence header type and 
 *					  returns the pointer to the next header.
 *
 ***************************************************************************/
extern 
UINT8* SnmpASN1ParseSequenceHeader(	UINT8	 *pack,
									INT16	 *hdrLength,
									INT16	 *hdrValue
						   		 );

/**************************************************************************
 *
 *	Function Name	: SnmpASN1ParseString()
 *					  
 *	Parameters		: UINT8 *data -- recvd packet
 *					  UINT8 *string -- recvd String 
 *					  INT32 *strlength -- Length of the string
 *					  
 *	Return Values	: pointer to recvd packet 
 *					  
 *	Description		: This function parses the string header type and 
 *					  returns the pointer to the next header.
 *
 ***************************************************************************/
 
extern UINT8 *
SnmpASN1ParseString(	UINT8 *data,
					 	UINT8 *string,
					 	INT32 *strlength
				   );

/**************************************************************************
 *
 *	Function Name	: SNMPASNBuildLen()
 *					  
 *	Parameters		: UINT8 *data -- Data
            	 	  INT32 length -- Length to be caluculated
 *					  
 *	Return Values	: None.
 *					  
 *	Description		: This function build the length of the header
 *
 ***************************************************************************/
extern UINT8 *
SNMPASNBuildLen( UINT8 *data,
            	 UINT16 length
               );

/**************************************************************************
 *
 *	Function Name	: SNMPASNBuildHeader()
 *					  
 *	Parameters		: UINT8 *data -- Data
            	 	  INT32 length -- Length to be caluculated
 *					  
 *	Return Values	: None.
 *					  
 *	Description		: This function builds the header and the length
 *
 ***************************************************************************/
 
extern UINT8 *
SNMPASNBuildHeader (    UINT8   *data,
		                UINT8   type,
		                INT32   length
		           ) ;

/**************************************************************************
 *
 *	Function Name	: SNMPASNBuildObjId()
 *					  
 *	Parameters		: UINT8 *data -- Data
 *					  UINT16 *objid -- OID
 *					  UINT16 objidlength -- OID Length
 *					  
 *	Return Values	: None.
 *					  
 *	Description		: This function builds the Object Id
 *
 ***************************************************************************/

extern UINT8 *
SNMPASNBuildObjId(	UINT8 *data,
					UINT16 *objid,
					UINT16 objidlength
				);
/**************************************************************************
 *
 *	Function Name	: SNMPASNBuildInt()
 *					  
 *	Parameters		: UINT8 *data -- Data
 *           	 	  INT32 length -- Length to be caluculated
 *  				  type  -- data type          	 	  
 *					  intp  -- data value
 *
 *	Return Values	: None.
 *					  
 *	Description		: This function builds an interger data type header
 *
 ***************************************************************************/
 
extern UINT8 *
SNMPASNBuildInt(  UINT8     *data,
        	      UINT8     type,
        	      INT32     *intp
        	   );

/**************************************************************************
 *
 *	Function Name	: SNMPASNBuildString()
 *					  
 *	Parameters		: UINT8 *data -- Data
 *           	 	  INT32 length -- Length to be caluculated
 *  				  type  -- data type          	 	  
 *					  string  -- data value
 *					  strlength -- length of the string
 *
 *	Return Values	: None.
 *					  
 *	Description		: This function builds an String data type header
 *
 ***************************************************************************/
extern UINT8 *
SNMPASNBuildString(	 UINT8 *data,
					 UINT8	type,
					 UINT8 *string,
					 UINT32 strlength
				);

/**************************************************************************
 *
 *	Function Name	: SNMPASNBuildSequence()
 *					  
 *	Parameters		: UINT8 *data -- Data
 *					  UINT8	type  -- type of the header
 *            	 	  INT32 length -- Length to be caluculated
 *					  
 *	Return Values	: None.
 *					  
 *	Description		: This function builds sequence header
 *
 ***************************************************************************/
 
extern UINT8 *
SNMPASNBuildSequence( UINT8 *data,
            		  UINT8 type,
            		  UINT32 length
            	    );

/**************************************************************************
 *
 *	Function Name	: SNMPASNParseLength()
 *					  
 *	Parameters		: UINT8 *data -- Data
            	 	  INT32 length -- Length to be caluculated
 *					  
 *	Return Values	: None.
 *					  
 *	Description		: This function parses the length of the header
 *
 ***************************************************************************/
 
extern UINT8 *
SNMPASNParseLength( UINT8  *data,
		            UINT32  *length
		          );

/**************************************************************************
 *
 *	Function Name	: SNMPASNParseInt()
 *					  
 *	Parameters		: UINT8 *data,
 *	      			  INT32 *datalength,
 *					  
 *	Return Values	: None.
 *					  
 *	Description		: This function parses an integer data type header
 *
 ***************************************************************************/
extern UINT8* 
SNMPASNParseInt(  UINT8 *data,
			      INT32 *intp
			   );

/**************************************************************************
 *
 *	Function Name	: snmpSendTrap()
 *					  
 *	Parameters		: UINT8 	*pData - Data to be sent 
 *					  UINT16	Length - Length of the data
 *					  
 *	Return Values	: None
 *					  
 *	Description		: This function opens a socket on an UDP port 162
 *					  and sends a TRAP message 
 *
 ***************************************************************************/
 
extern INT16
snmpGenerateTrap
( 
	INT8					*userName,
	UINT8					*pMgrAddress,
	UINT8              			Type, 
	UINT32        				Code,
	UINT16       				NumObjects, 
	SNMPObj					*pObjList
);

extern void *malloc( COUNT_t size );

/**************************************************************************
 *
 *	Function Name	: snmpFormatAndSendV1Reply()
 *					  
 *	Parameters		: snmpAgentSockFd -- SocketFd 
 *					  
 *	Return Values	: None
 *					  
 *	Description		: This function formats the reply with the requried 
 *                    ASN1 coding
 *
 ***************************************************************************/
extern INT16
snmpFormatAndSendV1Reply
(
	SnmpReqMsg          *rqdp,
	struct sockaddr_in  *remote
);	

/**************************************************************************
 *
 *	Function Name	: SnmpBuildVarBind()
 *					  
 *	Parameters		: UINT8   *replyPktPtr   -- reply Pointer
 *           		  UINT8   *varBindId     -- Variable Object Id
 *          		  INT16   *varBindIdLen, -- Object Id Length
 *           		  UINT8   varBindType    -- Variable Bind Type
 *          		  INT16   varBindValLen  -- Value Length
 *           		  UINT8   *varBindVal    -- Var Bind Value
 *					  
 *	Return Values	: None
 *					  
 *	Description		: This function formats differnet Object Ids
 *
 ***************************************************************************/
extern UINT8 *
SnmpBuildVarBind(   UINT8   		*replyPktPtr,
            		UINT16   		*varBindId,
            		INT16   		varBindIdLen,
            		UINT8   		varBindType,
            		SNMPObjValue	*varBindVal
  //        		UINT32  		dataLength
            	 );

/**************************************************************************
 *
 *	Function Name	: soipequ()
 *					  
 *	Parameters		: UINT16 *	oidp -- Object Id
 *					  UINT32   pip   -- IP Address
 * 					  UINT16     len
 *					  
 *	Return Values	: TRUE/FALSE.
 *					  
 *	Description		: This function tests equality between an object id 
 *					  and an IP address
 *
 ***************************************************************************/
extern INT16 
soipequ
(
   UINT16 	*oidp,
   UINT32   pip,
   UINT16   len
);

/**************************************************************************
 *
 *	Function Name	: snmpOIDcmp()
 *					  
 *	Parameters		: UINT16 	*pOID1 -- first OID
 *  				  UINT16 	*pOID2  -- sec OID
                                  UINT16        OIDlen -- Length of the OID's
 *					  
 *	Return Values	:  neg, 0, or pos at to whether OID1 <, =, or > OID2
 *					  
 *
 ***************************************************************************/

extern int snmpOIDCmp( UINT16 *pOID1,
                UINT16 *pOID2,
                UINT16 OIDLen);
/**************************************************************************
 *
 *	Function Name	: so2ipcpy()
 *					  
 *	Parameters		: UINT16 *	oidp -- Object Id
 *					  UINT32   pip   -- IP Address
 *					  
 *	Return Values	: None.
 *					  
 *	Description		: This function copies 4 sub-object ids into an 
 *					  IP address array
 *
 ***************************************************************************/
extern void so2ipcpy(UINT32 *pip,UINT16 *oidp);

/**************************************************************************
 *
 *	Function Name	: snmpProcessV1Message()
 *					  
 *	Parameters		: struct req_desc	*rqdp -- pointer to a request 
 *					  structure
 *					  
 *	Return Values	:  
 *					  
 *	Description		: This function parses all the headers and saves all the
 *					  variable bindings and finds the requested MIB from the
 *					  MIB table
 *
 ***************************************************************************/

extern INT16 snmpProcessV1Message
(
   SnmpReqMsg	*rqdp  
) ;

/**************************************************************************
 *
 *	Function Name	: SnmpFindAndBuildMIBs()
 *					  
 *	Parameters		: SnmpReqMsg *rqdp -- Pointer to request structure 
 *					  
 *	Return Values	: None
 *					  
 *	Description		: This function gets the MIB and returns the value of 
 *					  the MIB
 *
 ***************************************************************************/
 
extern INT16 snmpFindAndBuildMIBs( SnmpReqMsg *rqdp) ;

/**************************************************************************
 *
 *	Function Name	: SnmpV1InitUDPSocket()
 *					  
 *	Parameters		: None
 *					  
 *	Return Values	: snmpSockFd -- socket Id if socket and bind calls are
 *					  successful
 *                    ZTPSNMP_ERR_SOCK_CREATE -- If socket creation fails
 *					  ZTPSNMP_ERR_SOCK_BIND -- If bind() fails
 *					  
 *	Description		: This function creates a socket and binds the socket
 *					  to the port 161.
 *
 ***************************************************************************/
extern 
VOID SnmpV1SendAnErrorPacket( SnmpReqMsg *rqd,
							  struct sockaddr_in remote
							);

/**************************************************************************
 *
 *	Function Name	: SNMPFreeVarbindList()
 *					  
 *	Parameters		: SNMPVarBindList     *pVarBinds -- List of variable 
 *                                                      bindings
 *					  
 *	Return Values	: None
 *					  
 *	Description		: This function frees up the memory that was 
 *                    allocated for all the variable bindings while
 *                    parsing the received request from the Manager
 *
 ***************************************************************************/
extern
VOID SNMPFreeVarbindList( SNMPVarBindList     *pVarBinds );

/**************************************************************************
 *
 *	Function Name	: snmpMIBTableInit()
 *					  
 *	Parameters		: None
 *					  
 *	Return Values	: None
 *					  
 *	Description		: This function links all the MIBs.
 *
 ***************************************************************************/
 
extern
VOID snmpMIBTableInit() ;

/**************************************************************************
 *
 *	Function Name	: snmpV1InitMIBs()
 *					  
 *	Parameters		: None
 *					  
 *	Return Values	: None.
 *					  
 *	Description		: This function initializes few of the MIBs
 *
 ***************************************************************************/
 
extern
void snmpV1InitMIBs() ;
