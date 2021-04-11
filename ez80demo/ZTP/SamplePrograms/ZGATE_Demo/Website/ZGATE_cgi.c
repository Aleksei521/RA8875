/************************************************************************************************
** File:        ZGATE_cgi.c
** Description: Common routines used in the generation of dynamic ZGATE web pages.
**
** Copyright 2012 Zilog Inc. ALL RIGHTS RESERVED.
*
*************************************************************************************************
* The source code in this file was written by an authorized Zilog employee or a licensed 
* consultant. The source code has been verified to the fullest extent possible. 
*
* Permission to use this code is granted on a royalty-free basis. However, users are cautioned to
* authenticate the code contained herein. 
* 
* ZILOG DOES NOT GUARANTEE THE VERACITY OF THIS SOFTWARE; ANY SOFTWARE CONTAINED HEREIN IS
* PROVIDED "AS IS." NO WARRANTIES ARE GIVEN, WHETHER EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
* IMPLIED WARRANTIES OF FITNESS FOR PARTICULAR PURPOSE OR MERCHANTABILITY. IN NO EVENT WILL ZILOG
* BE LIABLE FOR ANY SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES OR ANY LIABILITY IN TORT,
* NEGLIGENCE, OR OTHER LIABILITY INCURRED AS A RESULT OF THE USE OF THE SOFTWARE, EVEN IF ZILOG 
* HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. ZILOG ALSO DOES NOT WARRANT THAT THE USE 
* OF THE SOFTWARE, OR OF ANY INFORMATION CONTAINED THEREIN WILL NOT INFRINGE ANY PATENT, 
* COPYRIGHT, OR TRADEMARK OF ANY THIRD PERSON OR ENTITY.

* THE SOFTWARE IS NOT FAULT-TOLERANT AND IS NOT DESIGNED, MANUFACTURED OR INTENDED FOR USE IN 
* CONJUNCTION WITH ON-LINE CONTROL EQUIPMENT, IN HAZARDOUS ENVIRONMENTS, IN APPLICATIONS 
* REQUIRING FAIL-SAFE PERFORMANCE, OR WHERE THE FAILURE OF THE SOFTWARE COULD LEAD DIRECTLY TO 
* DEATH, PERSONAL INJURY OR SEVERE PHYSICAL OR ENVIRONMENTAL DAMAGE (ALL OF THE FOREGOING, 
* "HIGH RISK ACTIVITIES"). ZILOG SPECIFICALLY DISCLAIMS ANY EXPRESS OR IMPLIED WARRANTY TO HIGH 
* RISK ACTIVITIES.
*
************************************************************************************************/

#include "ZGATE_Web.h"

#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZThread.h"
#include "ZSemaphore.h"


char ZGATE_InfoBuf[ ZGATE_INFO_BUFFER_SIZE ];


typedef void        ( * FP_BUILD_LIST )( char * pList );


#define DISABLED_SETTINGS                 \
   "var FilterMode=\"Disabled\";\r\n"     \
   "var MaxTableSize=0;\r\n"              \
   "var PortList = \"\";\r\n"



UINT16
ZGATE_GetTableInfo
(
   char               * pBuf,
   UINT8                TableId
)
/*
 * This function queries ZAGATE to obtain filtering information regarding the
 * specified TableId.  The information returned to the caller is ASCII text in
 * the form of JavaScript variables.
 */
{
   UINT16               MaxTableSize;
   UINT16               FilterMode = ZGATE_FILTER_DISABLED;
   FP_BUILD_LIST        fpGetFilterList = 0;
   UINT16               Len;


   MaxTableSize  = ZGATE_get_list_size( TableId );
   if( TableId == ZGATE_ETH_FRAME_LIST )
   {
      FilterMode = ZGATE_eth_frame_filtering_type();
      fpGetFilterList = ZGATE_build_eth_frame_list;
   }
   if( TableId == ZGATE_ETH_ADDR_LIST )
   {
      FilterMode = ZGATE_eth_addr_filtering_type();
      fpGetFilterList = ZGATE_build_eth_addr_list;
   }
   if( TableId == ZGATE_IP_PROT_LIST )
   {
      FilterMode = ZGATE_ip_prot_filtering_type();
      fpGetFilterList = ZGATE_build_ip_protocol_list;
   }
   if( TableId == ZGATE_IP_ADDR_LIST )
   {
      FilterMode = ZGATE_ip_addr_filtering_type();
      fpGetFilterList = ZGATE_build_ip_addr_list;
   }
   if( TableId == ZGATE_TCP_PORT_LIST )
   {
      FilterMode = ZGATE_tcp_port_filtering_type();
      fpGetFilterList = ZGATE_build_TCP_port_list;
   }
   if( TableId == ZGATE_UDP_PORT_LIST )
   {
      FilterMode = ZGATE_udp_port_filtering_type();
      fpGetFilterList = ZGATE_build_UDP_port_list;
   }

   if( FilterMode == ZGATE_FILTER_DISABLED )
   {
      #ifdef ZGATE_DEBUG
         printf( "Filtering disabled for list %x\r\n", TableId );
      #endif
      strcpy( pBuf, DISABLED_SETTINGS );
      Len = sizeof( DISABLED_SETTINGS ) -1;
   }
   else
   {
      memcpy( pBuf, "var PortList=", 13 );
      fpGetFilterList( &pBuf[13] );
      Len = strlen( pBuf );
      pBuf[ 13 ] = '"';
      pBuf[ Len -1 ] = '"';
      pBuf[ Len++ ] = ';';
      pBuf[ Len++ ] = '\r';
      pBuf[ Len++ ] = '\n';
    
      xc_sprintf( &pBuf[Len], "var MaxTableSize=%u;\r\n", MaxTableSize );
      Len += strlen( &pBuf[Len] );

      if( FilterMode == ZGATE_BLACKLIST_FILTERING )
      {
         strcpy( &pBuf[Len], "var FilterMode=\"Blacklist\";\r\n" );
         Len += 29;
      }
      else
      {
         strcpy( &pBuf[Len], "var FilterMode=\"Whitelist\";\r\n" );
         Len += 29;
      }
   }

   return( Len );
}




INT16
ZGATE_GenCgiResponse
(
   struct http_request *request,
   UINT8                TableId
)
{
   UINT16         Size;
   char           Buf[ 16 ];
   INT16          Sta;


   RZKAcquireSemaphore( ZGATE_CgiSem, MAX_INFINITE_SUSPEND );

   Size = ZGATE_GetTableInfo( ZGATE_InfoBuf, TableId );
   __http_itoal( Size, Buf );

   http_add_header( request, HTTP_HDR_CONNECTION, "close" );
   http_add_header( request, HTTP_HDR_CONTENT_LENGTH, Buf );
   if( http_output_reply(request, HTTP_200_OK) >= 0 )
   {
      __http_write( request, ZGATE_InfoBuf, Size );
      Sta = OK;
   }
   else
   {
      Sta = -1;
   }

   RZKReleaseSemaphore( ZGATE_CgiSem );
   return( Sta );
}


#define ZGATE_CGI_UNKNOWN     0
#define ZGATE_CGI_GET         1
#define ZGATE_CGI_ADD         2
#define ZGATE_CGI_DEL         3



INT16
ZGATE_Conf_cgi
(
   struct http_request * request
)
{



   UINT8                i;
   struct staticpage  * pPage = 0;
   INT8                 tmpbuf2[ 28 ];
   ether_addr_t         EthAddr;
   UINT32               IpAddr;
   UINT8                CgiOp;
   UINT16               Val16;


KeepParsing:
   for(i=0; i<request->numparams; i++ )
   {
      #ifdef ZGATE_DEBUG
         printf( "Key %s Value %s\r\n", request->params[i].key, request->params[i].value );
      #endif

      if( memcmp(request->params[i].value, "get", 3) == 0 )
      {
         CgiOp = ZGATE_CGI_GET;
      }
      else
      {
         if( memcmp(request->params[i].value, "add+", 4) == 0 )
         {
            CgiOp = ZGATE_CGI_ADD;
         }
         else
         {
            if( memcmp(request->params[i].value, "del+", 4) == 0 )
            {
               CgiOp = ZGATE_CGI_DEL;
            }
            else
            {
               CgiOp = ZGATE_CGI_UNKNOWN;
            }
         }
      }

      if( strcmp((char*)request->params[i].key, "ZGATE_eth_addr") == 0 )
      {
         pPage = &Eth_Addr_htm;
         /*
          * During form submission the Browser URL-encodes the form data.
          * Consequently the ":" separator between bytes in the MAC address will
          * be transmitted as the ASCII sequence "%3A".  Un-escape the parameter
          * value before passing the Ethernet MAC address to ZGATE.
          *
          * The Browser script already validated the Ethernet address.
          */
         if( (CgiOp == ZGATE_CGI_ADD) || (CgiOp == ZGATE_CGI_DEL) )
         {
            strcpy( tmpbuf2, &request->params[i].value[4] );
            __http_uri_unescape( tmpbuf2 );
            eth_string_to_num( tmpbuf2, EthAddr );
            if( CgiOp == ZGATE_CGI_ADD )
            {
               ZGATE_add_eth_addr( EthAddr );
            }
            else
            {
               ZGATE_remove_eth_addr( EthAddr );
            }
         }
      }
      if( strcmp((char*)request->params[i].key, "ZGATE_eth_type") == 0 )
      {
         pPage = &Eth_Type_htm;
         if( (CgiOp == ZGATE_CGI_ADD) || (CgiOp == ZGATE_CGI_DEL) )
         {
            Val16 = atoi( (char *)&request->params[i].value[4] );
            if( CgiOp == ZGATE_CGI_ADD )
            {
               ZGATE_add_eth_frame( Val16 );
            }
            else
            {
               ZGATE_remove_eth_frame( Val16 );
            }
         }
      }
      if( strcmp((char*)request->params[i].key, "ZGATE_ip_addr") == 0 )
      {
         pPage = &IP_Addr_htm;
         if( (CgiOp == ZGATE_CGI_ADD) || (CgiOp == ZGATE_CGI_DEL) )
         {
            /*
             * Browser script already validated IP address
             */
            inet_pton( 1, (char *)&request->params[i].value[4], (UINT8 *)&IpAddr );
            if( CgiOp == ZGATE_CGI_ADD )
            {
               ZGATE_add_ip_addr( IpAddr );
            }
            else
            {
               ZGATE_remove_ip_addr( IpAddr );
            }
         }
      }
      if( strcmp((char*)request->params[i].key, "ZGATE_ip_prot") == 0 )
      {
         pPage = &IP_Prot_htm;
         if( (CgiOp == ZGATE_CGI_ADD) || (CgiOp == ZGATE_CGI_DEL) )
         {
            Val16 = atoi( (char *)&request->params[i].value[4] );
            if( CgiOp == ZGATE_CGI_ADD )
            {
               ZGATE_add_ip_prot( Val16 );
            }
            else
            {
               ZGATE_remove_ip_prot( Val16 );
            }
         }
      }
      if( strcmp((char*)request->params[i].key, "ZGATE_tcp_port") == 0 )
      {
         pPage = &TCP_Port_htm;
         if( (CgiOp == ZGATE_CGI_ADD) || (CgiOp == ZGATE_CGI_DEL) )
         {
            Val16 = atoi( (char *)&request->params[i].value[4] );
            if( CgiOp == ZGATE_CGI_ADD )
            {
               ZGATE_add_tcp_port( Val16 );
            }
            else
            {
               ZGATE_remove_tcp_port( Val16 );
            }
         }
      }
      if( strcmp((char*)request->params[i].key, "ZGATE_udp_port") == 0 )
      {
         pPage = &UDP_Port_htm;
         if( (CgiOp == ZGATE_CGI_ADD) || (CgiOp == ZGATE_CGI_DEL) )
         {
            Val16 = atoi( (char *)&request->params[i].value[4] );
            if( CgiOp == ZGATE_CGI_ADD )
            {
               ZGATE_add_udp_port( Val16 );
            }
            else
            {
               ZGATE_remove_udp_port( Val16 );
            }
         }
      }

      /*
       * Threshold Filtering forms use standard Name Value encodings on Post
       */
      if( strcmp((char*)request->params[i].key, "ZGATE_thresh") == 0 )
      {
         pPage = &Thresh_htm;
      }
      if( strcmp((char*)request->params[i].key, "thresh_Int") == 0 )
      {
         pPage = &Thresh_htm;
         ZGATE_set_th_interval( atoi(request->params[i].value) );
      }
      if( strcmp((char*)request->params[i].key, "thresh_Lo") == 0 )
      {
         pPage = &Thresh_htm;
         ZGATE_set_th_LW( atoi(request->params[i].value) );
      }
      if( strcmp((char*)request->params[i].key, "thresh_Hi") == 0 )
      {
         pPage = &Thresh_htm;
         ZGATE_set_th_HW( atoi(request->params[i].value) );
      }

      if( strcmp((char*)request->params[i].key, "ZGATE_save") == 0 )
      {
         pPage = &Boot_Cfg_htm;
         ZGATE_save_config_changes_to_persistent();
      }
      if( strcmp((char*)request->params[i].key, "ZGATE_restore") == 0 )
      {
         pPage = &Boot_Cfg_htm;
         ZGATE_use_default_config();
      }

      /*
       * Logging
       */
      if( strcmp((char*)request->params[i].key, "LogDisp") == 0 )
      {
         //printf( "Screen logging: %s\r\n", request->params[i].value );
         pPage = &Log_Cfg_htm;
         if( strcmp(request->params[i].value, "Enable") == 0 )
         {
            ZGATE_enable_logging_to_screen();
         }
         else
         {
            ZGATE_disable_logging_to_screen();
         }
      }
      if( strcmp((char*)request->params[i].key, "LogFile") == 0 )
      {
         //printf( "File logging: %s\r\n", request->params[i].value );
         pPage = &Log_Cfg_htm;
         if( strcmp(request->params[i].value, "Enable") == 0 )
         {
            ZGATE_enable_logging_to_file();
         }
         else
         {
            ZGATE_disable_logging_to_file();
         }
      }

      /*
       * When modifications are made to the filtering tables and the tables are
       * near capacity the ZGATE configuration functions will take several seconds
       * to complete.  Since this HTTP thread runs at high relatiove priority it 
       * will potentially starve the System thread from procesing inbound packets 
       * in a timely manner.  Therefore after each update to the ZGATE configuration
       * delay a few clock ticks to give other threads a chance to execute.
       */
      RZKSuspendThread( RZKGetCurrentThread(), 2 );
   }


   /*
    * Check for more parameters.
    */
   httpGetMoreParams( request );
   if( request->numparams )
   {
      goto KeepParsing;
   }


   /*
    * Return the requested page
    */
   if( pPage )
   {
      http_add_header(request,HTTP_HDR_CONNECTION,"close");
   
      __http_itoal( pPage->size,tmpbuf2 );
      http_add_header(request,HTTP_HDR_CONTENT_LENGTH,tmpbuf2);
      if(http_output_reply(request,HTTP_200_OK)<0)return -1;
      __http_write( request, (char *)pPage->contents, pPage->size );
   }
   return( OK );
}
