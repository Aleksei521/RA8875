/**************************************************************************
 * File       : ZMemory.h
 *
 * Description: Provides declaration of RZK Memory Partitions structures
 * and APIs.
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
 ***************************************************************************/

#ifndef _ZMEMORY
#define _ZMEMORY                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
	

/* STRUCTURE FOR MEMORY MANAGEMENT PARAMETERS */
typedef struct PARTITIONPARAMS_STRUCT_t
	{
		RZK_STATE_t uState;         /*State of partition*/
		COUNT_t uNumOfBlocks;            
		COUNT_t uBlockSize;		    /* Size of each memory block*/
		COUNT_t uBlocksUsed;		/* number of memory blocks used*/
		RZK_PTR_t pMemory;          /* Pointer to memory area which specifies for each block*/ 
	} RZK_PARTITIONPARAMS_t;


/* STRUCTURE FOR PARTITION CB MEMORY ALLOCATION */

typedef struct RZK_PARTITION_CB
{
    UINT8 CB[RZK_PARTITION_CB_SIZE] ;
} RZK_PARTITION_CB_t;

/********************************************
*	syntax for Function Prototype:
*	<return type> <function name>
*  (<type><argument name>)
*****************************************/

RZK_PARTITIONHANDLE_t RZKCreatePartition
							( 
								RZK_NAME_t  szName[8],			
								RZK_PTR_t pMemory,
								UINT uMemoryBlocks,
								UINT uBlockSize
							);

RZK_STATUS_t RZKDeletePartition 
					( 
						 RZK_PARTITIONHANDLE_t hPartition
					 );

RZK_PTR_t RZKAllocFixedSizeMemory
				  ( 
					 RZK_PARTITIONHANDLE_t	hPartition
				  );

RZK_STATUS_t RZKFreeFixedSizeMemory
				 ( 
					 RZK_PARTITIONHANDLE_t hPartition,
					 RZK_PTR_t pBlock
				 );

RZK_STATUS_t RZKGetPartitionParameters
				 ( 
					 RZK_PARTITIONHANDLE_t	hPartition,
					 RZK_PARTITIONPARAMS_t	*pPartitionParams
				 );

void FreeBuf(RZK_PTR_t pBuf);

#endif  /* _ZMEMORY */