/*****************************************************************************************
 * File       : ZContext.h
 *
 * Description: Declares the structure to hold the register value during
 * context switch.
 *              
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
 *****************************************************************************************/

#ifndef  _ZCONTEXT
#define  _ZCONTEXT

/* STRUCTURE TO HOLD THE REGISTER VALUES */
typedef struct CPU_REG
{
        volatile UINT32	 regAFz;
        volatile UINT32  regBCz;
        volatile UINT32  regDEz;
        volatile UINT32  regHLz;
        volatile UINT32  regIXz;
        volatile UINT32  regIYz;
        volatile UINT32  regPCz;
        volatile UINT32  regSPz;
}RZK_CPU_REG_t;

#endif  /* _ZCONTEXT */