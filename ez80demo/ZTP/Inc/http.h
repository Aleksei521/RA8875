/*
 * File       :     http.h
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


#ifndef	_HTTP_H
#define	_HTTP_H

#include "socket.h"

/*
 * http.h - This contains the public interfaces for the httpd
 */

#ifdef _IAR_CODE
#define strstr1   strstr
#endif

/*
 * HTTP run-time parameters
 */
#define HTTPSTK   1024
#define HTTPRQSTK (1024 + sizeof(struct http_request))
extern UINT16           HttpdStackSize;
extern UINT16           HttpReqStackSize;
extern UINT8            PersistentHttp;



/** A static page is defined for pages that exist statically in the program. */
struct staticpage {
        /** A pointer to the actual contents of the page. This could be the
         * actual string representing the entire page, or an array of bytes
         * (e.g. the array produced by the mkwebpage program).  */
	UINT8 *contents;
        /*  The size of the above array, since it is not null terminated.  If
         *  this is actually a string, it would be equal to strlen(array).
         */
	INT32	   size;
//	WORD	   size;
	};


struct mimetype {
	/* File extensions */
	INT8 * FileExtns;
	/* Associated mime types for the file extensions */
	INT8 * type;
};

struct header_rec {
	INT8    *name;
   UINT16    val;
	};

/* page types */
#define HTTP_PAGE_STATIC	1
#define HTTP_PAGE_DYNAMIC	2

#ifndef HTTP_REQUEST_BUF
#define HTTP_REQUEST_BUF	1024
#endif

#ifndef HTTP_MAX_HEADERS
#define HTTP_MAX_HEADERS	16
#endif

#ifndef HTTP_MAX_PARAMS
#define HTTP_MAX_PARAMS	16
#endif


/* Authentication Defines */

#define MAX_NONCE			64
#define MAX_CNONCE			128
#define MAX_REALM			64
#define MAX_USRNAME			64
#define NC_VALUE			4
//#define RESPONSE_STRING		300	/* 32 bytes of digest + '\0' */
#define	RSPAUTH_STRING		300	/* 32 bytes of digest + '\0' */





/* HTTP Methods */

#define HTTP_GET	1
#define HTTP_HEAD	2
#define HTTP_POST	3
#define HTTP_LINK	4
#define HTTP_UNLINK	5
#define HTTP_PUT	6
#define HTTP_DELETE	7
#define HTTP_OPTIONS	8
#define HTTP_TRACE	9
#define	HTTP_NOTIFY	10
#define	HTTP_SEARCH	11
#define	HTTP_MSEARCH	12
#define	HTTP_MPOST	13
#define	HTTP_SUBSCRIBE	14
#define	HTTP_UNSUBSCRIBE 15

typedef struct http_hdr {
	UINT8	key;
	INT8*	value;
} Http_Hdr;


/**
 * A key/value pair of strings.
 * @name http_params
 * @type typedef struct http_params
 */
struct http_params {
   /**  The key, typically an http header. */
	UINT8	*key;
   /**  The value associated with that key. */
	INT8	*value;
};

struct http_usrs {

	INT8 *user;
	INT8 *pwd;
};

typedef struct http_auth {

	INT8 *AuthPtr;
	UINT16 AuthLength;
	INT8 *nonce;
	INT8 *cnonce;
	INT8 *realm;
	INT8 *username;
	INT8 *nccount;
	INT8 *Method;
	INT8 *uri;
	INT8 *response;
	INT8 rspauth[RSPAUTH_STRING];
}Http_Auth;


typedef struct http_params Http_Params;

/** @memo The http_request structure.
 * @doc This structure is used to hold parsed incoming HTTP requests, and to
 * hold HTTP requests being constructed for outgoing requests.
 * It holds all the information associated with a request.
 * The getch and write fields are particularly noteworthy.  These
 * values are callbacks provided by the implementor, which are used to send and
 * receive HTTP data. In this way the package is isolated from the I/O
 * mechanism, and it is even possible to use different I/O mechanisms for each
 * request generated or serviced.
 * @name http_request
 * @type typedef struct http_request
 */
typedef struct http_request {
	UINT8	                     method;
	UINT16	                     reply;
	UINT8	                     numheaders;
	UINT8	                     numparams;
	UINT8	                     numrespheaders;

	INT16	                     fd;

//	SYSCALL                   (*getch)(DID fd);
//	SYSCALL                   (*write)(DID fd, char *buf,WORD size);
	const struct http_method * methods;
	const struct webpage	    * website;
	const struct header_rec	 * headers;
	INT8	*                    bufstart; /* first free space */
	UINT8	*                    extraheader;
	Http_Hdr                   rqstheaders[HTTP_MAX_HEADERS];
	Http_Hdr                   respheaders[HTTP_MAX_HEADERS];
	Http_Params                params[HTTP_MAX_PARAMS];
   int                        AvailContentLen;
   int                        UnreadContentLen;
	Http_Auth		     	   *AuthParams;	
	INT8	                   buffer[HTTP_REQUEST_BUF];
	INT8			     keepalive;	
} Http_Request;



typedef struct http_method 
{
	UINT8	key;
	INT8	*name;
	INT16 (*method)(Http_Request *);
} Http_Method;


/**
 *  A structure describing any particular webpage which the http server is
 *  expected to handle.  The server will be passed a list of such structures.
 */
struct webpage 
{
   /* Whether this is a static (HTTP_PAGE_STATIC) or dynamic
    * (HTTP_PAGE_DYNAMIC) page. 
    */
   UINT8	type;
   /*  The relative path to this page. */
	INT8	*path;
   /*  The mime type to be returned in the MIMETYPE header. */
	INT8	*mimetype;
   /*  Either a structure defining the static page, or the 'cgi' function
    *  which will generate this page.
    */
	union 
	{
	   const struct staticpage	*spage;
		INT16	(*cgi)(struct http_request *);
	} content;

};



typedef const struct webpage Webpage;


extern const Http_Method http_defmethods[];
extern const Http_Method https_defmethods[];
extern const Http_Method httpDigestAuth_defmethods[];
extern const Http_Method httpBasicAuth_defmethods[];
extern const struct header_rec httpdefheaders[];
extern struct header_rec defreplies[];
extern struct http_usrs Http_Users[]; 

extern INT8 *HTTP_404_Reply_Body;
extern INT16 http_init(const Http_Method *methods,const struct header_rec *headers, Webpage *webpages,UINT16 port);
extern INT16 httpDigestAuth_init(const Http_Method *methods,const struct header_rec *headers, Webpage *webpages,UINT16 port);
extern INT16 httpBasicAuth_init(const Http_Method *methods,const struct header_rec *headers, Webpage *webpages,UINT16 port);

#ifndef _IAR_CODE
extern INT8 * strstr1(INT8 *s1, INT8 *s2);
#endif
extern INT16 http_Auth_Parse( Http_Request  *request );
extern INT16 http_authenticate( Http_Request  *request );
extern INT16 http_send_Authenticate( Http_Request  *request );
extern struct http_usrs Http_Usrs[];
extern INT8 *Realm_String;
extern INT8 Nonce_String[];
extern INT8 Nonce_String_buff[];
extern INT8 HttpDateBuff[];
extern void random_nonce(void);
extern INT8 * http_strstr( INT8 * start, INT8 *search,INT16 length );


#endif	/* _HTTP_H */
