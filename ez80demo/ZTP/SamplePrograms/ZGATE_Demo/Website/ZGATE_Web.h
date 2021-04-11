/************************************************************************************************
** File:        ZGATE_Web.h
** Description: Publich header file for ZGATE website.
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

#ifndef _ZGATE_WEB_H_
#define _ZGATE_WEB_H_

#include "httpd.h"
#include "ZGATE.h"


/*
 * The buffer used to obtain ZGATE filtering table information must be at least
 * 1802 bytes long since ZGATE_build_eth_addr_list() could return a string of 
 * this length.  In addition the Web-CGI routines place Java Script around the 
 * ZGATE data requiring additional padding.
 */
#define ZGATE_INFO_BUFFER_SIZE         2048



/*
 * Global Variables
 */
extern char                   ZGATE_InfoBuf[ ZGATE_INFO_BUFFER_SIZE ];
extern RZK_SEMAPHOREHANDLE_t  ZGATE_CgiSem;


/*
 * Function Prototypes
 */
extern const struct staticpage Eth_Addr_htm;
extern const struct staticpage Eth_Type_htm;
extern const struct staticpage IP_Addr_htm;
extern const struct staticpage IP_Prot_htm;
extern const struct staticpage TCP_Port_htm;
extern const struct staticpage UDP_Port_htm;
extern const struct staticpage Thresh_htm;
extern const struct staticpage Boot_Cfg_htm;
extern const struct staticpage Log_Cfg_htm;

extern INT16 ZGATE_GenCgiResponse( struct http_request * pReq, UINT8 TableId );



extern INT16 ZGATE_Eth_AddrList_cgi(struct http_request *request);
extern INT16 ZGATE_Eth_TypeList_cgi(struct http_request *request);
extern INT16 ZGATE_IP_AddrList_cgi(struct http_request *request);
extern INT16 ZGATE_IP_ProtList_cgi(struct http_request *request);
extern INT16 ZGATE_TCP_PortList_cgi(struct http_request *request);
extern INT16 ZGATE_UDP_PortList_cgi(struct http_request *request);
extern INT16 ZGATE_Thresh_cgi(struct http_request *request);
extern INT16 ZGATE_Log_Cfg_cgi(struct http_request *request);





#endif
