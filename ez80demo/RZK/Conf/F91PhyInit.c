/*
 * Copyright 2017, ZiLOG Inc.
 * All Rights Reserved
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of ZiLOG Inc., and might
 * contain proprietary, confidential and trade secret information of
 * ZiLOG, our partners and parties from which this code has been licensed.
 * 
 * The contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of ZiLOG Inc.
 */

#include <stdio.h>

#include "ZTypes.h"
#include "ZSysgen.h"
#include "ZThread.h"
#include "_ez80.h"
#include "emacF91.h"
#include "EtherMgr.h"
#include "eZ80DevKit.h"

#if( (DEV_KIT == eZ80F91_99C1322) || (DEV_KIT == eZ80F91_99C1380) || (DEV_KIT==eZ80F91_ZGATE) )
   #include "ICS1894_phy.h"
#else
   #include "AMD79C874_phy.h"
#endif


extern RZK_TIMERHANDLE_t emacTimerHdl ;


INT phyInit(void)

{
   
   UINT16         phy_data;
   UINT16         tempcount;
   UINT16         i;
   UINT8          Auto;
   
   // set phy address
   EMAC_FIAD = PHY_ADDRESS;
   
   // check if phy detected
   if (ReadPhyReg(PHY_ID1_REG, &phy_data) != RZKERR_SUCCESS )
      return(EMACDEV_ERR_PEMAC_PHYREAD_ERROR);
   else {
      if (phy_data != PHY_ID1)
         return(EMACDEV_ERR_PEMAC_PHY_NOT_FOUND);
   }
   if (ReadPhyReg(PHY_ID2_REG, &phy_data) != RZKERR_SUCCESS )
      return(EMACDEV_ERR_PEMAC_PHYREAD_ERROR);
   else {
      if (phy_data != PHY_ID2)
         return(EMACDEV_ERR_PEMAC_PHY_NOT_FOUND);
   }
   
   
   WritePhyReg(PHY_CREG, PHY_RST);
   
   //printf( "Attempting to establish Ethernet connection\n" );
   ReadPhyReg( PHY_SREG, &phy_data );
   if( phy_data & PHY_CAN_AUTO_NEG )
   {
      phy_data = PHY_RESTART_AUTO_NEG | PHY_AUTO_NEG_ENABLE;
      Auto = RZK_TRUE;
   }
   else
   {
      phy_data = 0;
      Auto = RZK_FALSE;
      EMAC_CFG1 = PADEN | CRCEN;
   }
   
   /*
   * Set desired link capabilities
   */
   switch( F91_emac_config.mode )
   {
      case( F91_AUTO ):
      {
         if( Auto )
         {
            WritePhyReg(PHY_ANEG_ADV_REG, 
               PHY_ANEG_100_FD
               | PHY_ANEG_100_HD
               | PHY_ANEG_10_FD
               | PHY_ANEG_10_HD 
               | PHY_ANEG_802_3 );
         }
         else
         {
            printf( "\nHW modifications required to enable Auto-Negotiation\n" );
         }
         break;
      }
      case( F91_100_FD ):
      {
         phy_data |= PHY_100BT | PHY_FULLD;
         WritePhyReg(PHY_ANEG_ADV_REG, 
            PHY_ANEG_100_FD
            | PHY_ANEG_100_HD
            | PHY_ANEG_10_FD
            | PHY_ANEG_10_HD
            | PHY_ANEG_802_3 );
         break;
      }
      case( F91_100_HD ):
      {
         phy_data |= PHY_100BT | PHY_HALFD;
         WritePhyReg(PHY_ANEG_ADV_REG, 
            PHY_ANEG_100_HD
            | PHY_ANEG_10_FD
            | PHY_ANEG_10_HD
            | PHY_ANEG_802_3 );
         break;
      }
      case( F91_10_FD ):
      {
         phy_data |= PHY_10BT | PHY_FULLD;
         WritePhyReg(PHY_ANEG_ADV_REG, 
            PHY_ANEG_10_FD
            | PHY_ANEG_10_HD
            | PHY_ANEG_802_3 );
         break;
      }
      case( F91_10_HD ):
      default:
      {
         phy_data |= PHY_10BT | PHY_HALFD;
         WritePhyReg(PHY_ANEG_ADV_REG, 
            PHY_ANEG_10_HD
            | PHY_ANEG_802_3 );
         break;
      }
   }
   WritePhyReg(PHY_CREG,   phy_data);
   
  /*
   * Wait for a link to be established
   */
   for( i=50, tempcount=0; i; i-- )
   {
      while( ++tempcount );

      ReadPhyReg( PHY_SREG, &phy_data );
      if( phy_data & (PHY_AUTO_NEG_COMPLETE | PHY_LINK_ESTABLISHED) )
      {
         break;
      }
      if( i%10 == 0 )
      {
         printf( "." );
      }
   }

   if( phy_data & PHY_LINK_ESTABLISHED )
   {
      if( Auto )
      {
         // Read the diagnostics register to determine link settings
         ReadPhyReg( PHY_DIAG_REG, &phy_data );
         if( phy_data & PHY_100_MBPS )
         {
            printf( "100 Mbps " );
         }
         else
         {
            printf( "10 Mbps " );
         }
         
         if( phy_data & PHY_FULL_DUPLEX )
         {
            printf( "Full-Duplex " );
            EMAC_CFG1 = PADEN | CRCEN | FULLD;
         }
         else
         {
            printf( "Half-Duplex " );
            #if( (DEV_KIT == eZ80F91_99C1322) || (DEV_KIT == eZ80F91_99C1380) || (DEV_KIT==eZ80F91_ZGATE) )
               // Use Full-duplex MII to communicate with ICS1894
               EMAC_CFG1 = PADEN | CRCEN | FULLD;
            #else
               EMAC_CFG1 = PADEN | CRCEN;
            #endif
         }
         printf( " Link established\n" );
      }         
      
   }
   else
   {
      #ifdef DEBUG
         printf( "Unable to establish Ethernet Connection\n" );
      #endif
      return EMACDEV_ERR_PEMAC_PHYINIT_FAILED ;
   }
   
   return EMACDEV_ERR_SUCCESS ;
   
   
}


UINT8
IsEthernetConnected( void )
{
   UINT16          phy_data;
   UINT8         Status = RZK_FALSE;

   /*
    * Read the Phy status register to determine if there is a physical connection.
    */
   if( ReadPhyReg( PHY_SREG, &phy_data ) == (UINT16)EMACDEV_ERR_SUCCESS )
   {
      if( phy_data & PHY_LINK_ESTABLISHED )
      {
         Status = RZK_TRUE;
      }
   }

   return( Status );
}



void emacConnThd(void)
{
UINT16 phy_data ;

   while(1)
   {

      ReadPhyReg( PHY_SREG, &phy_data );
      if( phy_data & PHY_LINK_ESTABLISHED )
      {
         // Read the diagnostics register to determine link settings
         ReadPhyReg( PHY_DIAG_REG, &phy_data );
         if( phy_data & PHY_100_MBPS )
         {
            printf( "100 Mbps " );
         }
         else
         {
            printf( "10 Mbps " );
         }
   
         if( phy_data & PHY_FULL_DUPLEX )
         {
            printf( "Full-Duplex " );
            EMAC_CFG1 = PADEN | CRCEN | FULLD;
         }
         else
         {
            printf( "Half-Duplex " );
            #if( (DEV_KIT == eZ80F91_99C1322) || (DEV_KIT == eZ80F91_99C1380) || (DEV_KIT==eZ80F91_ZGATE) )
               // Use Full-duplex MII to communicate with ICS1894
               EMAC_CFG1 = PADEN | CRCEN | FULLD;
            #else
               EMAC_CFG1 = PADEN | CRCEN;
            #endif
         }
         printf( " Link established\n" );
         break ;

      }
        else
      {
         RZKSuspendThread(RZKGetCurrentThread(), 50) ;
      }
   }
}
