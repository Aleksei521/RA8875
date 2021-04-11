/********************************************************************************
 * File       : IntFlash_InfoPage_Driver.h
 * Description: This file contains the Flash driver for use in data persistence
 *              where in the data is stored in information page of internal flash
 * Author     : C 
 * Created on : 3-FEB-2005
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
 ******************************************************************************/
#ifndef _INTFLASH_INFOPAGE_DRIVER_H_
#define _INTFLASH_INFOPAGE_DRIVER_H_

#include "ZTypes.h"


#define F9x_PAGES_PER_BLOCK            16

#ifdef _EZ80F91
#define F9x_INFO_PAGE_SIZE_BYTES       512
#define F9x_ROW_SIZE_BYTES             256
#define GETPAGE( x ) ((UINT8)( (0x0FF800&(x)) >> 11 ))   /* x[ 20:11 ] */
#define GETROW( x )  ((UINT8)( (0x000700&(x)) >> 8 ))    /* x[ 10:8 ] */
#define GETCOL( x )  ((UINT8)( (0x0000FF&(x)) ))         /* x[ 7:0 ] */
#endif

#ifdef _EZ80F92
#define F9x_INFO_PAGE_SIZE_BYTES       256
#define F9x_ROW_SIZE_BYTES             128
#define GETPAGE( x ) ((UINT8)( (0x01FC00&(x)) >> 10 ))   /* x[ 16:10 ] */
#define GETROW( x )  ((UINT8)( (0x000380&(x)) >> 7 ))    /* x[ 9:7 ] */
#define GETCOL( x )  ((UINT8)( (0x00007F&(x)) ))         /* x[ 6:0 ] */
#endif

#ifdef _EZ80F93
#define F9x_INFO_PAGE_SIZE_BYTES       256
#define F9x_ROW_SIZE_BYTES             128
#define GETPAGE( x ) ((UINT8)( (0x01FC00&(x)) >> 10 ))   /* x[ 16:10 ] */
#define GETROW( x )  ((UINT8)( (0x000380&(x)) >> 7 ))    /* x[ 9:7 ] */
#define GETCOL( x )  ((UINT8)( (0x00007F&(x)) ))         /* x[ 6:0 ] */
#endif

// Flash register values
//FLASH_PGCTL
#define FLASH_PGCTL_ROW_PGM_ENABLE     ( ( UINT8 ) 0x04 )
#define FLASH_PGCTL_ROW_PGM_DISABLE    ( ( UINT8 ) 0x00 )
#define FLASH_PGCTL_PG_ERASE_ENABLE    ( ( UINT8 ) 0x02 )
#define FLASH_PGCTL_PG_ERASE_COMPLETED ( ( UINT8 ) 0x00 )
#define FLASH_PGCTL_PG_ERASE_DISABLE   ( ( UINT8 ) 0x00 )
#define FLASH_PGCTL_MASS_ERASE_ENABLE  ( ( UINT8 ) 0x01 )
#define FLASH_PGCTL_MASS_ERASE_DISABLE ( ( UINT8 ) 0x00 )

//FLASH_PAGE
#define FLASH_PAGE_INFO_ACCESS         ( ( UINT8 ) 0x80 )
#define FLASH_PAGE_MEM_ACCESS          ( ( UINT8 ) 0x00 )

//FLASH_IRQ
#define FLASH_IRQ_DONE_IEN_ENABLE      ( ( UINT8 ) 0x80 )
#define FLASH_IRQ_DONE_IEN_DISABLE     ( ( UINT8 ) 0x00 )
#define FLASH_IRQ_ERR_IEN_ENABLE       ( ( UINT8 ) 0x40 )
#define FLASH_IRQ_ERR_IEN_DISABLE      ( ( UINT8 ) 0x00 )
#define FLASH_IRQ_DONE_ENABLE          ( ( UINT8 ) 0x20 )
#define FLASH_IRQ_DONE_DISABLE         ( ( UINT8 ) 0x00 )
#define FLASH_IRQ_WR_VIO               ( ( UINT8 ) 0x08 )
#define FLASH_IRQ_RP_TMO               ( ( UINT8 ) 0x04 )
#define FLASH_IRQ_PG_VIO               ( ( UINT8 ) 0x02 )
#define FLASH_IRQ_MASS_VIO             ( ( UINT8 ) 0x01 )

//FLASH_FDIV

// FLASH_CTRL
#define FLASH_CTRL_0_WAIT_STATES       ( ( UINT8 ) 0x00 )
#define FLASH_CTRL_1_WAIT_STATES       ( ( UINT8 ) 0x20 )
#define FLASH_CTRL_2_WAIT_STATES       ( ( UINT8 ) 0x40 )
#define FLASH_CTRL_3_WAIT_STATES       ( ( UINT8 ) 0x60 )
#define FLASH_CTRL_4_WAIT_STATES       ( ( UINT8 ) 0x80 )
#define FLASH_CTRL_5_WAIT_STATES       ( ( UINT8 ) 0xA0 )
#define FLASH_CTRL_6_WAIT_STATES       ( ( UINT8 ) 0xC0 )
#define FLASH_CTRL_7_WAIT_STATES       ( ( UINT8 ) 0xE0 )
#define FLASH_CTRL_ENABLE              ( ( UINT8 ) 0x08 )
#define FLASH_CTRL_DISABLE             ( ( UINT8 ) 0x00 )

#ifndef SUCCESS
#define SUCCESS (0x00)
#endif


/*
 * Info Page driver function prototypes
 */ 
INT IntFlash_IP_Init(  void *paddr, UINT num_bytes   );
INT IntFlash_IP_Read(  void *paddr, void *pbuf, UINT num_bytes );
INT IntFlash_IP_Write( void *paddr, void *pbuf, UINT num_bytes );
INT IntFlash_IP_Erase( void *paddr, UINT num_bytes );
INT IntFlash_IP_Close( void );


/*
 * Internal Flash driver function prototypes
 */ 
INT IntFlash_Init(  void *paddr, UINT num_bytes   );
INT IntFlash_Read(  void *paddr, void *pbuf, UINT num_bytes );
INT IntFlash_Write( void *paddr, void *pbuf, UINT num_bytes );
INT IntFlash_Erase( void *paddr, UINT num_bytes );
INT IntFlash_Close( void );


/*
 * File System Internal Flash driver function prototypes
 */ 
INT FS_IntFlash_Init(  void *paddr, UINT num_bytes ) ;
INT FS_IntFlash_Read(  void *paddr, void *pbuf, UINT num_bytes ) ;
INT FS_IntFlash_Write( void *paddr, void *pbuf, UINT num_bytes ) ;
INT FS_IntFlash_Erase( void *paddr, UINT num_bytes ) ;
INT FS_IntFlash_Close( void ) ;


#endif // _INTFLASH_INFOPAGE_DRIVER_H_

