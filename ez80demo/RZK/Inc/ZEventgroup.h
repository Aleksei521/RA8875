/*****************************************************************************************
 * File       : ZEventgroup.h
 *
 * Description: Provides the declaration of RZK Eventgroup structures
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
 *****************************************************************************************/

#ifndef _ZEVENTS
#define _ZEVENTS

#define EVENT_XOR			0x02
#define EVENT_CONSUME	    0x04


/* STRUCTURE FOR EVENTGROUP PARAMETERS */
typedef struct EVENTGROUPPARAMS_t
{
	RZK_EVENT_t eEventsReceived;	 /*Specifies the events received */
} RZK_EVENTGROUPPARAMS_t;	


/* STRUCTURE FOR EVENT GROUP CB MEMORY ALLOCATION */

typedef struct RZK_EG_CB
{
    UINT8 CB[RZK_EG_CB_SIZE] ;
} RZK_EG_CB_t;


/***************************************************************************
*	syntax for Function Prototype:
*	<return type> <function name>
*  (<type><argument name>)
**************************************************************************/

RZK_EVENTHANDLE_t  RZKCreateEventGroup
						( 

							RZK_NAME_t  szName[ MAX_OBJECT_NAME_LEN],										
							RZK_MASK_t mEventMask
						);

RZK_STATUS_t  RZKDeleteEventGroup
						(
							RZK_EVENTHANDLE_t hEventGroup
						);

RZK_STATUS_t RZKPostToEventGroup
						( 
							RZK_EVENTHANDLE_t hEventGroup,
							RZK_EVENT_t eEvent,
							RZK_EVENT_OPERATION_et etOperation
						);

RZK_STATUS_t RZKPendOnEventGroup
					( 
						RZK_EVENTHANDLE_t	hEventGroup,
						RZK_EVENT_t eEvent,
						TICK_t tBlockTime,
						RZK_EVENT_OPERATION_et etOperation
					);

RZK_STATUS_t   RZKGetEventGroupParameters
					( 
						RZK_EVENTHANDLE_t hEventGroup,
						RZK_EVENTGROUPPARAMS_t *pEventGroupParams	
					);


#endif   /* _ZEVENTS */