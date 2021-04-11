/*
 * File       :     snmpv1_struct.h
 *
 * Description:  This file contains the internal strcutres and macroes
 *				 used by SNMP V1
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
 
#ifndef _snmpv1_struct_h_
#define _snmpv1_struct_h_

typedef struct varBindList 
{
	SNMPObjLs			snmpObj;	
 	struct varBindList 	*nextVariable;	/* next node in the bind list*/
}SNMPVarBindList;

#endif
