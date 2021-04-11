
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
extern const struct staticpage call_cgi_htm;
extern const struct staticpage serial_to_tcp_htm;
extern const struct staticpage S2TCP_htm;
// CGIs
extern INT16 S2TCP_cgi(struct http_request *request);
extern INT16 SerialRead_cgi(struct http_request *request);

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
	{HTTP_PAGE_STATIC, "/", "text/html", &S2TCP_htm },
	{HTTP_PAGE_STATIC, "/S2TCP.htm", "text/html", &S2TCP_htm },
	{HTTP_PAGE_STATIC, "/serial_to_tcp.htm", "text/html", &serial_to_tcp_htm },
	{HTTP_PAGE_DYNAMIC, "/cgi-bin/serial_to_tcp", "text/html",(struct staticpage*)S2TCP_cgi },
	{HTTP_PAGE_STATIC, "/call_cgi.htm", "text/html",&call_cgi_htm },
	{HTTP_PAGE_DYNAMIC, "/cgi-bin/call_cgi", "text/html",(struct staticpage *) SerialRead_cgi },
	{0, NULL, NULL, NULL }
};
