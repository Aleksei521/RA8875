/****************************************************************************************
 * File       :     config.h
 *
 * Description:  This file contains the Telnet Client side API definitions.
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
 ***************************************************************************************/

#ifndef _ZSPI
#define _ZSPI  

#include "ZTypes.h"
#include "ZSysgen.h"
#include "ZDevice.h"

typedef volatile UINT8 __INTIO *PBINTIO;

#define IO8_WRITE(io_reg, b_val) io_reg=b_val

/* SPI baud rate, control and data registers */
#define spi_brg_l	(*(PBINTIO)(0xB8))
#define spi_brg_h	(*(PBINTIO)(0xB9))

#define spi_ctl		(*(PBINTIO)(0xBA))
#define spi_sr		(*(PBINTIO)(0xBB))
#define spi_tsr		(*(PBINTIO)(0xBC))
#define spi_rbr		(*(PBINTIO)(0xBC))

#define IRQ_EN				 0x80
#define SPI_EN				 0x20
#define MASTER_EN			 0x10
#define CPOL				 0x08
#define CPHA				 0x04

#define SPIF				 0x80
#define WCOL				 0x40
#define MODF				 0x10

#define SPI_BUSY		     0x01
/* Port registers */

#ifdef _EZ80F91

#define padr		(*(PBINTIO)(0x96))
#define paddr		(*(PBINTIO)(0x97))
#define paalt1		(*(PBINTIO)(0x98))
#define paalt2		(*(PBINTIO)(0x99))

#endif

#define pbdr		(*(PBINTIO)(0x9A))
#define pbddr		(*(PBINTIO)(0x9B))
#define pbalt1		(*(PBINTIO)(0x9C))
#define pbalt2		(*(PBINTIO)(0x9D))

#define pcdr		(*(PBINTIO)(0x9E))
#define pcddr		(*(PBINTIO)(0x9F))
#define pcalt1		(*(PBINTIO)(0xA0))
#define pcalt2		(*(PBINTIO)(0xA1))

#define pddr		(*(PBINTIO)(0xA2))
#define pdddr		(*(PBINTIO)(0xA3))
#define pdalt1		(*(PBINTIO)(0xA4))
#define pdalt2		(*(PBINTIO)(0xA5))

#ifdef _EZ80F91
	#define SPI_IRQ_NUM	0x7C
	#define PB2_IRQ_NUM	0xA8
#else
	#define SPI_IRQ_NUM	0x1E
	#define PB2_IRQ_NUM	0x34
#endif
/****************************************************************/
// Target related system clock frequencies

#define SET_BAUD		(RZK_DEV_BYTES_t)0x01
#define LOAD_CONTROL	(RZK_DEV_BYTES_t)0x02
#define SLAVE_SELECT	(RZK_DEV_BYTES_t)0x04
#define SLAVE_DESELECT	(RZK_DEV_BYTES_t)0x08
#define READ_STATUS		(RZK_DEV_BYTES_t)0x10

#define BAUD_9600			9600	            	//!< Baud rate 9600.
#define BAUD_19200			19200	            	//!< Baud rate 19200.
#define BAUD_38400			38400	            	//!< Baud rate 38400.
#define BAUD_57600			57600	            	//!< Baud rate 57600.
#define BAUD_115200			115200	    	        //!< Baud rate 115200.

/* SPI status values */
typedef enum {
	SPIDEV_ERR_SUCCESS = 0,
	SPIDEV_ERR_WCOL = -5,
	SPIDEV_ERR_MULTIMASTER,
	SPIDEV_ERR_INVALID_OPERATION,
	SPIDEV_ERR_KERNEL_ERROR,
	SPIDEV_ERR_BUSY
} SPIDEV_ERR_NUMS ;

typedef struct {
	RZK_THREADHANDLE_t	spiIntThreadHdl;
	RZK_THREADHANDLE_t	hSPIThread;
	UINT8 				cBaudRate;
	RZK_DEV_BYTES_t nBytesTransfered;
	RZK_DEV_BYTES_t nBytes;
	RZK_DEV_BUFFER_t *buf;
	UINT8 cError;
	UINT8 cReadOp;
}DEV_SPI_CB_t;

void spiisr(void);

DDF_STATUS_t AddSpi(void);
DDF_STATUS_t SPI_Open(RZK_DEVICE_CB_t *pDCB,RZK_DEV_NAME_t *devName,RZK_DEV_MODE_t *devMode);
DDF_STATUS_t SPI_Read(RZK_DEVICE_CB_t *pDCB,RZK_DEV_BUFFER_t *buf,RZK_DEV_BYTES_t nBytes);
DDF_STATUS_t SPI_Write(RZK_DEVICE_CB_t *pDCB,RZK_DEV_BUFFER_t *buf,RZK_DEV_BYTES_t nBytes);
DDF_STATUS_t SPI_Ioctl(RZK_DEVICE_CB_t *pDCB,RZK_DEV_BYTES_t uOperation,INT8 *addr1,INT8 *addr2);
DDF_STATUS_t SPI_Close(RZK_DEVICE_CB_t *pDCB);

#endif
