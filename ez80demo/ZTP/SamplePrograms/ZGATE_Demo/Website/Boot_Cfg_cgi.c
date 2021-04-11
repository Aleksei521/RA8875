/************************************************************************************************
** File:        Boot_Cfg_cgi.c
** Description: Dynamically generates ZGATE Startup Settings configuration page.
**
** Copyright 2016 Zilog Inc. ALL RIGHTS RESERVED.
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

#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZThread.h"
#include "ZDevice.h"
#include "http.h"
#include "httpd.h"
#include "socket.h"
#include "ZSemaphore.h"
#include "ZGATE_Web.h"
#include "cfileapi.h"


char BootCfgHdr[] = "<html><head><title>ZGATE Overview</title></head><body><textarea rows=\"10\" cols=\"80\" readonly wrap=\"OFF\"" \
                    "style=\"background-color:#FFFFDD;color:blue;font-family:courier;font-size:15px;overflow-x:auto;overflow-y:auto\">";
char BootCfgFtr[] = "</textarea></body></html>";

#define SIZE_BOOT_CFG_HDR     214
#define SIZE_BOOT_CFG_FTR     25



INT16
CfgStr_cgi
(
   struct http_request * request
)
{
   UINT16               Size = 0;
   char                 Buf[ 16 ];
   INT16                Sta;
   char             * * ppCfgStr;
   UINT8                NumStr = 0;
   UINT8                Len;


   /*
    * Count the length of the config strings
    */
   ppCfgStr = ZGATE_ConfigStrings;
   while( **ppCfgStr )
   {
      Size += strlen( *ppCfgStr++ );
      NumStr++;
   }
   //printf( "Total Len of Cfg strings %d NumStr %d\r\n", Size, NumStr );
   __http_itoal( Size + SIZE_BOOT_CFG_HDR + SIZE_BOOT_CFG_FTR + (NumStr<<1), Buf );



   http_add_header( request, HTTP_HDR_CONNECTION, "close" );
   http_add_header( request, HTTP_HDR_CONTENT_LENGTH, Buf );
   if( http_output_reply(request, HTTP_200_OK) >= 0 )
   {
      __http_write( request, BootCfgHdr, SIZE_BOOT_CFG_HDR );

      /*
       * Send each string followed by <CR><LF>
       */
      ppCfgStr = ZGATE_ConfigStrings;
      while( Size )
      {
         Len = strlen( *ppCfgStr );
         __http_write( request, *ppCfgStr++, Len );
         __http_write( request, "\r\n", 2 );
         Size -= Len;
      }

      __http_write( request, BootCfgFtr, SIZE_BOOT_CFG_FTR );
      Sta = OK;
   }
   else
   {
      Sta = -1;
   }

   return( Sta );
}



INT16
Boot_Cfg_cgi
(
   struct http_request * request
)
{
   char           Buf[ 16 ];
   INT16          Sta = -1;

   FILE         * pFp;
   int            Len;
   int            Cnt;


   pFp = fopen( "zg_rules.usr", "rb" );
   if( !pFp )
   {
      pFp = fopen( "zg_rules.def", "rb" );
   }
   if( pFp )
   {
      RZKAcquireSemaphore( ZGATE_CgiSem, MAX_INFINITE_SUSPEND );
      if( fseek(pFp, 0, SEEK_END) == 0 )
      {
         Len = ftell( pFp );
         fseek( pFp, 0, SEEK_SET );

         /*
          * Calculate amount of data to be sent to browser based on length of 
          * config file.  Start sendng output back to browser
          */
         __http_itoal( Len + SIZE_BOOT_CFG_HDR + SIZE_BOOT_CFG_FTR, Buf );

         http_add_header( request, HTTP_HDR_CONNECTION, "close" );
         http_add_header( request, HTTP_HDR_CONTENT_LENGTH, Buf );
         if( http_output_reply(request, HTTP_200_OK) >= 0 )
         {
            __http_write( request, BootCfgHdr, SIZE_BOOT_CFG_HDR );
            while( Len )
            {
               /*
                * Read file in blocks of ZGATE_INFO_BUFFER_SIZE bytes.
                */
               Cnt = fread( ZGATE_InfoBuf, 1, ZGATE_INFO_BUFFER_SIZE, pFp );
               if( Cnt > 0 )
               {
                  __http_write( request, ZGATE_InfoBuf, Cnt );
                  Len -= Cnt;
                  //printf( "Read Cnt %u  remaining Len %u\r\n", Cnt, Len );
               }
               else
               {
                  break;
               }
            }
            __http_write( request, BootCfgFtr, SIZE_BOOT_CFG_FTR );
            Sta = OK;
         }
      }
      fclose( pFp );
      RZKReleaseSemaphore( ZGATE_CgiSem );
   }
   else
   {
      Sta = CfgStr_cgi( request );
   }
   return( Sta );
}
