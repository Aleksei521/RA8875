/************************************************************************************************
** File:        Log_Cfg_cgi.c
** Description: Dynamically generates ZGATE Logging configuration page.
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
ZGATE_Log_Cfg_cgi
(
   struct http_request * request
)
{

   UINT16               Size;
   char                 Buf[ 16 ];
   INT16                Sta;


   RZKAcquireSemaphore( ZGATE_CgiSem, MAX_INFINITE_SUSPEND );

   strcpy( ZGATE_InfoBuf, "var LogStr=\"" );
   ZGATE_get_logging_config( &ZGATE_InfoBuf[12] );
   #ifdef ZGATE_DEBUG
      printf( "ZG Logging String *%s*\r\n", ZGATE_InfoBuf );
   #endif

   // Overwrite \n chracters in logging_config string
   ZGATE_InfoBuf[ 0x21 ] = ' ';
   Size = strlen( ZGATE_InfoBuf );
   ZGATE_InfoBuf[ Size -1 ] = '"';
   ZGATE_InfoBuf[ Size++ ] = ';';

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