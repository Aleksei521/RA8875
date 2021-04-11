/*
 * File       : i2c.h
 *
 * Description: Contains macros and structure declarations for I2C driver
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
 */ 

#include <ez80.h>


#include "ZTypes.h"
#include "ZDevice.h"

/** I2C masks */
#define I2CMASK_SRESET_CONFIRM		0x38
#define I2CMASK_SET_STA				0x20
#define I2CMASK_SET_STP				0x10
#define I2CMASK_SET_AAK				0x04
#define I2CMASK_CLR_AAK				0xFB
#define I2CMASK_GET_IFLG			0x08
#define I2CMASK_CLR_IFLG			0xF7
#define I2CMASK_CLR_IFLGAAK			0xF3
#define I2CMASK_SET_INT				0x80
#define I2CMASK_CLR_INT				0x7F
#define I2CMASK_GCE_ENABLE			0x01

/**
 * macros
 */

#define COUNT_POLL_IFLG				5000
#define COUNT_POLL_SR				0x00FFFF
#define COUNT_DELAY					0xFFF

#define I2C_SRESET					0x12	/** arbitrary value */

/** I2C1 specific */
#define	I2C_POLL_IFLG( x, y )		while( (!(I2C_CTL & I2CMASK_GET_IFLG)) && (x < y) ) x++
#define	I2C_SetAAK()				I2C_CTL = I2C_CTL | I2CMASK_SET_AAK
#define	I2C_ResetAAK()				I2C_CTL = I2C_CTL & I2CMASK_CLR_AAK
#define I2C_ClearIFlag()			I2C_CTL = I2C_CTL & I2CMASK_CLR_IFLG
#define I2C_ResetAAKClearIFlag()	I2C_CTL = I2C_CTL & I2CMASK_CLR_IFLGAAK
#define I2C_SetAAKClearIFlag( x )	x = I2C_CTL ; \
									x = x | I2CMASK_SET_AAK ; \
									x = x & I2CMASK_CLR_IFLG ; \
									I2C_CTL = x
#define	I2C_Delay(x)				while(x--)

#ifdef _EZ80F91
#define I2C_VECTOR_OFFSET	0x78
#else
#define I2C_VECTOR_OFFSET	0x1C
#endif 


/**
 * I2C status codes --for use inside the i2c layer only
 */
enum i2c_status
{
	I2CSTAT_BUSERROR						= 0x00,
	I2CSTAT_STARTTXD						= 0x08,
	I2CSTAT_RPTDSTARTTXD					= 0x10,
	I2CSTAT_SLAWTXD_ACKRXD					= 0x18,
	I2CSTAT_SLAWTXD_ACKNRXD					= 0x20,
	I2CSTAT_DATABYTETXDMMODE_ACKRXD			= 0x28,
	I2CSTAT_DATABYTETXDMMODE_ACKNRXD		= 0x30,
	I2CSTAT_ARBLOST							= 0x38,
	I2CSTAT_SLARTXD_ACKRXD					= 0x40,
	I2CSTAT_SLARTXD_ACKNRXD					= 0x48,
	I2CSTAT_DATABYTERXDMMODE_ACKTXD			= 0x50,
	I2CSTAT_DATABYTERXDMMODE_NACKTXD		= 0x58,

	I2CSTAT_SLAWRXD_ACKTXD					= 0x60,
	I2CSTAT_SLDATARXD_ACKTXD				= 0x80,
	I2CSTAT_SLDATARXD_NACKTXD				= 0x88,
	I2CSTAT_SLSTOPREPSTART_RXD				= 0xA0,
	I2CSTAT_SLARRXD_ACKTXD					= 0xA8,
	I2CSTAT_SLDATATXD_ACKRXD				= 0xB8,
	I2CSTAT_SLDATATXD_ACKNRXD				= 0xC0,
	I2CSTAT_SLDATALASTTXD_ACKRXD			= 0xC8,


	I2CSTAT_ARBLOST_SLAWRXD_ACKTXD			= 0x68,
	I2CSTAT_ARBLOST_GLCLADRXD_ACKTXD		= 0x78,
	I2CSTAT_ARBLOST_SLARRXD_ACKTXD			= 0xb0,
	I2CSTAT_NORELEVANT_STATUS				= 0xf8
} ;


/**
 * I2C error codes --for use outside the i2c layer
 */
enum i2c_errors
{
	I2CERR_SUCCESS,
	I2CERR_TIMEOUT = -50,
	I2CERR_STARTFAIL,
	I2CERR_BUSY,	
	I2CERR_DEV_CLOSED,
	I2CERR_KERNELERROR,
	I2CERR_INVALID_OPERATION,
	I2CERR_BUSBUSY,
	I2CERR_CLOSEFAIL,
	/** status */
	I2CERR_BUSERROR,
	I2CERR_SLAWTXD_ACKNRXD,
	I2CERR_SLARTXD_ACKNRXD,
	I2CERR_SLAWRXD_ACKTXD,		
	I2CERR_SLARRXD_ACKTXD,	
	I2CERR_SLAWTXD_ACKRXD,		
	I2CERR_SLARTXD_ACKRXD,	
	I2CERR_DATABYTERXDMMODE_ACKTXD,
	I2CERR_DATABYTERXDMMODE_NACKTXD,
	I2CERR_DATABYTETXDMMODE_ACKNRXD,
	I2CERR_ARBLOST,
	I2CERR_ARBLOST_SLAWRXD_ACKTXD,
	I2CERR_ARBLOST_SLARRXD_ACKTXD,
	I2CERR_ARBLOST_GLCLADRXD_ACKTXD,
	I2CERR_DATA_TXD_NACKRXD,
	I2CERR_DATA_RXD_NACKRXD,
	I2CERR_LASTDATA_RXD_ACKRXD,
	I2CERR_FAILURE,
	I2CERR_IDLE
	
} ;



/** end of file */




typedef unsigned int I2C_STATUS;
typedef UINT8 I2C_BOOL;
typedef unsigned int I2C_SLAVE_ID;


/* I2C Control Operations */
#define I2C_SET_SLAVE_ADDR				0x01
#define I2C_SUBADDR_USED				0x02
#define I2C_SUBADDR_NOT_USED			0x03
#define I2C_SUBADDR_LEN 				0x04
#define I2C_SET_SPEED					0x05

/* I2C Sub address length*/
#define I2C_SUBADDR_SINGLE_BYTE			0x01
#define I2C_SUBADDR_TWO_BYTE			0x02
#define I2C_SUBADDR_THREE_BYTE			0x03


#define I2C_MASTER					0x01
#define I2C_SLAVE					0x00
#define I2C_MULTI_MASTER 			0x02	

#define I2C_ADDR_MODE_7BIT					0x01
#define I2C_ADDR_MODE_10BIT					0x02

#define I2CMODE_TRANSMIT    0xFE
#define I2CMODE_RECEIVE     0x01

#define I2C_BUS_BUSY        0x08

/*CR#75733 fix*/
#define I2C_SUBADDR_LITTLE_ENDIAN        0x01
#define I2C_SUBADDR_BIG_ENDIAN        0x02

typedef struct I2C_CONFIG
{
    UINT8   currSlaveAddr ;
    UINT8   selfAddr ;
    UINT8   speed ;
    UINT8   mode ;
    UINT8   useSubAddr ;
    UINT	subAddr ;
	UINT8	addrLen ;
	UINT8   subAddrFormat; /*  CR#75733 fix*/
}I2C_CONFIG_t ;

typedef struct CQ{
    UINT8 *pBuff;
    INT     front ;
    INT     rear ;
    INT     bufLen ;
    INT     avail ;
}CQ_t ;

/** Function prototypes */
void I2CDev_Init( void ) ;
INT I2CDev_SendStart( void ) ;
void I2CDev_SendStop( void ) ;
INT I2CDev_Close( void ) ;
INT I2C1_Stop( void ) ;
INT I2CDev_TransmitDataByte( INT8 data ) ;
INT I2CDev_ReceiveDataByte( INT8 *data ) ;
DDF_STATUS_t I2CPeek(void) ;

DDF_STATUS_t AddI2c( void ) ;
DDF_STATUS_t I2CInit( RZK_DEVICE_CB_t *pDCB ) ;
DDF_STATUS_t I2COpen( RZK_DEVICE_CB_t *pDCB, INT8 *SlaveName, INT8 * devMode ) ;
DDF_STATUS_t I2CControl( RZK_DEVICE_CB_t *pdev, RZK_DEV_BYTES_t uOperation, INT8 *addr1, INT8 *addr2 ) ;
DDF_STATUS_t I2CClose( RZK_DEVICE_CB_t * pDCB) ;
DDF_STATUS_t I2CWrite( RZK_DEVICE_CB_t * pDCB, INT8 * buf, RZK_DEV_BYTES_t size ) ;
DDF_STATUS_t I2CRead(RZK_DEVICE_CB_t *	pDCB, INT8 * buf, RZK_DEV_BYTES_t size) ;
DDF_STATUS_t I2C_Status_Check(DDF_STATUS_t stat, RZK_DEVICE_CB_t *	pDCB);

