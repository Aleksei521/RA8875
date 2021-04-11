
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
extern const struct staticpage main_htm;

// Pictures: JPG, GIF
extern const struct staticpage Acclmheading_jpg;
extern const struct staticpage ez80acclaimchip_jpg;

// CGIs
INT16 index_cgi(struct http_request *request);

// Java Applets


//Dummy. This is required if web files are stored in Flash Filesystem
struct mimetype mimetypes[] = {
	{".htm","text/html"},
	{".html","text/html"},
// If the file extensions does not match any then unknown will be used.
// This should always be at the end.
	{"unknown","unknown"}	
	};
UINT16 NoOfMimeTypes = sizeof(mimetypes)/sizeof(struct mimetype);


Webpage website[] = {
	{HTTP_PAGE_DYNAMIC, "/", "text/html", (struct staticpage	*)index_cgi },
	{HTTP_PAGE_STATIC, "/main.htm", "text/html", &main_htm },
	{HTTP_PAGE_STATIC, "/Acclmheading.jpg", "image/jpg", &Acclmheading_jpg },
	{HTTP_PAGE_STATIC, "/ez80acclaimchip.jpg", "image/gif", &ez80acclaimchip_jpg },

	{0, NULL, NULL, NULL }
};


// ------------------------------------------------------------------------------------------------------------------------


extern INT8 vers[];
INT8* VersionTable = "<h5> Software Version: ";

INT16 index_cgi(struct http_request *request)
{
	UINT16 Size;
	INT8 tmpbuf[64];
	INT8 tmpbuf2[16];

	Size = main_htm.size + 23;
	Size = main_htm.size + strlen(VersionTable)+strlen((INT8*)VERSION);

	http_add_header(request,HTTP_HDR_SEND_CLIENT_AUTH,request->AuthParams->rspauth);
	__http_date(tmpbuf,sizeof(tmpbuf));
	__http_itoal(Size,tmpbuf2);
		
	if ( request->keepalive) 
		http_add_header(request,HTTP_HDR_CONNECTION,"Keep-Alive");
	else
		http_add_header(request,HTTP_HDR_CONNECTION,"close");
	
	http_add_header(request,HTTP_HDR_DATE,tmpbuf);
	http_add_header(request,HTTP_HDR_CONTENT_LENGTH,tmpbuf2);
	if(http_output_reply(request, HTTP_200_OK) < 0 ) return -1;

	__http_write(request, (INT8*)main_htm.contents, main_htm.size);
	__http_write(request,VersionTable ,strlen(VersionTable));
	__http_write(request, (INT8*)VERSION, strlen((INT8*)VERSION));
	return( OK );
}
