/************************************************************************************************
** File:        Thresh_cgi.c
** Description: Dynamically generates ZGATE Threshold filtering settings configuration page.
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
#include "ZSemaphore.h"



INT16
ZGATE_Thresh_cgi
(
   struct http_request * request
)
{

   UINT16               Size;
   char                 Buf[ 16 ];
   char               * pDat = ZGATE_InfoBuf;
   UINT8                Idx = 0;
   char               * pThreshVal[3];
   INT16                Sta;


   RZKAcquireSemaphore( ZGATE_CgiSem, MAX_INFINITE_SUSPEND );

   ZGATE_get_th_config_string( pDat );
   #ifdef ZGATE_DEBUG
      printf( "FG Thresh String %s\r\n", pDat );
   #endif
   while( *pDat )
   {
      if( *pDat == '=' )
      {
         pDat += 2;
         pThreshVal[Idx++] = pDat;
         while( (*pDat != ' ') && (*pDat != '\n') )
         {
            pDat++;
         }
         *pDat++ = 0;
      }
      else
      {
         pDat++;
      }
   }
   //printf( "Parsed strings are Interval *%s*  Hi *%s* Lo *%s*\r\n", pThreshVal[0], pThreshVal[1], pThreshVal[2] );
   if( ZGATE_th_filtering_on() == FALSE )
   {
      //xc_sprintf( ZgateBuf, "var Thresh_Mode=\"Disabled\";\r\n" );
      xc_sprintf( ZGATE_InfoBuf, "var Thresh_Interval=0;\r\nvar Thresh_HW=0;\r\nvar Thresh_LW=0;\r\nvar Thresh_Mode=\"Disabled\";\r\n" );
   }
   else
   {
      xc_sprintf( ZGATE_InfoBuf, "var Thresh_Interval=%s;\r\nvar Thresh_HW=%s;\r\nvar Thresh_LW=%s;\r\nvar Thresh_Mode=\"Enabled\";\r\n",
         pThreshVal[0], pThreshVal[1], pThreshVal[2] );
   }

   Size = strlen( ZGATE_InfoBuf );
   __http_itoal( Size, Buf );

   http_add_header( request, HTTP_HDR_CONNECTION, "close" );
   http_add_header( request, HTTP_HDR_CONTENT_LENGTH, Buf );
   if( http_output_reply(request, HTTP_200_OK) < 0) 
   {
      Sta = -1;
   }
   else
   {
      __http_write( request, ZGATE_InfoBuf, Size );
      Sta = OK;
   }

   RZKReleaseSemaphore( ZGATE_CgiSem );
   return( Sta );
}