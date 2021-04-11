/*
 * Copyright 2016, ZiLOG Inc.
 * All Rights Reserved
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of ZiLOG Inc., and might
 * contain proprietary, confidential and trade secret information of
 * ZiLOG, our partners and parties from which this code has been licensed.
 * 
 * The contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of ZiLOG Inc.
 */
#ifndef _SSL_TYPES_H_
#define _SSL_TYPES_H_

#ifdef SSL_OS_RZK
	#include "ztypes.h"
	#include "zdevice.h"
	#include "xc_lib.h"

	typedef	RZK_DEVICE_CB_t 				*DID;
	typedef	RZK_DEVICE_CB_t				KE_DEV;
	#define	ANYFPORT	0	

	#define SSL_DID							INT16
#endif

#ifdef SSL_OS_XINU
	#include "basetypes.h"
	#define SSL_DID							DID
#endif


/*
 * SSL basic data types
 */
#define SSL_BN									EZ80_BN
#define SSL_BYTE 								BYTE
#define SSL_WORD 								WORD
#define SSL_DWORD 							DWORD
#define SSL_BOOL 								BYTE
#define SSL_STATUS 							BYTE
#define SYSCALL                        INT16

#define SSL_SUCCESS							0
#define SSL_FAILURE							-1

#define SSL_QWORD_BYTES						8
typedef SSL_BYTE 								SSL_QWORD[ SSL_QWORD_BYTES ];

typedef unsigned int							SSL_TRIO;
typedef unsigned long                  IPaddr;
typedef void                           KE_TASK;

typedef struct									SSL_DATA_BLOCK_S
{
	SSL_BYTE *									pData;
	SSL_WORD										Length;
} SSL_DATA_BLOCK_S;



/*
 * Map support routines used by SSL and the crypto library into kernel functions
 * or the compiler's "C" RTL.
 */
#ifdef SSL_OS_XINU
	#include "kernel.h"
	#include "network.h"

	#define SSL_GETMEM( Size )				getmem( (Size) )
	#define SSL_FREEMEM( Ptr, Size )		freemem( (Ptr), (Size) )
	#define SSL_PRINT 						kprintf
	#define SSL_BLKCOPY						blkcopy
	#define SSL_BLKCOPYR						blkcopyR
	#define SSL_MEMSET						xc_memset
	#define SSL_GETTIME( x )				((x) = KE_TaskGetTime())
#endif

#ifdef SSL_OS_RZK
	#include <string.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include "EtherMgr.h"
	#include "zsysgen.h"
	#include "zthread.h"
	#include "zmemory.h"

   #ifdef MD5_HTTP
      #define bn_getmem( Size )       malloc( (Size) )
      #define bn_freemem( Ptr, Size ) free( (Ptr) )
   #endif

	#define SSL_GETMEM( Size )				malloc( (Size) )
	#define SSL_FREEMEM( Ptr, Size )		free( (Ptr) )
	#define SSL_PRINT 						printf
	#define SSL_BLKCOPY						memcpy
	#define SSL_BLKCOPYR						memmove
	#define SSL_MEMSET						memset
	#define KE_DisablePreempt				RZKDisablePreemption
	#define KE_BpoolGetBuf					RZKAllocFixedSizeMemory
	#define KE_BpoolFreeBuf					FreeBuf
	#define KE_RestorePreempt				RZKRestorePreemption	
	#define KE_SemAcquire( x )				RZKAcquireSemaphore( (x), INFINITE_SUSPEND )
	#define KE_SemRelease					RZKReleaseSemaphore
	extern UINT32 htonl( UINT32 x );
	#define hl2net(x) htonl( (x) )
	extern INT16 gettime( UINT32 * );
	#define SSL_GETTIME( x ) 				gettime( &(x) )
#endif



#endif

