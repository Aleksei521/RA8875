/***********************************************************************************
 * File       : Ram_Driver.h
 * Description: This file contains the RAM driver for use in ZiLOG File System    : 
 * Created on : 30-APR-2003
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
 ***********************************************************************************/

#ifndef _RAM_DRIVER_H_
#define _RAM_DRIVER_H_

#include "ZTypes.h"

#ifndef SUCCESS
#define SUCCESS (0x00)
#endif

INT RamDrv_Init(  void *paddr, UINT num_bytes  ) ;
INT RamDrv_Read(  void *paddr, void *pbuf, UINT num_bytes ) ;
INT RamDrv_Write( void *paddr, void *pbuf, UINT num_bytes ) ;
INT RamDrv_Erase( void *paddr, UINT num_bytes ) ;
INT RamDrv_Close( void ) ;

#endif // _RAM_DRIVER_H_