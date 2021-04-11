/**********************************************************************************
 * File       : AM29LV008B_Driver.h
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
 *********************************************************************************/

#ifndef _AM29LV008_DRIVER_H_
#define _AM29LV008_DRIVER_H_

#include "ZTypes.h"


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

#define AM29LV008_MASK_BITS	(0xFFE000)

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
#define AMD_ID_29LV008BB  	0x37    
#define AMD_ID_29LV008TB  	0x3E

#ifndef SUCCESS
#define SUCCESS (0x00)
#endif

//StandAlone driver function prototypes
INT AM29LV008_Init(  void *paddr, UINT num_bytes ) ;
INT AM29LV008_Read(  void *paddr, void *pbuf, UINT num_bytes ) ;
INT AM29LV008_Write( void *paddr, void *pbuf, UINT num_bytes ) ;
INT AM29LV008_Erase( void *paddr, UINT num_bytes ) ;
INT AM29LV008_Close( void ) ;

//FileSystem integrated function prototypes
INT FS_AM29LV008_Init(  void *paddr, UINT num_bytes ) ;
INT FS_AM29LV008_Read(  void *paddr, void *pbuf, UINT num_bytes ) ;
INT FS_AM29LV008_Write( void *paddr, void *pbuf, UINT num_bytes ) ;
INT FS_AM29LV008_Erase( void *paddr, UINT num_bytes ) ;
INT FS_AM29LV008_Close( void ) ;


#endif // _AM29LV008_DRIVER_H_

