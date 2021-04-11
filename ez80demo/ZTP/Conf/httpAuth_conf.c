/*
 * File       :     httpA_conf.c
 *
 * Description:  This file contains the username and password pairs for authentication.
 *
 * Copyright 2004 ZiLOG Inc.  ALL RIGHTS RESERVED.
 *
 * This file contains unpublished confidential and proprietary information
 * of ZiLOG, Inc.
 * NO PART OF THIS WORK MAY BE DUPLICATED, STORED, PUBLISHED OR DISCLOSED 
 * IN ANY FORM WITHOUT THE PRIOR WRITTEN CONSENT OF ZiLOG, INC.
 * This is not a license and no use of any kind of this work is authorized
 * in the absence of a written license granted by ZiLOG, Inc. in ZiLOG's 
 * sole discretion 
 */


#include "httpAuth.h"
#include "http.h"

struct http_usrs Http_Usrs[]={ {"ez80","webserver"},
						  {"zilog","ez80"}, 
						  {"anonymous","anonymous"}};

INT16 Max_Http_Users= sizeof(Http_Usrs)/sizeof(struct http_usrs);

INT8 *Realm_String = "eZ80 WebServer";
