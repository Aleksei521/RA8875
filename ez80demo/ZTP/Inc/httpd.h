/*
 * File       :     httpd.h
 *
 * Description:  This file contains the HTTP Client side defines and API declarations.
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
 */
 /*
 * Copyright 2001, Metro Link, Inc.
 * All Rights Reserved
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Metro Link, Inc. and may
 * contain proprietary, confidential and trade secret information of
 * Metro Link, Inc. and/or its partners.
 *  
 * The contents of this file may not be disclosed to third parties, copied or
 * duplicated in any form, in whole or in part, without the prior written
 * permission of Metro Link, Inc.
 *
 */


#ifndef	_HTTPD_H
#define	_HTTPD_H	1

#include <ctype.h>
#include "socket.h"
#if 0 /* RajeshM */
#include "kernel.h"

#ifdef xinu
#include "kernel.h"
#endif
#ifdef unix
#include "inttypes.h"
#endif
#ifdef VXWORKS
#include "vxWorks.h"
#endif
#endif


#include "http.h"


/* HTTPD process info */
#define	HTTPPRI		24	/*AJ $ CR#5167 $ 21FEB05$ shift the band ,29*/		/* HTTPD priority		*/
#define	HTTPNAM	"httpd"		/* name of HTTPD process	*/

#define HTTPRQPRI	24	/*AJ $ CR#5167 $ 21FEB05$ shift the band,29 */
#define HTTPRQNAM	"http_rqst"



/* HTTP Headers */

/* Values 1  -100 are used for HTTP_HDR_* */
/* Values 101-200 are used for SSDP_HDR_* */
/* Values 201-300 are used for SOAP_HDR_* */
/* Values 301-400 are used for GENA_HDR_* */

#define HTTP_HDR_ACCEPT			1
#define HTTP_HDR_CACHE_CONTROL		2
#define HTTP_HDR_CONNECTION		3
#define HTTP_HDR_CONTENT_LENGTH		4
#define HTTP_HDR_CONTENT_TYPE		5
#define HTTP_HDR_TRANSFER_ENCODING	6
#define	HTTP_HDR_DATE			7
#define	HTTP_HDR_LOCATION		8
#define	HTTP_HDR_HOST			9
#define	HTTP_HDR_SERVER			10
#define	HTTP_HDR_ASK_CLIENT_AUTH		11
#define   HTTP_HDR_SEND_CLIENT_AUTH		12
#define   HTTP_HDR_SEND_SERVER_AUTH		13


/* UPnP-specific headers - These should probably move to upnp.h */
#define	HTTP_HDR_EXT			11
#define	HTTP_HDR_ST			12
#define	HTTP_HDR_USN			13
#define	HTTP_HDR_MAN			14
#define	HTTP_HDR_MX			15
#define	HTTP_HDR_NT			16
#define	HTTP_HDR_NTS			17
#define	HTTP_HDR_SOAPACTION		18
#define	HTTP_HDR_CALLBACK		19
#define	HTTP_HDR_TIMEOUT		20
#define	HTTP_HDR_SID			21
#define	HTTP_HDR_SEQ			22

/* HTTP Reply Codes */
#define HTTP_200_OK			200
#define	HTTP_401_AUTHORISATION_REQUIRED		401
#define HTTP_400_BAD_REQUEST		400
#define HTTP_403_FORBIDDEN		403
#define HTTP_404_NOT_FOUND		404
#define HTTP_411_LENGTH_REQUIRED	411
#define HTTP_412_PRECONDITION_FAILED	412
#define HTTP_414_REQUEST_URI_TOO_LONG	414
#define HTTP_500_INTERNAL_ERROR		500
#define HTTP_501_NOT_IMPLEMENTED	501
#define HTTP_503_SERVICE_UNAVAILABLE    503

//#ifndef __http_write
//#define __http_write(rqst,buf,size) rqst->write(rqst->fd,buf,size)
#define __https_write(rqst,buf,size) (*((rqst->httpsfd)->fnWrite) )( rqst->httpsfd, buf, size)
#define __http_write(rqst,buf,size)   {if (send(rqst->fd,buf,size,tcp_FlagPUSH) < 0) {rqst->keepalive=0;return -1;}}


//#endif

/* httpfind.c */
extern Webpage *http_findpage(struct http_request *request);
/* httpget.c */
extern INT16 http_get(struct http_request *request);
extern void https_get(struct http_request *request);
/* httpAget.c */
extern INT16 httpAuth_get(struct http_request *request);
/* httpAget.c */
extern INT16 httpBasic_get(struct http_request *request);
/* httphdr.c */
//RK : 6th Jan 2006 : Changed the below line during IAR porting
//extern void http_output_headers(struct http_request *rqst);
extern INT16 http_output_headers(Http_Request *request);
extern void https_output_headers(struct http_request *rqst);

extern INT16 https_get_header(struct http_request *rqst, INT16 Val);
extern INT16 http_get_header(struct http_request *rqst, INT16 Val);
extern void http_reset_headers(struct http_request *rqst);

extern void http_add_header(struct http_request *rqst, UINT16 header, INT8 *value);

extern INT8 *http_find_header(struct http_request *rqst, UINT8 key);
extern INT8 *http_find_argument(struct http_request *rqst, UINT8 *arg);
extern INT8 *http_find_param(Http_Params *params, UINT8 *key);
/* httphdr.c */
extern INT16 httpAuth_get_header(struct http_request *rqst, INT16 Val);
/* httporply.c */
extern INT16 http_output_reply(struct http_request *request, UINT16 reply);
extern INT16 https_output_reply(struct http_request *request, UINT16 reply);
/* httpparse.c */
extern INT16 https_parse_request(struct http_request *rqst);
extern INT16 http_parse_request(struct http_request *rqst);
extern INT16 http_parse_response(struct http_request *rqst);
extern INT16 http_parse_arguments(struct http_request *rqst, INT8 *ptr);
extern INT8 * __http_uri_unescape(INT8 *ptr);

/* httpAparse.c */
extern INT16 httpAuth_parse_request(struct http_request *rqst);
/* httpBasicParse.c */
extern INT16 httpBasic_parse_request(struct http_request *rqst);

extern INT16 httpAuth_parse_response(struct http_request *rqst);
extern INT16 httpAuth_parse_arguments(struct http_request *rqst, INT8 *ptr);


/* httpparse.c */
extern INT16 http_parse_arguments(struct http_request *rqst, INT8 *ptr);
extern INT16 http_parse_request(struct http_request *rqst);
extern void httpGetMoreParams( struct http_request * pReq );

/* httppost.c */
extern void https_post(struct http_request *request);
extern INT16 http_post(struct http_request *request);
/* httprqst.c */
extern INT16 http_request(struct http_request *request);
extern INT16 https_request(struct http_request *request);
/* httpArqst.c */
extern INT16 httpAuth_request(struct http_request *request);
/* httpBasicrqst.c */
extern INT16 httpBasicAuth_request(struct http_request *request);
/* httpitoa.c */
INT8 *__http_itoal(INT32 value, INT8 *s);
INT8 *__http_itoa(INT16 value, INT8 *s);
INT16 __http_atoi(INT8 *s);
/* httpd-os.c */
INT16 __http_date(INT8 *buf, UINT16 size);
void __http_init_request(struct http_request *req);
void __http_complete_request(struct http_request *req);
extern INT8 getc1( INT16 fd );
extern INT16 getc(RZK_DEVICE_CB_t *pdev);
#endif	/* _HTTPD_H */
