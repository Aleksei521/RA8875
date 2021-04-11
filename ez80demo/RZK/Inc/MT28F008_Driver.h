/*********************************************************************************
 * File       : MT28F008_Driver.h
 * Description: This file contains the function prototypes for flash driver
 *				routines
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
 ********************************************************************************/

#ifndef _MT28F008_DRIVER_H_
#define _MT28F008_DRIVER_H_

#include "ZTypes.h"

#ifndef SUCCESS
#define SUCCESS (0x00)
#endif

#define	MT28F008_IDENTIFY_DEVICE	(UINT8)0x90
#define	MT28F008_READ_ARRAY			(UINT8)0xff
#define	MT28F008_CLEAR_STATUS_REG	(UINT8)0x50
#define	MT28F008_READ_STATUS_REG	(UINT8)0x70
#define	MT28F008_ERASE_SETUP		(UINT8)0x20
#define	MT28F008_ERASE_CONFIRM		(UINT8)0xd0
#define	MT28F008_WRITE_SETUP		(UINT8)0x40
#define	MT28F008_BUSY				(UINT8)0x00	
#define	MT28F008_READY				(UINT8)0x80
#define	MT28F008_RESET 				(UINT8)0xff
#define	MT28F008_STATUS_BITS		(UINT8)0xf8


#define	MT28F008_ERASE_SUSPENDED	(UINT8)0x40
#define	MT28F008_ERASE_ERROR		(UINT8)0x20
#define	MT28F008_WRITE_ERROR		(UINT8)0x10
#define	MT28F008_NO_VOLTAGE			(UINT8)0x08

#define	ONE_SECOND	10

//StandAlone driver function prototypes
INT MT28F008_Init(  void *paddr, UINT num_bytes ) ;
INT MT28F008_Read(  void *paddr, void *pbuf, UINT num_bytes ) ;
INT MT28F008_Write( void *paddr, void *pbuf, UINT num_bytes ) ;
INT MT28F008_Erase( void *paddr, UINT num_bytes ) ;
INT MT28F008_Close( void ) ;

//FileSystem integrated function prototypes
INT FS_MT28F008_Init(  void *paddr, UINT num_bytes ) ;
INT FS_MT28F008_Read(  void *paddr, void *pbuf, UINT num_bytes ) ;
INT FS_MT28F008_Write( void *paddr, void *pbuf, UINT num_bytes ) ;
INT FS_MT28F008_Erase( void *paddr, UINT num_bytes ) ;
INT FS_MT28F008_Close( void ) ;


#endif // _MT28F008_DRIVER_H_
