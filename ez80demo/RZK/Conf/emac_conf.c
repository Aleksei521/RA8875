/********************************************************************************
 * File       : emac_conf.c
 *
 * Description: Defines the configurable system parameters for EMAC device
 *
 * Copyright 2017 ZiLOG Inc.  ALL RIGHTS RESERVED.
 *
 * This file contains unpublished confidential and proprietary information
 * of ZiLOG, Inc.
 * NO PART OF THIS WORK MAY BE DUPLICATED, STORED, PUBLISHED OR DISCLOSED 
 * IN ANY FORM WITHOUT THE PRIOR WRITTEN CONSENT OF ZiLOG, INC.
 * This is not a license and no use of any kind of this work is authorized
 * in the absence of a written license granted by ZiLOG, Inc. in ZiLOG's 
 * sole discretion 
 *******************************************************************************/


#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZThread.h"
#include "ZMessageQ.h"
#include "ZDevice.h"
#include "eZ80DevKit.h"
/*include for WLAN support*/
#include "EtherMgr.h"
#include "common.h"
#include <stdio.h>


#ifdef PPPoE
#include "PPPoE_Globals.h"
#endif

extern void emacisr(void) ;
extern DDF_STATUS_t IONULL( void );

// **************************
// CHANGE THE BELOW VALUES IF YOU KNOW WHAT YOU ARE DOING,
// REFER TO REFERENCE MANUAL FOR DESCRIPTIONS
// **************************
#define EMAC_THD_STACK_SIZEH            (4096)
#define EMAC_TASK_PRIOH                 (6)		/* CR#5167 */
#define EMAC_MQ_SIZEH                   (12)
#define EMAC_RX_BLOCK_TIMEH             (2)

INT8	f91_mac_addr[ETHPKT_ALEN] = {0x00,0x90,0x23,0x00,0x01,0x01};

#ifdef ZDOT_WLAN
UINT8 	cur_bssid [ETHPKT_ALEN] = {0x00,0x90,0x23,0x00,0x02,0x02};
#endif

#ifdef _EZ80F91	// This configuration is required for F91 only
#include "emacF91.h"

// F91 EMAC Configuration 
const F91_EMAC_CONF_t	F91_emac_config = 
{	
	1568,	   // Size of Mac transmit buffer
   /*
    * Phy link speed and duplex mode must be one of:
    *    F91_10_HD, F91_10_FD, F91_100_HD, F91_100_FD, F91_AUTO
    * Option is typically constrained by Phy pin-strapping.
    */
   #if( (DEV_KIT == eZ80F91_99C1322) || (DEV_KIT == eZ80F91_99C1380) || (DEV_KIT==eZ80F91_ZGATE) )
      F91_AUTO,
   #else
      F91_10_HD,
   #endif
	BUF32	   // Each packet buffer in EMAC_RAM will be 32 bytes long
};
#endif


/* Emac Device Control Block Initialization Structure */
RZK_DEVICE_CB_t EmacDev = {
			0, "EMAC", 
			(FNPTR_RZKDEV_INIT)IONULL, (FNPTR_RZKDEV_STOP)IONULL,
			(FNPTR_RZKDEV_OPEN)EmacOpen,(FNPTR_RZKDEV_CLOSE)EmacClose, 
			(FNPTR_RZKDEV_READ)EmacRead,(FNPTR_RZKDEV_WRITE)EmacWrite, 
			(FNPTR_RZKDEV_SEEK)IONULL,(FNPTR_RZKDEV_GETC)IONULL, 
			(FNPTR_RZKDEV_PUTC)IONULL, (FNPTR_RZKDEV_IOCTL)EmacControl,
			(RZK_PTR_t)emacisr,0000, NULL, 0, 0} ;


/* EMAC Configurations */


#ifdef RZK_STANDALONEH
#define MAX_IP_RX_BUFFH                (16)
UINT16 IP_MQ_SIZE ;
UINT8 IP_Message_Buff[MAX_IP_RX_BUFFH] ;
PKT_BUFF_t ipRxBuff[MAX_IP_RX_BUFFH] ;	
UINT MAX_IP_RX_BUFF = MAX_IP_RX_BUFFH ;
INT8 RZK_STANDALONE = RZK_TRUE ;
#else
INT8 RZK_STANDALONE = RZK_FALSE ;
#endif


// **************************
// DO NOT CHANGE ANYTHING BELOW IN THIS SECTION
// **************************
#define EMAC_MESSAGE_SIZEH              (sizeof(void *))
UINT      EMAC_THD_STACK_SIZE                     = EMAC_THD_STACK_SIZEH ;
UINT8     EMAC_TASK_PRIO                          = EMAC_TASK_PRIOH ;
UINT16    EMAC_MQ_SIZE                            = EMAC_MQ_SIZEH ;
TICK_t    rxBlockTime                             = EMAC_RX_BLOCK_TIMEH ;


extern void					ReceiveEthPkt(EMAC_FRAME_t * ) ;
extern RZK_MESSAGEQHANDLE_t rxQueueHandle ;
extern TICK_t				rxBlockTime ;
extern ETH_DEV_STRUCT_t		emac[];

void RxFunc(void)
{
	ETH_PKT_t *				pep;
	ETH_DEV_STRUCT_t	*	ped=&(emac[0]);
	RZK_STATUS_t status ;

	pep = (ETH_PKT_t*)AllocPktBuff() ;
	if( pep )
	{

   		ReceiveEthPkt( pep );
		if( pep->ethPktLen )
		{
			pep->ethPktOrder = EPO_NET;
			pep->ifNum = (UINT8)(ped->pDev->dvminor) ;

         #ifdef ZGATE_BUILD
         {
           #include "ZGATE.H"
 
            if( ZGATE_st_filter_eth(&pep->ethPktHeader) != ZGATE_FORWARD_PACKET )
            {
               FreePktBuff( pep );
               return;
            }
         }
         #endif

#ifdef PPPoE
			if( ( pep->PPPoEField == g_PPPoE_EthTypeDiscovery ) ||
				( pep->PPPoEField == g_PPPoE_EthTypeSession )

			  )
			{
				/** Post the message to the queue **/
				if( ( status = RZKSendToQueue( 	g_PPPoE_RxQueueHandle, 
												(RZK_PTR_t)&pep, 
												sizeof(RZK_PTR_t), 
												0 
											  ) ) != RZKERR_SUCCESS 
				  )
				{
					/** In case of error, free the packet **/
					FreePktBuff(pep) ;
				}
			}
			else
			{
				status = RZKSendToQueue( rxQueueHandle, (RZK_PTR_t)&pep, sizeof(RZK_PTR_t), 0 ) ;
				if( status != RZKERR_SUCCESS )
				{
					FreePktBuff(pep) ;
					/* On error free the memory and return */
				}
			}
#else
			status = RZKSendToQueue( rxQueueHandle, (RZK_PTR_t)&pep, sizeof(RZK_PTR_t), 0 ) ;
			if( status != RZKERR_SUCCESS )
			{
				FreePktBuff(pep) ;
				/* On error free the memory and return */
			}	

#endif

		}
		else
		{
			FreePktBuff(pep) ;
		}
	}
	else
	{
	/* Need to handle this */
	#if DEBUG
		printf( "emac: No Rx buffer avail\n" );
	#endif
	}
}


