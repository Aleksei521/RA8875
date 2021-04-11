/*
 * File       :     website.c
 *
 * Description:  This file contains web pages and link definitions.
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

#include "ZSysgen.h"
#include "ZTypes.h"
#include "ZThread.h"
#include "ZDevice.h"
#include "http.h"
#include "httpd.h"
#include "socket.h"

#include "version.h"


/*
 * Web Site Description
 */

/*
 * If a page is a static page, then the type HTTP_PAGE_STATIC should be
 * specified, and a struct staticpage used to describe the page data. If
 * a page is dynamic, then the type HTTP_PAGE_DYNAMIC should be used, and
 * the address of the CGI routine should be provided.
 */
// HTML pages
extern const struct staticpage indexf_htm;
extern const struct staticpage main_htm;
extern const struct staticpage ez80f91_htm;
extern const struct staticpage ez80f91f_htm;
extern const struct staticpage ez80f92_htm;
extern const struct staticpage ez80f92f_htm;

extern const struct staticpage header_htm;
extern const struct staticpage footer_htm;
extern const struct staticpage left_htm;
extern const struct staticpage siteinfo_htm;
extern const struct staticpage siteinfof_htm;
extern const struct staticpage overview_htm;
extern const struct staticpage overviewf_htm;
extern const struct staticpage tcpip_htm;
extern const struct staticpage tcpipf_htm;
extern const struct staticpage appletf_htm;
extern const struct staticpage cgi_htm;
extern const struct staticpage cgif_htm;
extern const struct staticpage javaapplets_htm;
extern const struct staticpage javascripts_htm;
extern const struct staticpage javascriptf_htm;

// Pictures: JPG, GIF
extern const struct staticpage Acclmheading_jpg;
extern const struct staticpage bar_gif;
extern const struct staticpage devkit_jpg;
extern const struct staticpage ez80acclaimchip_jpg;

// CGIs
INT16 increment_cgi(struct http_request *request);
INT16 index_cgi(struct http_request *request);

// Java Applets
extern const struct staticpage Lake_class;

//VG 
//The below table is used to fill the "Content type:" field in the the 
//HTTP replies. If the web files are added statically then "Content type:" is
//filled from the "char * mimetype" member of the Webpage structure.
//But for files added to the filesystem the "Content type:" will be filled 
//based on the file extensions.  
struct mimetype mimetypes[] = {
	{".htm","text/html"},
	{".html","text/html"},
	{".jpg","image/jpg"},
	{".class","application/octect-stream"},
	{".gif","image/gif"},
// If the file extensions does not match any then unknown will be used.
// This should always be at the end.
	{"unknown","unknown"}	
	};

UINT16 NoOfMimeTypes = sizeof(mimetypes)/sizeof(struct mimetype);

Webpage website[] = {
	{HTTP_PAGE_STATIC, "/", "text/html", &indexf_htm },
	{HTTP_PAGE_STATIC, "/index.htm", "text/html", &indexf_htm },
	{HTTP_PAGE_DYNAMIC, "/index_cgi.htm", "text/html", (struct staticpage*)index_cgi },
	{HTTP_PAGE_STATIC, "/main.htm", "text/html", &main_htm },
	{HTTP_PAGE_STATIC, "/ez80f91.htm", "text/html", &ez80f91_htm },
	{HTTP_PAGE_STATIC, "/ez80f91f.htm", "text/html", &ez80f91f_htm },
	{HTTP_PAGE_STATIC, "/ez80f92.htm", "text/html", &ez80f92_htm },
	{HTTP_PAGE_STATIC, "/ez80f92f.htm", "text/html", &ez80f92f_htm },
	{HTTP_PAGE_STATIC, "/header.htm", "text/html", &header_htm },
	{HTTP_PAGE_STATIC, "/footer.htm", "text/html", &footer_htm },
	{HTTP_PAGE_STATIC, "/left.htm", "text/html", &left_htm },
	{HTTP_PAGE_STATIC, "/siteinfo.htm", "text/html", &siteinfo_htm },
	{HTTP_PAGE_STATIC, "/siteinfof.htm", "text/html", &siteinfof_htm },
	{HTTP_PAGE_STATIC, "/overview.htm", "text/html", &overview_htm },
	{HTTP_PAGE_STATIC, "/overviewf.htm", "text/html", &overviewf_htm },
	{HTTP_PAGE_STATIC, "/tcpip.htm", "text/html", &tcpip_htm },
	{HTTP_PAGE_STATIC, "/tcpipf.htm", "text/html", &tcpipf_htm },
	{HTTP_PAGE_STATIC, "/appletf.htm", "text/html", &appletf_htm },
	{HTTP_PAGE_STATIC, "/javaapplets.htm", "text/html", &javaapplets_htm },
	{HTTP_PAGE_STATIC, "/javascripts.htm", "text/html", &javascripts_htm },
	{HTTP_PAGE_STATIC, "/javascriptf.htm", "text/html", &javascriptf_htm },
	{HTTP_PAGE_STATIC, "/Lake.class", "application/octect-stream", &Lake_class },
	{HTTP_PAGE_STATIC, "/Acclmheading.jpg", "image/jpg", &Acclmheading_jpg },
	{HTTP_PAGE_STATIC, "/bar.gif", "image/gif", &bar_gif },
	{HTTP_PAGE_STATIC, "/devkit.jpg", "image/jpg", &devkit_jpg },
	{HTTP_PAGE_STATIC, "/ez80acclaimchip.jpg", "image/gif", &ez80acclaimchip_jpg },
	{HTTP_PAGE_STATIC, "/cgi.htm", "text/html", &cgi_htm },
	{HTTP_PAGE_STATIC, "/cgif.htm", "text/html", &cgif_htm },
	{HTTP_PAGE_DYNAMIC, "/cgi-bin/increment", "text/html", (struct staticpage	*)increment_cgi },
	{0, NULL, NULL, NULL }
};


// ------------------------------------------------------------------------------------------------------------------------

static INT8 pagetop[] = "<HTML><TITLE>HTTP Post Demonstration - 25 Parameters</TITLE><center><H1>Returned Values</H1><p><table border=0 width=600 cellspacing=0 cellpadding= 0><tr><td><div align=center><table border=1 width=38% height=139 bgcolor=#6B85C7>";
static INT8 pagefoot1[] = "<P><A HREF=/cgi.htm>Click Here to Go Back</A>";
static INT8 pagefoot[] = "<P><A HREF=/ target=_top>Click Here to Go to Home</A></BODY></HTML>";
static INT8 endtablerow[] = "</tr>";
static INT8 endtable[] = "</tr></table>";

INT16 increment_cgi(struct http_request *request)
{
	INT8 								Buffer[20];
	INT8 *							str;
	UINT16 								i=0,j;


	http_add_header(request,HTTP_HDR_CONNECTION,"close");
	if(http_output_reply(request,HTTP_200_OK)<0)return -1;
	__http_write(request,pagetop,sizeof(pagetop)-1);

	/*
	 * Even though IPWorks can process upto 16 parameters, the libraries will not completely parse
	 * the string pointed to by request->params[HTTP_MAX_PARAMS-1].value if there are MORE than 16 
	 * parameters present.
	 * In addition the last Value in the request->params[HTTP_MAX_PARAMS-1].value will include all
	 * remaining parameters concatenated together.  Individual parameters are separated by the "&" 
	 * character.  To process the remaining parameters you could either write code to manually parse 
	 * the concatenated parameters, or just reset the number of parameters in the request structure 
	 * to 0 and have the libraries parse the next block of parameters after the '&' delimiter.
	 */
   while( request->numparams )
   {
      for(j=0; j<request->numparams; j++,i++ )
      {
         if( i%5 == 0 )
         {
            __http_write(request, "<tr>",4);
         }
         xc_sprintf( Buffer, "<td>%d</td>", (atoi(request->params[j].value)+1) );
         __http_write(request,Buffer,strlen(Buffer));
         if( i%5 == 4 )
         {
            __http_write(request, endtablerow, 5);
         }
      }

      /*
       * Check for more parameters.
       */
      httpGetMoreParams( request );
   }

	__http_write(request, endtable,13);

	__http_write(request,pagefoot1,sizeof(pagefoot1)-1);
	__http_write(request,pagefoot,sizeof(pagefoot)-1);	
	request->keepalive = 0;

	return( OK );
}



extern INT8 vers[];

INT8* VersionTable = "<h5> Software Version: ";

INT16 index_cgi(struct http_request *request)
{
	UINT16 Size;
	INT8 tmpbuf[64];
	INT8 tmpbuf2[16];

	Size = footer_htm.size + strlen(VersionTable)+strlen((INT8*)VERSION);

	__http_date(tmpbuf,sizeof(tmpbuf));
	__http_itoal(Size,tmpbuf2);
	
	if ( request->keepalive) 
		http_add_header(request,HTTP_HDR_CONNECTION,"Keep-Alive");
	else
		http_add_header(request,HTTP_HDR_CONNECTION,"close");
	
	http_add_header(request,HTTP_HDR_CONTENT_TYPE,"text/html");
	http_add_header(request,HTTP_HDR_DATE,tmpbuf);
	http_add_header(request,HTTP_HDR_CONTENT_LENGTH,tmpbuf2);
	if( http_output_reply(request, HTTP_200_OK) < 0 ) return -1;

	__http_write(request,(INT8*)footer_htm.contents,footer_htm.size);
	__http_write(request,VersionTable,strlen(VersionTable));
	__http_write(request,(INT8*)VERSION,strlen((INT8*)VERSION));
	return( OK );
}
