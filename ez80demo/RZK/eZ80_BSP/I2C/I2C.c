/*
 * File       : i2c.c
 * Scope      : 
 *
 * Description:  This contains implementations for I2C protocol
 *
 *
 * Copyright 2003 ZiLOG Inc.  ALL RIGHTS RESERVED.
 *
 * This file contains unpublished confidential and proprietary information
 * of ZiLOG, Inc.
 * NO PART OF THIS WORK MAY BE DUPLICATED, STORED, PUBLISHED OR DISCLOSED 
 * IN ANY FORM WITHOUT THE PRIOR WRITTEN CONSENT OF ZiLOG, INC.
 * This is not a license and no use of any kind of this work is authorized
 * in the absence of a written license granted by ZiLOG, Inc. in ZiLOG's 
 * sole discretion 
 */


// IAR changed
#ifdef _IAR_CODE

#ifdef _EZ80F91
#include <ioez80f91.h>
#endif

#ifdef _EZ80F92
#include <ioez80f92.h>
#endif

#ifdef _EZ80F93
#include <ioez80f92.h>
#endif

#ifdef _EZ80L92
#include <ioez80l92.h>
#endif

#ifdef _EZ80190
#include <ioez80190.h>
#endif

#else
#include <ez80.h>
#endif
#include <stdio.h>
#include "i2c.h"
#include "ZTypes.h"
#include "ZSysgen.h"
#include "ZInterrupt.h"



extern RZK_THREADHANDLE_t i2cIntThreadHdl ;
extern void i2cisr(void) ;
extern I2C_CONFIG_t i2cConfigParams ;

/*
 * function name       : I2CDev_Init
 * function description: Initializes I2C1
 * Return values	   : VOID
 * Global variables affected/used: none
 */

VOID I2CDev_Init( VOID )
{
	/** select I2C1 from UZI1 */
#if 0
	UZI1_CTL = 0x03 ;
#endif 
	/** soft reset the I2C1 */
	I2C_SRR = I2C_SRESET ;

	/**
	 * set the clock register for configuring the I2C
	 * sampling frequency (Fsamp) and the I2C clock output
	 * frequency (Fscl)
	 * 
	 * The Fsamp should at least be 1MHz (for 100kbps) and
	 *  be 4 MHz (for 400kbps)
	 *
	 * For Fsamp=50 MHz, it would result in Fscl=416 KHz.
	 * and it turns out to: N=0 and M=11.
	 *
	 * CCR Register: field --0(1):M(11):N(0)
	 *               value --0:1011:000 = 0x58
	 */
	I2C_CCR = i2cConfigParams.speed ;//0x58 ;//0x05 ;

	/** set aak and enab bit */
	I2C_CTL =0x44 ;

    if((i2cConfigParams.mode == I2C_SLAVE)||(i2cConfigParams.mode == I2C_MULTI_MASTER))
    {
		RZKInstallInterruptHandler((RZK_FNP_ISR)i2cisr, I2C_VECTOR_OFFSET ) ;
		I2C_SAR = i2cConfigParams.selfAddr ;
    	I2C_CTL =	0xC4 ; 	    
    }
	

	return ;

}/** end of I2C1_Init() */



/*
 * function name       : I2CDev_SendStart
 * function description: Send START condition and return the status to the caller
 * Return values	   : status /success or error
 * Global variables affected/used: none
 */


INT I2CDev_SendStart( VOID )
{
	UINT32 count = 0 ;
	INT status ;
	UINT8 temp ;

	/**
	 * Enter master transmit mode: send a start condition to begin
	 * the transmission
	 */
	
	temp = I2C_CTL ;
	temp = temp | I2CMASK_SET_STA ;
	temp = temp & I2CMASK_CLR_IFLG ;
	I2C_CTL = temp ;

	
	/** poll on iflag */
	I2C_POLL_IFLG( count, COUNT_POLL_IFLG ) ;

	I2C_CTL  &= ~I2CMASK_SET_INT ;		/* Disabling the I2C interrupts 
											This is done*/
	
	/** iflag set */
	if( I2C_CTL & I2CMASK_GET_IFLG )
	{
		/** START or repeated START transmitted */
		if( (I2C_SR == I2CSTAT_STARTTXD) || (I2C_SR == I2CSTAT_RPTDSTARTTXD) )
			status = I2CERR_SUCCESS ;
		/** START fail */
		else
			{
				if(I2C_SR == 0x00)
				{
					I2C_SRR = 0x01;          /*  Handling the Bus error */
					return I2CERR_BUSERROR;
				}
				status = I2CERR_STARTFAIL ;
			}
	

	}/** end 'iflag set' */

	/** timeout */
	else
	{
		status = I2CERR_TIMEOUT ;
		I2CDev_SendStop();
	}/** end 'timeout' */
	return status ;
}/** end of I2C1_SendStart() */



/*
 * function name       : I2CDev_SendStart
 * function description: send stop condition
 * Return values	   : status /success or error
 * Global variables affected/used: none
 */

VOID I2CDev_SendStop( VOID )
{
	UINT8 temp ;

	temp = I2C_CTL ;
	temp = temp | I2CMASK_SET_STP ;
	temp = temp & I2CMASK_CLR_IFLG ;
	I2C_CTL = temp ;

	if(i2cConfigParams.mode == I2C_MULTI_MASTER)/*in case of Multi-Master only*/
	{
		I2C_ClearIFlag() ;							/* clear IFLAG */
		I2C_CTL  |= I2CMASK_SET_INT ;				/* Enabling the I2C interrupts */
	}

	return ;
}/** end of I2C_SendStop() */


/*
 * function name       : I2CDev_Close
 * function description: Close the I2C communication by soft-resetting the setup
 * Return values	   : status /success or error
 * Global variables affected/used: none
 */

INT I2CDev_Close( VOID )
{
	/**
	 * Make sure the reset has happened by checking the
	 * STA, STP and IFLG bits for zeros
	 */
	if( I2C_CTL & I2CMASK_SRESET_CONFIRM )
		return I2CERR_CLOSEFAIL ;

	return I2CERR_SUCCESS ;
	
}/** end of I2C1_Close() */


/*
 * function name       : I2CDev_TransmitDataByte
 * function description: transmits the given data byte over the I2C bus
 * Return values	   : status /success or error
 * Global variables affected/used: none
 */


INT I2CDev_TransmitDataByte( INT8 data )
{
	UINT32 count = 0 ;
	INT status ;

	/** place the data byte to be transmitted in the data register */
	I2C_DR = data ;

	/** clear iflag bit to transmit the data byte */
	I2C_ClearIFlag() ;

	/** poll on iflag to see if the data byte transfer is complete */
	I2C_POLL_IFLG( count, COUNT_POLL_IFLG ) ;

	/** iflag is set */
	if( I2C_CTL & I2CMASK_GET_IFLG )
	{
		UINT8 ch = I2C_SR ;
		switch( ch )
		{
			case I2CSTAT_SLAWTXD_ACKNRXD:
				{
					status = I2CERR_SLAWTXD_ACKNRXD;
					break;
				}
			case I2CSTAT_DATABYTETXDMMODE_ACKRXD:
				{
					status = I2CERR_SUCCESS ;
					break ;
				}

	    	case I2CSTAT_DATABYTETXDMMODE_ACKNRXD:
				{
					status = I2CERR_DATABYTETXDMMODE_ACKNRXD ;
					break ;
				}

			case I2CSTAT_SLAWTXD_ACKRXD:
				{
					status = I2CERR_SUCCESS ;
					break ;
				}
			case I2CSTAT_SLARTXD_ACKRXD:
				{
					status = I2CERR_SUCCESS ;
					break ;
				}
			case I2CSTAT_SLDATATXD_ACKRXD:
				{
					status = I2CERR_SUCCESS ;
					break ;
				}
		
			case I2CSTAT_ARBLOST:					/* arbitration lost */
				{
					status = I2CERR_ARBLOST ;
					break;
				}
			case I2CSTAT_ARBLOST_SLAWRXD_ACKTXD:	/*Arb Lost, Master has received its own SLAVE ADDRESS and WRITE 
		  												mode also acknowledment is transmitted */
				{
					status = I2CERR_ARBLOST_SLAWRXD_ACKTXD; 
					break;
				}
			
			case I2CSTAT_ARBLOST_GLCLADRXD_ACKTXD:	/*Arb Lost, but Master has received a GENERAL CALL ADDRESS and WRITE 
		  												mode also acknowledment is transmitted */
				{
					status = I2CERR_ARBLOST_GLCLADRXD_ACKTXD; 
					break;
				}
	
			case I2CSTAT_ARBLOST_SLARRXD_ACKTXD:		/*Arb Lost, but Master has received its own SLAVE ADDRESS and READ 
		 												 mode also acknowledment is transmitted */
				{
					status = I2CERR_ARBLOST_SLARRXD_ACKTXD; 
					break;
				}

	
			case I2CSTAT_SLDATATXD_ACKNRXD:
				{
					I2C_SRR = 0x01;   					/* Resetting the I2C */
					status = I2CERR_IDLE;		
					break;			
				}
			case I2CSTAT_SLARTXD_ACKNRXD:
				{
					status = I2CERR_SLARTXD_ACKNRXD;
					break;	
				}	

			default:
				{
					I2C_SRR = 0x01;   					/* Resetting the I2C */
					status = I2CERR_FAILURE ;
					break ;
				}

		}/** end of 'switch' */

	}/** end 'iflag set' */

	/** timeout */
	else
	{
		I2C_SRR = 0x01;   /* Resetting the I2C */
		status = I2CERR_TIMEOUT ;

	}/** end 'timeout' */
	


	return status ;

}/** end of I2C1_TransmitDataByte() */



/*
 * function name       : I2CDev_ReceiveDataByte
 * function description: receives a data byte over the I2C bus
 * Return values	   : status /success or error
 * Global variables affected/used: none
 */


INT I2CDev_ReceiveDataByte( INT8 *data )
{
	UINT32 count = 0 ;
	INT status ;

	/** poll on iflag */
	I2C_POLL_IFLG( count, COUNT_POLL_IFLG ) ;

	/** iflag is set */
	if( I2C_CTL & I2CMASK_GET_IFLG )
	{
		while( (I2C_SR == I2CSTAT_SLARTXD_ACKRXD) && (count < COUNT_POLL_SR) ) count++ ;

		/** read the data */
		*data = I2C_DR ;
		
	
		if( I2C_SR == I2CSTAT_DATABYTERXDMMODE_ACKTXD )
		{
			status = I2CERR_SUCCESS ;

		}

		else if( I2C_SR == I2CSTAT_SLDATARXD_ACKTXD )
		{
			status = I2CERR_SUCCESS ;

		}

		else if( I2C_SR == I2CSTAT_SLAWRXD_ACKTXD )
		{
			status = I2CERR_SUCCESS ;

		}

		
		else if( I2C_SR == I2CSTAT_DATABYTERXDMMODE_NACKTXD )/* data byte received, not ack transmitted */
		{
			status = I2CERR_DATABYTERXDMMODE_NACKTXD ;

		}

		else if( I2C_SR == I2CSTAT_ARBLOST )			/* arbitration lost */
		{
			status = I2CERR_ARBLOST ;

		}
			
		else if(I2C_SR == I2CSTAT_ARBLOST_SLAWRXD_ACKTXD)/*Arb Lost, but Master has received its own SLAVE ADDRESS and WRITE 
		  													mode also acknowledment is transmitted */
		{
			status = I2CERR_ARBLOST_SLAWRXD_ACKTXD; 
		}

		else if(I2C_SR == I2CSTAT_ARBLOST_GLCLADRXD_ACKTXD)		/*Arb Lost, but Master has received a GENERAL CALL ADDRESS and WRITE 
		  														mode also acknowledment is transmitted */
		{
			status = I2CERR_ARBLOST_GLCLADRXD_ACKTXD; 
		}
		
		else if(I2C_SR == I2CSTAT_ARBLOST_SLARRXD_ACKTXD)/*Arb Lost, but Master has received its own SLAVE ADDRESS and READ 
		  													mode also acknowledment is transmitted */
		{
			status = I2CERR_ARBLOST_SLARRXD_ACKTXD; 
		}
	

		/** unknown error */
		else
		{
			I2C_SRR = 0x01;   /* Resetting the I2C */
			status = I2CERR_FAILURE ;

		}/** end 'unknown' error */

	}/** end 'iflag set' */

	/** timeout */
	else
	{
		I2C_SRR = 0x01;   /* Resetting the I2C */
		status = I2CERR_TIMEOUT ;
	}/** end 'timeout' */
	
	return status ;

}/** end of I2C1_ReceiveDataByte() */

/** end of file */




