/********************************************************************************
 * File       : i2c_conf.c
 *
 * Description: Defines the configurable system parameters for I2C device
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


#include "ZTypes.h"
#include "ZSysgen.h"
#include "ZDevice.h"
#include "i2c.h"


#define I2C_TASK_PRIORITYH 		6								
#define I2C_TASK_STACK_SIZEH 	1024
#define I2C_RX_MAX_BUFF_SIZEH   100
#define I2C_TX_MAX_BUFF_SIZEH   100

extern DDF_STATUS_t IOERR( void );
extern DDF_STATUS_t IONULL( void );
extern void i2cisr( void );

I2C_CONFIG_t    i2cConfigParams = { 0xA0, 0x00, 0x58, I2C_MASTER, RZK_TRUE, 0x00, 2, I2C_SUBADDR_BIG_ENDIAN};
/* I2C Device Control Block Initialization Structure */
RZK_DEVICE_CB_t I2cDevice =
{ 
	RZK_FALSE, "I2C",
	(FNPTR_RZKDEV_INIT)IONULL, (FNPTR_RZKDEV_STOP)IOERR, I2COpen, I2CClose,
	I2CRead, I2CWrite, (FNPTR_RZKDEV_SEEK)IOERR,
	(FNPTR_RZKDEV_GETC)IOERR, (FNPTR_RZKDEV_PUTC)IOERR, (FNPTR_RZKDEV_IOCTL)I2CControl,
	(RZK_PTR_t)i2cisr, 0000, (UINT8*)&i2cConfigParams,
	 0,0
};


UINT8 I2C_TASK_PRIORITY = I2C_TASK_PRIORITYH ; 
UINT I2C_TASK_STACK_SIZE = I2C_TASK_STACK_SIZEH ;
UINT8 i2cRxBuff[I2C_RX_MAX_BUFF_SIZEH] ;
UINT8 i2cTxBuff[I2C_TX_MAX_BUFF_SIZEH] ;
UINT I2C_RX_MAX_BUFF_SIZE = I2C_RX_MAX_BUFF_SIZEH ;
UINT I2C_TX_MAX_BUFF_SIZE = I2C_TX_MAX_BUFF_SIZEH ;

