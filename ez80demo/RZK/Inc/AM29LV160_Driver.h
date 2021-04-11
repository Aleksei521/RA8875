/***********************************************************************************
 * File       : AM29LV160_Driver.h
 * Description: This file contains the Flash driver for use in ZiLOG File System
 * Author     : 
 * Created on : 04-OCT-2004
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
 **********************************************************************************/

#ifndef _AM29LV160_DRIVER_H_
#define _AM29LV160_DRIVER_H_

#include "ZTypes.h"

/* AT49BV1614A Codes */
#define FLASH_CODE_AA	0xAA
#define FLASH_CODE_55	0x55
#define ID_IN_CODE		0x90
#define ID_OUT_CODE		0xF0


#define CMD_READ_ARRAY		0xF0
#define CMD_UNLOCK1			0x00AA
#define CMD_UNLOCK2			0x0055
#define CMD_ERASE_SETUP		0x0080
#define CMD_ERASE_CONFIRM	0x0030
#define CMD_PROGRAM			0x00A0
#define CMD_UNLOCK_BYPASS	0x0020

#define AM29LV160_MASK_BITS	(0xFFE000)

#define BIT_ERASE_DONE		(( UINT8 ) 0x80)
#define BIT_RDY_MASK		(( UINT8 ) 0x80)
#define BIT_PROGRAM_ERROR	(( UINT8 ) 0x20)
#define BIT_TIMEOUT			0x80000000 /* our flag */

#define FLASH_IO3			((UINT8 ) 0x08)
#define FLASH_IO5			((UINT8 ) 0x20)

#define READY 1
#define ERR   2
#define TMO   4

#define AMD_ID_MANUFACTURE	0x0001
#define AMD_ID_29LV160BB  	0x49    /* AM29LV160BB */
#define AMD_ID_29LV160TB  	0xC4    /* AM29LV160TB */
#define AMD_ID_S29GL064NTB 0x7E    /* S29GL064NTB */

#ifndef SUCCESS
#define SUCCESS (0x00)
#endif

//StandAlone driver function prototypes
INT AM29LV160_Init(  void *paddr, UINT num_bytes ) ;
INT AM29LV160_Read(  void *paddr, void *pbuf, UINT num_bytes ) ;
INT AM29LV160_Write( void *paddr, void *pbuf, UINT num_bytes ) ;
INT AM29LV160_Erase( void *paddr, UINT num_bytes ) ;
INT AM29LV160_Close( void ) ;

//FileSystem integrated function prototypes
INT FS_AM29LV160_Init(  void *paddr, UINT num_bytes ) ;
INT FS_AM29LV160_Read(  void *paddr, void *pbuf, UINT num_bytes ) ;
INT FS_AM29LV160_Write( void *paddr, void *pbuf, UINT num_bytes ) ;
INT FS_AM29LV160_Erase( void *paddr, UINT num_bytes ) ;
INT FS_AM29LV160_Close( void ) ;


#endif // _AM29LV160_DRIVER_H_

