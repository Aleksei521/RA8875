/*******************************************************************************
 * File       : spi_conf.c
 *
 * Description: Defines the configurable system parameters for SPI device
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


#include "spi.h"

#define SPI_TASK_PRIOH                  (10)
#define SPI_THD_STACK_SIZEH             (1024)

extern DDF_STATUS_t IONULL( void );
extern RZK_STATUS_t DummyDevInit( RZK_DEVICE_CB_t *) ;
extern DEV_SPI_CB_t spiDevStruct ; 

/* SPI Device Control Block Initialization Structure */
RZK_DEVICE_CB_t SpiDev = 
	{
		0, "SPI", 
		(FNPTR_RZKDEV_INIT)IONULL, (FNPTR_RZKDEV_STOP)IONULL,
		(FNPTR_RZKDEV_OPEN)SPI_Open, (FNPTR_RZKDEV_CLOSE)SPI_Close, 
		(FNPTR_RZKDEV_READ)SPI_Read,(FNPTR_RZKDEV_WRITE)SPI_Write,
		(FNPTR_RZKDEV_SEEK)IONULL, (FNPTR_RZKDEV_GETC)IONULL, 
		(FNPTR_RZKDEV_PUTC)IONULL, (FNPTR_RZKDEV_IOCTL)SPI_Ioctl, 
		(RZK_PTR_t)spiisr,0000, (UINT8 *)&spiDevStruct, 0, 0
	} ;


// **************************
// DO NOT CHANGE ANYTHING BELOW IN THIS SECTION
// **************************
UINT8     SPI_TASK_PRIO                           = SPI_TASK_PRIOH ;
UINT      SPI_THD_STACK_SIZE                      = SPI_THD_STACK_SIZEH ;
UINT8     SPI_Task_Stack[SPI_THD_STACK_SIZEH] ;

