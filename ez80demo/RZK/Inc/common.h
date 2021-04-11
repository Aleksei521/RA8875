/*************************************************************************************
 * File       : common.h
 *
 * Description: Defines Queue structures used by EMAC 
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
 *************************************************************************************/



#ifndef _COMMON
#define _COMMON

#include "ZTypes.h"
#include <defines.h>

/* Tx Queue management */
 
typedef struct QUEUE_NODE {
	struct QUEUE_NODE *next ;		/* Pointer to the next node */
	struct QUEUE_NODE *previous ;   /* Pointer to the previous node */
} QUEUE_NODE_t ;

typedef struct QUEUE_REF {
	struct QUEUE_NODE *head ;	    /* Pointer to the head of the queue */
	struct QUEUE_NODE *tail ;		/* Pointer to the tail of the queue */
} QUEUE_REF_t ;


void * AllocPktBuff(void) ;
void FreePktBuff(void *) ;
UINT8 * AllocFrgMem(void);  
void FreeFrgMem(UINT8 *); 

#endif
