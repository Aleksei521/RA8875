/*******************************************************************************
 * File       : ZRegion.h
 *
 * Description: Provides the declaration of RZK Regions structures
 * and APIs.
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
 *******************************************************************************/

#ifndef _ZREGION
#define _ZREGION

/* STRUCTURE FOR REGION PARAMETERS */
typedef struct REGIONPARAMS_STRUCT_t
{
	RZK_STATE_t uState;		/*Specifies the state of the REGION */	
	COUNT_t uUnit_Size;  	/*Specifies the initial count of the REGION */	
	UINT8 uRnDelete;		/*Specifies the number of threads that are waiting on the REGION*/ 
} RZK_REGIONPARAMS_t;


/* STRUCTURE FOR REGIONS CB MEMORY ALLOCATION */

typedef struct RZK_REGION_CB
{
    UINT8 CB[RZK_REGION_CB_SIZE] ;

} RZK_REGION_CB_t;

typedef struct RZK_SEGMENT_CB
{
    UINT8 CB[RZK_REGIONTAB_CB_SIZE] ;

} RZK_SEGMENT_CB_t;


/********************************************
*	syntax for Function Prototype:
*	<return type> <function name>
*  (<type><argument name>)
*****************************************/

RZK_REGIONHANDLE_t RZKCreateRegion
						( 
							RZK_NAME_t  szName[MAX_OBJECT_NAME_LEN],	
							UINT8 * RnAddr,
							RZK_LENGTH_t uLength,
							COUNT_t uUnit_Size,
							UINT8	uRnDelete,
							RZK_RECV_ATTRIB_et etAttrib		
						);

RZK_STATUS_t RZKDeleteRegion
				 ( 
					 RZK_REGIONHANDLE_t hRegion		
				 );


RZK_PTR_t RZKAllocSegment
				 ( 
					 RZK_REGIONHANDLE_t	hRegion,  
					 COUNT_t uSize,
					 TICK_t tBlockTime
				 );

RZK_STATUS_t RZKFreeSegment
				 ( 
					 RZK_REGIONHANDLE_t	hRegion,
					 RZK_PTR_t pSegment  
				 );

RZK_STATUS_t RZKGetRegionParameters
				 ( 
					 RZK_REGIONHANDLE_t	hRegion,
					 RZK_REGIONPARAMS_t *pRegionParams 
				 );

UINT8 init_malloc( void );

void *malloc( COUNT_t size );

void free( void *seg );

RZK_LENGTH_t RZKQueryMem(RZK_REGIONHANDLE_t hRegion);

#endif  /* _ZREGION  */